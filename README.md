# 🍑 nue

The Nue Programming Language

**Nue** is an experimental programming language designed with simplicity and versatility in mind. The goal is to create a modern yet lightweight language that supports a wide range of use cases and doesn't compromise any advanced functionality in favour of simple language design.

<!-- [What does nue look like? (coming soon)](./LEARN.md) -->

## 🚦 Project Status

Nue is currently in development, with a primary focus on building the core components such as the tokeniser and parser to a functional state.

Functionality is currently being prioritised over organisation, which means:

- There is likely to be licence incompatibility during the development phase due to the potential (future) use of various libraries and components

Once Nue is "finished" (i.e. in a basic working state), the repository will be re-organised, and all dependencies will be reviewed for licensing compatibility.

## Testing the Tokeniser/Lexer

Currently, the main entry point of the entire project is the original tokeniser itself, with some extra features.

### As a standalone REPL
```shell
nue
```

Running `nue` without any arguments opens a REPL which allows you to test the functionality of the current tokeniser.

![Screenshot 2025-05-22 at 23 10 39](https://github.com/user-attachments/assets/962a43c8-fc3f-4f95-9a4e-73a3e3789e37)

> [!NOTE]
>
> ```shell
> nue --silent
> ```
>
> Running the `nue` repl with the `--silent` argument removes token usage statistics being printed upon every new line in REPL mode

### Testing with a nue file
This will print all the tokens used in a provided nue file:
```shell
nue file.nue
```

![Screenshot 2025-05-22 at 23 13 43](https://github.com/user-attachments/assets/fa12ef33-b7b1-42dd-9157-be5d9512dd79)

## ⚠️ Disclaimer

This project is in early development and is subject to significant changes. Use or reference the code at your own risk, as it may contain incomplete features, broken C code and licensing conflicts. (don't want to anger the Stallman wannabes, do I?)
