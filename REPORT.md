# REPORT.md — Operating Systems Programming Assignment 01

## Feature 2: Multi-file Project using Make Utility

**Q1: Explain the linking rule `$(TARGET): $(OBJECTS)` in this part's Makefile. How does it differ from a Makefile rule that links against a library?**

The rule `$(TARGET): $(OBJECTS)` tells `make` that the final executable depends directly on all the object files (`.o` files). When this rule runs, it calls `gcc` and passes every object file to it, and the linker combines them all into one executable. All the code is right there in the object files, so nothing else is needed.

This is different from a Makefile rule that links against a library. In that case, the object files are first packed into a library (either a `.a` static library using `ar`, or a `.so` shared library using `gcc -shared`). Then the final executable rule doesn't list all the object files — it only links the `main.o` file plus the library, using the `-L` flag (to tell the linker where the library folder is) and the `-l` flag (to tell it which library to use). So instead of listing every object file, we just say "link against this library," and the linker figures out which functions it needs from it.

**Q2: What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?**

A git tag is basically a permanent bookmark on a specific commit. It gives a meaningful name (like `v1.0` or `v0.2.1-static`) to a particular point in the project's history, so that we can easily go back to that exact version later without having to remember a long commit hash.

Tags are useful because they let us mark stable, working versions of the project — for example, "this is the version that passed all tests" or "this is what we submitted for grading." Without tags, we would have to scroll through commit history to find a specific version.

A **simple tag** is just a name pointing to a commit — nothing else is saved with it. An **annotated tag** stores extra information: the tagger's name, email, date, and a message explaining what the tag is for. Annotated tags are treated as full objects in Git's database (they can even be signed with GPG), which makes them the recommended choice for marking releases — that's why we use `git tag -a` with a `-m` message for our project.

**Q3: What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?**

A GitHub Release is a way to package a specific tagged version of the project and present it as a distributable, downloadable product. It shows up in a dedicated "Releases" section of the repo with a title, description, and download links — making it easy for anyone to find and use a stable version without digging through the code.

Attaching binaries (like our compiled `client` executable) is significant because it means a user doesn't need to have a C compiler, doesn't need to clone the whole repository, and doesn't need to run any build commands. They can simply download the executable and run it directly. This is exactly how real-world software is distributed — for example, downloading a `.exe` or `.deb` file instead of compiling an application from its source code yourself.

---

## Feature 3: Creating and using Static Library

**Q1: Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?**

In Part 2's Makefile, all `.c` files (including `main.c`) were compiled into object files and linked together directly in one step, using a single rule: `$(TARGET): $(OBJECTS)`.

In Part 3's Makefile, we introduced two new variables: `AR` (set to `ar`, the archiver tool) and `ARFLAGS` (set to `rcs`, the flags used when creating the archive). We also split the source files into two groups: the library sources (`mystrfunctions.c` and `myfilefunctions.c`) and `main.c` separately, since `main.c` is not part of the library.

A brand-new rule was added: `$(LIB_NAME): $(LIB_OBJECTS)`, which runs `ar rcs libmyutils.a mystrfunctions.o myfilefunctions.o` to pack the two object files into a static library archive. The final executable rule changed too — instead of linking all object files, it now depends on `main.o` and the library file, and links using `-L../lib -lmyutils` instead of listing every object file by name. So essentially, an extra "packaging" step (`ar`) was added, and the final link step now points to the library instead of raw object files.

**Q2: What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?**

The `ar` command stands for "archiver." Its job is to bundle multiple object (`.o`) files together into a single archive file, usually with a `.a` extension, which we call a static library. This lets us distribute or link against a whole collection of compiled functions as one single file instead of many separate object files.

