#include "startup.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_video_if.h"
#include "stm32h7xx_hal.h"
#include "rtc.h"
#include "Screen.h"
#include "espectrum/keyboard/ps2.h"

volatile uint32_t kbd_rxof = 0, mouse_rxof = 0;

extern JPEG_HandleTypeDef hjpeg;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern USBD_HandleTypeDef hUsbDeviceFS;

using namespace Display;
static Screen screen;

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

	screen.SetAttribute(0x3F10);
	screen.Clear();

    // Frame
	screen.SetAttribute(0x2A10);
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

    // 64 colors
	char buf[20];
    for (int i = 0; i < 64; i++)
    {
    	screen.SetAttribute((i << 8) | 0x10);
    	screen.PrintAt(3 + (i % 5) * 7, 3 + (i / 5) * 2, "\xDF\xDF\xDF\xDF\xDF\xDF"); // ▀▀▀▀▀▀

    	screen.SetAttribute(0x2A10);
    	sprintf(buf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));
    	screen.PrintAt(3 + (i % 5) * 7, 2 + (i / 5) * 2, buf);
    }

	screen.SetAttribute(0x3F10);
    //screen.PrintAlignCenter(0, " Hello, world! ");
}

extern "C" void loop()
{
	char showTime[20];

	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	HAL_Delay(1000);

	RTC_DateTypeDef dateStruct;
	RTC_TimeTypeDef timeStruct;
	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &dateStruct, RTC_FORMAT_BIN); // important
	sprintf(showTime, " %.2d:%.2d:%.2d ", timeStruct.Hours, timeStruct.Minutes, timeStruct.Seconds);
    screen.PrintAlignCenter(0, showTime);

    uint8_t ch;
    while (ps2_kbd_getkey(&ch) == 1)
    { /* recevied keyboard data */
      if(ch >= 32)
        printf("Kbd: '%c', 0x%X\r\n", ch, ch);
      else
        printf("Kbd:       0x%X\r\n", ch);
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

// ----------------------------------------------------------------------------
void ps2_kbd_cbrx(uint8_t kbd_data_rx)
{
}

void ps2_kbd_cbrxerror(uint32_t rx_errorcode)
{
  if(rx_errorcode == PS2_ERROR_OVF)
  {
    printf("Kbd overflow error\r\n");
  }
  else if(rx_errorcode == PS2_ERROR_PARITY)
    printf("Kbd parity error\r\n");
}
