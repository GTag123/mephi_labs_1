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

//asdf asd\nhui
//asdf asd
//hui


/* Курсор вверх */
class MoveCursorUpCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply MoveCursorUpCommand" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        int i = cursorPosition;
        cout << "cursor: " << cursorPosition << "size buff: " << buffer.size() << endl;
        while(i > 0 && buffer[i - 1] != '\n'){
            i--; // i будет указывать на начало строки
        }
        int diff = cursorPosition - i;
        if (i <= 0){
            return;
        }
        cursorPosition = i - 1;
        int j = cursorPosition;
        while(j > 0 && buffer[j - 1] != '\n'){
            j--; // j указывает на начало строки выше
        }
        // сюда if j + diff > i и тд...
        cursorPosition = j + diff;
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl;

        // todo:  сохранять свою позицию относительно начала строки, если это возможно
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorUpCommand(*this);
    }
};

/* Курсор вниз */
class MoveCursorDownCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply MoveCursorDownCommand" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        int i = (int) cursorPosition;
        int j = (int) cursorPosition;
        while(i < ((int) buffer.size()) && buffer[i] != '\n'){
            i++; // i будет указывать на конец строки
        }
        while(j > 0 && buffer[j - 1] != '\n'){
            j--; // i будет указывать на начало строки
        }
        int diff = cursorPosition - j;
        cout << "cursor: " << cursorPosition << " size buff: " << buffer.size() << " DIFF: " << diff << " I: " << i << endl;
        if (i >= (int)buffer.size()){ // мб тут проблема с больше и равно и с сайзом
            return;
        }
        cursorPosition = i + 1 + diff;
//        int j = (int) cursorPosition;
//        while(j < (buffer.size() - 1) && buffer[j + 1] != '\n'){
//            j++; // j указывает на начало строки выше
//        }
        // сюда if j + diff > i и тд...
//        cursorPosition = j + diff;
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl;

        // todo:  сохранять свою позицию относительно начала строки, если это возможно
    }

    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveCursorDownCommand(*this);
    }
};

/* Выделить текст */
class SelectTextCommand : public ICommand {
private:
    int SelectionSize_;
public:
    explicit SelectTextCommand(int size): SelectionSize_(size){};

    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply SelectTextCommand" << endl;
        editor.SelectText(cursorPosition, SelectionSize_);
        cout << "Selection pos: " << editor.GetSelection().first << " " << editor.GetSelection().second << endl;
//        cursorPosition += SelectionSize_; // мб не надо
    }

    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitSelectCommand(*this);
    }
};

/* Ввести текст */
class InsertTextCommand : public ICommand {
private:
    std::string texttoinsert_;
public:
    explicit InsertTextCommand(std::string texttoinsert): texttoinsert_(std::move(texttoinsert)){}
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply InsertTextCommand" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;

        if (editor.HasSelection()){
            buffer = buffer.substr(0, editor.GetSelection().first) + texttoinsert_ +
                     buffer.substr(editor.GetSelection().second, buffer.size() - editor.GetSelection().second);
            cout << "buffer during: " << buffer << ":   cursor pos: " << cursorPosition << endl;
            cursorPosition = editor.GetSelection().first + texttoinsert_.size();
            editor.UnselectText();

        } else {
            buffer = buffer.substr(0, cursorPosition) + texttoinsert_ + buffer.substr(cursorPosition, buffer.size() - cursorPosition);
            cout << "buffer during: " << buffer << ":   cursor pos: " << cursorPosition << endl;
            cursorPosition += texttoinsert_.size();
        }
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        cout << endl;
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
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply CopyTextCommand" << endl;
        if(editor.HasSelection()){
            clipboard = buffer.substr(editor.GetSelection().first,
                                      editor.GetSelection().second - editor.GetSelection().first);
            cout << "Clipboard :" << clipboard <<": First pos: " <<
                 editor.GetSelection().first << " Second pos:" << editor.GetSelection().second << endl;
            editor.UnselectText();
        } else {
            clipboard = buffer[cursorPosition - 1];
            cout << "One selection, clipborad: " << clipboard << endl;
        }
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitCopyTextCommand(*this);
    }
};

