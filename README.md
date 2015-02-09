3D avatars
===============

# Description

School project involving Irrlicht 3D engine (C++) and Qt. The program plays a 3D scene with volleyball avatars and a ball, using given positions.

Features:
* Irrlicht scene
* Cameras
    * FPS camera
    * Dynamic camera using a given trajectory
* Trajectories
    * Real coordinates to virtual coordinates transformation
* Avatars
    * Animation according to speed (stand, walk, run)
    * Shirt number rendering on texture at run-time
* Video recording usign Revel library
* Qt controller window

# Build

Use QtCreator to build, and then run the program from a context directory using a configuration file:

```
cd context-BrazilRussia
../../code/build/debug/Avatars config_BrazilRussia_GUI.xml
```

You can find a context folder [here](http://www.pwalch.net/myfiles-public/projects/avatars3d/context-BrazilRussia.7z).
