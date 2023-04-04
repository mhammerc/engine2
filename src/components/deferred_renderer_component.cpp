#include "deferred_renderer_component.h"

using namespace engine;

auto reflection::register_deferred_renderer_component() -> void {
    auto factory = entt::meta<DeferredRendererComponent>();
    factory.prop("name"_hs, "DeferredRendererComponent");

    factory.data<&DeferredRendererComponent::destination>("destination"_hs).prop("name"_hs, "destination");
    factory.data<&DeferredRendererComponent::position>("position"_hs).prop("name"_hs, "position");
    factory.data<&DeferredRendererComponent::size>("size"_hs).prop("name"_hs, "size");
    factory.data<&DeferredRendererComponent::camera>("camera"_hs).prop("name"_hs, "camera");
    factory.data<&DeferredRendererComponent::wireframe>("wireframe"_hs).prop("name"_hs, "wireframe");
    factory.data<&DeferredRendererComponent::exposure>("exposure"_hs).prop("name"_hs, "exposure");
    factory.data<&DeferredRendererComponent::tone_mapping>("tone_mapping"_hs).prop("name"_hs, "tone_mapping");
}