cmake_minimum_required(VERSION 3.20)
project(opengl_engine VERSION 0.1.0)

# Tests
# include(CTest)
# enable_testing()

# Executable and sources
add_executable(opengl_engine)

set(SOURCES
        src/core/game_loop.cpp
        src/core/game_loop.h
        src/core/locator.h
        src/core/resource_cache.h
        src/graphics/opengl/framebuffer.cpp
        src/graphics/opengl/framebuffer.h
        src/graphics/opengl/image.cpp
        src/graphics/opengl/image.h
        src/graphics/opengl/mesh.cpp
        src/graphics/opengl/mesh.h
        src/graphics/opengl/model.cpp
        src/graphics/opengl/model.h
        src/graphics/opengl/shader_loader.cpp
        src/graphics/opengl/shader_loader.h
        src/graphics/opengl/shader_program.cpp
        src/graphics/opengl/shader_program.h
        src/graphics/opengl/shader_source.cpp
        src/graphics/opengl/shader_source.h
        src/graphics/opengl/skybox.cpp
        src/graphics/opengl/skybox.h
        src/graphics/opengl/texture.cpp
        src/graphics/opengl/texture.h
        src/graphics/opengl/texture_loader.cpp
        src/graphics/opengl/texture_loader.h
        src/graphics/opengl/vertex_array_object.cpp
        src/graphics/opengl/vertex_array_object.h
        src/graphics/opengl/vertex_buffer_object.cpp
        src/graphics/opengl/vertex_buffer_object.h
        src/graphics/opengl/vertex_buffer_object_from_cube.cpp
        src/graphics/opengl/vertex_buffer_object_from_quad.cpp
        src/main.cpp
        src/platform/glfw.cpp
        src/platform/glfw.h
        src/platform/opengl.cpp
        src/platform/opengl.h
        src/scene/camera.cpp
        src/scene/camera.h
        src/scene/component.cpp
        src/scene/component.h
        src/scene/components/material_component.cpp
        src/scene/components/material_component.h
        src/scene/components/renderer_component.cpp
        src/scene/components/renderer_component.h
        src/scene/components/transform_component.cpp
        src/scene/components/transform_component.h
        src/scene/game_object.cpp
        src/scene/game_object.h
        src/scene/light.h
        src/scene/node.cpp
        src/scene/node.h
        src/scene/scene.cpp
        src/scene/scene.h
        src/ui/gui.cpp
        src/ui/gui.h
        src/utils/platform_info.cpp
        src/utils/platform_info.h
        src/utils/read_file_to_string.cpp
        src/utils/read_file_to_string.h
        src/utils/type_index.h
        )

target_sources(opengl_engine PRIVATE ${SOURCES})

# C++23
set_property(TARGET opengl_engine PROPERTY CXX_STANDARD 23)

# Enable warnings, and warnings as errors
if (MSVC)
    target_compile_options(opengl_engine PRIVATE /W4 /WX)
else ()
    target_compile_options(opengl_engine BEFORE PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

# spdlog
add_subdirectory(vendors/spdlog EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC spdlog)

# GLFW
add_subdirectory(vendors/glfw EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC glfw)

# Open Asset Import Library (assimp)
add_subdirectory(vendors/assimp EXCLUDE_FROM_ALL)
target_compile_options(assimp PRIVATE -Wno-error)
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
