#ifndef __MCU_H__
#define __MCU_H__

#include "stdint.h"
#include "usbd_video_conf.h"

/* 420
typedef struct
{
	uint8_t y[4][8][8];
	uint8_t cb[8][8];
	uint8_t cr[8][8];
} __attribute__((packed)) Mcu;

#define MCU_WIDTH (UVC_WIDTH / 8 / 2)
#define MCU_HEIGHT (UVC_WIDTH / 8 / 2)
*/

typedef struct
{
	uint8_t y[8][8];
	uint8_t cb[8][8];
	uint8_t cr[8][8];
} __attribute__((packed)) Mcu;

#define MCU_WIDTH (UVC_WIDTH / 8)
#define MCU_HEIGHT (UVC_HEIGHT / 8)

extern uint8_t canvas[MCU_WIDTH * MCU_HEIGHT * sizeof(Mcu)];

void Clear();
void Set8x8Pixels(int x, int y, uint8_t color);

#endif
