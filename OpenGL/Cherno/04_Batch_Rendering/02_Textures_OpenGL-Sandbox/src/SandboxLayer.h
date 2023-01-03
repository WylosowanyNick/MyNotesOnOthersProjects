#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;

	std::unique_ptr<GLCore::Utils::Shader> m_Shader;
	uint32_t m_QuadVA, m_QuadVB, m_QuadEB, m_Texture1, m_Texture2;
};
