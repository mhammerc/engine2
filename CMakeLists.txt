cmake_minimum_required(VERSION 3.20)
project(opengl_engine VERSION 0.1.0)

# Tests
# include(CTest)
# enable_testing()

# Executable and sources
add_executable(opengl_engine)

set(SOURCES
        src/core/entity.cpp
        src/core/entity.h
        src/core/game_loop.cpp
        src/core/game_loop.h
        src/core/reflection.cpp
        src/core/reflection.h
        src/graphics/cube_map.cpp
        src/graphics/cube_map.h
        src/graphics/cube_map_cache.cpp
        src/graphics/cube_map_cache.h
        src/graphics/framebuffer.cpp
        src/graphics/framebuffer.h
        src/graphics/mesh.cpp
        src/graphics/mesh.h
        src/graphics/mesh_cache.cpp
        src/graphics/mesh_cache.h
        src/graphics/mesh_from_cube.cpp
        src/graphics/mesh_from_quad.cpp
        src/graphics/shader_cache.cpp
        src/graphics/shader_cache.h
        src/graphics/shader_program.cpp
        src/graphics/shader_program.h
        src/graphics/shader_source.cpp
        src/graphics/shader_source.h
        src/graphics/texture.cpp
        src/graphics/texture.h
        src/graphics/texture_cache.cpp
        src/graphics/texture_cache.h
        src/graphics/texture_image.cpp
        src/graphics/texture_image.h
        src/main.cpp
        src/platform/glfw.cpp
        src/platform/glfw.h
        src/platform/opengl.cpp
        src/platform/opengl.h
        src/scene/components/camera_component.cpp
        src/scene/components/camera_component.h
        src/scene/components/material_component.cpp
        src/scene/components/material_component.h
        src/scene/components/name_component.cpp
        src/scene/components/name_component.h
        src/scene/components/skybox_component.cpp
        src/scene/components/transform_component.cpp
        src/scene/components/transform_component.h
        src/scene/entity_from_model.cpp
        src/scene/entity_from_model.h
        src/scene/light.h
        src/scene/scene.cpp
        src/scene/scene.h
        src/ui/entity_editor/entity_property_editor.cpp
        src/ui/entity_editor/ui_window_entity_editor.cpp
        src/ui/ui.cpp
        src/ui/ui.h
        src/ui/ui_window_hierarchy.cpp
        src/ui/ui_window_scene.cpp
        src/ui/ui_window_system.cpp
        src/ui/utils.cpp
        src/ui/utils.h
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
    target_compile_options(opengl_engine BEFORE PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-error=unused-variable -Wno-error=unused-parameter)
endif ()

# spdlog
add_subdirectory(vendors/spdlog EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC spdlog)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(vendors/glfw EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC glfw)

# Open Asset Import Library (assimp)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
add_subdirectory(vendors/assimp EXCLUDE_FROM_ALL)
target_compile_options(assimp PRIVATE -Wno-error)
target_link_libraries(opengl_engine PUBLIC assimp)

# EnTT
add_subdirectory(vendors/entt)
target_link_libraries(opengl_engine PUBLIC EnTT)

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
