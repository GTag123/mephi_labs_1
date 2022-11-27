#include "task.h"
#include <cassert>
#include <sstream>

void ApplyMultiple(TextEditor& editor, const CommandPtr& command, size_t times) {
    for (size_t i = 0; i < times; ++i) {
        editor.ApplyCommand(command);
    }
}

// shortcuts
using CommandBuilder::Type::MoveCursorLeft;
using CommandBuilder::Type::MoveCursorRight;
using CommandBuilder::Type::MoveCursorUp;
using CommandBuilder::Type::MoveCursorDown;
using CommandBuilder::Type::SelectText;
using CommandBuilder::Type::InsertText;
using CommandBuilder::Type::DeleteText;
using CommandBuilder::Type::CopyText;
using CommandBuilder::Type::PasteText;
using CommandBuilder::Type::UppercaseText;
using CommandBuilder::Type::LowercaseText;
using CommandBuilder::Type::MoveToEnd;
using CommandBuilder::Type::MoveToStart;
using CommandBuilder::Type::DeleteWord;
using CommandBuilder::Type::Macro;

CommandBuilder CB(CommandBuilder::Type t) {
    return CommandBuilder().WithType(t);
}

void TestOneLine() {
    TextEditor editor;
    assert(editor.GetText().empty());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(InsertText).Text("Hello world").build());
    assert(editor.GetText() == "Hello world");
    assert(editor.GetCursorPosition() == 11);

    ApplyMultiple(editor, CB(MoveCursorLeft).build(), 6);
    assert(editor.GetCursorPosition() == 5);
    assert(editor.GetCharUnderCursor() == ' ');

    editor.ApplyCommand(CB(InsertText).Text(",").build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 6);
    assert(editor.GetCharUnderCursor() == ' ');

    editor.ApplyCommand(CB(MoveToEnd).build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(InsertText).Text("!").build());
    assert(editor.GetText() == "Hello, world!");
    assert(editor.GetCursorPosition() == 13);
    assert(editor.GetCharUnderCursor() == 0);
}

void TestCopyPaste() {
    TextEditor editor;

    editor.ApplyCommand(CB(InsertText).Text("Vasya was here\n").build());
    assert(editor.GetText() == "Vasya was here\n");
    assert(editor.GetCursorPosition() == 15);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(MoveCursorUp).build());
    editor.ApplyCommand(CB(SelectText).SelectionSize(15).build());
    assert(editor.GetSelection() == std::make_pair((size_t)0, (size_t)15));
    assert(editor.GetCharUnderCursor() == 'V');

    editor.ApplyCommand(CB(CopyText).build());
    editor.ApplyCommand(CB(SelectText).SelectionSize(15).build());
    assert(editor.GetClipboard() == editor.GetText());

    editor.ApplyCommand(CB(PasteText).build());  // paste in place of selection
    editor.ApplyCommand(CB(MoveCursorUp).build());
    assert(editor.GetClipboard() == editor.GetText());
    assert(editor.GetCharUnderCursor() == 'V');
    assert(editor.GetCursorPosition() == 0);
    assert(!editor.HasSelection());

    editor.ApplyCommand(CB(PasteText).build());  // paste on position 0
    assert(editor.GetText() == "Vasya was here\nVasya was here\n");
    assert(editor.GetCharUnderCursor() == 'V');
    assert(editor.GetCursorPosition() == 15);
    assert(!editor.HasSelection());

    editor.ApplyCommand(CB(MoveToEnd).build());
    editor.ApplyCommand(CB(InsertText).Text("\nIvan is cool").build());
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == editor.GetText().size() - 1);
    assert(editor.GetCharUnderCursor() == '\n');

    editor.ApplyCommand(CB(MoveCursorUp).build());
    assert(editor.GetCursorPosition() == 27);
    assert(editor.GetCharUnderCursor() == 'r');

    ApplyMultiple(editor, CB(MoveCursorUp).build(), 8);
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 'r');

    editor.ApplyCommand(CB(MoveToStart).build());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 'V');

    CommandPtr macroCommand = CommandBuilder()
            .WithType(Macro)
            .AddSubcommand(CB(DeleteWord).build())
            .AddSubcommand(CB(InsertText).Text("Fedor").build())
            .AddSubcommand(CB(MoveCursorDown).build())
            .AddSubcommand(CB(MoveToStart).build())
            .build();

    ApplyMultiple(editor, macroCommand, 3);
    assert(editor.GetText() == "Fedor was here\nFedor was here\nFedor is cool\n");
}

void TestLogging() {
    TextEditor editor;
    std::stringstream logStream;

    editor.ApplyCommand(CB(InsertText).LogTo(logStream).Text("Quick brown fox jumps\nover the lazy dog").build());
    editor.ApplyCommand(CB(MoveCursorUp).LogTo(logStream).build());
    editor.ApplyCommand(CB(MoveToStart).LogTo(logStream).build());
    editor.ApplyCommand(CB(MoveCursorDown).LogTo(logStream).build());
    editor.ApplyCommand(CB(MoveToEnd).LogTo(logStream).build());
    editor.ApplyCommand(CB(MoveToStart).LogTo(logStream).build());
    editor.ApplyCommand(CB(SelectText).LogTo(logStream).SelectionSize(4).build());
    editor.ApplyCommand(CB(CopyText).LogTo(logStream).build());
    editor.ApplyCommand(CB(DeleteWord).LogTo(logStream).build());
    editor.ApplyCommand(CB(PasteText).LogTo(logStream).build());

    assert(editor.GetText() == "Quick brown fox jumps\nover the lazy dog");

    ApplyMultiple(editor, CB(MoveCursorLeft).LogTo(logStream).build(), 26);
    editor.ApplyCommand(CB(SelectText).LogTo(logStream).SelectionSize(5).build());
    editor.ApplyCommand(CB(UppercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "QUICK brown fox jumps\nover the lazy dog");

    editor.ApplyCommand(CB(SelectText).LogTo(logStream).SelectionSize(5).build());
    editor.ApplyCommand(CB(LowercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "quick brown fox jumps\nover the lazy dog");

    CommandPtr macroCommand = CommandBuilder()
            .WithType(Macro)
            .AddSubcommand(CB(DeleteWord).build())
            .AddSubcommand(CB(InsertText).Text("Fedor").build())
            .AddSubcommand(CB(MoveCursorDown).build())
            .AddSubcommand(CB(MoveToStart).build())
            .LogTo(logStream)
            .build();

    ApplyMultiple(editor, macroCommand, 2);
    assert(editor.GetText() == "Fedor brown fox jumps\nFedor the lazy dog");

    assert(logStream.str() == "ik0j$0vydEphhhhhhhhhhhhhhhhhhhhhhhhhhvUvudEij0dEij0");
}

int main () {
    TestOneLine();
    TestCopyPaste();
    TestLogging();
    return 0;
}
