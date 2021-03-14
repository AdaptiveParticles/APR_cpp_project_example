cmake_minimum_required(VERSION 3.10)
project(APR_project)

set(CMAKE_CXX_STANDARD 14)

# external libraries needed for APR
find_package(HDF5 REQUIRED)
find_package(TIFF REQUIRED)

include_directories(${HDF5_INCLUDE_DIRS} ${TIFF_INCLUDE_DIR} )

find_package(OpenMP REQUIRED)

find_package(APR REQUIRED)

add_executable(HelloAPR main.cpp)

#target_link_libraries(HelloAPR  ${HDF5_LIBRARIES} ${TIFF_LIBRARIES} APR::staticLib)
target_link_libraries(HelloAPR  ${HDF5_LIBRARIES} ${TIFF_LIBRARIES} APR::sharedLib)



