namespace engine {

/**
 * This empty component marks an entity as being selected in the editor.
 */
struct EditorSelectedComponent {};

namespace reflection {
    auto register_editor_selected_component() -> void;
}

}  // namespace engine