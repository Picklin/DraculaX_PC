#ifndef _PALETTE_KEYFRAMES
#define _PALETTE_KEYFRAMES

#include <vector>

struct PaletteKeyframes
{
	float millisecsPerKeyframe;
	std::vector<float> paletteRows;
	int nextPaletteAnim = -1;
};


#endif // _ANIMATION_KEYFRAMES


