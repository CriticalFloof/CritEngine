# CritEngine

CritEngine is an application and rendering engine that is currently supported on Windows. Not much of the engine's features are implemented currently, however this project is being actively developed.

## Getting Started
Using Visual Studio 2022 is recommended, while other platforms are currently untested.

<ins>**1. Clone the repository**<ins>

Start by cloning this repository in your preferred directory using the command: `git clone https://github.com/CriticalFloof/CritEngine`

<ins>**2. Setting up the engine**<ins>

Run one of the [shell build files](https://github.com/CriticalFloof/CritEngine/tree/scene-management/Binaries%2FDotNET%2FCritBuildTool), this will create a Visual Studio solution of the project.

If you make changes to the engine, or if you want to regenerate project files, rerun the build_workspace_vs2022.bat script file.

## Future Plans

The main goal for CritEngine is to become a fast and flexible 2D/3D engine, allowing easy customizability for its users.

**Plugin Manager**

Currently the engine is built on a monolithic structure, we're planning on creating a plugin manager which would be first class to the core engine.
This would allow easier customization of the features provided by the engine, allowing users to keep features that only matter to their projects, in the future this may also be automated during the project exporting process.
Dependency resolution is another feature we want the Plugin Manager to have, as the power of allowing developers to build upon eachother's features is important to us.

**Platform Support**

We want to work towards supporting operating systems such as: Linux, MacOS, Android and iOS. <br>
Support for additional graphics APIs are also planned: Vulkan, DirectX, and Metal.
