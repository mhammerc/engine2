cmake_minimum_required(VERSION 3.25)
project(opengl_engine VERSION 0.1.0)

# Executable and sources
add_executable(opengl_engine)

set(SOURCES
        src/components/base_component.cpp
        src/components/base_component.h
        src/components/camera_component.cpp
        src/components/camera_component.h
        src/components/deferred_renderer_component.cpp
        src/components/deferred_renderer_component.h
        src/components/light_component.cpp
        src/components/light_component.h
        src/components/material_component.cpp
        src/components/material_component.h
        src/components/outline_component.cpp
        src/components/outline_component.h
        src/components/skybox_component.cpp
        src/core/entity.cpp
        src/core/entity.h
        src/core/game_loop.cpp
        src/core/game_loop.h
        src/core/input.cpp
        src/core/input.hpp
        src/core/noncopyable.h
        src/core/profiler.cpp
        src/core/profiler.h
        src/core/reflection.cpp
        src/core/reflection.h
        src/core/transform.cpp
        src/core/transform.h
        src/create_scene.cpp
        src/create_scene.h
        src/entity_from_model.cpp
        src/entity_from_model.h
        src/graphics/deferred_renderer/deferred_renderer.cpp
        src/graphics/deferred_renderer/deferred_renderer.h
        src/graphics/deferred_renderer/deferred_renderer_cache.cpp
        src/graphics/deferred_renderer/deferred_renderer_cache.h
        src/graphics/deferred_renderer/deferred_renderer_pass_bloom.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_geometry.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_gizmo.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_opaque.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_outline.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_postprocess.cpp
        src/graphics/deferred_renderer/deferred_renderer_pass_skybox.cpp
        src/graphics/framebuffer.cpp
        src/graphics/framebuffer.h
        src/graphics/framebuffer_cache.cpp
        src/graphics/framebuffer_cache.h
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
        src/graphics/texture/texture.cpp
        src/graphics/texture/texture.h
        src/graphics/texture/texture_cache.cpp
        src/graphics/texture/texture_cache.h
        src/graphics/texture/texture_from_file_cubemap.cpp
        src/graphics/texture/texture_image.cpp
        src/graphics/texture/texture_image.h
        src/graphics/texture/texture_internal.cpp
        src/load.cpp
        src/load.h
        src/main.cpp
        src/platform/glfw.cpp
        src/platform/glfw.h
        src/platform/opengl.cpp
        src/platform/opengl.h
        src/systems/camera_system.cpp
        src/systems/deferred_rendering.cpp
        src/systems/draw_shadow_maps.cpp
        src/systems/should_close_system.cpp
        src/systems/systems.h
        src/ui/entity_editor/entity_property_editor.cpp
        src/ui/entity_editor/ui_window_entity_editor.cpp
        src/ui/ui.cpp
        src/ui/ui.h
        src/ui/ui_window_hierarchy.cpp
        src/ui/ui_window_input_debugger.cpp
        src/ui/ui_window_profiler/ui_window_profiler.cpp
        src/ui/ui_window_scene.cpp
        src/ui/ui_window_system.cpp
        src/ui/ui_window_texture_viewer/deferred_renderer_viewer.cpp
        src/ui/ui_window_texture_viewer/draw_framebuffer.cpp
        src/ui/ui_window_texture_viewer/draw_texture.cpp
        src/ui/ui_window_texture_viewer/draw_textures.cpp
        src/ui/ui_window_texture_viewer/framebuffer_viewer.cpp
        src/ui/ui_window_texture_viewer/texture_viewer.cpp
        src/ui/ui_window_texture_viewer/ui_window_texture_viewer.cpp
        src/ui/ui_window_texture_viewer/ui_window_texture_viewer.h
        src/ui/utils.cpp
        src/ui/utils.h
        src/utils/opengl_handle_to_pointer.cpp
        src/utils/opengl_handle_to_pointer.h
        src/utils/platform_info.cpp
        src/utils/platform_info.h
        src/utils/read_file_to_string.cpp
        src/utils/read_file_to_string.h
        src/utils/type_index.h
        )

target_sources(opengl_engine PRIVATE ${SOURCES})

# C++23
set_property(TARGET opengl_engine PROPERTY CXX_STANDARD 23)

# Enable warnings, and enable warnings as errors
if (MSVC)
    target_compile_options(opengl_engine PRIVATE /W4 /WX)
else ()
    target_compile_options(opengl_engine BEFORE PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-error=unused-variable -Wno-error=unused-parameter)
endif ()

# fmt
add_subdirectory(vendors/fmt EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC fmt)

# spdlog
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(vendors/spdlog EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC spdlog)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(vendors/glfw EXCLUDE_FROM_ALL)
target_link_libraries(opengl_engine PUBLIC glfw)

# Open Asset Import Library (ASSIMP)
# set(ASSIMP_BUILD_TESTS OFF) # It generate a CMake warning
# set(ASSIMP_INSTALL OFF) # It generate a CMake warning
# Disable ASSIMP M3D and PBRT support because they mess up with stb_image
set(ASSIMP_BUILD_M3D_IMPORTER OFF)
set(ASSIMP_BUILD_M3D_EXPORTER OFF)
set(ASSIMP_BUILD_PBRT_EXPORTER OFF)
add_subdirectory(vendors/assimp EXCLUDE_FROM_ALL)
if (MSVC)
    target_compile_options(assimp PRIVATE /w)
else ()
    target_compile_options(assimp PRIVATE -Wno-error)
endif ()
target_link_libraries(opengl_engine PUBLIC assimp)

# EnTT
add_subdirectory(vendors/entt)
target_link_libraries(opengl_engine PUBLIC EnTT)

# GLM
add_subdirectory(vendors/glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)
target_link_libraries(opengl_engine PUBLIC glm::glm)

# ImGUI (must be included after GLFW because ImGUI requires it)
add_subdirectory(vendors/imgui)
target_link_libraries(opengl_engine PUBLIC imgui)

# stb_image
add_subdirectory(vendors/stb_image)
target_link_libraries(opengl_engine PUBLIC stb_image)

# stb_include
add_subdirectory(vendors/stb_include)
target_link_libraries(opengl_engine PUBLIC stb_include)

# GLAD and OpenGL
add_subdirectory(vendors/glad)
target_link_libraries(opengl_engine PUBLIC glad)
