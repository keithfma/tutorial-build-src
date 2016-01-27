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

For all but the smallest programming projects, it is convenient to break up the source code into multiple files. Typically, these include a main function in one file, and one or more other files containing functions / subroutines called by main(). In addition, a custom header file is usually used to share custom data types, function prototypes, preprocessor macros, etc. 

We will use a simple example program in the `src/multi` folder, which consists of:
- `main.c`: The main driver function, which calls a subroutine and exits
- `WriteMyString.c`: a module containing the subroutine called by main
- `header.h`: one function prototype and one macro definition 

The easiest way to compile such a program is to include all the required source files at the `gcc` command line:
```shell
gcc main.c WriteMyString.c -o write
./write
```

It is also quite common to separate out the process into two steps:
1. source code -> object code
3. object code -> executable (or library)

The reason is that this allows you to reduce compiling time by only recompiling objects that need to be updated. This seems (and is) silly for small projects, but becomes important quickly. We will use this approach later when we discuss automating the build process.
```shell
gcc -c WriteMyString.c
gcc -c main.c
gcc WriteMyString.o main.o -o write
./write
```

### Including header files

Note that it is **not** necessary to include the header file on the `gcc` command line. This makes sense since we know that the (bundeled) preprocessing step will append any required headers to the source code before it is compiled.

There is one caveat: the preprocessor must be able to find the header files in order to include them. Our example works because `header.h` is in the working directory when we run `gcc`. We can break it by moving the header to a new subdirectory, like so:
```shell
mkdir hdr
mv header.c hdr
gcc main.c WriteMyString.c -o write
```

The above commands give the output error:
```shell
main.c:4:20: fatal error: header.h: No such file or directory
 #include "header.h"
                    ^
compilation terminated.
```

We can fix this by specifically telling `gcc` where it can find the requisite headers, using the **`-I`** flag:
```shell
gcc -Ihdr main.c WriteMyString.c -o write
```

This is most often need in the case where you wish to use external libraries installed in non-standard locations. We will explore this case below.

### Challenge

In the folder `multi_fav_num` you will find another simple multi-file program. Build this source code to a program named `fav_num` using separate compile and link steps. Once you have done this successfully, change the number defined in `other.c` and rebuild. You should *not* have to recompile `main.c` to do this.

Solution:
```shell
gcc -c main.c
gcc -c other.c
gcc main.o other.o -o fav_num
./fav_num

vim other.c

gcc -c other.c
gcc main.o other.o -o fav_num
./fav_num
```

## Linking external libraries

NOTE: content in this section is (lightly) modified from [](https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html)

A **library** is a collection of pre-compiled object files that can be linked into
 your programs via the linker. In simpler terms, they are machine code files
 that contain functions, etc, you can use in your programs. 
 
 A few example functions that come from libraries are:
 - `printf()` from the `libc.so` shared library
 - `sqrt()` from the `libm.so` shared library
 We will return to these in a moment.

### Shared libraries vs static libraries 

- A **static library** has file extension of `.a` (archive file).  When your program links a static library, the machine code of external functions used in your program is copied into the executable. At runtime, everything your program needs is wrapped up inside the executable. 

- A **shared library** has file extension of ".so" (shared objects). When your program is linked against a shared library, only a small table is created in the executable. At runtime, the exectutable must be able to locate the functions listed in this table. This is done by the operating system - a process known as dynamic linking. 

Static libraries certainly seem simpler, but most programs use shared libraries and dynamic linking. There are several reasons why the added complexity is thought to be worth it:
- Makes executable files smaller and saves disk space, because one copy of a library can be shared between multiple programs. 
- Most operating systems allow one copy of a shared library in memory to be used by all running programs, saving memory.
- If your libraries are updated, programs using shared libraries automatically take advantage of these updates, programs using static libraries would need to be recompiled. 

Because of the advantage of dynamic linking, GCC, by default, links to the shared library if it is available.

### Building with shared libraries in default (known) locations

Let's start with an example that uses the `sqrt()` function from the math library:
```c
#include <stdio.h>
#include <math.h>

void main()
{ 
    int i;

    printf("\t Number \t\t Square Root of Number\n\n");

    for (i=0; i<=360; ++i)
        printf("\t %d \t\t\t %d \n", i, sqrt((double) i));

}
```
Notice the function `sqrt`, which we use, but do not define. The (machine) code for this function is stored in `libm.so`, and the function definition is stored in the header file `math.h`.

To build successfully, we must:
1. `#include` the header file for the external library
2. Make sure that the preprocessor can *find* this header file
3. Instruct the linker to link to the external library

