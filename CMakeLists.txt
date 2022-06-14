cmake_minimum_required(VERSION 3.10)
project(APR_project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# external libraries needed for APR
find_package(HDF5 REQUIRED)
find_package(TIFF REQUIRED)
find_package(APR CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

#required tiff flag for APR lib
set(CMAKE_C_FLAGS "${CMAKE_CFLAGS} -DHAVE_LIBTIFF")
set(CMAKE_CXX_FLAGS "${CMAKE_CXXFLAGS} -DHAVE_LIBTIFF")

#required for openmp
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_OPENMP ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_OPENMP ${OpenMP_CXX_FLAGS}")

if(WIN32)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN_COMPILE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN_COMPILE")

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

        message(STATUS "Compiling on windows with CLANG!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -fcxx-exceptions")

        set(CMAKE_CXX_FLAGS_DEBUG "-Xclang -O0 -Xclang -g  -Xclang -Wall -Xclang -pedantic")
        set(CMAKE_CXX_FLAGS_RELEASE "/MD /EHsc /std:c++17 /arch:AVX2 -Xclang -O3 /nologo /fp:fast") #-flto=thin -march=native /O2 /Ob2

    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "Compiling on windows with MSVC!")

        set(CMAKE_CXX_FLAGS_RELEASE "/MD /EHsc /std:c++17 /arch:AVX2 /O2 /Ob2 /nologo /fp:fast")

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN_VS")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN_VS")

    endif()

else()
    #unix
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

    if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS_RELEASE "-O4 -ffast-math")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g  -Wall -pedantic")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Bdynamic")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ldl -lz")

    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g  -Wall -pedantic")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lz")
    endif()
endif()


include_directories(${HDF5_INCLUDE_DIRS} ${TIFF_INCLUDE_DIR})

add_executable(HelloAPR main.cpp)

#target_link_libraries(HelloAPR  ${HDF5_LIBRARIES} ${TIFF_LIBRARIES} APR::staticLib)
target_link_libraries(HelloAPR OpenMP::OpenMP_CXX APR::staticLib ${HDF5_LIBRARIES} ${TIFF_LIBRARIES})



