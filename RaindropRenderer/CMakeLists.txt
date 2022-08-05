cmake_minimum_required(VERSION 3.7)

project(RaindropRenderer)

add_library(RaindropRenderer SHARED
    RaindropRenderer.cpp
    RaindropRenderer.h
    RD_API.cpp
    RD_API.h
    RD_ShaderLoader.h
    RD_ShaderLoader.cpp
    RD_RenderPass.h
    RD_RenderPass.cpp
    RD_Pipeline.h
    RD_Pipeline.cpp
    RD_VertexBuffer.cpp RD_VertexBuffer.h RD_Buffer.h RD_Buffer.cpp RD_UniformBuffer.cpp RD_UniformBuffer.h)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 3.3 REQUIRED FATAL_ERROR)

target_link_libraries(RaindropRenderer ${Vulkan_LIBRARY})
target_include_directories(RaindropRenderer PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(RaindropRenderer glfw)
target_include_directories(RaindropRenderer PUBLIC ${glfw_INCLUDE_DIRS})

target_include_directories(RaindropRenderer PUBLIC "../ExpansionMath/")

install(TARGETS RaindropRenderer RUNTIME DESTINATION bin)
