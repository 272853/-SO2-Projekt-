# -SO2-Projekt-
Repo do Projektów z SO - inż. Damian Raczkowski

Projekt1-Problem Jedzących Filozofów
Realizacaj w języku c++ z bibloteką "<thread>"

Program morzna uruchmić za pomoca plku "makefile" wpisując w cmd:
     make -> ./main 4 (4-liczba filozofów)
     lub
     make run (wtedy odrazu zostanie uruchomiony program z argumętem wpisanym w miejscu * -> ./$(TARGET) * w "makefile")


Rozwiązanie problemu "Jedzących Filozofów" polega na:
1. zarządzanu zasobami
   
    1.1 należy przydzielać potrzebne zasoby do danego Filozofa
   
    1.2 nie pozwolić aby dwóch Filozofów używało jednego zasobu jednocześnie (race condition)
   
    1.3 uniknąć zakleszczenia gdy każdy Filozof trzyma jeden widelec i czeka na drugi (deadlock)
   
    1.4 zwalnianiu nieużywanych zasobów
   
3. kontroleowaniu dostępności
   
    2.1 należy równo przydzielać czas dostępności do zasobu dla każdego Filozofa
   
    2.2 nie należy dopuścić do tego by któryś z Filozofów nie miał dostępu do zasobów

Rozwiązanie w Programie:

1. Rozwiązanie problemów współbieżności
1.1 Przydzielanie potrzebnych zasobów do danego Filozofa

    Każdy filozof w programie ma przypisane dwie widelce (mutexy). Są one przypisane w konstruktorze klasy Philosopher, gdzie dla filozofa o indeksie i:

    leftFork to mutex forks[i]

    rightFork to mutex forks[(i + 1) % philosopherCount]

    Dzięki temu każdy filozof ma określone dwa widelce i próbuje je zdobyć przed rozpoczęciem jedzenia.
1.2 Zapobieganie wyścigowi (race condition)

    Mutexy (mutex): Zapobiegają jednoczesnemu używaniu tego samego widelca przez dwóch filozofów.

    Atomowe zmienne (atomic<int>): lastEat dla każdego filozofa oraz running zapewniają bezpieczne aktualizowanie wartości bez konieczności używania dodatkowych mutexów.

    Mutex priorityMutex: Zapobiega jednoczesnemu sprawdzaniu priorytetów przez wielu filozofów, co mogłoby prowadzić do błędnych decyzji.

1.3 Unikanie zakleszczenia (deadlock)

    Priorytet głodnych filozofów: Przed próbą zdobycia widelców filozof sprawdza, czy jest najbardziej "głodny" (jego lastEat jest najniższe w porównaniu do innych).

    try_lock() zamiast lock(): Zamiast blokować wątek do momentu uzyskania dostępu do obu widelców, try_lock() sprawdza, czy widelec jest dostępny. Jeśli nie jest, filozof odkłada zdobyty już widelec i próbuje później.

    Mechanizm zwiększania lastEat: Jeśli filozof nie może jeść, jego licznik głodu (lastEat) zwiększa się, co oznacza, że jego priorytet do jedzenia rośnie.

1.4 Zwalnianie nieużywanych zasobów

    Widelec jest zwalniany natychmiast po skończeniu jedzenia: Po zakończeniu posiłku filozof resetuje swój lastEat i odblokowuje widelce.

    Filozofowie nie blokują widelców, jeśli nie mogą ich zdobyć obu: Dzięki try_lock(), jeśli filozof nie może zdobyć obu widelców, nie zatrzymuje on zasobów, co pozwala innym filozofom z nich korzystać.

2. Kontrola dostępności
2.1 Równomierne przydzielanie czasu dostępu do zasobów

    Dzięki mechanizmowi lastEat filozofowie, którzy długo nie jedli, zyskują wyższy priorytet. Przed próbą zdobycia widelców sprawdzają, czy nie ma filozofa bardziej "głodnego", co prowadzi do bardziej sprawiedliwego przydzielania zasobów.

2.2 Zapobieganie zagłodzeniu (starvation)

    Mechanizm lastEat: Filozofowie, którzy długo nie jedli, stopniowo zwiększają swoją wartość lastEat. Przy każdej próbie zdobycia widelców filozofowie sprawdzają, czy ktoś inny jest bardziej głodny i czekają na swoją kolej.

    Mutex priorityMutex: Zapewnia, że ocena priorytetów jest wykonywana w sposób spójny i zapobiega sytuacjom, w których jeden filozof byłby niesprawiedliwie pomijany.
    



Działanie Programu:
Stworzyłem odzielny niezależny wątek który jest w stanie stale wczytywać dane z klawiatóry i jeśli użytkownik wpiszę q lub Q 
i zatwirdzi enter'em program się wyłącza. Pozwala to zakończyć działanie programu który domyślnie działa w nieskończonośc.

Co 1s syswietlana jest informacja jakie wątki  zakińczyły pracę i jake zaczeły pracę tworząc sekwęcje zarządzania wspułdzielonymi zasobami w wilewątkowości.
