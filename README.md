# `dynamic-segment-tree` - a template dynamic segment tree library.

## Build status

[
![
    GitHub Actions main build CI Status
](
https://github.com/gogagum/dynamic-segment-tree/workflows/build-main/badge.svg
)](https://github.com/gogagum/dynamic-segment-tree/actions/workflows/main-build.yml)
[
![
GitHub Actions dev build CI Status
](
https://github.com/gogagum/dynamic-segment-tree/workflows/build-dev/badge.svg
)](https://github.com/gogagum/dynamic-segment-tree/actions/workflows/dev-build.yml)

## Project structure:

### `DynamicSegmentTree` class

The base class of seg tree is `DynamicSegmentTree`. It is customizable 
by the following template parameters:

- `KeyT`  - indices type.
- `ValueT` - values type.
- `GetValueT` - rangeGet operation type.
- `SegGetComb` - segments combining operation.
- `SegGetInit` - a function which implements `get` operation on a segment
filled with equal elements.
- `UpdateOp` - one or two arguments operation (see description), which is
applied to elements in update operation.
- `UpdateArgT` - argument of update operation. Must be void if operation
is unary or there is no update operation.
- `Allocator` - custom alocator.

`SegGetComb` must receive either two or five arguments. In first case these two
parameters are `rangeGet` results of two combined segments (simplified version),
in second -- `rangeGet` result of two combined segments first segment beginning, 
separating index and second segment ending.

`SegGetInit` must receive either one or three arguments.
In first case this parameter is a value of a range (simplified version),
in second -- value, beginning index and ending index.

`UpdateOp` equals NoUpdateOp means that `update` operation is not supported.
For `update` operation to be supported, UpdateOp must recieve one or two
arquments. In first case one argument is an updated value. In second -- updated
value and and update operation argument. In both cases UpdateOp must return 
updated value.

`UpdateArgT` must be void id `update` operation must not be supported or if it 
must not take arguments.

### Other classes

Examples of `DynamicSegmentTree` template usage can be seen in the
`include/curried/` directory. They are:

- `DynamicMinSegmentTree`
- `DynamicMaxSegmentTree`
- `DynamicNegateSegmentTree`
- `DynamicSumSegmentTree`
- `DynamicSimpleGetSetSegmentTree`
- `DynamicAvgSegmentTree`

You can also see examples of library usage in tests or read a header of 
DynamicSegmentTree.

### Tests

To enable tests building use `DST_TESTS` option (see `CMakeLists.txt` in the
repo root directory). 

    cmake -S . -B build -DDST_TESTS=1

For VSCode default project settings are set to build tests.

### Doxygen

Doxygen documentation is configured for this project.

    doxygen Doxyfile

## Requirements:

- `C++20` (`clang 16+` or `gcc 13+`)
- `CMake 3.15+`

## Usage:

### Git submodule
To include library into your project you 
can add it as git submodule. After that you should write the following in 
your CMakeLists.txt:

    ...
    add_subdirectory(path/to/submodule/dynamic-segment-tree/)
    ...
    target_link_libraries(your_target_name dynamic-segment-tree)

### CMake FetchContent

`FetchContent` option is also available:

    ...
    include(FetchContent)
    FetchContentDeclare(
        dynamic-segment-tree
        GIT_REPOSITORY https://github.com/gogagum/dynamic-segment-tree.git
    )
    FetchContent_MakeAvailable(dynamic-segment-tree)
    ...
    target_link_libraries(your_target_name dynamic-segment-tree)

## Plans:

TODO list (with no priority):

- Add benchmarks.
