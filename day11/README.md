# The Program
The folder, in which this `README.md` is located in, contains the source code and the build and configuration files for a C program.

The program opens a window.
The window is presenting multiple "scenes" using a modern OpenGL backend.

The first scene displays a quadriliteral, the second scene a triangle.
The geometrics are "Malachite"-colored, the background is "Capri"-colored.

To exit the program either close the window or press `Escape` when the window has keyboard focus.
To switch between the two scenes, press `Enter`.

We introduce the concept of a scene and a scene execution environment:
A scene is a independent unit of (audial/visual/haptical) input and output which is created, destroyed, and operated by a scene execution environment.
This interaction is based on an interface between scenes and scene execution environments.

# Building the Program
To compile this program, you generate your environment-specific build files using the KitWare's CMake.

- Ensure KitWare's CMake is installed on your system and is in your path.
  To assert CMake is properly installed, open a console and enter `cmake --help`. The programs should be found and print its help text.
- Check out this repository to some folder. Henceforth we will denote that folder by the placeholder name `<source>`.
- Create a folder outsode of `<source>`. Henceforth, we will denote that folder by the placeholder name `<build>`.
- To generate the Visual C++ project files in the folder `<build>`, open a console, enter the folder `<build>` and enter `cmake <source>`.

The above will use the x64 of x86 generator depending on your system.
To specifically choose one generator, enter `cmake -A x64 <source>` for x64 and `cmake -A Win32 <source>` for x86.
