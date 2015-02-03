project(FuncationMesh)

set(CMAKE_CXX_COMPILER g++4.8)
set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(functionalMesh main.cxx MeshView.cxx)
