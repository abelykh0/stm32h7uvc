#include "stm32h7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_video_if.h"

#include "startup.h"
#include "screen/screen.h"
#include "keyboard/ps2_keyboard.h"
#include "demo_colors/demo_colors.h"
#include "xonix/platform.h"

extern JPEG_HandleTypeDef hjpeg;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern USBD_HandleTypeDef hUsbDeviceFS;

static demo_mode demoMode = demo_mode::DEMO_COLORS;
static bool demoInitialized = false;

static void USB_DEVICE_Init();

extern "C" void initialize()
{
}

extern "C" void setup()
{
	USB_DEVICE_Init();

	JPEG_ConfTypeDef config;
	config.ImageWidth = UVC_WIDTH;
	config.ImageHeight = UVC_HEIGHT;
	config.ColorSpace = JPEG_YCBCR_COLORSPACE;
	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;
	config.ImageQuality = 90;
	HAL_JPEG_ConfigEncoding(&hjpeg, &config);

	Ps2_Initialize();
}

extern "C" void loop()
{
	int32_t key;
	switch (demoMode)
	{
	case demo_mode::XONIX:
		if (demoInitialized)
		{
			key = GameUpdate();
		}
		else
		{
			GameInit();
			demoInitialized = true;
		}
		break;
	case demo_mode::DEMO_COLORS:
	default:
		if (demoInitialized)
		{
			key = loop_demo_colors();
		}
		else
		{
			init_demo_colors();
			demoInitialized = true;
		}
		break;
	}

	switch (key)
	{
	case KEY_F1:
		if (demoMode != demo_mode::DEMO_COLORS)
		{
			demoMode = demo_mode::DEMO_COLORS;
			demoInitialized = false;
		}
		break;
	case KEY_F2:
		if (demoMode != demo_mode::XONIX)
		{
			demoMode = demo_mode::XONIX;
			demoInitialized = false;
		}
		break;
	}
}

void USB_DEVICE_Init()
{
	  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }

	  // Defaults are 128, 64, 128 (320 total)
	  HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_FS,    48);
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 0, 16);
	  HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_FS, 1, 256);

	  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_VIDEO) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_VIDEO_RegisterInterface(&hUsbDeviceFS, &USBD_VIDEO_fops_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
	  {
	    Error_Handler();
	  }
}

