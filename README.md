# mkpj
> a simple command line tool used to create simple C++ projects !

## What it does ?

`mkpj` is a simple yet very useful program that lets you create projects folder.  
Theses projects follow a simple yet efficient template:

```
ProjectName/
    |_ bin/
    |_ include/
    |_ obj/
    |_ src/
    |   |_ main.cpp
    |_ Makefile
```

The `mkpj` programm creates a Makefile based on the inputs you specified.  
The Makefile includes the `all`, `install` and `clean` targets. (The `install` target shall be run as root).

By default, make will put the object files (.o) in the `obj` folder and output the binary in the `bin` folder.

---

<br>

## How to use ?

To create a project, `mkpj` will ask you several questions such as
- The project name
- The target (the ouput from the Makefile)
- The output file extension 

Just type
```
$ mkpj -c
```
to create a new project.

You can also use
```
$ mkpj -m
```
to re create the Makefile of an existing project, it is useful if you edited the configuration file (`.mkpj.conf` at the root of your project).

Finally, you can use
```
$ mkpj -t
```
to export the project to a tarball.

---

<br>

## Compilation
> GNU g++ is expected to be present on the system, as well as curl.  

You can compile the programm on your system using
```
$ make
```
It will generate a mkpj file in the `bin` folder.  
You can already use this binary file to execute the program but it is recommanded to install it.  

---

<br>

## Installation
On linux, use
```
# sudo make install
```
to install the program in the `/usr/local/bin` folder.

> **If you don't want to compile from the source, you can use the provided binaries in the release section of the repository.**  

To install the binary file on linux, use
```
# sudo mv mkpj /usr/local/bin
```

---

<br>

This projects uses part of the [CXXOPTS project](https://github.com/jarro2783/cxxopts).