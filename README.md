# APR_cpp_project_example

Simple example C++ project that uses LibAPR (https://github.com/AdaptiveParticles/LibAPR), the current tested path involves using static linking of the libarary.

Step 1: Go to LibAPR repo (https://github.com/AdaptiveParticles/LibAPR) and follow instructions for your operating system to install LibAPR to default location.

Step 2: From the root directory of this cloned repo, do the following:

##Windows (Clang)

This assumes you have installed LibAPR using clang, and vcpkg as per instructions, with your vcpkg dependencies 
"PATH_TO_VCPKG_DEPENDENCIES"= *PATH_TO_VCPKG*\vcpkg\scripts\buildsystems\vcpkg.cmake

``
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A x64 -T ClangCL -DCMAKE_TOOLCHAIN_FILE="PATH_TO_VCPKG_DEPENDENCIES"
      -DVCPKG_TARGET_TRIPLET=x64-windows ..
cmake --build . --config Release
``

##Unix

``
mkdir build
cd build
cmake ..
cmake --build . --config Release
``
or make. 






