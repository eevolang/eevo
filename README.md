# eevo script

<p align="center"><a href="https://eevo.pub">Home</a> | <a href="https://eevo.pub/try">Try</a> | <a href="https://eevo.pub/get">Get</a> | <a href="https://eevo.pub/tour">Tour</a> | <a href="https://eevo.pub/#features">Features</a> | <a href="https://eevo.pub/learn">Learn</a></p>

**eevo is a simple yet expressive high-level functional scripting language,
an experiment in how simple a practical programming language can be.**

A general-purpose language in the Lisp family,
ideal for scripting, acting as an interactive shell, data/configuration files, or a scientific
calculator.
eevo's portable, simple design makes it easy to *embed* and *extend*,
enabling integration into existing projects and straightforward use of established libraries.
A highly modular core allows you to choose which parts of the language to include,
or swap out components for alternate versions that better suit your needs.
Even core features, such as the parser or evaluator, are customizable.
Forget the boilerplate and needless ceremony: focus on the what, and let eevo handle the how.

```eevo
def (fib n)
  "Fibonacci number of n"
  if (< n 2)
    n
    + (fib (- n 1)) (fib (- n 2))

fib 25  ; = 46368
```

> [!WARNING]
> eevo is still in active development and not yet stable. Until the `v1.0`
> release expect breaking non-backwards compatible changes.

# Install

## Interpreter

eevo provides a command line interpreter by default, found in [`main.c`][main].

[main]: https://codeberg.org/eevo/eevo/src/branch/main/main.c

- Dependencies:
  - **C99 compiler**
  - **libc**: C standard library
  - **libdl**: import static primitives and libraries at run-time
  - **libm**: math functions
  - **GNU make**: build system
  - **xxd**: build `core.evo.h` to include eevo core at compile time
- *Optional*:
  - **rlwrap** (`evo`): more interactive REPL with readline support
  - **[markman][1]**: regenerate man pages from markdown

[1]: https://codeberg.org/edryd/markman

Modify `config.mk` to your liking, or leave it as is for default Unix-like systems.

```
$ git clone https://codeberg.org/eevo/eevo && cd eevo
$ make
$ sudo make install
```

## Library

eevo can also be statically embedded as a library in other C programs with
*almost* zero dependencies.
Simply drop the amalgamated `eevo0.2.c`, `eevo0.2.h`, into your project to use
the necessary functions for your program.

- Dependencies:
  - **C99 compiler**
  - **libc**
  - **libm**
  - **libdl**

```
$ git clone https://codeberg.org/eevo/eevo && cd eevo
$ make
$ cp eevo0.2.c eevo0.2.h /path/to/your/project/
```

# Usage

```
$ eevo
> def best-friend "Sisko"
> def (greet name) f"Hello, {name}!"
> map greet [best-friend "O'Brien" 'Quark]
["Hello, Sisko!"
 "Hello, O'Brien!"
 "Hello, Quark!"]
```

```
$ cat > fact.evo
def (factorial n)
  if (= n 1)
    1
    * n factorial((- n 1))
$ eevo fact.evo -e "(factorial 5)"
120
```

# Contributing

All contributions are welcome!
I am actively looking for collaborators to help with numerous ideas.
Check out the [help wanted][] for a list of potential projects,
just reach out before embarking on any major work!

[help wanted]: https://codeberg.org/eevo/eevo/issues?q=&state=open&labels=1154278

If you have any questions, suggestions, or find any bugs please
[open an issue](https://codeberg.org/eevo/eevo/issues).

The C code follows the [suckless coding style](https://suckless.org/coding_style/).
Commit messages should follow [these general guidelines][commit].

[commit]: https://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html

# License

Zlib © Edryd van Bruggen
