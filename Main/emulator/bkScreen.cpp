#include <unordered_map>
#include "bkScreen.h"
#include "screen/canvas.h"

using namespace bk;
using namespace std;

// 00 - 512x256, no inversion
// 01 - 512x256, inversion
// 82 - 256x256, no inversion, colors
// 83 - 256x256, inversion, colors
// 84 - 256x256, no inversion, black and white
// 85 - 256x256, inversion, black and white
unordered_map<uint8_t, uint8_t> mode512x256 {
	{16, 0},
	{235, 1}
};

uint8_t bkScreen::getVideoRam(uint16_t address)
{
	uint8_t result = 0;

	if ((this->mode & 0x80) == 0)
	{
		// 512x256 - 1 bit per pixel
		uint16_t offset = address - 0x4000;
		uint16_t charX = offset % 64;
		uint16_t y = offset / 64;
		for (uint8_t x = charX * 8; x < charX * 8 + 8; x++)
		{
			YCbCr pixel = GetPixel(x, y);
			if (mode512x256[pixel.y])
			{
				result &= 1;
			}

			if ((this->mode & 0x01) != 0)
			{
				// inversion
				result ^= 1;
			}

		    result <<= result;
		}
	}

	return result;
}

void bkScreen::setVideoRam(uint16_t address, uint8_t value)
{
	if ((this->mode & 0x80) == 0)
	{
		// 512x256 - 1 bit per pixel
		uint16_t offset = address - 0x4000;
		uint16_t charX = offset % 64;
		uint16_t y = offset / 64;
		for (uint8_t x = charX * 8; x < charX * 8 + 8; x++)
		{
			uint8_t pixelBits = (value & 0x80) == 0 ? 0 : 1;
			if ((this->mode & 0x01) != 0)
			{
				// inversion
				pixelBits ^= 1;
			}

			uint8_t color;
			if (pixelBits)
			{
				color = 0x3f;
			}
			else
			{
				color = 0x00;
			}

			SetPixel(x, y, color);
		}
	}
}
