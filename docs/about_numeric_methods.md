# This file contains helpful notes about `numerical_methods.hpp` file

## - What's inside?

- Pre implemented numeric-methods (some of them only work with `matrix_<>`)
  - what numeric methods?
    - [gauss_elimination][]
    - [determined][]
    - [least_squares][]
    - [cramer][]
    - [simpson][]
    - [newton][]
    - [back_substitution][]
    - [Newton's Forward Difference Formula][]
    - and a helper function called `change_with_R`

## Note :- check the `tests` folder to look at the usage

[gauss_elimination]: https://en.wikipedia.org/wiki/Gaussian_elimination
[determined]: https://en.wikipedia.org/wiki/Determination
[least_squares]: https://en.wikipedia.org/wiki/Least_squares
[cramer]: https://en.wikipedia.org/wiki/Cramer%27s_rule
[simpson]: https://en.wikipedia.org/wiki/Simpson%27s_rule
[newton]: https://en.wikipedia.org/wiki/Newton%27s_method
[back_substitution]: https://algowiki-project.org/en/Backward_substitution#:~:text=Backward%20substitution%20is%20a%20procedure,is%20a%20lower%20triangular%20matrix.
[Newton's Forward Difference Formula]: https://mathworld.wolfram.com/NewtonsForwardDifferenceFormula.html#:~:text=the%20falling%20factorial%2C%20the%20formula,the%20development%20of%20umbral%20calculus.&text=The%20derivative%20of%20Newton's%20forward%20difference%20formula%20gives%20Markoff's%20formulas.
