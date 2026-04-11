# Table of Contents

* [greentea-client](#greentea-client)
  * [Compatibility](#compatibility)
  * [Terms](#terms)
    * [Test suite](#test-suite)
    * [Test case](#test-case)
    * [key-value protocol](#key-value-protocol)
* [Adding greentea-client to a project](#adding-greentea-client-to-a-project)
  * [Build support](#build-support)
  * [Building examples](#building-examples)
  * [Stream of I/O](#stream-of-IO)
    * [stdio](#stdio)
    * [Alternative I/O](#alternative-IO)

# greentea-client

[greentea-client](https://github.com/ARMmbed/greentea-client.git) is a C++ client library which can be used with the [the Greentea test tool](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-greentea). This package implements the slave side of the simple key-value protocol used for communication between the device under test (DUT) and the host. [htrun](https://github.com/ARMmbed/greentea/tree/host-tests) on the host side implements the protocol's master behaviour.

```
      DUT  <--- serial port connection --->   host
    (slave)         .                       (master)
                    .
[greentea-client]   .       [conn_process]               [htrun]
     =====          .      ================             =========
       |            .             |                         |
       |            .             |                         |
       |    {{ key ; value }}     |                         |
       |------------------------->| (key, value, timestamp) |
       |            .             |------------------------>|
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             |                         |
       |            .             | (key, value, timestamp) |
       |    {{ key ; value }}     |<------------------------|
       |<-------------------------|                         |
       |            .             |                         |
                    .
```

## Compatibility

```greentea-client``` is compatible with:
* [Greentea](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-greentea)
* [htrun](https://github.com/ARMmbed/mbed-os-tools/tree/master/packages/mbed-host-tests)

## Terms

### Test suite
A test suite is a binary containing test cases we execute on hardware. The test suite has a beginning and an end (like your ```main()``` function would. The test suite may pass, fail or be in an error state (for example if the test suite times out or there was a serial port connection problem).

### Test case

A test case is a set of actions executed to verify a particular functionality. It contains test steps, test data, precondition, postcondition for specific test scenario to verify that functionality requirement. The test case has the beginning and the end. During test case execution you will use assert macros, schedule callbacks, check for timeouts in your code. Your test cases may pass, fail or be in an error state which means something went wrong and we were unable to determine exactly what that was (you may have to check the logs).

### key-value protocol

The key-value protocol (also called ```KiVi```) is a simple protocol introduced to the ```Greentea``` test tools. It is used to send simple text messages (events) between the DUT and the host. Each message consists of a _key_ and corresponding _value_ pair. 
A ```KiVi``` message is defined as a string encapsulated between double curly braces. The key and value are separated by a semicolon (```;```).
For example, the ```{{timeout;120}}}``` string is a simple key-value message where the key "_timeout_" is associated with the value "_120_". Both ```greentea-client``` and ```Greentea``` understand this format and can detect key-value messages in a data stream. Both _key_ and _value_ are ASCII strings.
This protocol is a master-slave protocol. The host has the role of _master_ and the DUT is the _slave_.

```greentea-client``` implements the key-value protocol tokenizer and parser.

## Greentea protocol sequence and programming guide

The sequence of key-value events during a Greentea test and the use of the greentea-client API are explained in the documentation of [htrun](placeholder).

The greentea-client API is declared in [test_env.h](./include/greentea-client/test_env.h).

# Adding greentea-client to a project

## Build support

greentea-client supports CMake. To add greentea-client to a project, call
[`add_subdirectory`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html)
pointing to the relative path of the `greentea-client` project directory.
For example,

```cmake
add_subdirectory(greentea-client)
```

Then link `greentea::client` or `greentea::client_userio` to a CMake target that requires it with
[`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html).
They are explained in detail in [Stream of I/O](#stream-of-IO) below.

## Building examples

A few examples are provided and described below. To build them,

    cmake -S . -B cmake_build -GNinja
    cmake --build cmake_build

## Stream of I/O

In order to communicate with a host, a stream of input/ouput (I/O) needs to be available to
greentea-client.

### stdio

By default, greentea-client uses `<cstdio>` (`stdio.h`) from the C standard library (libc).
On a PC, this normally points to the terminal's input and output. On embedded systems,
some libc implementations support retargeting system I/O functions (e.g. to use a serial port).
For example,
* Arm Compiler 6: [Redefining target-dependent system I/O functions in the C library](https://developer.arm.com/documentation/100073/0615/The-Arm-C-and-C---Libraries/Redefining-target-dependent-system-I-O-functions-in-the-C-library?lang=en)
* newlib (included in the GNU Arm Embedded Toolchain): [Reentrant system calls](https://sourceware.org/git/?p=newlib-cygwin.git;a=blob;f=newlib/libc/include/reent.h;h=2b01fbe8f329860fc7d76b681ea063375cb7eb72;hb=415fdd4279b85eeec9d54775ce13c5c412451e08#l13)

To link an application to greentea-client and use stdio:

```cmake
target_link_libraries(my_app
    PRIVATE
        greentea::client
)
```

An example can be found in [`examples/stdio`](examples/stdio). Once you have built the examples
[as above](#building-examples), the generated executable `./cmake_build/examples/stdio/greentea-client-example-stdio`
prints a key-value pair when you run it.

### Alternative I/O

If stdio retargeting is not available or a project needs to use a different stream of I/O
from regular printing, an option is providing a custom implementation of the Greentea
I/O functions declared in [`test_io.h`](./include/greentea-client/test_io.h). In this case:

```cmake
target_link_libraries(my_app
    PRIVATE
        greentea::client_userio
)
```

Two examples showing how to implement alternative I/O are provided,
* [`examples/custom_io`](examples/custom_io)
* [`examples/pty`](./examples/pty)

Once you have built the examples [as above](#building-examples),
* the custom_io example binary `./cmake_build/examples/custom_io/greentea-client-example-custom-io`
writes a key-value pair to `out.txt`.
* the pty example binary `./cmake_build/examples/pty/greentea-client-example-pty` creates
a terminal device node (`/dev/tty*` or `/dev/pts/*` depending on the OS) that htrun can talk to.
Run the mbedhtrun command line printed by the example to run a full device-and-host demo
for Greentea. (**Note**: This example requires macOS or Linux and is skipped on Windows).
