
<!-- README.md is generated from README.Rmd. Please edit that file -->

# simplecall - demo package for calling C code with `.Call()`

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg)
<!-- badges: end -->

`simplecall` is a small demo package showing how C code could be
included in a package and called with `.Call()`

## Rough comparison of `.C()`, `.Call()` and `{Rcpp}`

|                | .C()                                                   | .Call()                                                      | Rcpp                                                         |
| -------------- | ------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Overview       | No real understanding of R objects                     | Need to understand SEXP macros & internals                   | C++ classes hide the complexity of the SEXP internals        |
| What code?     | Basic C code. Numeric calcs.                           | Complex C code. Can manipulate R objects from C              | Complex C and C++ code involving numerics and R objects      |
| Pros           | Simple to understand and use                           | Simple. No unnecessary copying.                              | Great documentation. Wrapping of R objects very intuitive.   |
| Cons           | Too simple for most interesting things                 | Need to understand SEXP & R internals                        |                                                              |
| Cons           | Performs copying of data to call functions             |                                                              |                                                              |
| Demo R package | [{simplec}](https://github.com/coolbutuseless/simplec) | [{simplecall}](https://github.com/coolbutuseless/simplecall) | [{simplercpp}](https://github.com/coolbutuseless/simplercpp) |
| Compiled size  | 17 kB                                                  | 17 kB                                                        | 92 kB                                                        |

## Installation

You can install from
[GitHub](https://github.com/coolbutuseless/simplecall) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/simplecall)
```

## What’s in the box?

Package contains 2 C functions, and 2 functions in R which call the C
functions using `.Call()`.

| C function                    | R function       |
| ----------------------------- | ---------------- |
| `SEXP add_(SEXP x_, SEXP y_)` | `add_Call(x, y)` |
| `SEXP mul_(SEXP x_, SEXP y_)` | `mul_Call(x, y)` |

## What’s in the R functions?

  - A call using `.Call()`
  - First argument is the C function name e.g. `add_`
  - `x` and `y` arguments are passed through from the R function
  - `@useDynLib [packagename] [C function name]` is used to generate the
    NAMESPACE entry `useDynLib(simplecall,add_)`

<!-- end list -->

``` r
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Add two numbers
#'
#' @param x,y numbers to add
#'
#' @useDynLib simplecall add_
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_Call <- function(x, y) {
  .Call(add_, x, y)
}
```

## What’s in the C functions?

  - C function returns an `SEXP` pointer which is a real R object
  - all arguments passed as SEXP pointers

<!-- end list -->

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Add 2 numbers
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    SEXP add_(SEXP x_, SEXP y_) {
    
      int xtype = TYPEOF(x_);
      int ytype = TYPEOF(y_);
    
      if (xtype != REALSXP | ytype != REALSXP) {
        Rf_error("add_() expects only floating point numbers i.e. doubles or reals");
      }
    
      int xlen = length(x_);
      int ylen = length(y_);
    
      if (xlen != 1 | ylen != 1) {
        error("add_() expects arguments to be length = 1");
      }
    
      SEXP result = PROTECT(allocVector(REALSXP, 1));
      REAL(result)[0] = asReal(x_) + asReal(y_);
      UNPROTECT(1);
    
      return result;
    }

## What does the C function look like in R?

  - An object of class `NativeSymbolInfo`
  - Holds an `externalptr` to the loaded function

<!-- end list -->

``` r
simplecall:::add_
#> $name
#> [1] "add_"
#> 
#> $address
#> <pointer: 0x7f8b0b80ed90>
#> attr(,"class")
#> [1] "RegisteredNativeSymbol"
#> 
#> $dll
#> DLL name: simplecall
#> Filename:
#>         /Library/Frameworks/R.framework/Versions/4.0/Resources/library/simplecall/libs/simplecall.so
#> Dynamic lookup: FALSE
#> 
#> $numParameters
#> [1] 2
#> 
#> attr(,"class")
#> [1] "CallRoutine"      "NativeSymbolInfo"
```

## Resources

  - [David Vaughan’s post - Now you C
    me](https://blog.davisvaughan.com/2019/03/02/now-you-c-me/)
  - [Peter Dalgaard’s Keynote from
    UseR 2004](http://www.ci.tuwien.ac.at/Conferences/useR-2004/Keynotes/Dalgaard.pdf)
    discusses R’s language interfaces
  - [Using R — Calling C Code ‘Hello
    World’](http://mazamascience.com/WorkingWithData/?p=1067)
  - [Hadley’s Advanced R book chapter - ‘Rs C
    interface’](http://adv-r.had.co.nz/C-interface.html)
  - [Rcpp](https://cran.r-project.org/package=Rcpp)

## Acknowledgements

  - R Core for developing and maintaining such a wonderful language.
  - CRAN maintainers, for patiently shepherding packages onto CRAN and
    maintaining the repository
