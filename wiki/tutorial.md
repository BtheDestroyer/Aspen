Tutorial
========

# Foreward: So you've decided to try Aspen {#foreward}

NOTE: Currently this tutorial is for Windows only. I'm sorry, but I haven't had time to port everything to Linux or macOS yet (and likely won't ever port it to macOS due to [Apple trying to push Metal so hard](https://arstechnica.com/gadgets/2018/06/the-end-of-opengl-support-other-updates-apple-didnt-share-at-the-keynote/)).

Aspen is a C++ game engine designed to make writing object-oriented code for games (and other software) easier. That being said, using it won't replace basic programming knowledge. The basics of C++ you should know before continuing are:

* How to compile "hello world" with g++ (Skip to [0.0. Installing MinGW](#installing-mingw) for now, then come back)
* [Basic data types](http://www.cplusplus.com/doc/tutorial/variables/#fundamental) (`char`, `short`, `int`, `float`, etc.)
* [Operators](http://www.cplusplus.com/doc/tutorial/operators/) and [Precedence](http://www.cplusplus.com/doc/tutorial/operators/#precedence)
* [Conditionals](http://www.cplusplus.com/doc/tutorial/control/#ifelse) and [Loops](http://www.cplusplus.com/doc/tutorial/control/#while) (`if`, `&&`, `||` `else`, `while`, and `for`)
* [Functions](http://www.cplusplus.com/doc/tutorial/functions/) (`void foo(int bar);`)
* [Pointers](http://www.cplusplus.com/doc/tutorial/pointers/) (`char foo;` vs `char * foo;` vs `char ** foo;` vs `char foo; &amp;foo;`)
* [Structs](http://www.cplusplus.com/doc/tutorial/structures/)
* [Enums](http://www.cplusplus.com/doc/tutorial/other_data_types/#enumerated_types)
* [Classes](http://www.cplusplus.com/doc/tutorial/classes/)
  * [Inheritence](http://www.cplusplus.com/doc/tutorial/inheritance/#inheritance)
  * public vs protected vs private
  * Constructors
* [Namespaces](https://www.geeksforgeeks.org/namespace-in-c/)
* (optional) What a [Format String Attack](https://www.owasp.org/index.php/Format_string_attack) is and how to prevent it.
* (optional) [Recursion](http://www.cplusplus.com/articles/D2N36Up4/)
* (optional) [Makefiles](https://www.cs.bu.edu/teaching/cpp/writing-makefiles/)

Make sure that you're comfortable (or at least know the name of so you can look it up later) everything in that list before moving on. If not, you'll likely run into a lot of issues that you won't be able to solve.

# 0. Compiling Aspen {#compiling}

Let's start by just compiling the engine/library. This will walk you through installing everything you need and setting up the necessary environment before compiling the actual library.

## 0.0. Installing MinGW {#installing-mingw}

If you haven't used MinGW (because you skipped here from the Foreward) yet or don't know what its name actually means, don't worry.

* MinGW stands for "Minimum GNU for Windows."

* GNU stand for "GNU's Not Unix."

* If that's confusing, don't worry about it too much.

MinGW is just a port of a bunch of software used to compile programs from Linux (more accurately, Unix) to Windows. To install it, just follow this list:

1. Go to [MinGW's website](http://www.mingw.org/).
1. Click "Download" in the list of tabs in top right.
1. Scroll down on this new page and click the (somewhat hidden) "Windows Download" button for `mingw-get-setup.exe`
1. Save the file when prompted and open it when it's finished downloading.
  1. Click "Install".
  1. Click "Continue".
  1. Once it's finished installing, click "Continue".
1. Once that install has completed, the "MinGW Installation Manager" will open.
  1. Click the white box next to "mingw32-base-bin" and select "Mark for Installation."
  1. Click the white box next to "mingw32-gcc-g++" and select "Mark for Installation."
  1. Click the white box next to "msys-base-bin" and select "Mark for Installation."
  1. In the top left, click "Installation," then "Apply changes."
  1. Click "Apply."
1. Once that completes, the GCC (GNU Compiler Collection) packages for `g++`, `make`, `gdb`, etc. will be installed. Now we have to tell Windows how to find them.
  1. Push the Windows key and type `path` and select "Edit the system environment variables."
  1. Click "Environment Variables" in the bottom right.
  1. Double click "Path" to edit it.
  1. Click "New" and enter `C:\MinGW\bin`
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  1. Click "New" again and enter `C:\MinGW\msys\1.0\bin`
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  1. Select one of the new entries and click "Move Up" until it's at the top. Do the same for the other entry.
  1. Click "OK" in all of the windows we just opened to save the changes.
1. That should be everything, so test your computer by opening a command window and running `g++`.
  1. Hold the Windows key and press R.
  1. Type `cmd` and push enter.
  1. Type `g++` and push enter.
  1. You should get a `fatal error: no input files` message.

## 0.1. Installing Visual Studio Code {#installing-vsc}

Really, you can use any text editor you like, I just prefer Visual Studio Code as it hooks into MinGW really easily. You can download and install it from [here](https://code.visualstudio.com/).

## 0.2. Installing Git {#installing-git}

Git is a great source control option used by open-source developers the world over. As Aspen's source code is hosted on Github and it updates often, it's best to get it via a `git clone` rather than just downloading it once so we can update it easily. Git for Windows can be downloaded and installed from [here](https://gitforwindows.org/).

## 0.3. Downloading the SDL Development Libraries {#downloading-sdl}

Aspen uses SDL and a few of its extensions for graphics, audio, and fonts. Install them with these steps:

1. Download the SDL development libraries from [here](https://www.libsdl.org/release/SDL2-devel-2.0.9-mingw.tar.gz).
1. Download the SDL2_image development libraries from [here](https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-mingw.tar.gz).
1. Download the SDL2_ttf development libraries from [here](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-mingw.tar.gz).
1. Download the SDL2_mixer development libraries from [here](https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz).
1. Windows doesn't have a way to unpack `.tar.gz` files by default, so install [7-zip](https://www.7-zip.org/), [WinRAR](https://www.rarlab.com/download.htm), or some other program that can (I prefer 7-zip).
1. Extract all 4 of those `.tar.gz` files into a new folder.
  * It should then contain `docs`, `i686-***`, `test`, and `x86_64-***`
1. Now we'll copy the correct files into where we installed MinGW.
  * If you are running a 64-bit version of Windows, open `i686-w64-mingw32` and copy everything into `C:\MinGW\`.
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  * If you are running a 32-bit version of Windows, open `x86_64-w64-mingw32` and copy everything into `C:\MinGW\`.
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  * If you are unsure what version of Windows you're running, try the 64-bit version first, then if it doesn't work, try the 32-bit version instead.

## 0.4. Downloading Aspen {#downloading-aspen}

Now let's finally download Aspen!

1. Open a new command window (Windows+R, `cmd`) and navigate to the folder you want to work within with `cd FOLDERNAME`.
  * For example, if you're going to be working in `C:\Users\YOURNAME\Documents`, just type `cd Documents` and push enter.
1. Type `git clone https://github.com/BtheDestroyer/Aspen --recurse-submodules` to download Aspen and it's submodules (other projects it uses) into `CURRENTFOLDER\Aspen`.
1. Type `cd Aspen` to enter the newly downloaded folder.
1. Type `make project` to try building the example project.
1. If you got no errors, you should now have a `build` folder in Aspen's folder.
  1. If you got errors about SDL, redo [0.3](#downloading-sdl).
  1. If you got errors about imgui or imgui_sdl, delete the `Aspen` folder you created and reclone the project with `--recurse-submodules`.
1. This will contain a file called `aspen.exe` which you can run to see the example project.
  1. If you get an error with the title "aspen.exe - Entry Point Not Found", go back to the folder from [0.3](#downloading-sdl) that you copied into your MinGW instalation (either `i686-***` or `x86_64-***`) and open the `bin1 folder.
  1. Copy all of the `.dll` files into `Aspen\build`.
  1. Try running `aspen.exe` again.

Congratulations! You can now build your own Aspen project by replacing `Aspen\src\main.cpp` with whatever code you want!
