# Compiler Design Laboratory — Internal Exam Programs

Programs for the listed Compiler Design Lab questions. This repository is designed for **Windows 11 + VS Code + MSYS2 UCRT64** and also works in a suitable Linux/WSL environment.

## Contents

| Folder | Question | Program |
|---|---|---|
| `week1-lexical-analyzer-c` | Week 1, Q1 | Lexical analyzer written manually in C |
| `week2-flex-lexer` | Week 2, Q2 | Lexical analyzer generated using Flex |
| `week3-first-follow` | Week 3, Q3 | Compute FIRST and FOLLOW sets |
| `week6-ast-yacc` | Week 6, Q6 | Generate an Abstract Syntax Tree using Flex + YACC/Bison |
| `week6-validate-expression` | Week 6, Q7 | Validate arithmetic expressions using Flex + YACC/Bison |
| `week7-arithmetic-operations` | Week 7, Q8 | Evaluate arithmetic expressions using Flex + YACC/Bison |

> The question numbering follows the list provided for this lab. The heading says Week 7 for Q8, even though it is sometimes referred to as Week 8 in notes.

---

## 1. Install the tools on Windows

### Install MSYS2
Install MSYS2 from the official site: https://www.msys2.org/

Open **MSYS2 UCRT64** from the Windows Start menu and run:

```bash
pacman -Syu
```

If the terminal asks you to close it, close and reopen **MSYS2 UCRT64**, then run the update command again if required.

Install the compiler and lexer/parser tools:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc flex bison
```

Check that the tools are available:

```bash
gcc --version
flex --version
bison --version
```

Run the commands below from the **MSYS2 UCRT64 terminal**, not a regular PowerShell terminal, unless you have separately configured PATH.

## 2. Clone this repository

After you upload this folder to your GitHub account, open MSYS2 UCRT64 and run:

```bash
git clone https://github.com/YOUR-USERNAME/compiler-design-lab.git
cd compiler-design-lab
```

Replace `YOUR-USERNAME` with your GitHub username. If Git is not installed in MSYS2, install it with:

```bash
pacman -S --needed git
```

You can also download the repository as a ZIP from GitHub and extract it.

---

# Program instructions

## Week 1 — Q1: Lexical analyzer in C

**File:** `week1-lexical-analyzer-c/lexer.c`

```bash
cd week1-lexical-analyzer-c
gcc lexer.c -o lexer.exe
./lexer.exe
```

Enter a statement, for example:

```c
int total = 25; // sample comment
```

The program identifies keywords, identifiers, numbers, operators, and special symbols. It skips spaces/tabs/newlines and both `//` and `/* ... */` comments.

This is an educational lexer for a small subset of C-like syntax, not a complete C compiler lexer.

## Week 2 — Q2: Flex lexical analyzer

**Files:** `week2-flex-lexer/lexer.l`

From the repository root:

```bash
cd week2-flex-lexer
flex -o lex.yy.c lexer.l
gcc lex.yy.c -o lexer.exe
./lexer.exe
```

Type the input. To finish input in the MSYS2 terminal on Windows, press **Ctrl+Z**, then **Enter**. On Linux/WSL, press **Ctrl+D**.

The rules classify keywords, identifiers, integer/decimal numbers, operators, and special symbols; whitespace and comments are ignored.

## Week 3 — Q3: FIRST and FOLLOW

**File:** `week3-first-follow/first_follow.c`

```bash
cd week3-first-follow
gcc first_follow.c -o first_follow.exe
./first_follow.exe
```

Enter the number of productions, then enter one production per line. Use `#` for epsilon. You may write productions with `=` or `->`; each alternative should be a separate production.

Example:

```text
Enter number of productions: 5
E=TR
R=+TR
R=#
T=FY
Y=*FY
```

That example is intentionally incomplete because it does not include productions for `F` and the full grammar; use the complete example below instead:

```text
Enter number of productions: 8
E=TR
R=+TR
R=#
T=FY
Y=*FY
Y=#
F=(E)
F=i
```

The program assumes **single-character grammar symbols**. Uppercase symbols that occur on a left-hand side are treated as non-terminals; other symbols are terminals. Enter alternatives as separate productions.

## Week 6 — Q6: AST using Flex + YACC/Bison

**Files:** `week6-ast-yacc/ast.l`, `week6-ast-yacc/ast.y`

Run these commands in the MSYS2 UCRT64 terminal:

```bash
cd week6-ast-yacc
bison -d -o ast.tab.c ast.y
flex -o lex.yy.c ast.l
gcc ast.tab.c lex.yy.c -o ast.exe
./ast.exe
```

Enter an expression such as `a+b*c`. To finish input, press **Ctrl+Z**, then **Enter** in MSYS2 on Windows (or **Ctrl+D** on Linux/WSL).

The program prints the AST in a readable tree form. Supported operators: `+`, `-`, `*`, `/`, parentheses, identifiers, and integers.

## Week 6 — Q7: Validate arithmetic expressions using YACC

**Files:** `week6-validate-expression/valid.l`, `week6-validate-expression/valid.y`

```bash
cd week6-validate-expression
bison -d -o valid.tab.c valid.y
flex -o lex.yy.c valid.l
gcc valid.tab.c lex.yy.c -o valid.exe
./valid.exe
```

Examples:
- Valid: `a+b*c`
- Valid: `(12+3)*x`
- Invalid: `a+*b`
- Invalid: `(a+b`

Finish input using **Ctrl+Z**, then **Enter** on Windows MSYS2 or **Ctrl+D** on Linux/WSL.

## Week 7 — Q8: Arithmetic operations using YACC

**Files:** `week7-arithmetic-operations/calc.l`, `week7-arithmetic-operations/calc.y`

```bash
cd week7-arithmetic-operations
bison -d -o calc.tab.c calc.y
flex -o lex.yy.c calc.l
gcc calc.tab.c lex.yy.c -o calc.exe
./calc.exe
```

Examples:
- `10+20` → `Result = 30`
- `2+3*4` → `Result = 14`
- `(2+3)*4` → `Result = 20`
- `20/5` → `Result = 4`

The calculator uses integer arithmetic, so `7/2` returns `3`. It checks division by zero. Finish input with **Ctrl+Z**, then **Enter** on Windows MSYS2 or **Ctrl+D** on Linux/WSL.

---

## Upload this project to GitHub

1. Sign in to GitHub and create a **new empty repository** named `compiler-design-lab`.
2. Do not initialize it with a README if you are going to push this folder's existing README.
3. Open MSYS2 UCRT64 in this project folder. If needed, navigate to it using `cd`.
4. Run:

```bash
git init
git add .
git commit -m "Add Compiler Design Lab programs"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/compiler-design-lab.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username. If Git asks you to configure your identity, run:

```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
```

Then repeat the commit and push commands. GitHub may ask you to authenticate in a browser.

## Important notes

- Generated files such as `lex.yy.c`, `*.tab.c`, and `*.tab.h` are build outputs and are not required to be committed; `.gitignore` excludes them.
- For Flex/Bison programs, keep each `.l` and `.y` pair in the same folder.
- These are lab-practice programs using a deliberately small grammar, not production-grade compiler tools.
