# -SO2-Projekt-
Repo do Projektów z SO - inż. Damian Raczkowski

Projekt1-Problem Jedzących Filozofów
Realizacaj w języku c++ z bibloteką <thread>

Program morzna uruchmić za pomoca plku "makefile" wpisując w cmd:
     make -> ./main 4 (4-liczba filozofów)
     lub
     make run (wtedy odrazu zostanie uruchomiony program z argumętem wpisanym w miejscu * -> ./$(TARGET) * w "makefile")


Rozwiązanie problemu "Jedzących Filozofów" polega na:
1. zarządzanu zasobami
   
    1.1 należy przydzielać potrzebne zasoby do danego Filozofa
   
    1.2 nie pozwolić aby dwóch Filozofów używało jednego zasobu jednocześnie
   
    1.3 uniknąć zakleszczenia gdy dwóch Filozofów rząda dostępu do wspulnego zasobu
   
    1.4 zwalnianiu nieużywanych zasobów
   
3. kontroleowaniu dostępności
   
    2.1 należy równo przydzielać czas dostępności do zasobu dla każdego Filozofa
   
    2.2 nie należy dopuścić tego by któryś z Filozofów miał ograniczony dostęp do zasobów

Rozwiązanie w Programie:




Działanie Programu:
Stworzyłem odzielny niezależny wątek który jest w stanie stale wczytywać dane z klawiatóry i jeśli użytkownik wpiszę q lub Q 
i zatwirdzi enter'em program się wyłącza. Pozwala to zakończyć działanie programu który działa w nieskończonośc.

Co 1s syswietlana jest informacja jakie wątki  zakińczyły pracę i jake zaczeły pracę tworząc sekwęcje zarządzania wspułdzielonymi zasobami w wilewątkowości.
