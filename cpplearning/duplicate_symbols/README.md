# Quick Example of "Duplicate Symbols" Error

Things to try (both in `DuplicateSymbols.cpp`):

1. Change `constexpr int a = 5;` to `int a = 5;`. This will result in *duplicate symbols* because
`const` global variables have internal linkage by default, but non-`const` global variables have external linkage
by default.
2. Take `someFunc()` out of the anonymous namespace. This will result in *duplicate symbols* because now it has
external linkage.

Note: http://www.goldsborough.me/c/c++/linker/2016/03/30/19-34-25-internal_and_external_linkage_in_c++/ is a good blog post about external/internal linkage.
