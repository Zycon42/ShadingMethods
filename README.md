ShadingMethods
==============

My master thesis focused on implementing efficient shadow mapping.

source paper: http://www.cg.tuwien.ac.at/research/publications/2011/bittner-2011-scc/bittner-2011-scc-draft.pdf

dependencies
------------

  * gcc >= 4.7 || msvc >= 11
  * OpenGL 4.2.
  * GLEW
  * GLM
  * SDL 2
  * Boost 1.51.

build
-----

This project is using cmake.

GCC:

  * `mkdir build && cd build`
  * `cmake ..`
  * `make`
  
MSVC:

  * `mkdir build && cd build`
  * `cmake -G Visual Studio 11`
  * Open MSVC and build solution
