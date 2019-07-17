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

-# Go to [MinGW's website](http://www.mingw.org/).
-# Click "Download" in the list of tabs in top right.
-# Scroll down on this new page and click the (somewhat hidden) "Windows Download" button for `mingw-get-setup.exe`
-# Save the file when prompted and open it when it's finished downloading.
  -# Click "Install".
  -# Click "Continue".
  -# Once it's finished installing, click "Continue".
-# Once that install has completed, the "MinGW Installation Manager" will open.
  -# Click the white box next to "mingw32-base-bin" and select "Mark for Installation."
  -# Click the white box next to "mingw32-gcc-g++" and select "Mark for Installation."
  -# Click the white box next to "msys-base-bin" and select "Mark for Installation."
  -# In the top left, click "Installation," then "Apply changes."
  -# Click "Apply."
-# Once that completes, the GCC (GNU Compiler Collection) packages for `g++`, `make`, `gdb`, etc. will be installed. Now we have to tell Windows how to find them.
  -# Push the Windows key and type `path` and select "Edit the system environment variables."
  -# Click "Environment Variables" in the bottom right.
  -# Double click "Path" to edit it.
  -# Click "New" and enter `C:\MinGW\bin`
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  -# Click "New" again and enter `C:\MinGW\msys\-#0\bin`
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  -# Select one of the new entries and click "Move Up" until it's at the top. Do the same for the other entry.
  -# Click "OK" in all of the windows we just opened to save the changes.
-# That should be everything, so test your computer by opening a command window and running `g++`.
  -# Hold the Windows key and press R.
  -# Type `cmd` and push enter.
  -# Type `g++` and push enter.
  -# You should get a `fatal error: no input files` message.

## 0.1. Installing Visual Studio Code {#installing-vsc}

