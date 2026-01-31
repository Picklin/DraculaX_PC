#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform float yOffset;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec2 scrollPosition = vec2(texCoordFrag.x, texCoordFrag.y + yOffset);
	vec4 texColor = texture(tex, scrollPosition);
	outColor = color * texColor;
}

