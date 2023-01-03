#include "TestTexture2D.h"

namespace test {

    TestTexture2D::TestTexture2D()
        : m_Translation(0.0f, 0.0f, 0.0f), m_Proj(glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))
    {
        // Tablica dla Vertex Buffer
        float positions[] = 
        {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f
        };

        // Tablica dla Index Buffer
        unsigned int indicies[] =
        {
            0,1,2,
            2,3,0
        };

        // Vertex Array i Vertex Buffer
        m_VAO = std::make_unique<VertexArray>();
        m_VB = std::make_unique<VertexBuffer>(positions, 4*4*sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VB, layout);

        // Index Buffer
        m_IB = std::make_unique<IndexBuffer>(indicies, 6);
        
        // Shader i tekstura
        m_Shader = std::make_unique<Shader>("src/shaders/Basic.shader");
        m_Shader->Bind();
        
        m_Texture = std::make_unique<Texture>("res/textures/Pinguin.png");
        m_Shader->SetUniform1i("u_Texture", 0); // 0 jest slotem textury w OpenGLu
    }

    TestTexture2D::~TestTexture2D()
    {}

    void TestTexture2D::OnUpdate(float deltaTime)
    {}

    void TestTexture2D::OnRender()
    {
        // Renderer
        Renderer renderer;

        m_Texture->Bind();

        // Macierze Model View Projection (MVP) zdefiniowane dla utrwalenia w odwrotnym
        // porządku z powodu ustawienia grafiki w kolumnowym, a nie wierszowym, porządku.
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        // PVM, a nie MVP z powodu ustawienia grafiki w kolumnowym, a nie wierszowym, porządku
        glm::mat4 mvp = m_Proj * m_View * model;

        // Kolor tła
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Wypełnianie buforów zdefiniowanym powyżej kolorem
        glClear(GL_COLOR_BUFFER_BIT);

        // MVP i Shader
        model = glm::translate(glm::mat4(1.0f), m_Translation);
        mvp = m_Proj * m_View * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        // Rysowanie
        renderer.Draw(*m_VAO, *m_IB, *m_Shader);
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::Begin("ImGUI window");
        ImGui::SliderFloat("Translation x", &m_Translation.x, -1.5f, 1.5f);
        ImGui::SliderFloat("Translation y", &m_Translation.y, -1.5f, 1.5f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    
}