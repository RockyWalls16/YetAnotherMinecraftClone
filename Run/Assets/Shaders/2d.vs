#version 330 core
in vec3 aPos;
in vec2 aTex;

out vec2 outTex;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
    outTex = aTex;
}