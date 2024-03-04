#version 330 core
layout (location = 0) in vec2 aPos;

varying vec2 Pos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    Pos = aPos;
}