Let's go ahead and build the program. To compile and link this in separate steps, we would run:
```shell
gcc -c roots.c
gcc roots.o -lm -o roots
```

The first command preprocesses `roots.c`, appending the header files, and then translates it to object code. This step does need to find the header file, but it does not yet require the library.

The second command links all of the object code into the executable. It does not need to find the header file (it is already compiled into `roots.o`) but it does need to find the library file. 

Library files are included using the `-l` flag. Thier names are given *exlcuding the `lib` prefix* and *exluding the `.so` suffix*

Just as we did above, we can combine the build steps into a single command:
```shell
gcc roots.c -lm -o roots
```

**IMPORTANT** Because we are using shared libraries, the linker *must* be able to find the linked libraries at runtime, otherwise the program will fail. You can check the libraries required by a program, and whether they are being found correctly or not using the `ldd` command. For out roots program, we get the following
```shell
ldd roots
```

```
linux-vdso.so.1 =>  (0x00007fff8bb8a000)
libm.so.6 => /lib64/libm.so.6 (0x00007ffc69550000)
libc.so.6 => /lib64/libc.so.6 (0x00007ffc691bc000)
/lib64/ld-linux-x86-64.so.2 (0x00007ffc69801000)
```
Which shows that our executable requires a few basic system libraries as well as the math library we explicitly included, and that all of these dependencies are found by the linker.

#### Mini-challenge

Before moving on, let's take a few minutes to break this build process. Try the following and read the error messages carefully. These are your hints to fixing a broken build process.
1. Delete `#include <math.h>` from `roots.c`
2. Omit `-lm` from the linking step



#### Sidebar: where does the preprocessor look to find header files?

The preprocessor will search some default paths for included header files. Before we go down the rabbit hole, it is important to note that you **do not** have to do this for a typical build, but the commands may prove useful when you are trying to work out why something fails to build.

o look for the header, we can run the following commands to show the preprocessor search path and look for files in therein:

```shell:
cpp -Wp,-v
```

Which has the following output:
```
ignoring nonexistent directory "/usr/local/include"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-redhat-linux/4.4.7/../../../../x86_64-redhat-linux/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include
  /usr/include
  End of search list.
  ^C
```

The last few lines show the paths where GCC will search for header files by default. We can then search these include paths for the file we want, `math.h` like so:
```shell
find /usr/include /usr/lib/gcc/x86_64-redhat-linux/4.4.7/include -name math.h
```
Which has the following output:
```
/usr/include/FL/math.h
/usr/include/c++/4.4.4/tr1/math.h
/usr/include/math.h
```

If we are really curious, we could open the header and see what it contains, but this is rarely necessary.

#### Sidebar: where does the linker look to find libraries?

The linker will search some default paths for included library files. Before we go down the rabbit hole, it is important to note that you **do not** have to do this for a typical build, but the commands may prove useful when you are trying to work out why something fails to build.

To look for the library, we can run the following command to get a list of all library files the linker is aware of, then search that list for the math library we need:
```shell
ldconfig -p 
ldconfig -p | grep libmath
```

The latter command gives the output:
```
libm.so.6 (libc6,x86-64, OS ABI: Linux 2.6.18) => /lib64/libm.so.6
libm.so.6 (libc6, hwcap: 0x0028000000000000, OS ABI: Linux 2.6.18) => /lib/i686/nosegneg/libm.so.6
libm.so.6 (libc6, OS ABI: Linux 2.6.18) => /lib/libm.so.6
libm.so (libc6,x86-64, OS ABI: Linux 2.6.18) => /usr/lib64/libm.so
libm.so (libc6, OS ABI: Linux 2.6.18) => /usr/lib/libm.so
```

We certainly have the math library available. In fact, there are a few versions of this library known to the linker. Thankfully, we can let the linker sort out which one to use.

We might also want to peek inside a library file (or any object code for that matter) to see what functions and variables are defined within. We can list all the names, then search for the one we care about, like so:
```shell
nm /lib/libm.so.6
nm /lib/libm.so.6 | sqrt
```
The output of this command contains the following line, which shows us that it does indeed include *something* called `sqrt`.
```
0000000000025990 W sqrt
```
### Building with shared libraries in non-default (unknown) locations

*internal note: the following command lines build the libctest.so shared library:*
```shell
gcc -Wall -fPIC -c ctest1.c ctest2.c
gcc -shared -Wl,-soname,libctest.so -o libctest.so ctest1.o ctest2.o
```

