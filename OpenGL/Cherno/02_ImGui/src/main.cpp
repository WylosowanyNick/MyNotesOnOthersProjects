#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 800;

int main()
{
// Inicjalizacja programu
    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if(!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGL())
    {
        std::cout << "Failed to initialize Glew" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << '\n';

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Dla demonstracji, redukowalne tu określenie viewportu OpenGLa w utworzonym oknie

    // Inicjalizacja ImGUI
    // IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
	// ImGuiIO& io = ImGui::GetIO(); // Tu redukowalne

// Ustawienie blendowania
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

// Dane do rysowania
    { // Scope dla poprawnego usunięcia stack allocated vb i ib
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
    VertexArray va;
    VertexBuffer vb(positions, 4*4*sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // Index Buffer
    IndexBuffer ib(indicies, 6);
    
    // Shader i tekstura
    Shader shader("src/shaders/Basic.shader");
    shader.Bind();
    
    Texture texture("res/textures/Pinguin.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0); // 0 jest slotem textury w OpenGLu

    // Odwiązanie wszystkiego (tu dla demonstracji,
    // ale w ogólności dla zapobiegnięcia ich zmodyfikowania)
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    // texture.Unbind(); // tekstury dla optymalizacji nie odwiązujemy

    // Renderer
    Renderer renderer;

    // Macierze Model View Projection (MVP) zdefiniowane dla utrwalenia w odwrotnym
    // porządku z powodu ustawienia grafiki w kolumnowym, a nie wierszowym, porządku.
    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    // PVM, a nie MVP z powodu ustawienia grafiki w kolumnowym, a nie wierszowym, porządku
    glm::mat4 mvp = proj * view * model;

    // Główna pętla programu
    while (!glfwWindowShouldClose(window))
    {
        // Kolor tła
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Wypełnianie buforów zdefiniowanym powyżej kolorem
        glClear(GL_COLOR_BUFFER_BIT);

        // Mówienie OpenGLowi o nowej ramce
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();        

        // MVP i Shader
        model = glm::translate(glm::mat4(1.0f), translation);
        mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        // Rysowanie
        renderer.Draw(va, ib, shader);

        // ImGUI
        ImGui::Begin("ImGUI window");
        ImGui::SliderFloat("Translation x", &translation.x, -1.5f, 1.5f);
        ImGui::SliderFloat("Translation y", &translation.y, -1.0f, 1.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Podmiana front bufora zawartością back bufora
        glfwSwapBuffers(window);

        // Eventy okna
        glfwPollEvents();
    }
    }
    
    // Zamykanie aplikacji
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
