![project logo](assets/logo.png)

# Opis
Sieciowa adaptacja znanej i lubianej gry w okręty.

# Założenia projektowe
- Architektura klient-serwer:
  - **Serwer:** udostępnia "pokoje" które odpowiadają rozgrywce między klientami.
  - **Klient:** wyświetla "pokoje" z do których może połączyć się klient oraz po połączeniu z "pokojem" wyświetla rozgrywkę.
- Możliwość ustalenia zasad pokoju:
  - ilość statków poszczególnych rodzajów,
  - umożliwienie salw,
  - zmiana czasu tury.

# Aktualny stan
- serwer umożliwia tylko jedną rozgrywkę,
- serwer akceptuje wszystkie połączenia, brak walidacji klienta,
- prosta komunikacja między klientem i  serwerem,
- zmiany reguł gry nie są sprawdzane między klientami,

# Wspomaganie projektu
Wymagania/czego można się nauczyć:
- C++20
- cmake
- [raylib](https://github.com/raysan5/raylib) bardzo prosta biblioteka graficzna napisana w języku C
- [asio](https://think-async.com/Asio/) biblioteka sieciowa umożliwiająca asynchroniczny model komunikacji

Dobrym miejscem na początek będzie karta ["issues"](https://github.com/skni-kod/BattleShips/labels/good%20first%20issue).