`ranlib` is used right after `ar` because a static library needs an internal index (a symbol table) so that the linker can quickly find which object file inside the archive defines a given function, without having to search through every object file one by one. Without this index, linking would be much slower, especially for large libraries. In our project, we actually use `ar` with the `rcs` flags together — the `s` flag automatically creates this symbol index, which is why we don't need to run `ranlib` as a separate step; the `s` in `ARFLAGS = rcs` already does what `ranlib` would do.

**Q3: When you run `nm` on your `client_static` executable, are the symbols for functions like `mystrlen` present? What does this tell you about how static linking works?**

Yes — when we run `nm bin/client_static | grep mystrlen`, the symbol for `mystrlen` shows up, marked with a `T` (meaning it is defined and present in the text/code section of the executable).

This tells us that static linking actually **copies** the machine code of the functions we used from the library directly into the final executable at link time. The executable becomes a completely self-contained file — it doesn't need `libmyutils.a` to exist anymore once it's built, because the function code is already baked inside it. This also explains why statically linked executables are larger in file size than dynamically linked ones — they are literally carrying their own copy of the library code inside them.

---

## Feature 4: Creating and using Dynamic Library

**Q1: What is Position-Independent Code (`-fPIC`) and why is it a fundamental requirement for creating shared libraries?**

Position-Independent Code means machine code that can run correctly no matter where in memory it gets loaded. Normally, compiled code might use fixed, absolute memory addresses to refer to variables and functions. But a shared library (`.so` file) can be loaded into different memory addresses by different programs, or even loaded at a different address every time a program runs (this is used for security, called ASLR — Address Space Layout Randomization).

If the library's code was not position-independent, it would only work correctly if it was always loaded at exactly the same memory address, which is not realistic since many different programs might use the same shared library at the same time, each needing it at a different spot in their own memory space. That's why we compile the library source files with `-fPIC` — it makes the compiler generate code that uses relative addressing instead of hardcoded absolute addresses, so the same `.so` file can be safely shared and loaded anywhere in memory by any program.

**Q2: Explain the difference in file size between your static and dynamic clients. Why does this difference exist?**

When we compared file sizes using `ls -lh bin/client_static bin/client_dynamic`, the static executable was noticeably larger than the dynamic one.

This happens because `client_static` contains an actual copy of all the library function code (`mystrlen`, `mystrcpy`, `wordCount`, `mygrep`, etc.) baked directly into the executable file at link time — so the executable is self-sufficient but bigger. On the other hand, `client_dynamic` only contains a small reference/pointer to the shared library (`libmyutils.so`) that it needs, without actually including the library's code inside the executable file. The real function code stays in the separate `.so` file and gets loaded into memory only when the program runs. This makes `client_dynamic` much smaller on disk, but it depends on `libmyutils.so` being available at runtime.

**Q3: What is the `LD_LIBRARY_PATH` environment variable? Why was it necessary to set it for your program to run, and what does this tell you about the responsibilities of the operating system's dynamic loader?**

`LD_LIBRARY_PATH` is an environment variable that tells the operating system's dynamic loader (`ld.so`) which extra directories to search when it's looking for shared library files (`.so` files) that a program needs at runtime.

It was necessary to set it in our case because when we tried to run `./bin/client_dynamic` for the first time, we got an error: `error while loading shared libraries: libmyutils.so: cannot open shared object file`. This happened because our `libmyutils.so` file lives in our project's own `lib/` folder, which is not one of the operating system's standard, default library search locations (like `/lib` or `/usr/lib`). By running `export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH`, we added our custom `lib` folder to the search list, and the loader was then able to find and load `libmyutils.so` successfully.

This shows that the responsibility of actually connecting a dynamically linked program to its needed library code doesn't happen at compile time — it happens every single time the program is *run*, by the operating system's dynamic loader. The loader has to search through a list of directories (standard system paths plus anything in `LD_LIBRARY_PATH`) to locate every shared library the program depends on, and only then can the program actually execute. This is very different from static linking, where all the needed code is already permanently embedded in the executable and no such runtime search is required.
