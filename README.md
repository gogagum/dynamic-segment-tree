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
- `UpdateArgT` - argument of update operation. Must be void if operation
is unary or there is no update operation. 

`SegGetComb` must receive either two or four arguments. In first case these two parameters
are `rangeGet` results of two combined segments, in second -- `rangeGet` result of two
combined segments and two segments lengths.

`SegGetInit` must receive either one or two arguments. In first case this parameter
is a value of a range, in second -- value and length.

`UpdateOp` equals void means that `update` operation is not supported. To support `update` operation
`UpdateOp` must recieve updated value and `update` argument and return result of update.

### Other classes

Examples of `DynamicSegmentTree` template usage can be seen in the
`DynamicMinSegmentTree`, `DynamicMaxSegmentTree`, `DynamicNegateSegmentTree`
and `DynamicSumSegmentTree` (to be extended).
You can also see examples of library usage in tests or read a header of 
DynamicSegmentTree.

### Tests

To enable tests building use `BUIILD_TEST` option (see `CMakeLists.txt` in the
repo root directory). 

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

- Add benchmarks.
- Custom allocators checks.
- Test non-default constructible functors.
- Add doxygen documentation.
- Add more classes with operations which are often used with segment tree.
- Possibly add a combine operation option which supports taking not lengths of segments but their borders.
