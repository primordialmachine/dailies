# The Program
The folder, in which this `README.md` is located in, contains the source code and the build and configuration files for a C program.

The program emits `Hello, World!\n` to the standard output.
Furthermore, it emits information on its startup and shutdown.
If no error occurs, it terminates with success. Otherwise it terminates with failure.

This program introduces concepts of message-based programming in the form of reference counted message and the ability to create new message types via sub-typing.
The program introduces the message type "emit" that indicates that a string, which is stored in the message of that type as a message parameter, shall be emitted.

The program creates an "emit" message with the message parameter `Hello, World!\n`.
Furthermore, it creates a message interpreter that properly interpreters the message and hence emits `Hello, World!\n`.

