#pragma once
#include "task.h"

/*
 * класс-обертка над командами, который добавляет вызов логирования
 * обертка (wrapper, decorator) позволяет динамически добавлять дополнительную функциональность к объектам, сохраняя интерфейс
 */
class LoggedCommandWrapper : public ICommand {
public:
    LoggedCommandWrapper(std::ostream& logStream, CommandPtr command);
    void Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) final;
    void AcceptVisitor(CommandVisitor& visitor) final;
private:
    CommandPtr command_;
    std::ostream& logStream_;
};
