#version 330 core
in vec2 aPos;
in vec2 aTex;

out vec2 outTex;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    outTex = aTex;
}