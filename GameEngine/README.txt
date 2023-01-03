Ten projekt powstał na podstawie tutorialu "Game Engine" z kanału na "The Cherno":
(https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)

Wykorzystane biblioteki i narzędzia:
    spdlog (System logowania) (https://github.com/gabime/spdlog)
    Premake (Generator plików budowy projketu) (https://github.com/premake/premake-core)

Od   1 do   3: Wprowadzenie
    "Silnik do tworzenia gier komputerowych" (który w całym projekcie skrótowo nazwano "silnikiem") jest platformą dla tworzenia aplikacji interaktywnych (niekoniecznie gier). Silnik stosuje się w:
        -tworzeniu gier,
        -wirtualnej/rozszerzonej rzeczywistości,
        -wizualizacji w czasie rzeczywistym,
        -wizualizacji architektonicznych,
        -symulacjach.

    Podstawą działania silnika jest transformacja danych z jednego formatu do innego, zrozumiałego dla tego silnika (przykładowo: transformacja plików z dysku do interaktywnych elementów wyświetlalnych na ekranie). Silnik nie tworzy wszystkich rodzajów danych od zera (przykładowo tekstur, czy modelów trójwymiarowych), ale wczytuje te dane (utworzone w innym programie) i transformuje je do formatu zrozumiałego dla tego silnika. Przy tej transformacji danych dodatkowo uwzględnia się inne informacje dotyczące tych danych (przykładowo: kategorię postaci dla modelu trójwymiarowego).

    Poza tym rolą silnika jest:
        -abstrakcja platformy docelowej (Windows/macOS/Linux/Android/iOS/Xbox/PlayStation/Nintendo),
        -renderowanie grafiki,
        -symulowanie fizyczne,
        -obsługa audio,
        -dostarczenie interfejsu graficznego dla edytora poziomów,
        -obsługa języka skryptowego (przykładowo Lua, czy C#),
        -obsługa własnego języka do tworzenia (kompilowalnych) shaderów.

    Konstrukcja silnika uwzględnia konstrukcję:
        -punktu wejścia,
            Czyli tego co się stanie z aplikacją klienta tego silnika, gdy zostanie ona uruchomiona.
        -warstwy aplikacji,
            Warstwy kontrolującej działanie aplikacji, czyli: wydarzeń (zamknięcia i zmienienia wielkości okna, wydarzeń myszy i klawiatury), pętli aplikacji, renderowania klatek, czasu aplikacji, wykonywania kodu silnika.
        -warstwy okna,
            Inaczej wygląda warstwa okna dla aplikacji tworzonych dla Windowsa, a inaczej dla Androida i PlayStation.
        -wsparcia do debugowania,
        -renderera,
        -systemu symulacji fizycznej,
        -entity-component system (w skrócie ECS),
            Definiującego zachowywanie się elementów.
        -edytora poziomów,
        -wbudowanego języka skryptowego,
        -języka do tworzenia (kompilowalnych) shaderów,

        -systemu pamięci (alokator i monitor pamięci),
        -obsługi operacji wejścia/wyjścia na plikach,
        -systemu wirtualnych plików,
        -obsługi audio,
        -systemu budowy plików dla silnika.
Od   4 do   7: Konfiguracja projektu Visual Studio
    4: Konfiguracja
        -Utworzenie repozytorium Gita.
        -Utworzenie rozwiązania Visual Studio.
        -Utworzenie projektu silnika Hazel i aplikacji Sandbox wewnątrz tego rozwiązania.
        -Konfiguracja projektu silnika Hazel i aplikacji Sandbox:
            -Usunięcie platformy x86 (Win32).
            -Konfiguracja lokalizacji plików budowy obu projektów.
            -Wynik budowy silnika: dynamiczna biblioteka dll. Wynik budowy aplikacji Sandbox: plik wykonywalny exe.
            -Utworzenie (niepełnej) referencji aplikacji Sandbox do silnika Hazel. Budowa silnika (biblioteki dll) -> aktualizacja plików projektu aplikacji Sandbox.
        -Napisanie prostej aplikacji wypisującej "Welcome to Hazel Engine!\n" dla zweryfikowania poprawności powyższych kroków.
    5: Punkt wejścia
        -Wewnątrz silnika zdefiniowano klasę Application, którą potencjalnie ma wykorzystać aplikacja klienta. Ta klasa powinna zostać wykorzystana w aplikacji klienta poprzez dziedziczenie po niej oraz zdefiniowanie funkcji zwracającej odpowiedni obiekt klasy (możliwie, że pochodnej) Application (Hazel::Application* Hazel::CreateApplication()).
        -Zdefiniowano makro HZ_BUILD_DLL, które definiuje makro HAZEL_API w zależności od tego, czy silnik jest budowany, czy wykorzystywany w aplikacji klienta (__declspec(dllexport) lub __declspec(dllimport)).
        -Zdefiniowano punkt wejścia aplikacji (EntryPoint.h) wewnątrz silnika gry. Ten punkt wejścia jest zawarty w pliku nagłówkowym Hazel.h, co pozwala utworzyć punkt wejścia aplikacji poprzez dołączenie tego pliku wewnątrz aplikacji klienta.
        -Skonigurowano oba projekty o potrzebę zdefiniowania odpowiednich makr (HZ_PLATFORM_WINDOWS i HZ_BUILD_DLL).
    6: System logowania
        -Dodano do projektu podmoduł Gita: spdlog, który potrzebny jest dla logowania.
        -Zabstraktowano ten moduł w kodzie w taki sposób, aby możliwe było ewentulane, łatwe zastąpienie tego modułu inną biblioteką.
    7: Premake (Generator plików budowy projektu)
        Dla umożliwienia działania silnika na różnych platformach (nie tylko dla Windowsa przy użyciu środowiska Visual Studio, ale również dla platform: macOS, Linux, Android, iOS przy użyciu środowisk/IDE/zestawu narzędzi tym platformom dedykowanych) potrzebny jest generator plików budowy projektu pozwalający utworzyć te pliki budowy projektu (Makefile) dla środowisk dedykowanych poszczególnym platformom (przykładowo; plik budowy projektu Xcode potrzebny do budowy plików dla platformy iOS).

        1.Potrzebny jest plik wykonywalny Hazel\vendor\bin\premake\premake5.exe. (Uwzględniono licencję)
        2.Skrypt Hazel\premake5.lua definiuje w sposób uniwersalny (niezależnie od środowiska/platformy) te pliki budowy projektu.
        3.Skrypt Hazel\GenerateProjects.bat wywołuje odpowiednio premake5.exe.
        4.Ustawić w Visual Studio projekt Sandbox poprzez "Set as Startup Project".
Od   8 do   8: System zdarzeń
    8: Planowanie systemu zdarzeń
        Wprowadzenie:
            System zdarzeń w pewien sposób obsługuje zdarzenia (i je notuje). Przykładami zdarzeń są:
                -zdarzenia okna (zamknięcie i zmienienie rozmiaru okna),
                -zdarzenia wejściowe (używanie myszy i klawiatury).

            Klasa "Application" reprezentuje i obsługuje całą aplikację z wykorzystaniem głównej pętli. Klasa "Application" otrzymuje eventy, przetwarza je i przesyła je dalej do rozpatrzenia do tzw. "warstw" (layers). Wspomniane, kolejne warstwy rozpatrują te zdarzenia i przesyłają je do dalszych warstw po uprzedniej, odpowiedniej filtracji tych zdarzeń.
        Notowanie zdarzeń okna aplikacji:
            Klasa "Window" reprezentuje okno aplikacji (i tworzy abstrakcję rzeczywistego okna aplikacji. Dla poszczególnych platform klasa "Window" obsługuje okno z wykorzystaniem dedykowanej tej platformie biblioteki. Dla platformy Windows jest to biblioteka "Windows API"). Ta klasa jako pierwsza odnotowuje zdarzenia okna i wejściowe. Klasa "Window" informuje (w pewien sposób) o tych zdarzeniach klasę "Application". Niepożądane jest, aby klasa "Window" miała dostęp do klasy "Application". Zamiast tego klasa "Application" tworzy system notujący zdarzenia, które to ona obsługuje. W następnym kroku klasa "Application" tworzy klasę "Window" i wiąże ona tą klasę z tym systemem notującym zdarzenia (systemem wywołania zwrotnego). Kiedy zdarzenie wystąpi w rzeczywistym oknie, klasa "Window" otrzyma wywołanie zwrotne (event callback), a następnie zadaniem tej klasy jest skonstruowanie wywołania zwrotnego do klasy "Application" (skonstruowanie zdarzenia aplikacji Hazel), które (w pewien sposób) klasa "Application" obsłuży. 

            Dla wytłumaczenia, w najprostszym przypadku to może być zrealizowane przez przekazanie klasie "Window" wskaźnika na funkcję (ew. std::function) zezwalającą na wywołanie zwrotne do klasy "Application", która to klasa jest właśnie właścicielem tej funkcji (przykładowo, "OnEvent(Event& e)"). To wywołanie jest możliwe, o ile ten wskaźnik na funkcję nie jest nullptrem. Klasa "Window" wywołuje tą funkcję z argumentem będącym strukturą danych reprezentującą wszystkie, potrzebne szczegóły związane ze zdarzeniem, które należy obsłużyć. Kiedy rzeczywiste zdarzenie nastąpi, dane zdarzenia są przechowywane na stosie, a omawiana funkcja jest wywoływana.

            Omawiany sposób tworzenia zdarzeń aplikacji (wywołania zwrotnego do klasy "Application") blokuje w pewien sposób wszystkie, inne czynności. Inny sposób tworzenia i notowania zdarzeń aplikacji wykorzystuje bufory, a w szczególności kolejki. Zbierane są zdarzenia w kolejce przy pomocy wskaźników, ale zbierane zdarzenia nie są obsługiwane od razu. Są one za to obsługiwane raz na klatkę. Nie ma jednak potrzeby implementować na razie takiego systemu obsługi zdarzeń, a może to być zrobione wraz z rozszerzaniem silnika.

            Omówiony sposób tworzenia zdarzeń aplikacji z wykorzystaniem wskaźnika na funkcję zostanie dla wygody zabstraktowany. Zostanie utworzony iterfejs słuchacza zdarzeń (IEventListener). Wtedy klasa "Window" będzie miała tabelę implementacji tych interfejsów. W tym przypadku, kiedy zdarzenie nastąpi, to zostanie wywołana odpowiednia implementacja interfejsu wywołująca omawianą metodę "OnEvent(Event& e)". Klasa "Application" i omawiane "layers" wtedy poniekąd stają się "IEventListener'ami", a tym samym klasa "Application" musi zaimplementować "OnEvent(Event& e)" oraz stać się słuchaczem klasy "Window".
        Wtrącając, oprócz wszystkiego co zostało przedstawione, należy:
            -utworzyć system zdarzeń poprzez utworzenie wszystkich danych, które są potrzebne dla wszystkich zdarzeń. Wszystkie zdarzenia będą posiadały podstawowy typ "Event".
            -utworzyć zdarzenia aplikacji (zdarzenia głównej pętli, OnUpdate, OnTick, OnRender), które mogłyby być wysłane do "layers" jako zdarzenia.
            -ułatwić debugowanie poprzez powiązanie systemu zdarzeń ze systemem do debugowania (logowania) oraz uwzględnienie w tym nazw i kategorii tych zdarzeń.
            -utworzyć rozdzielacz zdarzeń, który wysyła zdarzenia do obsłużenia ze względu na typ argumentu funkcji "OnEvent(Event& e)". Klasa "EventDispatcher" będzie opakowywać (wrap) zdarzenia, domyślać się typu tych zdarzeń oraz wysyłać je do odpowiednich (ze względu na typy tych zdarzeń) i danych wskaźników na funkcje.