## Xcode Development

The following details how to set up this project for development in
[Xcode](https://developer.apple.com/xcode/).

1. Download the contents of this folder.
2. Open the downloaded Xcode project. The [Glut](https://www.opengl.org/resources/libraries/glut/) and [OpenGL](https://www.opengl.org/) libraries should already be linked.
4. Set the working directory for Xcode in order for the level data files to be found at runtime.
  * In the menu, navigate to *Product* -> *Scheme* -> *Edit Scheme...*.
  * Click on the *Options* tab.
  * Make sure *Working Directory* is checked, and choose the appropriate path to the project directory.
5. Download [irrKlang](http://www.ambiera.com/irrklang/downloads.html).
  * Copy `libirrklang.dylib` into `/usr/local/lib/` for the dynamic library to be used. Please refer to [this](https://developer.apple.com/library/content/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/UsingDynamicLibraries.html) for more information.

Please create a new issue if the above instructions are insufficient.

