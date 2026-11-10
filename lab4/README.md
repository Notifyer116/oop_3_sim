# Лабораторная работа 4. Перегрузка функций и преобразование типов

[Задание](https://github.com/mvodya/vvsu-study/tree/master/2026-S2/cpp-programming/labs/04-overloads-conversions)

Сборка и запуск из папки `lab4` (C++17):

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp weather.cpp -o weather
./weather
```

В Windows имя программы: `weather.exe`. Для корректного отображения русского
текста терминал должен использовать UTF-8 (в Windows: `chcp 65001`).
Можно передать путь к другому CSV первым аргументом: `./weather other.csv`.
Без аргумента используется `weather_day.csv` из текущей папки.

Формат входных строк и допустимость значений считаются корректными по условию.
Количество строк произвольное; файл без измерений обрабатывается отдельно.

[Пояснение к коду](explanation.pdf)
