// I understand how game HDR works and what tone mappings does. But I do not understand what ACES
// is, what white-points are (D60, D65) etc etc.
// This is why I use this code without understanding it much.

// There is two ACES implementation: one more precise, one less precise and more saturate colors.

// The code in this file was originally written by Stephen Hill (@self_shadow), who deserves all
// credit for coming up with this fit and implementing it.

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
const mat3 ACESInputMat = mat3(0.59719, 0.35458, 0.04823, 0.07600, 0.90834, 0.01566, 0.02840, 0.13383, 0.83777);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
const mat3 ACESOutputMat = mat3(1.60475, -0.53108, -0.07367, -0.10208, 1.10813, -0.00605, -0.00327, -0.07276, 1.07602);

vec3 RRTAndODTFit(vec3 v) {
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return a / b;
}

vec3 aces_fitted(vec3 color) {
    color = clamp(color, 0., 2000.);

    color = color * ACESInputMat;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = color * ACESOutputMat;

    color = clamp(color, 0., 1.);

    return color;
}

// This ACES approximation was written from Krzysztof Narkowicz and sourced here:
// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/

vec3 aces_approx(vec3 v) {
    v = clamp(v, 0., 2000.);

    v *= 0.6;

    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;

    return clamp((v * (a * v + b)) / (v * (c * v + d) + e), 0.0, 1.0);
}