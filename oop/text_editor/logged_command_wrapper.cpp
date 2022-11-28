#include "logged_command_wrapper.h"

LoggedCommandWrapper::LoggedCommandWrapper(std::ostream& logStream, CommandPtr command): command_(command), logStream_(logStream) {}

void LoggedCommandWrapper::Apply(std::string& buffer, size_t& cursorPosition, std::string& clipboard, TextEditor& editor) {
    CommandLoggerVisitor visitor(logStream_);
    command_->AcceptVisitor(visitor);
    command_->Apply(buffer, cursorPosition, clipboard, editor);
}

void LoggedCommandWrapper::AcceptVisitor(CommandVisitor &visitor) {
    command_->AcceptVisitor(visitor);
}
