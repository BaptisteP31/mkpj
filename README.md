![mkpj](mkpj_768.png)


![licence](https://img.shields.io/github/license/BaptisteP31/mkpj)
![release](https://img.shields.io/github/v/release/BaptisteP31/mkpj?include_prereleases)
![Downloads](https://img.shields.io/github/downloads/BaptisteP31/mkpj/total)
![size](https://img.shields.io/github/languages/code-size/BaptisteP31/mkpj)
![issue](https://img.shields.io/github/issues-raw/BaptisteP31/mkpj)

# MKPJ
> a simple command line tool used to create simple C++ projects !

## What it does ?

`mkpj` is a simple yet very useful program that lets you create projects folder as well as adding hpp/cpp files and download utility [pairs](##pairs).
Theses projects follow a simple yet efficient template:

```
project_name/
 ├── bin/
 ├── include/
 │   └── *.hpp
 ├── src/
 │   └── *.cpp
 ├── obj/
 ├── Makefile
 ├── .mkpj.conf
 ├── .pairs.conf
 └── README.md
```

The `mkpj` program creates a Makefile based on the inputs you specified.  
The Makefile includes the `all`, `install` and `clean` targets. (The `install` target shall be run as root).

By default, make will put the object files (.o) in the `obj` folder and output the binary in the `bin` folder.

<br>

## How to use ?

To create a project, `mkpj` will ask you several questions such as
- The project name
- The target (the ouput from the Makefile)
- The output file extension 
- ...

Just type
```
$ mkpj
```
To open the menu.  
This menu will let you create a project, once in a project, you will be able to add cpp/hpp files, add a pair from the list, you can regenerate the Makefile, and export the project to a tarball.
> You can add additional files to the tarball by modifying the `additional_files` property in the `.mkpj.conf` file at the root of your project.

You can add a pair to your project using
```
$ mkpj -p <pair_name>
```
see the [Pairs](##pairs) section for more information.

<br>

## Pairs
> Since version 0.6 mkpj can download pairs of files from the internet.  

The pairs are simple utility classes that can be used in your projects.
You can find the list of available pairs [here](https://github.com/BaptisteP31/mkpj-core/tree/main/pairs).  

You can add your own pairs by adding them to the `pairs` folder in the `mkpj-core` repository and making a pull request. We will review your pair and add it to the list if it is relevant.

If you want to get more information about the pairs, you can check the [mkpj-core](https://github.com/BaptisteP31/mkpj-core) repository which contains the pairs as well as a more in depth documentation containing a template for the pairs and a guide on how to create your own pairs.  

To list the available pairs, use
```
$ mkpj -l
```
or use the menu.

To download a pair, use
```
$ mkpj -p <pair_name>
``` 

<br>

## Compilation
> GNU g++ is expected to be present on the system, as well as libcurl and ncurse.  
> We only recommend compiling from source if you are familiar with the compilation process.


To check if libcurl is installed on your system, use
```
$ ldconfig -p | grep libcurl
```
To check if ncurse is installed on your system, use
```
$ ldconfig -p | grep libncurses
```

You shall prepare the project for compilation using
```
$ make configure
```

You can compile the program on your system using
```
$ make
```
It will generate a mkpj file in the `bin` folder.  
You can already use this binary file to execute the program but it is recommended to install it.  

<br>

## Installation

> The installation the install script is recommended for beginners and for people who don't want to compile from source.

### The simple way, using the install script via curl:
```bash
sudo /bin/bash -c "$(curl -fsSL "https://raw.githubusercontent.com/BaptisteP31/mkpj/main/install.sh")"
```
Note that the install script will install the program in the `/usr/local/bin` folder.

<br>

### From source:  
On linux, use
```
# make install
```
to install the program in the `/usr/local/bin` folder.

### From binary:
> **If you don't want to compile from the source, you can use the provided binary in the release section of the repository.**  

To install the binary file on linux, use
```
# sudo mv mkpj /usr/local/bin
```

<br>

---
This projects uses part of the [CXXOPTS project](https://github.com/jarro2783/cxxopts) and is licensed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html) license.
