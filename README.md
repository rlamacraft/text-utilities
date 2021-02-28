# text-utilities
A bunch of simple utility programs written in C for viewing and manipulating text files; just like the standard cat, less, ed, and others.

## cat.c
A simple program just like `cat`, in that it prints to stdout the contents of the file given to it.
- To compile do `gcc cat.c -o bin/cat`, or other similar in your c compiler of choice.
- To run do `./cat <file>`.

## less.c
A simple program just like `less`, in that it displays the contents of the file in terminal interactively.
- To compile do `gcc less.c -o bin/less`.
- To run do `./less <file>`.
- To use, type a command then enter

### Commands
- `j` scroll down one line
- `k` scroll up one line
- `q` quit
