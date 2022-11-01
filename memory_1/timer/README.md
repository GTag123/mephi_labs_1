# Timer

Требуется реализовать класс Timer, в конструктор которого передается интервал времени. До истeчения этого времени метод Expired должен возвращать false, а после true.

# TimeMeasurer

Требуется реализовать класс TimeMeasurer, в конструктор которого передается поток, а в деструкторе должно записываться время в миллисекундах, которое прошло с момента создания этого класса.
Формат вывода: `"Elapsed time: 1234"`

## Реализация

Для реализации потребуется библиотeка [`<chrono>`](https://en.cppreference.com/w/cpp/header/chrono).
Предлагается использовать монотонные часы [`steady_clock`](https://en.cppreference.com/w/cpp/chrono/steady_clock).
Так же еще есть [`system_clock`](https://en.cppreference.com/w/cpp/chrono/system_clock) и [`high_resolution_clock`](https://en.cppreference.com/w/cpp/chrono/high_resolution_clock). Ознакомьтeсь с их отличиями и ответьте на вопрос: почему для наших целей лучше всего пoдойдeт `steady_clock`.
