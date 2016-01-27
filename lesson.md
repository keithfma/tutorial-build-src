# Building Software from Source Code in Linux

## Preparation

1. Sign into the Etherpad: https://etherpad.wikimedia.org/p/bu_tutorial_build_src
  - collaborative note-taking utility
  - also a convenient way to share and resolve error messages for the code challenges
2. Log into the SCC, either using the Mobaxterm on the lab computers or your favorite terminal on your laptop. 
3. Create a new working directory, then download and unpack the lesson materials (LINK NEEDED) 

## Overview by example

Let's start with a simple example: building a "hello world" C program with the GCC compiler.

Our program (`hello.c`) looks like this:
```c
#include <stdio.h>
main()
{
    (void) printf("Hello World\n");
    return (0);
}
```

To build a working executable from this file in the simplest way possible, run:
```
$ gcc hello.c
```

This command creates an executable with a default name of `a.out`. Running this prints the familiar message:
```
$ a.out
Hello World
``` 

More happened here than meets the eye. In fact, this command wraps up 4 steps of the build process:

![](fig/GCC_CompilationProcess.png)
Figure source: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

### Preprocessing: interpret *preprocessor directives* and **modify the source code** accordingly. 

Some common directives are:
  - `#include` 
    - includes contents of the named file, typically a *header* file 
    - e.g. `#include <stdio.h>`

  - `#define` 
    - macro substitution
    - e.g. `#define PI 3.14159

  - `#ifdef ... #end`
    - conditional compilation, the code block is included only if a certain macro is defined 
    - e.g.: 

        ```c
        #ifdef TEST_CASE
        a=1; b=0; c=0;
        #endif
        ```
We *could* perform just this step of the build process like so:

```shell
cpp hello.c hello_pp.c
```

Examining the output file (`vim hello_pp.c`) shows that the long and messy `stdio.h` header has been appended to our simple code. You may also like to explore adding `#define` statements, or conditional code blocks.

### Compilation: translate (modified) source code into *assembly code*
  - Assembly code is...


## Compilers

- Many compilers available, three of the most popular are:
  - GCC 
  - Intel
  - Portland Group
- (NEEDS WORK) How alike and how different are these compilers?
- (NEEDS WORK) What about compatibility between compilers? Probably too soon to discuss this.
- We will use the GCC compiler family in this tutorial, but the others would work just as well.

## References

- [GCC and Make: Compiling, Linking and Building C/C++ Applications](https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html)
- [Programming in C: C/C++ Program Compilation](https://www.cs.cf.ac.uk/Dave/C/node3.html)
- [Programming in C: Writing Larger Programs](https://www.cs.cf.ac.uk/Dave/C/node35.html) 