/* Вставить скопированный текст */
class PasteTextCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply PasteTextCommand" << endl;
        editor.ApplyCommand(CommandBuilder().WithType(
                CommandBuilder::Type::InsertText).Text(clipboard).build());
        cout << "End PasteTextCommand" << endl << endl;
//        clipboard = "";
        if (editor.HasSelection()) editor.UnselectText();

    }
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
        cout << "Apply MoveToEnd" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        while (buffer[cursorPosition] != '\n' && cursorPosition < buffer.size()){
            cursorPosition++;
        }
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl << endl;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveToEndCommand(*this);
    }
};

/* Перенести курсор в начало строки */
class MoveToStartCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply MoveToStart" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        while (buffer[cursorPosition - 1] != '\n' && cursorPosition > 0){
            cursorPosition--;
        }
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl << endl;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitMoveToStartCommand(*this);
    }
};

/* Удалить часть строки, начиная от позиции курсора до первого пробела или конца строки */
class DeleteWordCommand : public ICommand {
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        cout << "Apply DeleteWordCommand" << endl;
        cout << "buffer before: " << buffer << ":   cursor pos: " << cursorPosition << endl;
        int i = (int) cursorPosition;
        while (buffer[i] != ' ' && buffer[i] != '\n' && i < (int) buffer.size() ){
            i++;
        }
        buffer = buffer.substr(0, cursorPosition) + buffer.substr(i, buffer.size() - i);
        cout << "buffer after: " << buffer << ":   cursor pos: " << cursorPosition << endl << endl;
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        visitor.VisitDeleteWordCommand(*this);
    }
};

/* Макрос */
class MacroCommand : public ICommand {
private:
    std::list<CommandPtr> commands_;
public:
    MacroCommand(std::list<CommandPtr> cmds): commands_(std::move(cmds)){}
    void Apply(std::string &buffer, size_t &cursorPosition, std::string &clipboard, TextEditor &editor) override{
        for (auto command: commands_) {
            editor.ApplyCommand(command);
            cout << endl;
            cout << "=====================Debug start=====================" << endl;
            cout << "Apply command, buffer\n:" << editor.GetText() << ":\ncursor: " << cursorPosition << endl;
            cout << "=====================Debug end=====================" << endl;
            cout << endl;
        }
    }
    void AcceptVisitor(CommandVisitor &visitor) override{
        for (auto cmd: commands_) {
            cmd->AcceptVisitor(visitor);
        }
    }

};
CommandPtr CommandBuilder::build() {
    // todo: сбросить селект во всех командах
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
            return make_shared<MoveCursorUpCommand>(MoveCursorUpCommand());
            break;
        case Type::MoveCursorDown:
            return make_shared<MoveCursorDownCommand>(MoveCursorDownCommand());
            break;
        case Type::SelectText:
            return make_shared<SelectTextCommand>(SelectTextCommand((int)this->selectionSize_));
            break;
        case Type::DeleteText:
            break;
        case Type::CopyText:
            return make_shared<CopyTextCommand>(CopyTextCommand());
            break;
        case Type::PasteText:
            return make_shared<PasteTextCommand>(PasteTextCommand());
            break;
        case Type::UppercaseText:
            break;
        case Type::LowercaseText:
            break;
        case Type::MoveToStart:
            return make_shared<MoveToStartCommand>(MoveToStartCommand());
            break;
        case Type::DeleteWord:
            return make_shared<DeleteWordCommand>(DeleteWordCommand());
            break;
        case Type::Macro:
            return make_shared<MacroCommand>(MacroCommand(std::move(this->subcommands_)));
            break;
    }
    return nullptr;
}