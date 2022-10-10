**Код решения нужно писать в файл `array.h`**

## Class:
В данном задании необходимо написать класс, реализующий динамический массив целых чисел. Этот класс является аналогом std::vector, но не полностью его повторяет.
* Объявить класс для реализации. В нём должны храниться указатель на участок памяти с данными, реальный размер, размер аллоцированной памяти и для целей проверки знаний ссылку на поток вывода ostream&
* sizeof этого класса должен быть равен 32 байта

## Конструктор и деструктор:
Необходимо реализовать конструкторы и деструктор, которые логируют состояние. Набор конструкторов и деструкторов содержится в array.h из задания.
Конструкторы логируют в переданный выходной поток печатает`<constructor name> <print array>\n`. Имена конструкторов:
* `Array(std::ostream& ostream)` печатает имя `Constructed.`
* `Array(const Array& array)` печатает имя `Constructed from another Array`
* `Array(size_t size, std::ostream& ostream = std::cout, int defaultValue = 0)` печатает имя `Constructed with default.`
А `<print array>` - результат вызова оператора `<<`, который описан ниже в разделе операторов
Например, `Array(2, 0)` должен напечатать `Constructed with default. Result Array's capacity is 4 and size is 2, elements are: 0, 0`
Деструктор должен в ostream вывести строчку `"Destructed <size>\n"`, где size - размер массива на момент удаления.
Логика определения зарезервированного размера:
1. Если в конструкторе не задаётся `Size`, то `Capacity = 2`
1. Если в конструкторе задаётся `Size`, то `Capacity = 2 * Size`
1. Если надо добавить элемент, а `Size == Capacity`, то `Capacity *= 2`
1. `Capacity` может быть изменено через `Reserve` только в большую сторону

## Методы:
Необходимо реалиовать следующий публичные методы:
* `PushBack(int)` - добавляет в конец массива новый элемент
* `Size()` - возвращает количество элементов в массиве
* `Resize(int)` - меняет размер используемой памяти до значения int
* `Reserve(int)` - меняет размер аллоцированной памяти до значения int. Только увеличивает, но не уменьшает
* `PopBack()` - удаляет последний элемент

## Операторы:
Необхоимо реализовать следующие операторы:
* `int& operator [](const size_t& i)` - осуществляет доступ к элементу int и возвращает ссылку на элемент
* `operator bool() const` - Возвращает пустой массив или нет (true означает, что массив непустой)
* `Array& operator <<(const int& value)` - реализуешь `PushBack(int)`
* `Array& operator <<(const Array& other)` - "дописывает" второй массив в конец к первому
* `bool operator <(const Array& it) const` - оператор лексикографического сравнения на меньше
* Другие операторы сравнения, которые выражаются через "меньше"
  * `bool operator >(const Array& it) const`
  * `bool operator !=(const Array& it) const`
  * `bool operator ==(const Array& it) const`
  * `bool operator <=(const Array& it) const`
  * `bool operator >=(const Array& it) const`
* `std::ostream& operator <<(std::ostream& ostream, const Array& array)` - оператор вывода (он же используется в конструкторе). Выводит следующий текст: `Result Array's capacity is <Array.Capacity> and size is <Array.Size>, elements are: <Array[0]>, <Array[1]>, ..., <Array[Array.Size() - 1]>`. Если массив пустой, то не выводит список элементов. Обратите внимание, что перевода строки в конце быть не должно, это важно.
Примеры резульата вызова:
* `Result Array's capacity is 16 and size is 10, elements are: 123, 345, 456, 1, 5, 81, 234, 345, 0, 9`
* `Result Array's capacity is 16 and size is 0`
