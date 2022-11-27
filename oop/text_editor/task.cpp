#include "task.h"
#include "logged_command_wrapper.h"

/* Курсор влево */
class MoveCursorLeftCommand : public ICommand {
};

/* Курсор вправо */
class MoveCursorRightCommand : public ICommand {
};

/* Курсор вверх */
class MoveCursorUpCommand : public ICommand {
};

/* Курсор вниз */
class MoveCursorDownCommand : public ICommand {
};

/* Выделить текст */
class SelectTextCommand : public ICommand {
};

/* Ввести текст */
class InsertTextCommand : public ICommand {
};

/* Удалить текст */
class DeleteTextCommand : public ICommand {
};

/* Скопировать текст */
class CopyTextCommand : public ICommand {
};

/* Вставить скопированный текст */
class PasteTextCommand : public ICommand {
};

/* Привести выделенный текст в ВЕРХНИЙ регистр */
class UppercaseTextCommand : public ICommand {
};

/* Привести выделенный текст в нижний регистр */
class LowercaseTextCommand : public ICommand {
};

/* Перенести курсор в конец строки */
class MoveToEndCommand : public ICommand {
};

/* Перенести курсор в начало строки */
class MoveToStartCommand : public ICommand {
};

/* Удалить часть строки, начиная от позиции курсора до первого пробела или конца строки */
class DeleteWordCommand : public ICommand {
};

/* Макрос */
class MacroCommand : public ICommand {
};
