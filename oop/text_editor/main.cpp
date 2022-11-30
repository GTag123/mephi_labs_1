#include "task.h"
#include <cassert>
#include <sstream>
#include "iostream"
using namespace std;


void ApplyMultiple(TextEditor& editor, const CommandPtr& command, size_t times) {
    for (size_t i = 0; i < times; ++i) {
        editor.ApplyCommand(command);
    }
}

CommandBuilder CB(CommandBuilder::Type t) {
    return CommandBuilder().WithType(t);
}

void TestOneLine() {
    cout << "Debug TestOneLine" << endl << endl;
    TextEditor editor;
    assert(editor.GetText().empty());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 0);
    cout << "tut" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("Hello world").build());
    cout << "tut" << endl;
    assert(editor.GetText() == "Hello world");
    assert(editor.GetCursorPosition() == 11);
    cout << "tut" << endl;
    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorLeft).build(), 6);
    assert(editor.GetCursorPosition() == 5);
    assert(editor.GetCharUnderCursor() == ' ');
    cout << "tut2" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text(",").build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 6);
    assert(editor.GetCharUnderCursor() == ' ');
    cout << "tut3" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToEnd).build());
    assert(editor.GetText() == "Hello, world");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 0);

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("!").build());
    assert(editor.GetText() == "Hello, world!");
    assert(editor.GetCursorPosition() == 13);
    assert(editor.GetCharUnderCursor() == 0);
    cout << endl << endl << endl;
}

void TestCopyPaste() {
    cout << "Debug TestCopyPaste" << endl << endl;
    TextEditor editor;

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).Text("Vasya was here\n").build());
    assert(editor.GetText() == "Vasya was here\n");
    assert(editor.GetCursorPosition() == 15);
    assert(editor.GetCharUnderCursor() == 0);
    cout << "tut" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).SelectionSize(15).build());
    assert(editor.GetSelection() == std::make_pair((size_t)0, (size_t)15));
    assert(editor.GetCharUnderCursor() == 'V');
    cout << "tut2 64" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::CopyText).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).SelectionSize(15).build());
    assert(editor.GetClipboard() == editor.GetText());
    cout << "tut3 68" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::PasteText).build());  // paste in place of selection
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    cout << "main.cpp 71: getclipboard :" << editor.GetClipboard() << ":" << endl;
    cout << "main.cpp 72: buffer :" << editor.GetText() << ":" << endl;
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
    cout << "tut 86, GetText() :" << editor.GetText() << ":" << endl;
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == editor.GetText().size() - 1);
    assert(editor.GetCharUnderCursor() == '\n');
    cout << "main.cpp 90: cur pos: " << editor.GetCursorPosition() << " char: " << editor.GetCharUnderCursor() << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).build());
    cout << "main.cpp 92: buffer\n:" << editor.GetText() << ":" << endl;
    cout << "main.cpp 93: cur pos: " << editor.GetCursorPosition() << " char: " << editor.GetCharUnderCursor() << endl;

    assert(editor.GetCursorPosition() == 27);
    assert(editor.GetCharUnderCursor() == 'r');

    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorUp).build(), 8);
    assert(editor.GetText() == "Vasya was here\nVasya was here\nIvan is cool\n");
    assert(editor.GetCursorPosition() == 12);
    assert(editor.GetCharUnderCursor() == 'r');
    cout << "tut 102!!!!" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).build());
    assert(editor.GetCursorPosition() == 0);
    assert(editor.GetCharUnderCursor() == 'V');
    cout << "tut 106" << endl;
    cout << "--------------------------MacroComands---------------------------" << endl;
    cout << "main.cpp 107 GetText()\n:" << editor.GetText() << ":\nCursor pos: " << editor.GetCursorPosition() << endl;
    // в реализации apply в классе макрокоманд нужно пройтись циклом и применить к каждому apply
    CommandPtr macroCommand = CommandBuilder()
            .WithType(CommandBuilder::Type::Macro)
            .AddSubcommand(CB(CommandBuilder::Type::DeleteWord).build())
            .AddSubcommand(CB(CommandBuilder::Type::InsertText).Text("Fedor").build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveCursorDown).build())
            .AddSubcommand(CB(CommandBuilder::Type::MoveToStart).build())
            .build();

    ApplyMultiple(editor, macroCommand, 3);
    cout << "main.cpp 118 GetText()\n:" << editor.GetText() << ":\nCursor pos: " << editor.GetCursorPosition() << endl;
    assert(editor.GetText() == "Fedor was here\nFedor was here\nFedor is cool\n");
    cout << endl << endl << endl << endl;
}

void TestLogging() {
    cout << "Debug TestLogging" << endl << endl;
    TextEditor editor;
    std::stringstream logStream;

    editor.ApplyCommand(CB(CommandBuilder::Type::InsertText).LogTo(logStream).Text("Quick brown fox jumps\nover the lazy dog").build());
    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorUp).LogTo(logStream).build());
    cout << endl << "main.cpp 131 after MoveCursorUp, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).LogTo(logStream).build());
    cout << endl << "main.cpp 133 after MoveToStart, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::MoveCursorDown).LogTo(logStream).build());
    cout << endl << "main.cpp 135 after MoveCursorDown, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToEnd).LogTo(logStream).build());
    cout << endl << "main.cpp 137 after MoveToEnd, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::MoveToStart).LogTo(logStream).build());
    cout << endl << "main.cpp 139 after MoveToStart, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(4).build());
    cout << endl << "main.cpp 141 after SelectText, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::CopyText).LogTo(logStream).build());
    cout << endl << "main.cpp 143 after CopyText, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::DeleteWord).LogTo(logStream).build());
    cout << endl << "main.cpp 145 after DeleteWord, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    editor.ApplyCommand(CB(CommandBuilder::Type::PasteText).LogTo(logStream).build());
    cout << endl << "main.cpp 147 after PasteText, buffer\n:" << editor.GetText() << ":\ncursor pos: " << editor.GetCursorPosition() << endl << endl;


    cout << "main.cpp 140 GetText()\n:" << editor.GetText() << endl;
    assert(editor.GetText() == "Quick brown fox jumps\nover the lazy dog");


    ApplyMultiple(editor, CB(CommandBuilder::Type::MoveCursorLeft).LogTo(logStream).build(), 26);
    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(5).build());

    cout << "main.cpp 173 tut" << endl;
    editor.ApplyCommand(CB(CommandBuilder::Type::UppercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "QUICK brown fox jumps\nover the lazy dog");

    editor.ApplyCommand(CB(CommandBuilder::Type::SelectText).LogTo(logStream).SelectionSize(5).build());
    editor.ApplyCommand(CB(CommandBuilder::Type::LowercaseText).LogTo(logStream).build());
    assert(editor.GetText() == "quick brown fox jumps\nover the lazy dog");
    cout << "main.cpp 180 tut" << endl;
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
//    cout << "ПМИ СОСАТЬЬЬЬЬ!!!!!!!!!!!!!" << endl;
    cout << logStream.str() << endl;


    assert(logStream.str() == "ik0j$0vydEphhhhhhhhhhhhhhhhhhhhhhhhhhvUvudEij0dEij0");
}

int main () {
    TestOneLine();
    TestCopyPaste();
    TestLogging();
    return 0;
}
