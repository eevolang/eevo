# eevo \- easy expression value organizer

The default interpreter for the eevo programming language.
Read and evaluate all files in order given, if file name is `-` read from `stdin`.
If no files are supplied launch the read-evaluate-print-loop (REPL) for
interactive running of commands.

## Options

#### -e EXPRESSION

Read *EXPRESSION* as a line of eevo code, evaluate, and print result

#### -i

Enter interactive mode by launching the REPL prompt.
Default behavior if no arguments are given.
Explicit option used to load files and then run REPL,
enabling interaction with the code.
Equivalent to -e '(repl)'.

#### -h

Print help and exit

#### -v

Print version info and exit

## Usage

Run the program from the command line to type a single command and press enter
to see the result.

```
$ eevo
> Pair 1 2
[1 ... 2]
> list 1 (+ 1 1) 3 (* 2 2)
[1 2 3 4]
```

Alternatively you can pass a file name which will be opened and run, outputting
the result before exiting.

```
$ echo '((Func (x) (+ x 1)) 10)' > inc.evo
$ eevo inc.evo
11
```

Commands can also be piped directing into eevo through the command line.

```
$ echo '(= "foo" "foo")' | eevo
True
```

Or given directly to eevo as an argument:

```
$ eevo -e "(reverse '(1/2 1/4 1/8 1/16))"
(1/16 1/8 1/4 1/2)
```

## See Also

eevo(7)

See project at <https://edryd.org/projects/eevo>

View source code at <https://git.edryd.org/eevo>

## Author

Edryd van Bruggen <ed@edryd.org>

## License

zlib License
