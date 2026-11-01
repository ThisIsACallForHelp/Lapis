# Lapis: Language Architecture & Design Summary

**Lapis** is a new general-purpose systems programming language designed to unify the best features of modern languages into a single, cohesive toolset. It combines the raw hardware control of C++ with the memory safety of Rust, layered with the developer ergonomics of languages like C#, Python, and Swift.

---

## 1. Core Architecture (The "Mix")

*   **Memory & Safety (Rust + C):** Strict compile-time safety (Borrow Checker) by default. Variables are immutable by default (`let` vs `let mut`). For raw pointer manipulation and hardware access, developers can opt-out using C-style `unsafe { ... }` blocks.
*   **Concurrency (Go + C# + JS):** Under the hood, Lapis utilizes a non-blocking Event Loop (JS) and lightweight virtual threads (Go's goroutines). Developers control this using ergonomic `async/await` syntax (C#).
*   **Type System (Swift + TS + Kotlin):** 
    *   *Null Safety:* Variables cannot be null unless explicitly marked as Optional (e.g., `let name: string?`).
    *   *Structural Typing:* Interfaces match based on object "shape" (TypeScript), avoiding strict inheritance trees.
    *   *Extensions:* Developers can add methods to external types without inheritance (Kotlin).
*   **Logic & Data (Haskell + C#):** Complex control flow uses Pattern Matching (`match` / `case`), and array manipulation uses built-in LINQ (`users.where(u => u.age > 18)`).
*   **Compiler Pipeline (Java + Dart + Lua):** Compiles to an Intermediate Representation (IR) for portability. Features a JIT compiler for instant hot-reloading during dev, and AOT compilation for optimized native release binaries. Exposes a tiny C-API for easy embedding (Lua).

---

## 2. "Loved by Devs" Killer Features

1.  **Zero-Friction C Interop:** No messy bindings required. Uses `extern` for manual FFI, with future plans to use Clang (`libclang`) to natively import `.h` headers directly into Lapis.
2.  **`comptime` Execution:** Execute standard Lapis code at compile-time to generate types or data, replacing messy C-macros.
3.  **Uniform Function Call Syntax (UFCS):** `Reverse(str)` and `str.Reverse()` are treated identically by the compiler, ending the OOP vs. Functional debate.
4.  **Mentor-Style Errors:** Elm/Rust style compiler diagnostics that point directly to the code error and explain exactly *how* to fix it.
5.  **The "One Command" Toolchain:** A single binary for the whole ecosystem (`lapis build`, `lapis test`, `lapis fmt`).
6.  **Multiple Returns & Destructuring:** Easily return and unpack multiple values: `let (x, y, success) = getCoords();`.
7.  **Custom Allocators:** Easy integration of Arena allocators for game/systems devs (e.g., `new(Arena) GameObject()`).
8.  **Algebraic Data Types:** Enums that hold data (`Success(data)` / `Error(code)`) to replace slow try/catch exception blocks.
9.  **`defer`:** Guaranteed resource cleanup at the end of a scope (e.g., `defer file.Close()`).

---

## 3. Next Steps: Lexer (`Token.h` updates)

To support this architecture, the following tokens need to be added to the C++ `Token::Tokens` enum in `Token.h`:

**Structural & Literal Tokens:**
*   `identifierToken`: For user-defined names (variables, functions).
*   `integerLiteralToken`, `floatLiteralToken`, `stringLiteralToken`: For raw parsed values (e.g., `42`, `"hello"`).
*   `eofToken`: End of file marker.
*   `colonToken` (`:`): For type annotations.
*   `dotToken` (`.`): For object/property access and UFCS.

**Keywords & Operators:**
*   `comptimeToken`, `deferToken`, `mutToken`.
*   `ampersandToken` (`&`): For bitwise AND / Borrowing references.
*   `fatArrowToken` (`=>`): For Lambdas/LINQ.
*   `bitwiseShiftLeftToken` (`<<`), `bitwiseShiftRightToken` (`>>`).
*   `rangeToken` (`..`): For loops (e.g., `0..10`).
