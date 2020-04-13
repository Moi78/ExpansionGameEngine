# ExpansionGameEngine

Expansion Game Engine is a little 3D game engine in developpement.

Target List
 - Multi-API rendering (OpenGL, Vulkan, and DirectX11/12 if I want to export to XBox)
 - Going PBR
 - Lightweight build (<30MB for all engine files) & SDK (<300MB for the whole sdk)
 - Light and Shadow fully realtime
 - Deffered rendering
 - Cross-Platorm (Windows, Linux, Console, maybe MacOS)
 - Optimise as f\*ck -> I want to have a very well optimised engine to run everywhere
 - An easy to understand & easy to use engine (w/ an editor and everything that goes with)
 - JS Scripting

TODO List :

 - Check compatibility with linux (and make it compatible)
 - Reduce RAM usage
 - Impl. Shadows to Raindrop Renderer
 
 Dependencies :
  - OpenAL Soft (by @Kcat)
  - GLFW3
  - OpenGL
  - ASSIMP (For tools)
  - Qt (For tools)
  - GLAD
  - GLEW
  - GLM
  - libsnd
  - Bullet Physics
  NB1 : All libs that are precompiled and in this repo are compiled for Windows x64. If you want to compile this engine for Windows x86 you need to do it yourself. If you want to compile this engine for linux you need to install deps via your package manager.
  NB2 : libsnd for windows is not provided by me, you have to install it via the library website.
  NB3 : CMakeLists.txt are specially made for Linux, if you want to compile for Windows, you can just use my Visual Studio Solution (I normally update it at each VS major release, for now it is a VS2019 solution)
