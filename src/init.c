
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// In https://github.com/hadley/r-internals , Hadley recommends
// "use R_NO_REMAP so all API functions have the prefix R_ or Rf_"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// #define R_NO_REMAP
#include <R.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Again from: https://github.com/hadley/r-internals
// Including <Rinternals.h> seems like bad form. However, it doesn't actually
// give you access to the "internal" internal API unless you set some additional
// flags. Instead it lets you access the "public" internal API, which is both
// necessary and safe. Yes, this is confusing.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <Rinternals.h>


// This file presents 3 ways of registering all the C functions
//
// Methods
// 1. Declare a generic R init, but don't pass in any R_CallMethodDef or
//    R_CMethodDef.  Functions will still be callable, but no checking for
//    the correct number of arguments will be done.  This can lead to segfaults
//    if you aren't careful
//
//
// 2. Construct function information by hand, by carefully declaring the
//    name of the function, its address and the number of arguments it takes
//
//
// 3. Use pkgbuild to auto generate this init.c file from source
//    (a) delete this current src/init.c
//    (b) run `pkgbuild::compile_dll(force = TRUE, register_routines = TRUE)`
//    (c) a new `src/init.c` has been generated for you!



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// [Method 1] This is a very generic bit of DLL registration boilerplate
//
// By initiating in this way, R has no idea how many arguments each
// function takes.  This can add some flexibility, but will also crash your
// R session if you are not careful
//
// Change the '_simplecall' suffix to match your package name
//
// Reference:
// https://stackoverflow.com/questions/42313373/r-cmd-check-note-found-no-calls-to-r-registerroutines-r-usedynamicsymbols#42339658
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void R_init_simplecall(DllInfo *info) {
//   R_registerRoutines(info, NULL, NULL, NULL, NULL);
//   R_useDynamicSymbols(info, TRUE);
// }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// [Method 2] This is the more robust way to initialize the C code
//
// We tell R the name of the function and how many arguments it takes
//
//  1. Declare them as 'extern' as they're defined in a different location
//  2. Define the name and address and number of arguments
//  3. Initialize the DLL
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP add_(SEXP x_, SEXP y_);
extern SEXP create_data_frame_in_c_(void);
extern SEXP create_list_in_c_(void);
extern SEXP create_array_in_c_(void);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// .C      R_CMethodDef
// .Call   R_CallMethodDef
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static const R_CallMethodDef CEntries[] = {
  // name                      pointer                            Num args
  {"add_"                   , (DL_FUNC) &add_                   , 2},
  {"create_data_frame_in_c_", (DL_FUNC) &create_data_frame_in_c_, 0},
  {"create_list_in_c_"      , (DL_FUNC) &create_list_in_c_      , 0},
  {"create_array_in_c_"     , (DL_FUNC) &create_array_in_c_     , 0},
  {NULL                     , NULL                              , 0}   // Placeholder to indicate last one.
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Register the methods
//
// Change the '_simplecall' suffix to match your package name
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void R_init_simplecall(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



