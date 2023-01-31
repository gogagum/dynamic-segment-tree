# `dynamic-segment-tree` - a template dynamic segment tree library.

## About project structure:

### `DynamicSegmentTree` class

The base class of seg tree is `DynamicSegmentTree`. It is customizable 
by the following template parameters:

- `KeyT`  - indices type.
- `ValueT` - values type.
- `SegCombiner` - segments combining operation.
- `SegInitializer` - a function which should implement `get` operation on
a segment filled with equal elements.
- `UpdateOp` - one argument operation which is applied to elements in update operation.

`SegCombiner` must receive either two or four arguments. In first case these two parameters
are `rangeGet` results of two combined segments, in second -- `rangeGet` result of two
combined segments and two segments lengths.

`SegInitializer` must receive either one or two arguments. In first case this parameter
is a value of a range, in second -- value and length.

`UpdateOp` equals void means that `update` operation is not supported. To support `update` operation
`UpdateOp` must recieve updated value and `update` argument and return result of update.

### Other classes

Examples of DynamicSegmentTree` template usage can be seen in the
`DynamicMinSegmentTree`, `DynamicMaxSegmentTree` and `DynamicSumSegmentTree` (to be extended).
You can also see examples of library usage in tests.

### Tests

To enable tests building use `BUIILD_TEST` option (see `CMakeLists.txt` inside of the root directory). 

## Requirements:

- `C++20`
- `CMake 3.15+`

## Usage:

To include library into your project you 
can add it as submodule. After that you should write the following in 
your CMakeLists.txt:

    ...
    add_subdirectory(path/to/submodule/dynamic-segment-tree/)
    ...
    target_link_libraries(your_project_name dynamic-segment-tree)

## Plans:

TODO list (with no priority):

- Separate `get` result type and `ValueT`
- Think of different overloads of functions that take value as argument.
- Do something about default filling value of segment tree constructor.
- Add benchmarks.
- Custom allocators checks.
- Support non-default_constructible functors.
- Add doxygen documentation.
- Add more classes with operations which are often used with segment tree.
- Possibly add a combine operation option which supports taking not lengths of segments but their borders.
