# Kraken :octopus:

**Table of Contents:**

* [Introduction](#introduction)
* [Requirement](#requirement)
* [Contents](#contents)
* [Installation](#installation)
  * [Debian_os](#debian)
  * [Windows](#windows)

## Introduction

* `Kraken` is a modern cross-platform mini-math library similar to [cmath][] but in a different approaches (good for both learners and actual work) that gives accurate results, compile time checks and simplicity

## Requirement

* c++20 or later
  * Tested on:
    * g++10 std=c++2a
    * g++11 std=c++2a
    * clang++11 std=c++2a
    * clang++10 std=c++2a

* Catch2 for testing

## contents

* A matrix_<> class `(check docs/about_matrix.md)`

* namespace `kraken` which has:-

  * namespace `constants` : contains mathematical and physical constants presented as symbols, just like in julia
  * namespace `cal`       : contains functions that do some calculations
  * namespace `op`        : contains variadic function objects
  * and a bonus namespace! `num_methods` : contains implementation of some numerical methods

## Installation

1. Download the library.
2. Make it so that the compiler will find the `<Kraken/all.hpp>` header.

3. Test it by compiling and running the Kraken “Pi” example.

## 1. Download

* A simple way is to download this library from `github` as `zip` then `unzip` it maybe using your os built-in unzip tool but the preferred way is to use [PeaZip][] for windows and linux, for mac you can use [keka][]

## 2. Make the compiler find the `<Kraken/all.hpp>` header

* If you downloaded it ZIP and unzipped it, then in the unzipped library the root of the header folder hierarchy is `Kraken-master/source/library`.

* I.e. there is a `Kraken-master/source/library/all.hpp` header, which is the one that you want the compiler to find as <Kraken/all.hpp>.

### A simple way is to do that

* copy the `Kraken-master/source/library` folder from the unzipped archive, to a folder that your compiler searches for headers.

A that folder is
> /usr/include

### So, how to do that?

## Debian

### If you're on `Ubuntu/POP!_os/any debian based os/` you can do the following :-

* Ui version
  * if you're using `nautilus` file manager
    * open terminal then do `sudo apt install nautilus-admin` then when it's done do `nautilus -q`
  * if your're using `nemo` no need to install anything `run as-administer` is built-in

* open `usr/include` folder as `administrator`
* paste `library` folder into `usr/include`, you may rename library into `Kraken` after you're done!

* Terminal version
  * if you have any file manger installed you can open a folder as administer as follows:
    `sudo` **(your-file-manager)** ` /usr/include `
  * paste `library` folder into `usr/include`, you may rename library into `Kraken` after you're done!

## windows

### If you're on `Windows` :-

* in Windows for the MinGW g++ compiler you might have a folder such as
  > C:\installed\mingw\nuwen\MinGW **(version)**\include
* just paste the `Kraken` folder inside

* and in Windows for the Visual C++ compiler you might have a folder such as
  > C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\ **(version)** \include
* again just paste the `Kraken` folder inside

## 3. Check if the library is working by running this code

* ```cpp
  #include <Kraken/all.hpp>
  #include <iostream>

  using namespace kraken;

  auto main()
      -> int
  {
    std::cout << constants::π << '\n';
  }
  ```

[cmath]: https://en.cppreference.com/w/cpp/header/cmath
[PeaZip]: https://peazip.github.io/
[keka]: https://www.keka.io/en/
