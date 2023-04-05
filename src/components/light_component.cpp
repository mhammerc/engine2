#include "light_component.h"

#include "entt/entt.hpp"

using namespace engine;

auto reflection::register_light_component() -> void {
    auto factory = entt::meta<LightComponent>();
    factory.prop("name"_hs, "LightComponent");

    factory.data<&LightComponent::type>("type"_hs).prop("name"_hs, "type");

    factory.data<&LightComponent::ambient>("ambient"_hs).prop("name"_hs, "ambient").prop("type"_hs, "color");
    factory.data<&LightComponent::diffuse>("diffuse"_hs).prop("name"_hs, "diffuse").prop("type"_hs, "color");
    factory.data<&LightComponent::specular>("specular"_hs).prop("name"_hs, "specular").prop("type"_hs, "color");

    factory.data<&LightComponent::constant>("constant"_hs).prop("name"_hs, "constant");
    factory.data<&LightComponent::linear>("linear"_hs).prop("name"_hs, "linear");
    factory.data<&LightComponent::quadratic>("quadratic"_hs).prop("name"_hs, "quadratic");

    factory.data<&LightComponent::inner_cut_off>("inner_cut_off"_hs).prop("name"_hs, "inner_cut_off");
    factory.data<&LightComponent::outer_cut_off>("outer_cut_off"_hs).prop("name"_hs, "outer_cut_off");
}
