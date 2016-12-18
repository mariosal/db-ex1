# K18 - [Project 1](docs/Assignment.pdf)

## Requirements

- [GNU Compiler Collection](https://gcc.gnu.org/)
- [GNU Make](https://gnu.org/software/make/)

## Compilation

```sh
make
```

## Execution

```sh
./build/hash.out < etc/dataset/100.csv  # [Default: -b 127]
./build/hash.out -b 127 < etc/dataset/100.csv
./build/hash.out --nocreate  # Without creating a new index

./build/exhash.out < etc/dataset/100.csv # [Default: -d 1]
./build/exhash.out -d 5 < etc/dataset/100.csv
./build/exhash.out --nocreate  # Without creating a new index
```

## Cleaning

```sh
make clean
```

## Implementation

Static Hashing has been fully implemented. From Extendible Hashing only
CreateIndex, OpenIndex and CloseIndex have been implemented.

### Operating systems

- [Fedora](https://getfedora.org/) 25
- [Ubuntu](https://www.ubuntu.com/) 15.04

### Tools

- [Clang](http://clang.llvm.org/) 3.8.0
- Git 2.9.3
- GNU Compiler Collection 6.2.1
- GNU Make 4.1
- [Valgrind](http://valgrind.org/) 3.12.0
