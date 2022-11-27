#include "text_editor.h"
#include "task.h"

void TextEditor::ApplyCommand(CommandPtr command) {
    command->Apply(buffer_, cursorPosition_, clipboard_, *this);
}

void TextEditor::SelectText(size_t selectionStart, size_t selectionEnd) {
    if (selectionEnd < selectionStart) {
        std::swap(selectionEnd, selectionStart);
    }
    if (selectionEnd == selectionStart) {
        return;
    }
    if (selectionEnd > buffer_.size() || selectionStart > buffer_.size()) {
        return;
    }
    cursorPosition_ = selectionStart;
    selectionEnd_ = selectionEnd;
}

void TextEditor::UnselectText() {
    selectionEnd_ = std::string::npos;
}

std::pair<size_t, size_t> TextEditor::GetSelection() const {
    return {cursorPosition_, selectionEnd_};
}

bool TextEditor::HasSelection() const {
    return selectionEnd_ != std::string::npos;
}

const std::string& TextEditor::GetText() const {
    return buffer_;
}

size_t TextEditor::GetCursorPosition() const {
    return cursorPosition_;
}

std::string::value_type TextEditor::GetCharUnderCursor() const {
    if (cursorPosition_ < buffer_.size()) {
        return buffer_[cursorPosition_];
    }
    return 0;
}

const std::string& TextEditor::GetClipboard() const {
    return clipboard_;
}
