#include "SandboxLayer.h"
#include <stb_image\stb_image.h>

using namespace GLCore;
using namespace GLCore::Utils;

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoords;
	float TexID;
};

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

static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
{
	constexpr float size = 1.0f;

	Vertex v0, v1, v2, v3;
	v0.Position = { x, y, 0.0f };
	v1.Position = { x+size, y, 0.0f };
	v2.Position = { x+size, y+size, 0.0f };
	v3.Position = { x, y+size, 0.0f };

	v0.Color = v1.Color = v2.Color = v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

	v0.TexCoords = { 0.0f, 0.0f };
	v1.TexCoords = { 1.0f, 0.0f };
	v2.TexCoords = { 1.0f, 1.0f };
	v3.TexCoords = { 0.0f, 1.0f };

	v0.TexID = v1.TexID = v2.TexID = v3.TexID = textureID;

	return { v0, v1, v2, v3 };
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
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB); // Dynamic Vertex Buffer
	glBufferData(GL_ARRAY_BUFFER, 1000*sizeof(Vertex), /* maximum size of the vertex buffer */
		nullptr /* not suppling with any data */, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

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

	// Creating Quads
	auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0.0f);
	auto q1 = CreateQuad( 0.5f, -0.5f, 1.0f);

	Vertex vertices[8];
	memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
	memcpy(vertices+q0.size(), q1.data(), q1.size() * sizeof(Vertex));

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Sending the data directly to the GPU buffer
	// Optional way of writing to the memory
	// glMapBuffer(); // Returning pointer function, which allows to write directly to necessary memory
	// glUnmapBuffer(); // Uploading the buffer to the GPU

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
	ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
	ImGui::End();
}
