#version 330 core

uniform vec2 radius = {0,0};
uniform vec3 color = {1,1,0};

uniform vec2 center = {0,0};
varying vec2 Pos;

out vec4 FragColor;



void main()
{
    if ((Pos.x - center.x) * (Pos.x - center.x) + (Pos.y - center.y) * (Pos.y - center.y) > radius.x * radius.x ||
        (Pos.x - center.x) * (Pos.x - center.x) + (Pos.y - center.y) * (Pos.y - center.y) < radius.y * radius.y){
        discard;
    }
    FragColor = vec4(color, 1.0f);
}
