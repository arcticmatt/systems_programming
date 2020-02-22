# FlatBuffers

## Installing

    $ git clone https://github.com/google/flatbuffers.git
    $ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
    $ make
    $ ls flatc                                                                                                                                                                                                                                                                           -- INSERT --
    flatc
    $ ./flattests                                                                                                                                                                                                                                                                        -- INSERT --
    ALL TESTS PASSED

After doing this, I just moved the `flatbuffers` directory into `systems_programming`.

## Generating FlatBuffers Code

```
// Inside flatbuffers_testing/cpp
$ ./../../flatbuffers/flatc --cpp --gen-object-api ../simple.fbs

// Inside flatbuffers_testing/python
$ ./../../flatbuffers/flatc --python ../simple.fbs
```

## Demo 1: Serialize in C++, Deserialize in Python

    $ buck run //flatbuffers_testing:simple_fbs_test
    // This creates test_struct_binary.out, test_struct_as_root_binary.out, and test_table_binary.out
    
    $ cd flatbuffers_testing/python
    $ python SimpleFbsTest.py                                                                                                                                                                                   -- INSERT --
    test_struct_binary.out file contents = '\x0f\x08'
    TestStruct a = 15
    TestStruct b = 8
    test_struct_as_root_binary.out file contents = '\x06\x00\x00\x00\x00\x00\x0f\x08'
    n = 6
    TestStruct a = 15
    TestStruct b = 8
    test_table_binary.out file contents = '\x0c\x00\x00\x00\x08\x00\x08\x00\x07\x00\x06\x00\x08\x00\x00\x00\x00\x00\x08\x0f'
    TestTable a = 15
    TestTable b = 8

In this demo, the `testStructToFile`, `testStructAsRootToFile`, and `testTableToFile` functions in `SimpleFbsTest.cpp` serialize FlatBuffer objects and write the binary data to files. Then, `[SimpleFbsTest.py](http://simplefbstest.py)` deserializes the data from the files and prints the fields of the corresponding struct/table.

This demonstrates:

- How data can be serialized in one language and deserialized in another.
- The difference between deserializing:
    - A raw struct.
    - A struct serialized by writing the root offset.
    - A table with the same fields as the above two.

## Demo 2: Size Difference Between Structs and Tables

    $ buck run //flatbuffers_testing:simple_fbs_test
    // This will print out some other stuff, and then the following
    === Print TestStruct Buf Size ===
    buf size = 2
    === Print TestStruct (as root) Buf Size ===
    buf size = 0
    buf size = 2
    buf size = 8
    === Print TestTable Buf Size ===
    buf size = 0
    buf size = 1
    buf size = 2
    buf size = 16
    buf size = 20

This shows the size difference between serializing:

- A raw struct.
- A struct serialized by writing the root offset.
    - The thing that makes this take more space is the root offset, which takes 4 bytes. Need for alignment can add a few more bytes, which is what happens in our demo.
- A table with the same fields as the above two.
    - The root offset takes up 4 bytes again. The vtable is responsible for the rest of the space.

In the future I hope to do a more in-depth, byte-level dive into this topic. For now, [https://github.com/mzaks/FlatBuffersSwift/wiki/FlatBuffers-Explained](https://github.com/mzaks/FlatBuffersSwift/wiki/FlatBuffers-Explained) is a good read.

## Resources

### Links

- [https://github.com/mzaks/FlatBuffersSwift/wiki/FlatBuffers-Explained](https://github.com/mzaks/FlatBuffersSwift/wiki/FlatBuffers-Explained)
- [https://google.github.io/flatbuffers/index.html](https://google.github.io/flatbuffers/index.html)

### Notes

- "If instead of creating a vector from an existing array you serialize elements individually one by one, take care to note that this happens in reverse order, as buffers are built back to front." ([source](https://google.github.io/flatbuffers/flatbuffers_guide_tutorial.html))
