# stm32h7uvc

I wanted to make some unusual USB device, for example, a web camera. I settled on an image size of 320x240 pixels because of limitations of USB 1.x (FS), which is present on the [Adafruit WeAct Studio STM32H750](https://www.adafruit.com/product/5032) (STM32H750VBT6 CPU @ 480 MHz with 128 Kb flash and 1 Mb of RAM). By leveraging the hardware JPEG encoder in MDMA mode, I managed to reach a frame rate of approximately 15 fps.

This demo shows screen with 64 colors and a clock. This is how it looks like in Windows Camera app:
![Screenshot](https://raw.githubusercontent.com/abelykh0/stm32h7uvc/refs/heads/main/doc/Camera.png)

Software: [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).

Special thanks to [STM32F4_UVC_Camera Project](https://github.com/iliasam/STM32F4_UVC_Camera) for inspiration.
