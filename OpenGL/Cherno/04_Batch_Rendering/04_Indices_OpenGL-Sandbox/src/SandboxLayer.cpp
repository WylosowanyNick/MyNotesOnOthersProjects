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

static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
{
	constexpr float size = 1.0f;

	target->Position = { x, y, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 0.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x+size, y, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 1.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x+size, y+size, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 1.0f, 1.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x, y+size, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 0.0f, 1.0f };
	target->TexID = textureID;
	target++;

	return target;
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shader.glsl.vert",
		"assets/shader.glsl.frag"
	));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	constexpr size_t MaxQuadCount = 1000;
	constexpr size_t MaxVertexCount = 4 * MaxQuadCount;
	constexpr size_t MaxIndexCount = 6 * MaxQuadCount;

	// Vertex Array
	uint32_t indicies[MaxIndexCount]; // This array holds indicies for every Quad to draw
	{
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxQuadCount; i += 6)
		{
			indicies[i + 0] = 0 + offset;
			indicies[i + 1] = 1 + offset;
			indicies[i + 2] = 2 + offset;

			indicies[i + 3] = 2 + offset;
			indicies[i + 4] = 3 + offset;
			indicies[i + 5] = 0 + offset;

			offset += 4;
		}
	}

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);
	glEnableVertexArrayAttrib(m_QuadVA, 0);
	glEnableVertexArrayAttrib(m_QuadVA, 1);
	glEnableVertexArrayAttrib(m_QuadVA, 2);
	glEnableVertexArrayAttrib(m_QuadVA, 3);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB); // Dynamic Vertex Buffer
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), /* maximum size of the vertex buffer */
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
	uint32_t indexCount = 0;
	std::array<Vertex, 1000> vertices;

	Vertex* buffer = vertices.data();
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			buffer = CreateQuad(buffer, (float)x, (float)y, (float)((x + y) % 2));
			indexCount += 6;
		}
	}

	buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 0.0f);
	indexCount += 6;

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()); // Sending the data directly to the GPU buffer
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
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr); // Drawing each Quad in a single draw call
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
	ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);
	ImGui::End();
}
