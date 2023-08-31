# Engine
#### C++ game engine with code interpretter, created using the raylib graphics library.

## T, Code Interpreter
T, for turtle (likely because it's probably slow), is the custom code interpreter 
created for the engine. Because it's quite a challenge to actually make a code 
compiler, and because there are some great ones out there, T can convert your
code to c++, even when working in the engine, and then compile it with g++. This
is slow, but a lot easier than actually making a compiler (which is, to my under-
standing, excrutiatingly difficult). Right now, T supports very basic language
features:

### Included Features

* Simple conditionals: >, <, ==, etc., but not and/or operators like && or |.
* Variable decleration with basic types (int, float, bool, string, list).
* Variable types that cannot be declared by the user currently (iterators and pointers).
* For loops: creating an iterator that loops from start to stop. These can be trans-
  formed into forever loops by making the end operator a colon.
* Nested conditionals/loops: Conditionals or loops that are inside other conditionals
  or loops.

### Planned Features

* Everything that a fully featured programming language should have.
* Code compiling.
* Integration with the physics and graphics engines.

## Physics
I plan to use Bullet for 3D physics and Chipmunk for 2D, because making my own
physics engine is scary!

## Graphics
I plan to use Raylib, a library for making games that makes OpenGL super simple.
I have used OpenGL a bit before, and I could never make anything beyond loading
a simple 3D model in it. Also, I find Raylib very similar to PyGame, a library
a Python graphics library which I've used quite a bit.