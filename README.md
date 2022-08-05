# Kraken :octopus:

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8c856b79282e4c8f90dae55dc026f3a2)](https://www.codacy.com/gh/yahya-mohammed07/Kraken/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=yahya-mohammed07/Kraken&amp;utm_campaign=Badge_Grade)

## Table of Contents

* [Introduction](#introduction)
* [Requirement](#requirement)
* [Contents](#contents)
* [Installation](#installation)

## Introduction

* **`Kraken`** is a modern math library written in a way that gives accurate results, compile time checks and simplicity.

## Requirement

* c++20 or later
  * Tested on:
    * g++10 std=c++2a
    * g++11 std=c++2a
    * clang++12 std=c++2a
    * clang++11 std=c++2a
    * clang++10 std=c++2a

* Catch2 for testing

## contents

* A matrix_<> class. For more info check: [about_matrix](https://github.com/yahya-mohammed07/Kraken/blob/master/docs/about_matrix.md)

* namespace `kraken` which has:-

  * namespace `constants`: contains mathematical and physical constants.
  * namespace `cal`: contains functions that do some calculations.
  * namespace `op`: contains variadic function objects.
  * and a bonus namespace! `num_methods` : contains implementation of some numerical methods.

## Installation

* [Download](#download)
* Adding the library to the intended path so the compiler can recognize it
  * [Any system](#all)
  * [Debian based systems](#debian)
  * [Windows](#windows)

### Download

* A simple way is to download this library from `github` as `zip` then `unzip` it maybe using your os built-in unzip tool but the preferred way is to use [PeaZip][] for windows and linux, for mac you can use [keka][].

## All

* If you downloaded it as `zip` and unzipped it, then in the unzipped library the root of the header folder hierarchy is `Kraken-master/source/library`.

* copy the **`library`** folder from the unzipped archive to your working directory as **`Kraken`** and include it this way.

* ```cpp
  #include "Kraken/all.hpp"
  #include <iostream>

  using namespace kraken;

  auto main()
      -> int
  {
    std::cout << constants::pi << '\n';
  }
  ```

### Debian

* If you downloaded it as `zip` and unzipped it, then in the unzipped library the root of the header folder hierarchy is `Kraken-master/source/library`.

* I.e. there is a `Kraken-master/source/library/all.hpp` header, which is the one that you want the compiler to find as <Kraken/all.hpp>.

* copy the **`library`** folder from the unzipped archive, to a folder that your compiler searches for headers.

And that folder is :-
> /usr/include

#### The procedure :-

* Ui version
  * if you're using `nautilus` file manager
    * open terminal then do `sudo apt install nautilus-admin` then when it's done do `nautilus -q`.
  * if your're using `nemo` no need to install anything `run as-administer` is built-in.

* open `usr/include` folder as `administrator`.
* paste `library` folder into `usr/include`, then rename `library` into `Kraken`. you're done!.

* Terminal version
  * if you have any file manger installed you can open a folder as administer as follows:
    `sudo` **(your-file-manager)** ` /usr/include `
  * paste `library` folder into `usr/include`, then rename `library` into `Kraken`. you're done!.

### windows

* If you downloaded it as `zip` and unzipped it, then in the unzipped library the root of the header folder hierarchy is `Kraken-master/source/library`.

* I.e. there is a `Kraken-master/source/library/all.hpp` header, which is the one that you want the compiler to find as <Kraken/all.hpp>.

* copy the **`library`** folder from the unzipped archive, to a folder that your compiler searches for headers.

* for the MinGW g++ compiler you might have a folder such as
  > C:\installed\mingw\nuwen\MinGW **(version)**\include
* just paste the `library` folder inside, then rename `library` into `Kraken`. you're done!

* for the Visual C++ compiler you might have a folder such as
  > C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\ **(version)** \include
* just paste the `library` folder inside, then rename `library` into `Kraken`. you're done!

## Check if the library is working by running this code:-

* ```cpp
  #include <Kraken/all.hpp>
  #include <iostream>

  using namespace kraken;

  auto main()
      -> int
  {
    std::cout << constants::pi << '\n';
  }
  ```
  
[PeaZip]: https://peazip.github.io/
[keka]: https://www.keka.io/en/
