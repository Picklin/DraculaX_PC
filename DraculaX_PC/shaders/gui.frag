#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform float hpPercent;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	if (texCoordFrag.y < (1 - hpPercent)) discard;
	vec4 texColor = texture(tex, texCoordFrag);
	outColor = color * texColor;
}