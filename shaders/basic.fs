#version 460 core

// output
layout(location=0) out vec4 outColor;

uniform vec3 u_Color;

void main()
{
    outColor = vec4(u_Color, 1.0f);
    //outColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}