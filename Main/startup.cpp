#include "startup.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_video_if.h"
#include "Screen.h"

extern JPEG_HandleTypeDef hjpeg;

using namespace Display;
static Screen screen;

extern "C" void initialize()
{
}

extern "C" void setup()
{
	JPEG_ConfTypeDef config;
	config.ImageWidth = UVC_WIDTH;
	config.ImageHeight = UVC_HEIGHT;
	config.ColorSpace = JPEG_YCBCR_COLORSPACE;
	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;
	config.ImageQuality = 90;
	HAL_JPEG_ConfigEncoding(&hjpeg, &config);

	screen.SetAttribute(0x3F01);
	screen.Clear();

    // Frame
	screen.SetAttribute(0x2A01);
    screen.PrintAt(0, 0, "\xC9"); // ╔
    screen.PrintAt(TEXT_COLUMNS - 1, 0, "\xBB"); // ╗
    screen.PrintAt(0, TEXT_ROWS - 1, "\xC8"); // ╚
    screen.PrintAt(TEXT_COLUMNS - 1, TEXT_ROWS - 1, "\xBC"); // ╝
    for (uint16_t i = 1; i < TEXT_COLUMNS - 1; i++)
    {
        screen.PrintAt(i, 0, "\x0CD"); // ═
        screen.PrintAt(i, TEXT_ROWS - 1, "\x0CD"); // ═
    }
    for (uint16_t i = 1; i < TEXT_ROWS - 1; i++)
    {
        screen.PrintAt(0, i, "\x0BA"); // ║
        screen.PrintAt(TEXT_COLUMNS - 1, i, "\x0BA"); // ║
    }

	screen.SetAttribute(0x3F01);
    screen.PrintAlignCenter(0, " Hello, world! ");

    // 64 colors
	char buf[20];
    for (int i = 0; i < 64; i++)
    {
    	screen.SetAttribute((i << 8) | 0x01);
    	screen.PrintAt(3 + (i % 5) * 7, 3 + (i / 5) * 2, "\xDF\xDF\xDF\xDF\xDF\xDF"); // ▀▀▀▀▀▀

    	screen.SetAttribute(0x2A01);
    	sprintf(buf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));
    	screen.PrintAt(3 + (i % 5) * 7, 2 + (i / 5) * 2, buf);
    }
}

extern "C" void loop()
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(1000);
	screen.PrintAlignCenter(15, "Hello, world!");
}
