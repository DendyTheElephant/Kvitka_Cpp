#version 460 core

// output
layout(location=0) out vec4 out_Color;

in float v2f_Height;

void main()
{
    vec3 LowColor = vec3(0.20,0.35,0.30);
    vec3 HighColor = vec3(0.25,0.5,0.25);
    float Interpolator = v2f_Height;
    out_Color = vec4(mix(LowColor,HighColor,Interpolator), 1);
}