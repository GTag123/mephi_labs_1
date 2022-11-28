#pragma once
#include <stack>
#include <algorithm>
#include <list>
#include <ostream>
#include <optional>
#include "text_editor.h"

class CommandVisitor;

/*
 * Паттерн "команда" позволяет представлять действия в виде объектов,
 * чтобы их можно было хранить, передавать, дополнять какой-то новой функциональностью, логировать и т.д
 * Все команды в данном задании имеют общий интерфейс ICommand, с помощью которого с ними общается TextEditor и visitor'ы
 */
class ICommand {
public:
    // деструктор должен быть виртуальным, чтобы правильно вызываться, когда объект уничтожается через указатель на базовый класс
    virtual ~ICommand() = default;

    /*
     * В этом методе реализуется полезное действие команды
     */
    virtual void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) = 0;

    /*
     * В каждом наследнике интерфейса ICommand должен быть реализован вызов функции в visitor'е,
     * чтобы заработала "двойная диспетчеризация"
     */
    virtual void AcceptVisitor(CommandVisitor& visitor) = 0;
};

/*
 * Дальше идут конкретные команды, которые надо реализовать в task.cpp
 * Описание команд см. в readme.md
 * Примеры и результаты их применения см. в main.cpp
 */

/* Курсор влево */
class MoveCursorLeftCommand;

/* Курсор вправо */
class MoveCursorRightCommand;

/* Курсор вверх */
class MoveCursorUpCommand;

/* Курсор вниз */
class MoveCursorDownCommand;

/* Выделить текст */
class SelectTextCommand;

/* Ввести текст */
class InsertTextCommand;

/* Удалить текст */
class DeleteTextCommand;

/* Скопировать текст */
class CopyTextCommand;

/* Вставить скопированный текст */
class PasteTextCommand;

/* Привести выделенный текст в ВЕРХНИЙ регистр */
class UppercaseTextCommand;

/* Привести выделенный текст в нижний регистр */
class LowercaseTextCommand;

/* Перенести курсор в конец строки */
class MoveToEndCommand;

/* Перенести курсор в начало строки */
class MoveToStartCommand;

/* Удалить часть строки, начиная от позиции курсора до первого пробела или конца строки */
class DeleteWordCommand;

/* Макрос */
class MacroCommand;

/*
 * Класс-строитель (builder), который используется для создания объектов команд, скрывая детали реализации классов команд
 * от вызывающего кода в main.cpp
 */
class CommandBuilder {
public:
    /* Типы команд, которые данный билдер умеет генерировать */
    enum class Type {
        MoveCursorLeft = 0,
        MoveCursorRight,
        MoveCursorUp,
        MoveCursorDown,
        SelectText,
        InsertText,
        DeleteText,
        CopyText,
        PasteText,
        UppercaseText,
        LowercaseText,
        MoveToEnd,
        MoveToStart,
        DeleteWord,
        Macro,
    };

    CommandBuilder();

    /* Эта функция создает объект команды в соответствии с настройками, которые были заданы в текущем объекта билдера */
    CommandPtr build();

    /*
     * Указать тип команды, которую надо создать
     * Функция возвращает ссылку на *this, чтобы можно было объединять вызовы методов билдера в цепочку (fluent interface)
     */
    CommandBuilder& WithType(Type type);

    /*
     * Указать размер выделения. Нужно для создания команды SelectText
     * Функция возвращает ссылку на *this, чтобы можно было объединять вызовы методов билдера в цепочку (fluent interface)
     */
    CommandBuilder& SelectionSize(size_t selectionSize);

    /*
     * Указать текст для ввода. Нужно для создания команды InsertText
     * Функция возвращает ссылку на *this, чтобы можно было объединять вызовы методов билдера в цепочку (fluent interface)
     */
    CommandBuilder& Text(std::string text);

    /*
     * Включить логирование команды. Используется для оборачивания объекта команды в логирующий декоратор LoggedCommandWrapper
     * Функция возвращает ссылку на *this, чтобы можно было объединять вызовы методов билдера в цепочку (fluent interface)
     */
    CommandBuilder& LogTo(std::ostream& logStream);

    /*
     * Добавить команду в макрос. Используется для создания команды MacroCommand
     * Функция возвращает ссылку на *this, чтобы можно было объединять вызовы методов билдера в цепочку (fluent interface)
     */
    CommandBuilder& AddSubcommand(CommandPtr subcommand);
private:
    Type type_;
    size_t selectionSize_;
    std::string text_;
    std::ostream* logStreamPtr_;
    std::list<CommandPtr> subcommands_;
};

/*
 * Базовый класс для visitor'ов
 * Паттерн visitor позволяет добавлять функциональность для работы с разными командами без необходимости модифицировать код самих команд
 */
class CommandVisitor {
public:
    virtual ~CommandVisitor() = default;

    virtual void VisitMoveCursorLeftCommand(MoveCursorLeftCommand& command) = 0;
    virtual void VisitMoveCursorRightCommand(MoveCursorRightCommand& command) = 0;
    virtual void VisitMoveCursorUpCommand(MoveCursorUpCommand& command) = 0;
    virtual void VisitMoveCursorDownCommand(MoveCursorDownCommand& command) = 0;
    virtual void VisitSelectCommand(SelectTextCommand& command) = 0;
    virtual void VisitInsertTextCommand(InsertTextCommand& command) = 0;
    virtual void VisitDeleteTextCommand(DeleteTextCommand& command) = 0;
    virtual void VisitCopyTextCommand(CopyTextCommand& command) = 0;
    virtual void VisitPasteTextCommand(PasteTextCommand& command) = 0;
    virtual void VisitUppercaseTextCommand(UppercaseTextCommand& command) = 0;
    virtual void VisitLowercaseTextCommand(LowercaseTextCommand& command) = 0;
    virtual void VisitMoveToEndCommand(MoveToEndCommand& command) = 0;
    virtual void VisitMoveToStartCommand(MoveToStartCommand& command) = 0;
    virtual void VisitDeleteWordCommand(DeleteWordCommand& command) = 0;
};

/*
 * Конкретный visitor, реализующий запись лога для каждого типа команд в task.cpp
 */
class CommandLoggerVisitor : public CommandVisitor {
public:
    explicit CommandLoggerVisitor(std::ostream& logStream);

    void VisitMoveCursorLeftCommand(MoveCursorLeftCommand& command) final;
    void VisitMoveCursorRightCommand(MoveCursorRightCommand& command) final;
    void VisitMoveCursorUpCommand(MoveCursorUpCommand& command) final;
    void VisitMoveCursorDownCommand(MoveCursorDownCommand& command) final;
    void VisitSelectCommand(SelectTextCommand& command) final;
    void VisitInsertTextCommand(InsertTextCommand& command) final;
    void VisitDeleteTextCommand(DeleteTextCommand& command) final;
    void VisitCopyTextCommand(CopyTextCommand& command) final;
    void VisitPasteTextCommand(PasteTextCommand& command) final;
    void VisitUppercaseTextCommand(UppercaseTextCommand& command) final;
    void VisitLowercaseTextCommand(LowercaseTextCommand& command) final;
    void VisitMoveToEndCommand(MoveToEndCommand& command) final;
    void VisitMoveToStartCommand(MoveToStartCommand& command) final;
    void VisitDeleteWordCommand(DeleteWordCommand& command) final;
private:
    std::ostream& logStream_;
};
