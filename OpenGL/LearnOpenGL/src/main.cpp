#include <GL/glew.h> // Biblioteka ładująca wskaźniki na funkcje OpenGLa. Ta biblioteka musi się znajdować przed biblioteką GLFW
#include <GLFW/glfw3.h> // Biblioteka tworząca okno

#include "Utilities/Utilities.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace Utilities;

// Przeskalowanie wielkości okna -> zmiana Viewportu
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main(void)
{
// Inicjalizacja okna
  GLFWwindow* window;

  // Inicjalizacja biblioteki
  if (!glfwInit())
    return -1;

  // Definicja używanej wersji OpenGLa (3.3 Core)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Tworzenie okna i jego kontekstu OpenGLa
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window); // Ustaw kontekst okna bieżącym wątkiem
  glfwSwapInterval(1); // Ustawienie minimalnej liczby aktualizacji ekranu do odczekania, aż nastąpi glfwSwapBuffers
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Przeskalowanie wielkości okna -> zmiana Viewportu

  // Ładowanie wskaźników (i rozszerzeń OpenGLa) na funkcje OpenGLa
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize Glew" << std::endl;
    glfwTerminate();
    return -1;
  }
  std::cout << glGetString(GL_VERSION) << '\n'; // Wersja OpenGLa

// Dane do rysowania
  // Tablica pozycji vertexów
  float positions[] =
  {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,

    -0.5f,  0.5f,
  };

  // Tablica indeksów. Aby narysować trójkąt, potrzebne są trzy vertexy.
  // Aby narysować prostokąt potrzebne są dwa trójkąty. Dwa trójkąty,
  // z których składa się prostokąt, mają dwa wierzchołki wspólne.
  // Niepotrzebne jest przechowywanie dwóch, takich samych wierzchołków.
  // Zamiast nich w tablicy indeksów przechowuje się indeksy wskazujące
  // na vertexy z bufora do narysowania. Bufor to tablica danych.
  unsigned int indicies[] =
  {
    0,1,2,
    2,3,0
  };

  // Vertex Array przechowuje wskaźnik na bufor vertexów, typ danych
  // przechowywanych przez ten bufor oraz bufor elementów (indeksów).
  unsigned int vao;
  glGenVertexArrays(1, &vao); // Generowanie Vertex Arrayów (tu akurat jednego z wielu możliwych)
  glBindVertexArray(vao); // Wiązanie jednego z wielu (możliwych) Vertex Arrayów

  // Etykieta (czytaj dalej)
  unsigned int vbo;
  glGenBuffers(1, &vbo); // Generowanie buffora
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // Wiązanie wygenerowanego bufora jako arraya
  glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions,
  GL_STATIC_DRAW); // Przypisanie danych związanemu array buforowi
  // Typ danych bufora
  glEnableVertexAttribArray(0); // Włączenie zerowego atrybutu vertexa związanego array bufora
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), /* 2*sizeof(float) można tu zastąpić 0 (OpenGL domyśli się stridu na podstawie poprzednich danych) */
  (const void*) 0); // Przypisanie typu danych pierwszemu atrybutowi związanego bufora
  // Te wszystkie dane do powyższej etykiety przechowuje związany
  // Vertex Array z linijki glBindVertexArray(vao);

  // Bufor indeksów
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indicies, GL_STATIC_DRAW);

// Shader
  Shader shader("res/shaders/shader.vs", "res/shaders/shader.fs");

  unsigned int program = shader.ID;
  shader.Use(); // Wiązanie shadera

  // Uniform (sposób ustawienia danych w shaderze z poziomu C++)
  shader.Set4Floats("u_Color", 0.8f, 0.3f, 0.8f, 1.0f); // R, G, B, alpha

  // Odwiązanie wszystkiego (dla demonstracji)
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glUseProgram(0);

// Pętla programu
  float r = 0.0f, increment = 0.05f;
  while(!glfwWindowShouldClose(window))
  {
    // Sprawdź eventy okna i wywołaj odpowiednie dla nich funkcje
    glfwPollEvents();

    // Klawiszowe eventy okna
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true); // Esc -> Zamknięcie okna

    // Działanie programu
    if(r > 1.0f)
      increment = -0.05f;
    else if(r < 0.0f)
      increment = 0.05f;

    r += increment;

    // Renderowanie
    glClear(GL_COLOR_BUFFER_BIT); // Czyszczenie tylnego bufora

    glBindVertexArray(vao); // Wiązanie Vertex Arraya (wiązanie array bufora, jego typu danych oraz Index Buffora)

    shader.Use(); // Wiązanie shader programu
		shader.Set4Floats("u_Color", r, 0.3f, 0.8f, 1.0f); // Ustawianie uniformu

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Renderowanie. Przykład assertu: GLCall i rysowanie

    glfwSwapBuffers(window); // Podmiana przedniego i tylnego bufora określonego okna
  }

// Koniec programu
  glfwTerminate(); // Zamknięcie biblioteki GLFW

  glDeleteVertexArrays(1, &vao); // Opcjonalnie wykonane tu zaznaczenie zasobów OpenGLa do usunięcia. Te zasoby są automatycznie usuwane przy zamknięciu kontekstu OpenGLa.
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteProgram(program);

  return 0;
}
