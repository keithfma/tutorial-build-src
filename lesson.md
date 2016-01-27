# Building Software from Source Code in Linux

## Preparation

1. Sign into the Etherpad: https://etherpad.wikimedia.org/p/bu_tutorial_build_src
  - collaborative note-taking utility
  - also a convenient way to share and resolve error messages for the code challenges
2. Log into the SCC, either using the Mobaxterm on the lab computers or your favorite terminal on your laptop. 
3. Create a new working directory, then download and unpack the lesson materials (LINK NEEDED) 

## Building a single-file program 

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
  1. Preprocess
  2. Compile
  3. Assemble
  4. Link
As a side note, this is why I use the word _build_rather than _compile_ in the name of this tutorial.

![](fig/GCC_CompilationProcess.png)
Figure source: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html

### Preprocess: interpret *preprocessor directives* and **modify the source code** accordingly. 

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

Examining the output file (**`vim hello_pp.c`**) shows that the long and messy `stdio.h` header has been appended to our simple code. You may also like to explore adding `#define` statements, or conditional code blocks.

### Compile: translate (modified) source code into *assembly code*

Assembly code is a low-level programming language with commands that correspond to machine instructions for a particular type of hardware. It is still just plain text --- you can read assembly and write it too if you so desire.

To perform just the compilation step of the build process, we would run:
```
gcc -S -c hello_pp.c -o hello_pp.s
```

Examining the output file (**`vim hello_pp.s`**) shows that processor-specific instructions needed to run our program on this specific system. Interestingly, for such a simple program as ours, the assembly code is actually shorter than the preprocesses source code (though not the original source code).

### Assemble

Assembly code is then translated into *machine code* or *object code* ([more](www.linfo.org/object_code.html)). This is a binary representation of the actions your computer needs to take to run your program. It is no longer human-readable, but it can be understood by your processor.

To perform just this step of the build process, we would run:
```shell
gcc -c hello_pp.s -o hello.o
```

You can try to view this *object file* like we did the other intermediate steps, but the result will not be terribly useful (`vim hello.o`). Your text editor is trying to interpret binary machine language commands as ASCII characters, and (mostly) failing. Perhaps the most interesting result of doing so is that there are intelligable bits --- these are the few variables, etc, that actually are ASCII characters.

Also note that object files are *not* executables, even if they are totally self-contained.

### Link

In the final step, the *linker* combines the object file with any external functions it needs (e.g. library functions or functions from other source files). In our case, this would include `printf` from the C standard library.

To perform just this step of the build process, we would run:
```shell
gcc hello.o -o hello
```
### Challege:

Compile and run the following program (`squares.c`):
```c
#include <stdio.h>
main()
{ 
	int i;

	printf("\t Number \t\t Square of Number\n\n");

	for (i=0; i<=25; ++i)
	printf("\t %d \t\t\t %d \n", i, i*i);
}
```
If you have some extra time, try walking through the process step-by-step and inspecting the results. 

Solution:
```shell
gcc squares.c -o squares
./squares
```

## Building a multi-file program

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
- [The Linux Information Project](http://www.linfo.org/)

