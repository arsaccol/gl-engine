# [Demo video (on YouTube)](https://www.youtube.com/watch?v=ni9_wUjT-tk)
[![](https://img.youtube.com/vi/ni9_wUjT-tk/maxresdefault.jpg)](https://www.youtube.com/watch?v=ni9_wUjT-tk)

# gl-engine

Basic game engine project I'm working on, made in C++ and using OpenGL for graphics. I am using it both for learning OpenGL and graphics, and to get a sense of how to build a game engine. We'll see how that goes! 

---


## How to build
Building the program is tested on **Windows 10** using **Visual Studio 2022**.


#### Make sure you have [vcpkg](https://vcpkgio/en/index.html) set up and integrated to your Visual Studio installation
I use [vcpkg](https://vcpkg.io/en/index.html) to obtain dependency libraries for the project. You gotta have it installed and integrated to your Visual Studio installation.
Please refer to the official [vcpkg setup instructions](https://vcpkg.io/en/getting-started.html) if needed, but the gist of it is given below. Note that my project uses `vcpkg` in
[manifest mode](https://vcpkg.readthedocs.io/en/latest/users/manifests/) with a `vcpkg.json` file, so there's no need to run `vcpkg install` instructions explicitly. So do the following:

+ Clone the `vcpkg` repository somewhere with:
    >git clone `https://github.com/Microsoft/vcpkg.git`

+ Navigate to where you cloned `vcpkg`, and set it up with the provided batch file
    >cd C:\folder\where\you\cloned\vcpkg

    >.\vcpkg\bootstrap-vcpkg.bat

+ Then, integrate `vcpkg` with your Visual Studio installation
    > vcpkg integrate install

#### Finally, clone and build the project proper:

+ Clone the project with:
    > git clone `https://github.com/lalilulelost/gl-engine`
+ Double-click the `gl-engine.sln` file to open it with Visual Studio
+ Finally, press `Ctrl+Shift+B` to build it; Visual Studio will rely on `vcpkg` to check the `vcpkg.json` manifest file, and will download and build the dependency libraries before finally building the repository code itself.

