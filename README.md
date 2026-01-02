# eevo

**eevo is a lightweight yet expressive, high-level scripting language for data-driven
functional programming.**
A general-purpose language in the Lisp family that is
ideal for scripting, acting as an interactive shell, data/configuration files, or a scientific
calculator.
eevo's portable, simple design makes it easy to *embed* and *extend*,
enabling integration into existing projects and straightforward use of established libraries.
Forget the boilerplate and needless ceremony: focus on the what, and let eevo handle the how.

> Everything should be as simple as possible, but not simpler
>
> - *Albert Einstein*

- **Simple**: Built on a minimal set of composable concepts.
          You can learn the entire language in an afternoon.
- **Small**: Extremely lightweight with less than **100KB binary** from **2k lines** of C99.
- **Symmetric**: unification of many ideas with uniform syntax.
             Same principals and syntax are applied universally.
- **Sweet**: Syntactic sugar for **easy** and **readable syntax**,
         with invisible s-expressions underneath.
- **Separable**: Modular design allows you to use only what you need, or modify each component to
             suit your needs.
- **Symbolic**: Identifiers are first class citizens,
            allowing code to be manipulated like any data.
- **Shareable**: Bundled as a single universal executable makes it easy to run eevo code anywhere.

```eevo
def fib(n)
  "Fibonacci number of n"
  if (< n 2)
    n
    + fib((- n 1)) fib((- n 2))

fib 25  ; = 46368
```

A lightweight modular core allows you to choose how much of the language you need.
This comes in a few layers:
  - **Read/Print**: Parse expressions and output data structures unevaluated
    (**eg** data or config files)
  - **Evaluate**: Execute the code; transform and simplify expressions
  - **Core**: Essential functions and syntax
    (**eg** flow control, list manipulation, basic math)
  - **Standard Library**: Additional functions needed for common operations (**eg** IO, math, doc)
  - **Contrib**: Community-driven libraries and scripts, officially sponsored (coming soon)

While eevo is a general purpose programming language,
that does not mean it is the best tool for every problem.
Like all languages, eevo has many design decisions
which have trade-offs in different situations.
To solve problems that are ill-suited to eevo's design
(such as requiring circular references or precise mutations)
other languages can be called upon through C-bindings (and soon WASM).

This interoperability makes it easy to offload computationally heavy code to another language
or gradually replace existing code with concise scripts.
This enables a dual approach to bridge languages in two directions:
 - **Extend** eevo by **embedding** another language as a library,
   - Out source computationally expensive code to low level libraries.
 - **Embed** eevo to **extend** another program,
   - Slowly replace existing low level code with a high-level hackable scripting.

Both methods can be done at the same time with different languages,
**allowing eevo to glue different libraries, ecosystems, and tools together.**

> [!WARNING]
> eevo is still in active development and not yet stable. Until the `v1.0`
> release expect breaking non-backwards compatible changes.

## Features

### High-level
  - Developer can focus on important logic, not boiler-plate.
  - Let eevo worry about implementation details, optimizations, data representation.
### Functional programming
  - [All you need is data][datafuncs], and functions to transform that data.
### Interactive
  - Read, Evaluate, Print, Loop (REPL).
### [First Class Types](learn/manual/#types)
  - Numbers: integers `Int`, decimals `Dec`, ratios `Ratio`.
  - Booleans: `True`, `Nil`.
  - Text: strings `Str`, symbols (identifiers/variables) `Sym`.
  - Lists, pairs `Pair`,
  - [Code expressions][sexpr].
  - Records: `Rec` (hash tables/maps),
    - Look up tables, accept key and return corresponding value.
    - Unordered set of key value pairs.
    - [Environments][env] (namespaces/modules/capabilities).
  - Functions: with closures `Func`,
    - `Func` for [anonymous functions][anon]
    - `Prim` for external functions written in host language
  - Macros:
    - `Macro` functions which transform code.
    - `Form` special forms which might not ovulate all their arguments. Written in host language.
  - Types
  - `Void`
  - Errors
