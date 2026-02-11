#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform sampler2D paletteLUT;
uniform float currentPaletteRow;
uniform float paletteIndexOffset;
uniform float xOffset;
uniform float yOffset;
uniform bool usePalette;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec2 scrollPosition = vec2(texCoordFrag.x + xOffset, texCoordFrag.y + yOffset);
	vec4 texColor = texture(tex, scrollPosition);
	if (usePalette)
	{
		float paletteIndex = texColor.r;
		vec2 lutCoord = vec2(paletteIndex + paletteIndexOffset, currentPaletteRow);
		vec4 finalColor = texture(paletteLUT, lutCoord);
		outColor = vec4(finalColor.rgb, texColor.a * color.a);
	}
	else outColor = color * texColor;
}

