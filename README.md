# 🍑 nue

The nue Programming Language

**Nue** is an experimental programming language designed with simplicity and versatility in mind. The goal is to create a modern yet lightweight language that supports a wide range of use cases. While still in its early stages, Nue aims to be both fun and functional for developers who want to explore a fresh take on programming paradigms.

[What does nue look like? (coming soon)](./LEARN.md)

## 🚦 Project Status
Nue is currently in development, with a primary focus on building the core components such as the lexer and parser to a functional state.

Functionality is currently being prioritised over organisation, which means:
- **License Notice**
    - A license is still being decided for this project
    - **Disclaimer**: There is likely to be license incompatibility during the development phase due to the use of various libraries and components without extensive legal consideration. Once the first Nue program is executed flawlessly:
        - All dependencies will be reviewed for licensing compatibility
        - The repository will be organised to ensure clarity and compliance
- **Lexer Testing**
    - The `tests/lexer.test.c` program currently uses **GNU Readline** to provide line history and inline editing for testing and debugging purposes. This may also raise licensing concerns, which will be addressed during the repository cleanup phase.

## ⭐️ Current Features
- **Lexer**
    - Parses tokens for numbers (including decimals, hex, binary, and octal)
    - Recognises various operators, keywords, and identifiers
    - Supports escape sequences for strings (WORK IN PROGRESS)
- **REPL Testing**
    - A simple REPL to test lexing capabilities interactively, with support for history and editing, powered by GNU Readline.
    - Specify the `--silent` argument when running to silence token usage statistics

## 📝 Contribution Guidelines

Due to the unorganised nature of the project at this stage, contributions are not yet open. However, we welcome **feedback, ideas, and suggestions** in the form of issues or discussions on the repository. Stay tuned for updates on how you can get involved!

Suggestions are also occasionally taken in from the social channels of the creator.

## ⚠️ Disclaimer

This project is in early development and is subject to significant changes. Use or reference the code at your own risk, as it may contain incomplete features, broken C code and licensing conflicts. (dont want to anger Stallman, do I?)