### Functions as the universal type
  - Records, arrays, dictionaries, environments, sets, types, strings are all modeled as functions.
  - Functions are records with infinite many keys.
  - Full unification still a work in progress.
### Metaprogramming
  - [Homoiconic][homoiconic] syntax allows for powerful and simple macro system.
  - Abstract away boiler-plate, cost free.
  - Customize syntax to extend language with new features.
### Quoted expressions
  - Code can be quoted to avoid (or delay) its evaluation.
  - Allows code to be manipulated like data.
  - Quasiquote enables some of the code to be unquoted and evaluated.
### Literate Programming
  - Write documentation directly inside an eevo script, which is converted to [Markdown][markdown].
### Tail call optimization
  - Recursive loops become computationally equivalent to imperative loops.
### Dyslexic friendly
  - Primarily designed by someone with dyslexia. The language, standard library, and documentation
    are written to be as easy as possible to read.

See the [language manual](https://eevo.pub/learn/manual) for complete set of features.

[datafuncs]: https://mckayla.blog/posts/all-you-need-is-data-and-functions.html
[homoiconic]: https://en.wikipedia.org/wiki/Homoiconicity
[env]: https://dl.acm.org/doi/10.1145/3689800
[sexpr]: https://stopa.io/post/265
[anon]: https://en.wikipedia.org/wiki/Lambda_calculus
[markdown]: https://daringfireball.net/projects/markdown/

## Anti-Features

Just like jazz, what is missing is often more insightful:

### No mutations
  - Mutations make programs difficult to predict and harder to read, increasing the mental load of
    every line of code.
    - Given the same code and data you always get the same result.
  - Prevents the mistakes that come from [half constructed values][halfcons].
  - Change should be modeled one way: function calls.
  - Leave mutations as an optimization performed by the compiler.
  - Allows for other easy optimizations such as automatic parallelization.
  - Removes circular references, making automatic reference counting easy.
### No dependencies (eg LLVM)
  - Dependencies are just someone else's code, which silently increase the surface error of
    potential bugs.
  - Only relies on a C compiler and libc.
### No statements
  - Everything is an expression: `5`, `"hello"`, `def`, `if`, etc.
### No keywords
  - All symbols are equal and can be redefined at will.
### No reader macros
  - Limit amount of the language that can be modified.
  - Should be easy to read anyone's scripts.
### No exceptions
  - Errors are just regular values which have to be explicitly handled.
### No multiple return values
  - Just return a list, and require callee to explicitly expand into multiple values.
### No explicit return
  - Last item of procedure is always returned.
  - Use Void to explicitly avoid returning last value.
### No arrays, linked lists, or tuples
  - Just pairs (2-tuples) and records.
  - Lists and trees are constructed out of pairs.
### No function currying
  - Partial application should be explicit to avoid hard to debug accidents when you forget an
    argument.
  - Functions only take one argument, but it is often a list of arguments.
  - You are welcome to write functions in the curry style, but the standard library is not written
    that way.
### No build systems
  - Simply run the primary `.evo` file (by convention the same name as the project) and let it
    handle including and running all other files as needed.
### No mandatory editor tools
  - Syntax should be easy to modify without [specialized tools][paraedit].
### No garbage collector
  - Memory is managed through a simple bump allocator, with reference counting in the works.

[halfcons]:  https://jerf.org/iri/post/2025/fp_lessons_half_constructed_objects/
[paraedit]: https://cursive-ide.com/userguide/paredit.html

## Coming Soon...

### Automatic reference counting memory management
  - Memory is initially allocated on the stack with a region allocator.
  - Values which escape their scope are [evicted to the heap][lazyalloc] and
    [reference counted][beans].
### Strong static typing with type inference
  - First class.
  - Algebraic data types (sum, product, exponential).
  - Physical units,
    - Uncertainty.
  - Polymorphism,
    - Row (record) polymorphism.
  - [Refinement types][refine].
  - [Codata][codata].
### Batteries included standard library
  - While still being minimal and orthogonal.
### Powerful string interpolation
  - Doubles as a [template processor][templ] (eg [mustache][mustache]).
### [First class][first-pat] [algebraic][alg-pat] pattern matching
  - Function calls are already a form of pattern matching on lists,
    - Generalize this to any data pattern.
### [Improve error messages][jankerr]
### Sandboxed
  - Manage security and effects through capabilities via first class environments.
### Full Unicode support
  - Encoded in [UTF-8][unicode].
### Interoperability
  - Support any programming language with C bindings.
  - C, Lisp, Python, Lua, Rust, Go, etc.
  - Remove requirement of C functions in specific form.
### [WebAssembly][rasm] [compiler][schism]
  - Web interface, environment, and REPL.
  - Javascript runtime.
### Environmental image
  - Restore the environment exactly how you left it.
### Hygienic macros
  - Avoid variable capture with macros.
  - Simple without need for `syntax-rules` or `gensym`, similar to [s7][hygienic-macros]
### Multithreading
### Optimizations
  - Total pre-computation
    - Anything that can be computed at compile time will be
    - Constant folding and propagation
  - Opportunistic in place mutation
  - Memorization (cache)
    - Strings and symbols are already [interned][str-intern]
  - Auto parallelism
    - GPU acceleration

[lazyalloc]: https://www.cs.tufts.edu/~nr/cs257/archive/henry-baker/cons-lazy-alloc.pdf
[beans]: https://arxiv.org/pdf/1908.05647
[refine]: https://arxiv.org/pdf/2010.07763
[codata]: https://www.microsoft.com/en-us/research/wp-content/uploads/2020/01/CoDataInAction.pdf
[jankerr]: https://jank-lang.org/blog/2025-03-28-error-reporting/
[first-pat]: https://www.cambridge.org/core/services/aop-cambridge-core/content/view/968C982CA9B727A2C04D216EEF4E6CFC/S0956796808007144a.pdf/first-class-patterns.pdf
[alg-pat]: https://arxiv.org/pdf/2504.18920
[templ]: https://en.wikipedia.org/wiki/Template_processor
[mustache]: https://mustache.github.io/
[rasm]: https://digitalcommons.calpoly.edu/cgi/viewcontent.cgi?article=4067&context=theses
[schism]: https://github.com/schism-lang/schism
[hygienic-macros]: https://ccrma.stanford.edu/software/snd/snd/s7.html#macros
[unicode]: https://tonsky.me/blog/unicode/
[str-intern]: https://en.wikipedia.org/wiki/String_interning

# Install

## Interpreter

A command line interpreter is provided with eevo by default, found in `main.c`.

- Dependencies:
  - **C99 compiler**
  - **libc**: C standard library
  - **GNU make**: build system
  - **xxd**: build `core.evo.h` to include eevo core at compile time
  - **sed**: construct amalgamated source files
- *Optional*:
  - [**markman**][1]: regenerate man pages from markdown
  - **rlwrap** (`evo`): more interactive REPL with readline support

[1]: https://github.com/edvb/markman

Modify `config.mk` for your system, or leave as is for default Unix-like systems.

```
$ git clone https://github.com/eevolang/eevo && cd eevo
$ make
$ sudo make install
```

## Library

eevo can also be statically embedded as a library in other C programs with
*almost* zero dependencies.
Simply drop `eevo0.1.c`, `eevo0.1.h`, into your project to use the
necessary functions for your program.

- Dependencies:
  - **C99 compiler**
  - **libc**

```
$ wget https://get.eevo.pub/latest/eevo.tar.gz
$ tar xf eevo.tar.gz -C path/to/your/project
```

# Contributing

All contributions are welcome!

Check out the [ROADMAP](doc/ROADMAP.org) and [TODO](doc/TODO.org) list for potential projects.

If you have any issues, questions, or suggestions, please open an issue
[here](https://codeberg.org/eevo/eevo/issues).

C code follows the [suckless coding style](https://suckless.org/coding_style/).

Commit messages should follow [this format][commit].

[commit]: https://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html

# License

Zlib © Edryd van Bruggen

