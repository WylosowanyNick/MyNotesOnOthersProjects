# O projektach

- Folder "01_Vertex_Arrays" zawiera kod przerobiony na podstawie pierwszych 12stu tutoriali. Zawiera on pojęcia:
    - Bibliotek: systemowych tworzących okno, tworzących kontekst OpenGLa, ładujących OpenGLa.
    - Vertex Buffer, Index Buffer, Vertex Array, Shader, Uniform.
- Folder "02_ImGui" zawiera kod przerobiony na podstawie tutoriali 13-23 włącznie. Ponadto, co zawiera "01_Vertex_Arrays", zawiera on pojęcia:
    - Abstrakcji OpenGLa do klas.
    - Texture, Blending.
    - Matematyki w OpenGLu, macierzy MVP (Model View Projection).
    - ImGui.
- Folder "03_Setting_up_a_Test_Framework" zawiera kod przerobiony na podstawie tutoriali 24-27 włącznie. Ponadto, co zawiera "02_ImGui", uwzględniono w nim Test Framework.
- Folder "04_Batch_Rendering" zawiera kod przerobiony na podstawie tutoriali 28-31 włącznie. Dla jego zbudowania należy wykorzystać projekt "../OpenGL/". Odpowiednie foldery należy wklejać do tego projektu jako foldery o nazwach "OpenGL-Sandbox". Następnie należy skonfigurować i zbudować projekt.

# O używanym kompilatorze (MSVC)

Kompilacja kompilatorem MSVC (Microsoft Visual C++) wymaga użycia komend wiersza poleceń developera dla Visual Studio. Używanie komend tego wiersza poleceń wymaga użycia tego wiersza poleceń (ze względu na znajdujące się w nim zmienne środowiskowe). Możliwe jest jednak zintegrowanie tego wiersza poleceń wewnątrz Visual Studio Code.

1. Otwórz Command Palette (wciśnij F1)
2. Wpisz Preferences: Open Settings (JSON)
3. Wstaw poniższy kod do pliku JSONa (odpowiednio modyfikując ścieżkę):

```json
"terminal.integrated.profiles.windows": {
    "My Command Prompt": {
    "path": "cmd.exe",
    "args": ["/k", "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat"]
    }
},
"terminal.integrated.defaultProfile.windows": "My Command Prompt",
```

Dla wyświetlenia opcji budowania, optymalizacji, debugowania i analizy kodu kompilatora cl wpisz "cl /?".

# O używanych bibliotekach

- *glfw3* jest to wieloplatformowa biblioteka pozwalająca na tworzenie i zarządzanie: oknami i kontekstem OpenGLa. Oprócz tego zarządza danymi wejściowymi z klawiatury i myszki.
- *glew32s* jest to biblioteka ładująca wskaźniki na funkcje OpenGLa ze sterowników karty graficznej. Biblioteka dostarcza również plik nagłówkowy funkcji OpenGLa.
- *opengl32.lib* zaznacza potrzebę dynamicznego linkowania z biblioteką *opengl32.dll* w czasie wykonywania programu, gdy nie została ona odnaleziona.
- *gdi32*, *shell32*, *user32* to statyczne biblioteki systemowe.
- *msvcrt* jest biblioteką dostarczającą kompilatorowi MSVC większość funkcji standardowej biblioteki C.

# Źródła

- [Dokumentacja OpenGLa](https://docs.gl/)