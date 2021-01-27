# Phoenix Engine

<br/>

![](Assets/Phoenix%20Engine%20Banner.png?raw=true)

<br/>

---
## Personal Game Engine Developed in C++19 and DirectX 11 (Dx11)

### It features the following major subsystems:-

1. [2D and 3D Forward Rendering system using DirectX 11 API](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Renderer) -
    1. [2D and 3D Particle System](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/ParticleSystem)
    2. [2D sprite sheets](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Renderer/SpriteSheet.hpp)
    3. [2D Sprite Animation support](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Renderer/SpriteAnimation.hpp)
    4. [DirectX 11 Profiler performance marking support](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Profilling)
    5. [Built-In Error Shader](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Renderer/ErrorShaderCode.hpp)
    6. [2D and 3D Debug Visualization](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Core/DebugRender.hpp)
2. [Audio System using FMOD](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Audio)
3. [Math Libraries](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Math) - includes 2D, 3D & 4D vectors (vec2, vec3, vec4) and more
4. [Primitives](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Primitives) - includes 2D & 3D Axis-Aligned bounding box (AABB2, AABB3), 2D & 3D Oriented bounding box(OBB2, OBB3) and more
5. [Job System](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Memory) - for multi threading
6. [Input System](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Input) - supports Keyboard, mouse and upto 4 Xbox Controllers
7. [Thread Safe Data Structures](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/DataStructures/ThreadSafe) -
    1. [Syncronized Lock Free Queue](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/DataStructures/ThreadSafe/SynchronizedLockFreeQueue.hpp) -  uses [std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic) for thread safety
    2. [Blocking Queue](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/DataStructures/ThreadSafe/BlockingQueue.hpp) - uses [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) for thread safety
8. [Event System](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Core/EventSystem.hpp) - basic event system
9. [Devconsole](https://github.com/neortls007idev/Engine/blob/main/Code/Engine/Core/DevConsole.hpp]) - triggers events when commands are enter in the given syntax
    1. supports keyboard navigational shortcuts
    2. supports clipboard text cut, copy and paste
10. [2D Physics system](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Physics) - can handle linear and angular impulses
    1. Disc and Convex Polygon Colliders and Rigidbodies
    2. Bounciness, Drag and Friction
11. [Network System](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Networking)- supports TCP and UDP messaging to a single connection
12. [Clocks and TImers](https://github.com/neortls007idev/Engine/tree/main/Code/Engine/Time) - for more explicit control over timing

### Third Party Librarires: -

1. [TinyXML2](https://github.com/leethomason/tinyxml2) - for parsing XML data
2. [stb Image](https://github.com/nothings/stb/blob/master/stb_image.h) - for loading image files
3. [ImGUI](https://github.com/ocornut/imgui) - for debug UI purposes
4. [ImGUI File Dialog](https://github.com/aiekick/ImGuiFileDialog) - for adding File Dialog to ImGUI
5. [ImPlot](https://github.com/epezent/implot) - for adding Graph Plotter to ImGUI

---

## [Checkout the Games and Tech developed using the Phoneix Engine](https://github.com/neortls007idev/GuildhallPublic)