Really, you can use any text editor you like, I just prefer Visual Studio Code as it hooks into MinGW really easily. You can download and install it from [here](https://code.visualstudio.com/).

*Note:* Visual Studio Code may be refered to as VSCode, VS Code, VSC, or just Code in this tutorial.

## 0.2. Installing Git {#installing-git}

Git is a great source control option used by open-source developers the world over. As Aspen's source code is hosted on Github and it updates often, it's best to get it via a `git clone` rather than just downloading it once so we can update it easily. Git for Windows can be downloaded and installed from [here](https://gitforwindows.org/).

## 0.3. Downloading the SDL Development Libraries {#downloading-sdl}

Aspen uses SDL and a few of its extensions for graphics, audio, and fonts. Install them with these steps:

-# Download the SDL development libraries from [here](https://www.libsdl.org/release/SDL2-devel-2.0.9-mingw.tar.gz).
-# Download the SDL2_image development libraries from [here](https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-mingw.tar.gz).
-# Download the SDL2_ttf development libraries from [here](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-mingw.tar.gz).
-# Download the SDL2_mixer development libraries from [here](https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz).
-# Windows doesn't have a way to unpack `.tar.gz` files by default, so install [7-zip](https://www.7-zip.org/), [WinRAR](https://www.rarlab.com/download.htm), or some other program that can (I prefer 7-zip).
-# Extract all 4 of those `.tar.gz` files into a new folder.
  * It should then contain `docs`, `i686-***`, `test`, and `x86_64-***`
-# Now we'll copy the correct files into where we installed MinGW.
  * If you are running a 64-bit version of Windows, open `i686-w64-mingw32` and copy everything into `C:\MinGW\`.
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  * If you are running a 32-bit version of Windows, open `x86_64-w64-mingw32` and copy everything into `C:\MinGW\`.
    * Change `C:` to another drive if you installed MinGW on a secondary drive.
    * Change `C:\MinGW\` if you installed MinGW to a custom path.
  * If you are unsure what version of Windows you're running, try the 64-bit version first, then if it doesn't work, try the 32-bit version instead.

## 0.4. Downloading Aspen {#downloading-aspen}

Now let's finally download Aspen!

-# Open a new command window (Windows+R, `cmd`) and navigate to the folder you want to work within with `cd FOLDERNAME`.
  * For example, if you're going to be working in `C:\Users\YOURNAME\Documents`, just type `cd Documents` and push enter.
-# Type `git clone https://github.com/BtheDestroyer/Aspen --recurse-submodules` to download Aspen and it's submodules (other projects it uses) into `CURRENTFOLDER\Aspen`.
-# Type `cd Aspen` to enter the newly downloaded folder.
-# Type `make project` to try building the example project.
-# If you got no errors, you should now have a `build` folder in Aspen's folder.
  -# If you got errors about SDL, redo [0.3](#downloading-sdl).
  -# If you got errors about imgui or imgui_sdl, delete the `Aspen` folder you created and reclone the project with `--recurse-submodules`.
-# This will contain a file called `aspen.exe` which you can run to see the example project.
  -# If you get an error with the title "aspen.exe - Entry Point Not Found", go back to the folder from [0.3](#downloading-sdl) that you copied into your MinGW instalation (either `i686-***` or `x86_64-***`) and open the `bin` folder.
  -# Copy all of the `.dll` files into `Aspen\build`.
    * The ones I've found 
  -# Try running `aspen.exe` again.

Congratulations! You can now build your own Aspen project by replacing `Aspen\src\main.cpp` with whatever code you want!

# 1. Aspen's "Hello World" {#hello-world}

## 1.1. Creating a Window {#hello-window}

Let's start by first emptying `main.cpp`. Open VS Code and click and drag the entire `Aspen` folder into it. It should refresh itself with a new display on the left listing all of the folders and their files that you have access to. Open `src` and then double click `main.cpp` to open it. You may get a notification in the bottom right about a "C++ extension" you can install - you should install it now, then click the "two papers" icon in the top left to switch back to the folder view.

Now that `main.cpp` is open, push Ctrl+A to select everything and push backspace to delete it.

What you should know by now is that every program needs a `main` function, so let's make one:

~~~~~~~~~~~~~{.cpp}
// If you don't know why there's argc and argv here, don't worry about it
int main(int argc, char **argv)
{

    return 0;
}
~~~~~~~~~~~~~

The first thing we want to do is start up Aspen's Engine. Go ahead and add the following `include` and code:

~~~~~~~~~~~~~{.cpp}
#include "Engine.cpp"

int main(int argc, char **argv)
{
    // START_FLAGS::ALL tells the Engine object to initialize all of its built-in modules
    Aspen::Engine::Engine engine(Aspen::Engine::START_FLAGS::ALL);
    return 0;
}
~~~~~~~~~~~~~

Finally, we have to have some kind of loop to keep our game from closing immediately:

~~~~~~~~~~~~~{.cpp}
#include "Engine.cpp"

int main(int argc, char **argv)
{
    Aspen::Engine::Engine engine(Aspen::Engine::START_FLAGS::ALL);
    while (engine) // While the engine is valid and active...
        engine();  // Continue running it
    return 0;
}
~~~~~~~~~~~~~

Now open a terminal from within VS Code with Ctrl+\` and type `make run` to build and run your project. This should create a graphics window with an "Object Tree" subwindow and output a bunch of information to the terminal.

## 1.1. Creating a GameState {#hello-gamestate}

Aspen uses GameStates to differentiate between different... game states.

* If you're coming from Unity, think of them like different Scenes.
* If you're coming from Clickteam Fusion, think of them like different Frames.

In order to create our own GameState, we can make a new `class` that inherits `Aspen::GameState::GameState`

~~~~~~~~~~~~~{.cpp}
// main.cpp

// other includes...

#include "GameState.hpp"
#include <string>

class MyState : public Aspen::GameState::GameState
{
public:
    // Any Object in Aspen needs to take a parent pointer and name, then pass it to its base class
    MyState(Aspen::Object::Object *parent = nullptr, std::string name = "My State")
      : Aspen::GameState::GameState(parent, name)
    {
        // TODO
    }
};

// other code
~~~~~~~~~~~~~

Now we can create *other* objects within our state's constructor. Let's start with something basic - a black square. I'll split this up over multiple lines so each step can be commented

~~~~~~~~~~~~~{.cpp}
// main.cpp

// includes

class MyState : public Aspen::GameState::GameState
{
public:
    MyState(Aspen::Object::Object *parent = nullptr, std::string name = "My State")
      : Aspen::GameState::GameState(parent, name)
    {
        // Create a new child
        AddChild(
            // The child is a new rectangle
            new Aspen::Graphics::Rectangle(
                // x = 0, y = 0, width = 32, height = 32
                SDL_Rect({0, 0, 32, 32}),
                // It's black and filled
                Aspen::Graphics::Colors::BLACK, true,
                // this is its parent and its name is "Rectangle" (this can be omitted)
                this, "Rectangle"
                )
            );
    }
};

// other code
~~~~~~~~~~~~~

Now we just have to add our GameState to the list of states within the Engine. We can do this by adding it to the GameStateManager. Again, this is split across multiple lines for commenting

~~~~~~~~~~~~~{.cpp}
// main.cpp

// includes

// our class

int main(int argc, char **argv)
{
    Aspen::Engine::Engine engine(Aspen::Engine::START_FLAGS::ALL);

    // Find the GameStateManager from the list of immediate children of the Engine (ie: its core modules)
    engine.FindChildOfType<Aspen::GameState::GameStateManager>()
        // Then load a state of the type MyState
        ->LoadState<MyState>
        // Activate it when it's loaded
            (true);

    while (engine)
        engine();
    return 0;
}
~~~~~~~~~~~~~

Now go ahead and `make run` again to build and run your project and there should be a black square in the top left (0, 0).

## 1.2. The Debugger {#hello-debugger}

Now time to talk about that "Object Tree" submenu. It's actually the Aspen::Debug::Debug object (which is really just a nice wrapper around imgui).

Click on the different objects in the tree to view their children and try to find the rectangle we made. By default, most objects have a Aspen::Transform::Transform child which allows you to get and set their position, rotation, and scale. Try messing with these values within the debugger to see how they affect your Rectangle.

You can also view certain uneditable properties of objects. This will be useful when trying to actually debug certain objects that may not behave as you intended (such as animations not containing the correct number of frames).

## 1.3. Logs {#hello-logs}

Speaking of debugging, let's try debug printing to get some information to the terminal. Aspen has its own wrapper around the normal C++ printing methods in the namespace Aspen::Log. We can call them like this:

~~~~~~~~~~~~~{.cpp}
// ... code ...

// [####] is the current line number
Aspen::Log::Debug("Hello"); // Prints "[####] DBG: Hello"
Aspen::Log::Info("Hello"); // Prints "[####] INF: Hello"
Aspen::Log::Warning("Hello"); // Prints "[####] WAR: Hello"
Aspen::Log::Error("Hello"); // Prints "[####] ERR: Hello"

// ... code ...
~~~~~~~~~~~~~

If you want to disable printing certain levels of messages, you can use `Log.TogglePrint()`:

~~~~~~~~~~~~~{.cpp}
Aspen::Log::Debug("Hello"); // Prints "[####] DBG: Hello"
Aspen::Log::Info("World"); // Prints "[####] INF: World"

Aspen::Log::Debug.TogglePrint(); // Toggles Debug off

Aspen::Log::Debug("Hello"); // Doesn't print
Aspen::Log::Info("World"); // Prints "[####] INF: World"
~~~~~~~~~~~~~

We can also set an output file with the following line. It's recommended to do this as the first thing in main before you create the Engine so you can also log any information from that to the file:

~~~~~~~~~~~~~{.cpp}
Aspen::Log::Log::SetFile("./Aspen.log");

// ... code ...
~~~~~~~~~~~~~

Finally, we can create our *own* log objects as well:

~~~~~~~~~~~~~{.cpp}
//                    message prefix      message suffix
Aspen::Log::Log MyLog("My own log: ", " That's all, folks!");

MyLog("Hello world!"); // Prints "[####] My own log: Hello world! That's all, folks!"
~~~~~~~~~~~~~

## 1.4. Custom Objects and Events {#hello-objects}

*TODO*

## 1.5. Input {#hello-input}

*TODO*

# 2. Drawing Images {#images}

*TODO*

## 2.1. Static Images {#images-static}

*TODO*

## 2.2. Animations {#images-animations}

*TODO*

# 3. Physics {#physics}

*TODO*

## 3.1. The Rigidbody {#physics-rigidbody}

*TODO*

## 3.2. Circle Colliders {#physics-circle}

*TODO*

## 3.3. Box/AABB Colliders {#physics-aabb}

*TODO*

## 3.4. Collision Events {#physics-aabb}

*TODO*

# 4. Audio {#audio}

*TODO*

# 4.1. Sound Effects {#audio-sfx}

*TODO*

# 4.2. Music {#audio-music}

*TODO*

# 5. Managing States {#state-management}

*TODO*
