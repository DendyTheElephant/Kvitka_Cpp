#version 460 core

// output
layout(location=0) out vec4 out_Color;

in float v2f_Height;

void main()
{
    // vec3 LightDir = vec3(0.5,0.5,0.0);

    vec3 WaterColor = vec3(67, 105, 127)/255.0;
    vec3 SandColor = vec3(242, 194, 118)/255.0;
    vec3 EarthColor = vec3(76, 47, 0)/255.0;
    vec3 GrassColor = vec3(102, 130, 102)/255.0;
    vec3 HightGrassColor = vec3(51, 90, 77)/255.0;
    vec3 StoneColor = vec3(186, 186, 136)/255.0;
    vec3 SnowColor = vec3(1.0, 1.0, 1.0);


    float Interpolator = v2f_Height;
    if (v2f_Height<0.07)
        out_Color = vec4(mix(WaterColor,SandColor,v2f_Height/0.07), 1);
        
    else if (v2f_Height<0.2)
        out_Color = vec4(mix(SandColor,EarthColor,(v2f_Height-0.07)/(0.2-0.07)), 1);
    else if (v2f_Height<0.204)
        out_Color = vec4(mix(EarthColor,GrassColor,(v2f_Height-0.2)/(0.204-0.2)), 1);
    else if (v2f_Height<0.34)
        out_Color = vec4(mix(GrassColor,HightGrassColor,(v2f_Height-0.204)/(0.34-0.204)), 1);
    else if (v2f_Height<0.75)
        out_Color = vec4(mix(HightGrassColor,StoneColor,(v2f_Height-0.34)/(0.78-0.34)), 1);
    else if (v2f_Height<0.80)
        out_Color = vec4(mix(StoneColor,SnowColor,(v2f_Height-0.75)/(0.80-0.75)), 1);
    else
        out_Color = vec4(mix(SnowColor, SnowColor,(v2f_Height-0.80)/(1.00-0.80)), 1);

    out_Color = vec4(HightGrassColor,1);
}