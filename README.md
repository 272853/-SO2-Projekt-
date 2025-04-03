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

1.1 Przydzielanie potrzebnych zasobów do danego Filozofa
    Każdy filozof dostaje dwa widelce (muteksy) – jeden po lewej stronie i jeden po prawej.
    W konstruktorze Philosopher(int i, mutex* left, mutex* right, atomic<int>* lastEatTime) przypisuje się im odpowiednie muteksy.
    Każdy filozof działa w osobnym wątku i próbuje zdobyć dwa widelce, aby móc jeść.

1.2 Zapobieganie wyścigowi (race condition)
    Potencjalny wyścig może wystąpić przy aktualizacji czasu ostatniego posiłku lastEat. Wykorzystanie atomic<int> minimalizuje to ryzyko, zapewniając, że operacja inkrementacji (*lastEat)++ i reset *lastEat = 0; są bezpieczne dla współbieżności.

1.3 Unikanie zakleszczenia (deadlock)
     try_lock(), powoduje, że filozof nie będzie czekał w nieskończoność na prawy widelec – jeśli go nie dostanie, puści lewy i spróbuje ponownie później.


1.4 Zwalnianie nieużywanych zasobów

    Po zakończeniu jedzenia filozof zwalnia prawy i lewy widelec za pomocą unlock(), co umożliwia innym filozofom skorzystanie z nich.

2. Kontrola dostępności
2.1 Równomierne przydzielanie czasu dostępu do zasobów

    

2.2 Zapobieganie zagłodzeniu (starvation)

    



Działanie Programu:
Stworzyłem odzielny niezależny wątek który jest w stanie stale wczytywać dane z klawiatóry i jeśli użytkownik wpiszę q lub Q 
i zatwirdzi enter'em program się wyłącza. Pozwala to zakończyć działanie programu który działa w nieskończonośc.

Co 1s syswietlana jest informacja jakie wątki  zakińczyły pracę i jake zaczeły pracę tworząc sekwęcje zarządzania wspułdzielonymi zasobami w wilewątkowości.
