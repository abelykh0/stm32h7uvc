#include "startup.h"

#include "stm32h7xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_video_if.h"
#include "quadspi.h"
#include "w25qxx_qspi.h"
#include "user_diskio.h"
#include "fatfs.h"

#include "screen/screen.h"
#include "keyboard/ps2_keyboard.h"
#include "emulator/bkEmu.h"

extern JPEG_HandleTypeDef hjpeg;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern USBD_HandleTypeDef hUsbDeviceFS;

static Display::Screen screen;

static void USB_DEVICE_Init();
static void MapFlash();

extern "C" void initialize()
{
}

extern "C" void setup()
{
	USB_DEVICE_Init();

	// Read ROMs from external flash
	// (the built-in flash is only 128K)
	MapFlash();
	if (f_mount(&USERFatFS, (TCHAR*)u"1:/", 1) == FR_OK)
	{
		FIL file;
		if (f_open(&file, (const TCHAR*)u"1:/BASIC10.ROM", FA_READ) == FR_OK)
		{
			UINT bytesRead = sizeof(basic);
			f_read(&file, basic, bytesRead, &bytesRead);
			f_close(&file);
		}

		if (f_open(&file, (const TCHAR*)u"1:/MONIT10.ROM", FA_READ) == FR_OK)
		{
			UINT bytesRead = sizeof(monitor);
			f_read(&file, monitor, bytesRead, &bytesRead);
			f_close(&file);
		}

		f_mount(nullptr, nullptr, 1);
	}

	JPEG_ConfTypeDef config;
	config.ImageWidth = UVC_WIDTH;
	config.ImageHeight = UVC_HEIGHT;
	config.ColorSpace = JPEG_YCBCR_COLORSPACE;
	config.ChromaSubsampling = JPEG_444_SUBSAMPLING;
	config.ImageQuality = 90;
	HAL_JPEG_ConfigEncoding(&hjpeg, &config);

	screen.SetAttribute(0x2A10);
	screen.Clear();

	Ps2_Initialize();
	bk_setup();
}

extern "C" void loop()
{
	bk_loop();
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(50);
}

static void USB_DEVICE_Init()
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

static void MapFlash()
{
	w25qxx_Init();
	w25qxx_EnterQPI();
	w25qxx_Startup(w25qxx_NormalMode); // w25qxx_DTRMode
}
