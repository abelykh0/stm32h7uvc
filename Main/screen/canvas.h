#ifndef __MCU_H__
#define __MCU_H__

#ifdef __cplusplus
extern "C" {
#endif

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
#define TEXT_COLUMNS (UVC_WIDTH / 8)
#define TEXT_ROWS (UVC_HEIGHT / 8)

extern uint8_t canvas[MCU_WIDTH * MCU_HEIGHT * sizeof(Mcu)]; // 230,400

void Clear(uint8_t color);
void Set8x8Pixels(int x, int y, uint8_t color);
void SetPixel(int x, int y, uint8_t color);

#ifdef __cplusplus
}
#endif

#endif
