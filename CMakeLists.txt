cmake_minimum_required(VERSION 3.18.1)

project(HelloGL VERSION 0.0.1)

# 使用 C++ 17 标准
set(CMAKE_CXX_STANDARD 17)

# 设置代码文件目录
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# 添加头文件
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
include_directories(${HEADER_DIR}/ ${LIB_DIR}/)

set(GLFW_LINK ${LIB_DIR}/libglfw.3.dylib)
set(ASSIMP_LINK_0 ${LIB_DIR}/libassimp.3.1.1.dylib)
set(ASSIMP_LINK_1 ${LIB_DIR}/libassimp.3.dylib)
set(ASSIMP_LINK_2 ${LIB_DIR}/libassimp.dylib)

set(SOURCES ${SRC_DIR}/glad.c ${SRC_DIR}/main.cpp)
add_executable(HelloGL ${SOURCES})

# 链接GLFW库
target_link_libraries(HelloGL ${GLFW_LINK} ${ASSIMP_LINK_0} ${ASSIMP_LINK_1} ${ASSIMP_LINK_2})
# 链接系统的 OpenGL 框架和其他必要的库
if (APPLE)
    target_link_libraries(HelloGL "-framework OpenGL")
    target_link_libraries(HelloGL "-framework Cocoa")
endif()
