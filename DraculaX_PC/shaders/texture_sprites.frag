#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform sampler2D paletteLUT;
uniform float currentPaletteRow;
uniform bool invert;
uniform bool usePalette;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec4 texColor = texture(tex, texCoordFrag);
	if (texColor.a < 0.5) discard;
	if (invert) outColor = vec4(vec3(1.f)-texColor.rgb, texColor.a);
	else if (usePalette)
	{
		float paletteIndex = texColor.r;
		vec2 lutCoord = vec2(paletteIndex, currentPaletteRow);
		vec4 finalColor = texture(paletteLUT, lutCoord);
		outColor = vec4(finalColor.rgb, texColor.a * color.a);
	}
	else outColor = color * texColor;
}

