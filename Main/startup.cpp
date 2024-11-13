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

	Clear(0b00000001); // 00RRGGBB
	screen.SetCursorPosition(0, 0);
	//screen.PrintAlignCenter(3, "Hello world!");
}

extern "C" void loop()
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(1000);
}
