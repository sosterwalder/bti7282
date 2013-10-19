Requirements
============
* Installation of  XQuartz
    http://xquartz.macosforge.org/landing/
* Installation of FreeGLUT
    - Download from  http://freeglut.sourceforge.net/index.php#download (version 2.8.1 if possible, 2.8.0 seems to have a bug under OS X)
    - Unpack and change to unpacked folder
    - Within commandline: (whereas /opt/X11/lib is the XQuartz folder)
        env CPPFLAGS="-I/opt/X11/include" LDFLAGS="-L/opt/X11/lib" ./configure 
        make all
        sudo make install

IDE
====================
* Eclipse CDT
    - Add http://download.eclipse.org/tools/cdt/releases/kepler
      according to http://www.eclipse.org/cdt/downloads.php
      to eclipse
    - Go to project settings
      -> C/C++ Build
      -> Settings
      -> MacOS X C++ Linker
      -> Libraries:
        Libraries (-l):
          gl
          glu
          glut
        Library search path (-L)
          /usr/local/lib
          /opt/X11/lib

* MacVIM
    No changed/adjustments necessary :)


Sourcecode
==========
* Change of includes (if not already made)
    #ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #else
    #include <GL/glut.h>
    #include <GL/gl.h>
    #endif
