#ifndef APOLOGY_HPP
#define APOLOGY_HPP

/// @version v0.4.8

/*

MIT License

Copyright (c) 2021 yahya mohammed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#if __cplusplus > 201703L

#if defined(_MSC_VER)
  #include <source_location>
  using src_loc = const std::source_location;
#endif

#if defined(__clang__) || defined(__GNUG__)
  #include <experimental/source_location>
  using src_loc = const std::experimental::source_location;
#endif

#endif // cplusplus standart must be 20

#ifdef APOLOGY

#include <string_view>
#include <unordered_map>
#include <iostream>

struct Err {
  std::string_view file_name{};
  std::string_view func_name{};
  int err_code{};
  std::uint32_t line{};
};

enum Err_codes {
  big_arg=0,
  neg_arg,
  zero,
  even,
  odd,
};

inline const
std::unordered_map<int, std::string_view> apologies
{
  { Err_codes::neg_arg, " -A negative value was given." },
  { Err_codes::big_arg,  " -A too big value was given." },
  { Err_codes::zero,  " -A zero value was given." },
  { Err_codes::even,  " -An even value was given." },
  { Err_codes::odd,  " -An odd value was given." },
};

constexpr
auto Apology = [](auto apology)
  -> bool
{
  auto [file_name, function_name, code, line] = apology();
  if ( apologies.find(code) == apologies.end() ) {
    std::cerr << "-Err_code not defined.\n";
    throw(1);
  }
  std::cerr << "[ -An error occurred in the file: `" << file_name
            << "` inside the function: `" << function_name
            << "` at line: " << line
            << " and the reason was: `"
            << apologies.at(code) << "` ]\n";
  throw(1);
};

#endif

#endif // APOLOGY_HPP