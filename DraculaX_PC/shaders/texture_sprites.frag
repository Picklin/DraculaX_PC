#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform bool invert;
uniform float xOffset;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec2 scrollPosition = vec2(texCoordFrag.x + xOffset, texCoordFrag.y);
	vec4 texColor = texture(tex, scrollPosition);
	if (invert) outColor = vec4(vec3(1.f)-texColor.rgb, texColor.a);
	else outColor = color * texColor;
}

