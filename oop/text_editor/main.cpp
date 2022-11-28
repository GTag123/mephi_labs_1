#include "task.h"
#include <cassert>
#include <sstream>

void ApplyMultiple(TextEditor& editor, const CommandPtr& command, size_t times) {
    for (size_t i = 0; i < times; ++i) {
        editor.ApplyCommand(command);
    }
}

CommandBuilder CB(CommandBuilder::Type t) {
    return CommandBuilder().WithType(t);
}

void TestOneLine() {
    TextEditor editor;
    assert(editor.GetText().empty());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("Hello world").build());
    assert(editor.GetText() == "Hello world");
    assert(editor.GetCursorPosition() == 11);

    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorLeft).build(), 6);
    assert(editor.GetCursorPosition() == 5);
    assert(editor.GetCharUnderCursor() == ' ');

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text(",").build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 6);
    assert(editor.GetCharUnderCursor() == ' ');

    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToEnd).build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("!").build());
    assert(editor.GetText() == "Hello, world!");
    assert(editor.GetCursorPosition() == 13);
    assert(editor.GetCharUnderCursor() == 0);
}

void TestCopyPaste() {
    TextEditor editor;

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("Vasya was here\n").build());
    assert(editor.GetText() == "Vasya was here\n");
    assert(editor.GetCursorPosition() == 15);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).SelectionSize(15).build());
    assert(editor.GetSelection() == std::make_pair((size_t)0, (size_t)15));
    assert(editor.GetCharUnderCursor() == 'V');

    editor.ApplyCommand(CB(CommandBuilder::Type::CopyText).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).SelectionSize(15).build());
    assert(editor.GetClipboard() == editor.GetText());

    editor.ApplyCommand(CB(CommandBuilder::Type::PasteText).build());  // paste in place of selection
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    assert(editor.GetClipboard() == editor.GetText());
    assert(editor.GetCharUnderCursor() == 'V');
    assert(editor.GetCursorPosition() == 0);
    assert(!editor.HasSelection());

    editor.ApplyCommand(CB(CommandBuilder::Type::PasteText).build());  // paste on position 0
    assert(editor.GetText() == "Vasya was here\nVasya was here\n");
    assert(editor.GetCharUnderCursor() == 'V');
    assert(editor.GetCursorPosition() == 15);
    assert(!editor.HasSelection());

    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToEnd).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("\nIvan is cool").build());
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == editor.GetText().size() - 1);
    assert(editor.GetCharUnderCursor() == '\n');

    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    assert(editor.GetCursorPosition() == 27);
    assert(editor.GetCharUnderCursor() == 'r');

    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorUp).build(), 8);
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 'r');

    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).build());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 'V');

    CommandPtr macroCommand = CommandBuilder()
            .WithType(CommandBuilder::Type::Macro)
            .AddSubcommand(CB(CommandBuilder::Type::DeleteWord).build())
            .AddSubcommand(CB(CommandBuilder::Type::InsertText).Text("Fedor").build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveCursorDown).build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveToStart).build())
            .build();

    ApplyMultiple(editor, macroCommand, 3);
    assert(editor.GetText() == "Fedor was here\nFedor was here\nFedor is cool\n");
}

void TestLogging() {
    TextEditor editor;
    std::stringstream logStream;

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).LogTo(logStream).Text("Quick brown fox jumps\nover the lazy dog").build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorDown).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToEnd).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(4).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::CopyText).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::DeleteWord).LogTo(logStream).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::PasteText).LogTo(logStream).build());

    assert(editor.GetText() == "Quick brown fox jumps\nover the lazy dog");

    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorLeft).LogTo(logStream).build(), 26);
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(5).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::UppercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "QUICK brown fox jumps\nover the lazy dog");

    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(5).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::LowercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "quick brown fox jumps\nover the lazy dog");

    CommandPtr macroCommand = CommandBuilder()
            .WithType(CommandBuilder::Type::Macro)
            .AddSubcommand(CB(CommandBuilder::Type::DeleteWord).build())
            .AddSubcommand(CB(CommandBuilder::Type::InsertText).Text("Fedor").build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveCursorDown).build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveToStart).build())
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
