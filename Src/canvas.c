#include <canvas.h>
#include "string.h"

uint8_t canvas[] __attribute__(( section(".sram2") ));
static Mcu* canvasMcu = (Mcu*)canvas;

void Clear()
{
	for (int i = 0; i < MCU_WIDTH * MCU_HEIGHT; i++)
	{
		Mcu* mcu = &canvasMcu[i];

		/* 420
		for (int i = 0; i < 4; i++)
		{
			memset(mcu->y[i], 16, 8 * 8); // black
		}
		*/

		memset(mcu->y, 16, 8 * 8); // black
		memset(mcu->cb, 128, 8 * 8);
		memset(mcu->cr, 128, 8 * 8);
	}
}

// 40 x 30 8x8 squares
void Set8x8Pixels(int x, int y, uint8_t color)
{
	/* 420
	Mcu* mcu = &canvasMcu[x / 2 + (y / 2 * MCU_WIDTH)];
	int blockIndex = (x % 2) + (y % 2 * 2);
	memset(mcu->y[blockIndex], color, 8 * 8);

	if (x == 2 && y == 2)
	{
		memset(mcu->cb, 166, 8 * 8);
		memset(mcu->cr, 16, 8 * 8);
	}
	*/

	Mcu* mcu = &canvasMcu[x + (y * MCU_WIDTH)];
	memset(mcu->y, color, 8 * 8);

	if (x == 2 && y == 2)
	{
		memset(mcu->cb, 166, 8 * 8);
		memset(mcu->cr, 16, 8 * 8);
	}
}

// 320 x 240
void SetPixel(int x, int y, int color)
{

}
