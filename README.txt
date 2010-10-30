A collection of C++ libraries / utilities. 
A very lightweight wrapper maps basic types to allow tight integration with openFrameworks (www.openframeworks.cc) and Cinder (www.libcinder.org).
See www.msalibs.org for demos.

MSABulletPhysics
-------------------------
A simple to use C++ wrapper for the bullet physics engine. See the provided example for usage. Not all functionality is wrapped, but you have access to underlying bullet objects so you can still do everything you want by combining the wrappers and underlying bullet objects.


MSACore
-------------------------
This contains core utility / math functions requred by most other classes. Also is the glue that maps the MSA types to openFrameworks or Cinder types. Pretty much every class will require this.


MSADataProtector
-------------------------
Simple way to check the hash of a resource file to make sure it hasn't been tampered with.


MSAFluid
-------------------------
Classes for solving and drawing 2D fluid systems based on Jos Stam's methods and code. Now much faster than before.
Demo at http://www.msavisuals.com/msafluid


MSAInterpolator (used to be called MSASpline)
-------------------------
A set of template classes for doing various types of interpolations on data with any number of dimensions. You can feed the system an arbitrary number of data, then resample at any resolution, or ask for the value at any percentage along the data. Input data can be floats (for 1D splines, Vec2f (for 2D splines), Vec3f (for 3D splines), or even matrices, or custom data types (e.g. biped pose).
Demo at http://www.msavisuals.com/msainterpolator


MSAObjCPointer
-------------------------
A simple ObjectiveC style Smart Pointer for C++ which keeps reference count and frees memory when reference count is zero. This is accomplished by providing retain/release methods similar to Objective C.


MSAOpenCL
-------------------------
A very simple to use C++ wrapper for OpenCL. All underlying openCL objects are accessible to allow advanced features too if need be.
Demo (1M particles @ 100-200 fps) at http://vimeo.com/7332496


MSAPerlin
-------------------------
Ken Perlin's noise and fractal noise (upto 3 dimensions).


MSAPhysics
-------------------------
A template based particle/constraint based physics library for openFrameworks with springs, attractors and collision. It uses a very similar api to the traer.physics library for processing to make getting into it as easy as possible. All classes are template based with typedefs for 2D and 3D physics. Potentially more. 
Demo at http://www.msavisuals.com/msaphysics


MSAPingPong
-------------------------
A handy template class for creating double-buffered objects suitable for PingPonging (e.g. FBOs, images, etc.). Example used by MSAOpenCL


MSAShape3D
-------------------------
A C++ wrapper for Vertex Arrays & Vertex Buffer Objects (coming soon) to allow Immediate mode style syntax on embedded systems / iPhone etc.
Demo at http://www.msavisuals.com/msashape3d


MSATimer
-------------------------
A class to keep track of ultra high resolution time on posix systems using mach_absolute_time (using host timing on windows and linux)


MSAUtils
-------------------------
A collection of small and useful utility C++ classes and functions. (will eventually be phased out into MSACore and/or other libraries)


ofxCocoa
-------------------------
A GLUT replacement add-on for openFrameworks 006+ to use native Cocoa OpenGL and windowing instead of GLUT. Can provide upto 200% better performance in certain situations (multiscreen + fullscreen)
Supports Fbos, shaders, FSAA, setting window stylemasks/decorations/transparency, programmatic windows, Interface Builder integration, fullscreen on single or more screens, NSTimer or CVDisplayLink callback.


ofxMSADirManager
-------------------------
openFrameworks only addon to manage a folder of resources of a certain type. (e.g. get next resource, get previous, get random etc.)


ofxMSAInteractiveObject
-------------------------
openFrameworks only addon which wraps up some poco functionality to make flash-like objects which auto updates/draws and adds mouse methods like onRollOver, onPress, OnRollout? etc with bounds checking with easy to remember functions to register/un-register callbacks.


ofxMSAThreadedImageSaver
-------------------------
openFrameworks only addon extension to ofImage which takes a screengrab and saves the file in a separate thread so it can encode and save the file without interrupting the app.


ofxSimpleGuiToo
-------------------------
openFrameworks only addon simple and very quick to setup GUI based on Todd Vanderlin's ofxSimpleGui. It uses a very similar (almost identical) API, but with a rewritten backend.