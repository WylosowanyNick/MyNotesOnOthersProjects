#include "SandboxLayer.h"
#include <stb_image\stb_image.h>

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	:m_CameraController(16.0f/9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb);

	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);

	return textureID;
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shader.glsl.vert",
		"assets/shader.glsl.frag"
	));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Vertex Array
	float vertices[] = { // This array holds coords (x,y,z), color (rgba), textures coords (x,y), texture id (float) for every Quad's vertice to draw
		-1.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
		 1.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,
	};

	uint32_t indicies[] = { // This array holds indicies for every Quad to draw
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,
	};

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);
	glEnableVertexArrayAttrib(m_QuadVA, 0);
	glEnableVertexArrayAttrib(m_QuadVA, 1);
	glEnableVertexArrayAttrib(m_QuadVA, 2);
	glEnableVertexArrayAttrib(m_QuadVA, 3);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10*sizeof(float), (const void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10*sizeof(float), (const void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10*sizeof(float), (const void*)(7*sizeof(float)));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(9 * sizeof(float)));

	glCreateBuffers(1, &m_QuadEB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// Textures
	m_Texture1 = LoadTexture("assets/Texture1.png");
	m_Texture2 = LoadTexture("assets/Texture2.png");

	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 }; // sampler2D u_Textures specifies indicies of the Loaded Textures
	glUniform1iv(loc, 2, samplers); // look for: glBindTextureUnit
	// Minimal amount of these slots is 16.
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
	// Events here
}

static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// Shader
	glUseProgram(m_Shader->GetRendererID());

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vp);

	// Textures
	glBindTextureUnit(0, m_Texture1);
	glBindTextureUnit(1, m_Texture2);

	// Rendering
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(m_QuadVA);

	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", // Here reducible
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr); // Drawing each Quad in a single draw call
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
