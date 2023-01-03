// Program's includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

// User's includes
#include "Renderer.h"
#include "Texture.h"

// Vendor's includes
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Tests' includes
#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

constexpr int WINDOW_WIDTH =  800;
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

    // Renderer
    Renderer renderer;

// Testy
    // testMenu
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;
    
    // Dodawanie testów
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    
    // Główna pętla programu
    while (!glfwWindowShouldClose(window))
    {
        // Czyszczenie okna
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();
        
        // Mówienie OpenGLowi o nowej ramce
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();        

        // Działanie testu
        if(currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if(currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        // Renderowanie ImGui
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Podmiana front bufora zawartością back bufora
        glfwSwapBuffers(window);

        // Eventy okna
        glfwPollEvents();
    }
        delete currentTest;
        if(currentTest != testMenu)
            delete testMenu;
    
    // Zamykanie aplikacji
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
