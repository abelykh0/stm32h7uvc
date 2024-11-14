# stm32h7uvc

I wanted to make some unusual USB device, for example, a web camera. I settled on an image size of 320x240 pixels because of limitations of USB 1.x, which is present on the [Adafruit WeAct Studio STM32H750](https://www.adafruit.com/product/5032). I managed to achieve about 13 fps by using hardware JPEG encoder in MDMA mode.

This demo shows screen with 64 colors and a clock.
