#include "task.h"
#include "logged_command_wrapper.h"
#include "iostream"
using namespace std;

CommandBuilder::CommandBuilder() {}

CommandBuilder &CommandBuilder::Text(std::string text) {
    this->text_ = text;
    return *this;
}

CommandBuilder& CommandBuilder::WithType(CommandBuilder::Type type) {
    this->type_ = type;
    return *this;
}

CommandBuilder& CommandBuilder::SelectionSize(size_t selectionSize) {
    this->selectionSize_ = selectionSize;
    return *this;
}

CommandBuilder& CommandBuilder::LogTo(std::ostream &logStream) {
    this->logStreamPtr_ = &logStream;
    return *this;
}

CommandBuilder& CommandBuilder::AddSubcommand(CommandPtr subcommand) {
    this->subcommands_.push_back(subcommand);
    return *this;
}





CommandLoggerVisitor::CommandLoggerVisitor(std::ostream &logStream): logStream_(logStream) {
}

void CommandLoggerVisitor::VisitMoveCursorLeftCommand(MoveCursorLeftCommand &command) {
    this->logStream_ << "h";
}

void CommandLoggerVisitor::VisitMoveCursorRightCommand(MoveCursorRightCommand &command) {
    this->logStream_ << "l";
}

void CommandLoggerVisitor::VisitMoveCursorUpCommand(MoveCursorUpCommand& command) {
    this->logStream_ << "k";
}

void CommandLoggerVisitor::VisitMoveCursorDownCommand(MoveCursorDownCommand& command) {
    this->logStream_ << "j";
}

void CommandLoggerVisitor::VisitSelectCommand(SelectTextCommand& command) {
    this->logStream_ << "v";
}

void CommandLoggerVisitor::VisitInsertTextCommand(InsertTextCommand& command) {
    this->logStream_ << "o";
}

void CommandLoggerVisitor::VisitDeleteTextCommand(DeleteTextCommand& command) {
    this->logStream_ << "d";
}

void CommandLoggerVisitor::VisitCopyTextCommand(CopyTextCommand& command) {
    this->logStream_ << "y";
}

void CommandLoggerVisitor::VisitPasteTextCommand(PasteTextCommand& command) {
    this->logStream_ << "p";
}

void CommandLoggerVisitor::VisitUppercaseTextCommand(UppercaseTextCommand& command) {
    this->logStream_ << "U";
}

void CommandLoggerVisitor::VisitLowercaseTextCommand(LowercaseTextCommand& command) {
    this->logStream_ << "u";
}

void CommandLoggerVisitor::VisitMoveToEndCommand(MoveToEndCommand& command) {
    this->logStream_ << "$";
}

void CommandLoggerVisitor::VisitMoveToStartCommand(MoveToStartCommand& command) {
    this->logStream_ << "0";
}

void CommandLoggerVisitor::VisitDeleteWordCommand(DeleteWordCommand& command) {
    this->logStream_ << "dE";
}




/* Курсор влево */
class MoveCursorLeftCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cursorPosition--;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorLeftCommand(*this);
    }
};

/* Курсор вправо */
class MoveCursorRightCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cursorPosition++;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorRightCommand(*this);
    }
};

/* Курсор вверх */
class MoveCursorUpCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << buffer << " " << cursorPosition << endl;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorUpCommand(*this);
    }
};

/* Курсор вниз */
class MoveCursorDownCommand : public ICommand {


    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorDownCommand(*this);
    }
};

/* Выделить текст */
class SelectTextCommand : public ICommand {


    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitSelectCommand(*this);
    }
};

/* Ввести текст */
class InsertTextCommand : public ICommand {
private:
    std::string texttoinsert_;
public:
    explicit InsertTextCommand(std::string texttoinsert): texttoinsert_(texttoinsert){}
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply InsertTextCommand" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        buffer = buffer.substr(0, cursorPosition) + texttoinsert_ + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
        cout << "buffer during: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        cursorPosition += texttoinsert_.size();
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        // todo: дописать когда есть выделение текста, выделение текста как я понимаю храниться в эдиторе
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitInsertTextCommand(*this);
    }
};

/* Удалить текст */
class DeleteTextCommand : public ICommand {


    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitDeleteTextCommand(*this);
    }
};

/* Скопировать текст */
class CopyTextCommand : public ICommand {

    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitCopyTextCommand(*this);
    }
};

/* Вставить скопированный текст */
class PasteTextCommand : public ICommand {
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitPasteTextCommand(*this);
    }
};

/* Привести выделенный текст в ВЕРХНИЙ регистр */
class UppercaseTextCommand : public ICommand {
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitUppercaseTextCommand(*this);
    }
};

/* Привести выделенный текст в нижний регистр */
class LowercaseTextCommand : public ICommand {

    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitLowercaseTextCommand(*this);
    }
};

/* Перенести курсор в конец строки */
class MoveToEndCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        for (int i = (int) cursorPosition; i < (int) buffer.size() && buffer[i-1] != '\n'; ++i) {
            if (buffer[i] == '\n') cout << "new line" << endl;
            cursorPosition++;
        }
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveToEndCommand(*this);
    }
};

/* Перенести курсор в начало строки */
class MoveToStartCommand : public ICommand {
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveToStartCommand(*this);
    }
};

/* Удалить часть строки, начиная от позиции курсора до первого пробела или конца строки */
class DeleteWordCommand : public ICommand {
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitDeleteWordCommand(*this);
    }
};

/* Макрос */
class MacroCommand : public ICommand {
};
CommandPtr CommandBuilder::build() {
    switch (this->type_) {
        case CommandBuilder::Type::MoveCursorLeft:
            return make_shared<MoveCursorLeftCommand>(MoveCursorLeftCommand());
            break;
        case CommandBuilder::Type::InsertText:
            return make_shared<InsertTextCommand>(InsertTextCommand(this->text_));
            break;
        case CommandBuilder::Type::MoveToEnd:
            return make_shared<MoveToEndCommand>(MoveToEndCommand());
            break;
        case Type::MoveCursorRight:
            return make_shared<MoveCursorRightCommand>(MoveCursorRightCommand());
            break;
        case Type::MoveCursorUp:
            break;
        case Type::MoveCursorDown:
            break;
        case Type::SelectText:
            break;
        case Type::DeleteText:
            break;
        case Type::CopyText:
            break;
        case Type::PasteText:
            break;
        case Type::UppercaseText:
            break;
        case Type::LowercaseText:
            break;
        case Type::MoveToStart:
            break;
        case Type::DeleteWord:
            break;
        case Type::Macro:
            break;
    }
    return nullptr;
}