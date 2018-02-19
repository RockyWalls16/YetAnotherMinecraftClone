#version 330 core
in vec3 aPos;
in vec3 aColor;
in vec2 aTex;

out vec3 outColor;
out vec2 outTex;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
    outColor = aColor;
    outTex = aTex;
}