Let's switch to a new bit of example code, called `use_ctest.c` that makes use of a (very simple) custom library in the `ctest` directory:
```c
#include <stdio.h>
#include "ctest.h"
 
int main(){
    int x;
    int y;
    int z;
    ctest1(&x);
    ctest2(&y);
    z = (x / y);
    printf("%d / %d = %d\n", x, y, z);
    return 0;
}
```

Trying to compile this fails with an error:
```shell
gcc -c use_ctest.c

use_ctest.c:2:19: error: ctest.h: No such file or directory
```
As the error message indicates, the problem here is that an included header file is not found by the preprocessor. We can use the `-I` flag to fix this problem:
```shell
gcc -I ctest_dir/include -c use_ctest.c
```

When we try to link the program to create an executable, we know we need to explicitly add the library with the `-l` flag, but in this case we still get an error:
```shell
gcc use_ctest.o -lctest -use_ctest

/usr/bin/ld: cannot find -lctest
collect2: ld returned 1 exit status
```

Just like for the header, we need to explicitly specify the path to the library file:
```shell
gcc -Lctest_dir/lib  use_ctest.o -lctest use_ctest
```

Success, or so it would seem. What happens when we try to run our shiny new executable?
```shell
./ctest

./ctest: error while loading shared libraries: libctest.so: cannot open shared object file: No such file or directory
```
We can diagnose this problem by checking to see if the dynamic linker is able to gather up all the dependencies at runtime:
```shell
ldd ctest

linux-vdso.so.1 =>  (0x00007fffd75ff000)
libctest.so => not found
libc.so.6 => /lib64/libc.so.6 (0x00007f802d21b000)
/lib64/ld-linux-x86-64.so.2 (0x00007f802d5dd000)
```

The output clearly shows that it does not. The problem here is that the dynamic linker will only search the default paths unless we:
1. Permanently add our custom library to this search path

This option is not covered here - I am assuming that many of you will be working on clusters and other systems where you do not have root permissions.

2. Specify the location of non-standard libraries using the LD_LIBRARY_PATH variable

LD_LIBRARY_PATH contains a colon (:) separated list of directories where the dynamic linker should look for shared libraries. The linker will search these directories *before* the default system paths. 

You can define the value of LD_LIBRARY_PATH for a particular command only by preceeding the command with the definintion, like so:
```shell
LD_LIBRARY_PATH=ctest_dir/lib:$LD_LIBRARY_PATH ./use_ctest
```

Or define it for your whole shell as an environment variable:
```shell
export LD_LIBRARY_PATH=/ctest_dir/lib:$LD_LIBRARY_PATH
./use_ctest
```

#### Sidebar: module system on the SCC

In fact, the `module` system used to manange software packages on the SCC mostly just manipulates the PATH and LD_LIBRARY_PATH environment variables. The effect is to make programs available to the user, and to make associate dynamic libraries available to those programs and others. Take a look at your favorite module to see for yourself, e.g.:
```shell
module show R
```

3. Hard-code the location of non-standard libraries into the executable

Setting (and forgeting to set) LD_LIBRARY_PATH all the time can be tiresome. An alternative approach is to burn the location of the shared libraries into the executable as an RPATH or RUNPATH. This is done by adding some additional flags for the linker, like so:
```shell
gcc -Lctest_dir/lib  use_ctest.o -lctest -Wl,rpath,ctest_dir/lib,--enable-new-dtags- use_ctest
```

We can confirm that this worked by running the program (resetting LD_LIBRARY_PATH first if needed), and more explicitly, by examining the executable directly:
```shell
./use_ctest
readelf -d use_ctest
```

### Challenge (skeleton only)

The program XXX makes use of the XXX library. This library is installed on the SCC in the following directory:

See if you can successfully build and run this program. For an additional challenge, try to do so using RUNPATH to hardcode the location of the shared library.

## Automating the build process with GNU Make

- return to multifile example 
- dependency analysis
- write simple makefile
- add "clean" target
- show "fancy" makefile 
- challenge: modify Makefile to work for the external lib example

## Building with Autotools: configure; make; make install

- discuss the motivation
- demo 
- help, setting options
- challenge, build and install in a custom location

## References

- [GCC and Make: Compiling, Linking and Building C/C++ Applications](https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html)
- [Programming in C: C/C++ Program Compilation](https://www.cs.cf.ac.uk/Dave/C/node3.html)
- [Programming in C: Writing Larger Programs](https://www.cs.cf.ac.uk/Dave/C/node35.html) 
- [The Linux Information Project](http://www.linfo.org/)
- [C/C++ library programming on Linux](http://www.techytalk.info/c-cplusplus-library-programming-on-linux-part-one-static-libraries/)

