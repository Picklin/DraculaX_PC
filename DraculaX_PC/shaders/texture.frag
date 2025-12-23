#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform bool invert;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec4 texColor = texture(tex, texCoordFrag);
	if (invert) outColor = vec4(vec3(1.f)-texColor.rgb, texColor.a);
	else outColor = color * texColor;
}

