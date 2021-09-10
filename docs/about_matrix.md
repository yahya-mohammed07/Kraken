# This file contains helpful notes about `matrix.hpp` file

## Questions you might ask:-

### - Is it built from scratch?

- No, it's built upon existing data-structure called `std::array`

### - Reasons for creating matrix_<> class?

- The elements are stored in a 1D array
- It supports basic matrix and scalar operations (+,-,*)
- It supports Comparing operations (==, !=)
- Performance :-
  - Uses templates to construct size, so it's static! which makes it `fast` but not resizable
- Compile time checks :-
  - Must `row_size > 0` And `column_size > 0`

## Usage :-

### - Creating a variable of type matrix_<> :-

- `matrix_<Type, row_size, column_size> var_name`

### - Initializing a matrix variable and adding data :-

#### - - Using a constructor

- `var_name { a1, a2, a3, a3 ... }`
- `note`: number of elements must be less than `(row_size * column_size)`

#### - - Using fill method

- `fill` method fills the matrix with a certain value

### - Methods built int with `matrix_<>` class

- `at()` :- get elements in that particular position
  - Has four overloads :-
    - 1- `at(row, col)` const (read-only)
    - 2- `at(row, col)` -> auto&
    - 3- `at<row, col>()` const (read-only)
      - comes with compile time check
    - 4- `at<row, col>()` -> auto&
      - comes with compile time check
- `size()` :- get the total size of the matrix (`row_size * column_size`)
- `print()` :- prints the contests in matrix style
  - if the type was float, it will print using precession of `12`

- `fill()` :- fills the matrix with a certain value
- `transpose_squared()` :- changes its rows into columns and its columns into rows (only `squared` matrices )
- `transpose_triangular()` :- changes its rows into columns and its columns into rows (only `triangular` matrices )
- `sort()` :- sort elements in certain order
  - `if` ``size < 256`` it will use (`insertion algorithm`)
  - `else` it will use (`std::sort`)
- `swap_rows()` :- swaps rows based of user's choice
- `swap_cols()` :- swaps columns based on user's choice
- `empty()` :- returns `false` if not empty else returns `true`
- `rotate_squared<true>()` :- rotates transposed matrix into 90 degree (only for squared matrices)
- `rotate_squared<false>()` :- rotates transposed matrix into -90 degree (only for squared matrices)
- `rotate_triangular<tROW,tCOL,true>` :- rotates transposed matrix into 90 degree (only for triangular matrices)
  - `tROW,tCOL` :- are the new (row, col) after transposing
- `rotate_triangular<tROW,tCOL,true>` :- rotates transposed matrix into -90 degree (only for triangular matrices)
  - `tROW,tCOL` :- are the new (row, col) after transposing
