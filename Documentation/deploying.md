Deploying KDAlgorithms with CMake
=================================

A minimal CMake project showing the deployment of KDAlgorithms can be found in the *Example* subdirectory.
What it basically shows is that you need to add something along these lines to your CMakeLists.txt file:

```
add_subdirectory(3rdparty/kdalgorithms) 
target_link_libraries(Example kdalgorithms)
```

Where you replace *Example* with whatever your build target is called.

Then in your sources, you simply **#include <kdalgorithms.h>**

That's all.

