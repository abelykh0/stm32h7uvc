#include <stdio.h>
#include <time.h>
#include "stm32h7xx_hal.h"
#include "rtc.h"

#include "platform.h"
#include "xonix.h"
#include "gamefont4x4.h"
#include "keyboard/ps2.h"
#include "screen/canvas.h"

#define LABEL_COLOR 0xFF00
#define SCORE_COLOR 0x3300

uint8_t gMyStatusArea[H_STEPS * V_STEPS];

time_t time(time_t* second)
{
	RTC_DateTypeDef dateStruct;
	RTC_TimeTypeDef timeStruct;
	timeStruct.TimeFormat = RTC_HOURFORMAT_24;
	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &dateStruct, RTC_FORMAT_BIN);

	struct tm currentDate;
	currentDate.tm_year = dateStruct.Year + 2000 - 1900;
	currentDate.tm_mon = dateStruct.Month - 1;
	currentDate.tm_mday = dateStruct.Date - 1;
	currentDate.tm_hour = timeStruct.Hours - 1;
	currentDate.tm_min = timeStruct.Minutes - 1;
	currentDate.tm_sec = timeStruct.Seconds - 1;

	time_t result = mktime(&currentDate);
	return result;

}

void DrawSprite(int xPos, int yPos, uint8_t spriteIndex)
{
	uint8_t* sprite = (uint8_t*)&gameFont4x4[spriteIndex * 4];
	for (uint8_t y = 0; y < 4; y++)
	{
		for (uint8_t x = 0; x < 4; x++)
		{
        	uint8_t color;
            if ((*sprite << x) & 0x08)
            {
            	// foreground color
            	color = (LABEL_COLOR >> 8) & 0x03F;
            }
            else
            {
            	// background color
            	color = LABEL_COLOR & 0x03F;
            }

            SetPixel(xPos * 4 + x, yPos * 4 + y, color);
		}

		sprite += 4;
	}
}

void DrawRunnerToGWorld()
{
	DrawSprite(gMyRunner.x, gMyRunner.y, 0x01);
}

void DrawWayToGWorld()
{
	DrawSprite(gMyRunner.x, gMyRunner.y, 0x05);
}

void DrawEaterToGWorld(int xPos, int yPos)
{
	DrawSprite(xPos, yPos, 0x02);
}

void DrawEmptyToGWorld(int xPos, int yPos)
{
	DrawSprite(xPos, yPos, 0x00);
}

void DrawFlyerToGWorld(int xPos, int yPos)
{
	DrawSprite(xPos, yPos, 0x03);
}

void DrawFilledToGWorld(int xPos, int yPos)
{
	DrawSprite(xPos, yPos, 0x04);
}

void DrawSmallFilledToGWorld(int xPos, int yPos)
{
	DrawSprite(xPos, yPos, 0x04);
}

void DrawCompleteInside()
{
	int i, j;

	for (j = 0; j < V_STEPS; j++)
	{
		for (i = 0; i < H_STEPS; i++)
		{
			unsigned char c = *(gMyStatusArea + (j * H_STEPS) + i);
			if ((c & FILLED) || (c & BORDER) || (c & EATER))
			{
				DrawSmallFilledToGWorld(i, j);
			}
			else if (c & FLYER)
			{
				DrawFlyerToGWorld(i, j);
			}
			else if (c & WAY)
			{
				DrawWayToGWorld();
			}
			else if (c & RUNNER)
			{
				DrawRunnerToGWorld();
			}
			else
			{
				DrawEmptyToGWorld(i, j);
			}
		}
	}

	for (i = 0; i < gEaterCount; i++)
	{
		DrawEaterToGWorld(gEater[i].x, gEater[i].y);
	}
}

void DrawCompleteBorder()
{
	int i;

	for (i = 0; i < H_STEPS; i += RATIO)
	{
		DrawFilledToGWorld(i, 0);
		DrawFilledToGWorld(i, V_STEPS - 2);

		if (RATIO == 1)
		{
			DrawFilledToGWorld(i, 1);
			DrawFilledToGWorld(i, V_STEPS - 1);
		}
	}

	for (i = 2; i < V_STEPS - 2; i += RATIO)
	{
		DrawFilledToGWorld(0, i);
		DrawFilledToGWorld(H_STEPS - 1, i);

		if (RATIO == 1)
		{
			DrawFilledToGWorld(1, i);
			DrawFilledToGWorld(H_STEPS - 2, i);
		}
	}
}

void GameInit()
{
 	Do_New();
}

int32_t GameUpdate()
{
	Animate();
	HAL_Delay(STEP_TIME);

	uint8_t kbd_key;
    int8_t result = ps2_kbd_getkey(&kbd_key);
    if (result)
    {
    switch (kbd_key)
    {
		case ' ': // STOP
			gMyRunner.dx = 0;
			gMyRunner.dy = 0;
			break;
		case PS2_LEFTARROW:
			gMyRunner.dx = -RATIO;
			gMyRunner.dy = 0;
			break;
		case PS2_RIGHTARROW:
			gMyRunner.dx = RATIO;
			gMyRunner.dy = 0;
			break;
		case PS2_UPARROW:
			gMyRunner.dy = -RATIO;
			gMyRunner.dx = 0;
			break;
		case PS2_DOWNARROW:
			gMyRunner.dy = RATIO;
			gMyRunner.dx = 0;
			break;
		}
    }

    return result;
}

void Quit()
{
	ExitXonix(0);
}

void ScoreLevel(int num)
{
    //Vga::printAt(0, 23, "Lvl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d ", num);
    //Vga::printAt(4, 23, buffer, SCORE_COLOR);
}

void ScorePercentage(int num)
{
    //Vga::printAt(7, 23, "Fl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d%% ", num);
    //Vga::printAt(10, 23, buffer, SCORE_COLOR);
}

void ScoreRunner(int num)
{
    //Vga::printAt(14, 23, "Pl:", LABEL_COLOR);

	char buffer[10];
	sprintf(buffer, "%02d ", num);
    //Vga::printAt(17, 23, buffer, SCORE_COLOR);
}

void ScorePoints(int points)
{
    //Vga::printAt(20, 23, "Score:", LABEL_COLOR);

	char buffer[20];
	sprintf(buffer, "%06d", points);
   // Vga::printAt(26, 23, buffer, SCORE_COLOR);
}

void DisplayHighScore()
{
	// COORD dwCursorPosition;
	// DWORD nb;
	// dwCursorPosition.X = 45;
	// dwCursorPosition.Y = V_STEPS + 1;

	// char buffer[100];
	// sprintf(buffer, "HighScore: %d", gHighScore);

	// WriteConsoleOutputCharacterA(hStdOut, buffer, strlen(buffer), dwCursorPosition, &nb);
}

void ExitXonix(int status)
{
}

