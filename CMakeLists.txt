cmake_minimum_required(VERSION 3.20)
project(opengl_engine VERSION 0.1.0)

# Tests
include(CTest)
enable_testing()

# Executable and sources
add_executable(opengl_engine src/mesh/mesh.h src/mesh/mesh.cpp src/model/model.h src/model/model.cpp)

set(SOURCES
        src/camera/camera.cpp
        src/camera/camera.h
        src/engine.h
        src/game_loop.cpp
        src/glfw/glfw.cpp
        src/glfw/glfw.h
        src/gui/gui.cpp
        src/gui/gui.h
        src/main.cpp
        src/opengl/opengl.cpp
        src/opengl/opengl.h
        src/scene/cube.h
        src/scene/light.h
        src/scene/scene.cpp
        src/scene/scene.h
        src/shader/shader_program.cpp
        src/shader/shader_program.h
        src/shader/shader_source.cpp
        src/shader/shader_source.h
        src/texture/image.cpp
        src/texture/image.h
        src/texture/texture.cpp
        src/texture/texture.h
        src/utils/platform_info.cpp
        src/utils/platform_info.h
        src/utils/read_file_to_string.cpp
        src/utils/read_file_to_string.h
        src/vertex_array_object/vertex_array_object.cpp
        src/vertex_array_object/vertex_array_object.h
        src/vertex_buffer_object/vertex_buffer_object.cpp
        src/vertex_buffer_object/vertex_buffer_object.h
        src/vertex_buffer_object/vertex_buffer_object_from_cube.cpp
        )

target_sources(opengl_engine PRIVATE ${SOURCES})

# C++20
set_property(TARGET opengl_engine PROPERTY CXX_STANDARD 20)

# spdlog
add_subdirectory(vendors/spdlog)
target_link_libraries(opengl_engine PUBLIC spdlog)

# GLFW
add_subdirectory(vendors/glfw)
target_link_libraries(opengl_engine PUBLIC glfw)

# Open Asset Import Library (assimp)
add_subdirectory(vendors/assimp)
target_link_libraries(opengl_engine PUBLIC assimp)

# GLM
set(glm_DIR vendors/glm/cmake/glm)
find_package(glm REQUIRED)
target_link_libraries(opengl_engine PUBLIC glm::glm)

# ImGUI
set(IMGUI_SOURCES
        vendors/imgui/imgui/imgui.cpp
        vendors/imgui/imgui/imgui.h
        vendors/imgui/imgui/imgui_internal.h
        vendors/imgui/imgui/imgui_impl_glfw.cpp
        vendors/imgui/imgui/imgui_impl_glfw.h
        vendors/imgui/imgui/imgui_impl_opengl3.cpp
        vendors/imgui/imgui/imgui_impl_opengl3.h
        vendors/imgui/imgui/imgui_impl_opengl3_loader.h
        vendors/imgui/imgui/imconfig.h
        vendors/imgui/imgui/imgui_demo.cpp
        vendors/imgui/imgui/imgui_draw.cpp
        vendors/imgui/imgui/imgui_tables.cpp
        vendors/imgui/imgui/imgui_widgets.cpp
        vendors/imgui/imgui/imstb_rectpack.h
        vendors/imgui/imgui/imstb_textedit.h
        vendors/imgui/imgui/imstb_truetype.h
        )
target_sources(opengl_engine PRIVATE ${IMGUI_SOURCES})
target_include_directories(opengl_engine PRIVATE vendors/imgui)

# stb_image
set(STB_IMAGE_SOURCES
        vendors/stb_image/stb_image/stb_image.cpp
        vendors/stb_image/stb_image/stb_image.h
        )
target_sources(opengl_engine PRIVATE ${STB_IMAGE_SOURCES})
target_include_directories(opengl_engine PRIVATE vendors/stb_image)

# GLAD and OpenGL
set(GLAD_SOURCES
        vendors/glad/glad/glad.cpp
        vendors/glad/glad/glad.h
        vendors/glad/glad/khrplatform.h
        )
target_sources(opengl_engine PRIVATE ${GLAD_SOURCES})
target_include_directories(opengl_engine PRIVATE vendors/glad)
