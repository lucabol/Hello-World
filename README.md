# Hello World

Test repo for JediMaster

## Building and Testing

### Normal Build
```bash
make
./hello
```

### Unit Testing

The `hello.c` file supports unit testing through conditional compilation using the `UNIT_TEST` macro.

When compiled with `-DUNIT_TEST`, the `main()` function is excluded, allowing the code to be linked with a separate test runner that defines its own `main()`. This enables testing of the `get_greeting()` function in isolation.

To run unit tests:
```bash
make unit-test
```

The unit tests compile `hello.c` as a library (with `-c -DUNIT_TEST`) and link it with `test/test_hello.c`, which contains the test cases and test runner's `main()` function.