#pragma once
#include <memory>
#include <string>

class ICommand;
using CommandPtr = std::shared_ptr<ICommand>;

class TextEditor {
public:
    TextEditor() : cursorPosition_(0), selectionEnd_(std::string::npos) {}

    void ApplyCommand(CommandPtr command);
    void SelectText(size_t selectionStart, size_t selectionEnd);
    void UnselectText();
    std::pair<size_t, size_t> GetSelection() const;
    bool HasSelection() const;
    const std::string& GetText() const;
    size_t GetCursorPosition() const;
    std::string::value_type GetCharUnderCursor() const;
    const std::string& GetClipboard() const;
private:
    std::string buffer_, clipboard_;
    size_t cursorPosition_;
    size_t selectionEnd_;
};
