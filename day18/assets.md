Assets of the daily reside in the folder `assets`.

The assets include shader programs.
Shader programs are currently backend-specific.
The only backend is currently OpenGL / GLSL, consequently only OpenGL / GLSL shader programs are provided.

The shaders for OpenGL / GLSL reside in the `asset/gl`.
There is only one shader (a mega-shader) that is customized add load time by the engine.
Its vertex shader is `asset/gl/3.vs` and its fragment shader is `asset/gl3.fs`.
