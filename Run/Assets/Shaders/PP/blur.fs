#version 330 core
out float gl_Color;

in vec2 outTex;

uniform sampler2D ambiantOcclusionTex;
uniform vec2 uPixelSize;

void main()
{
	float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * uPixelSize;
            result += texture(ambiantOcclusionTex, outTex + offset).r;
        }
    }
    gl_Color = result / (4.0 * 4.0);
} 