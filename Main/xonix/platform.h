#pragma once

#include <stdint.h>

#define H_STEPS             80      /* Horizontal moegl. Positionen	*/
#define V_STEPS             58      /* Vertikal moegl. Positionen 	*/
#define RATIO               1       /* Faktor von Eater zu Runner   */
#define STEP_TIME           50		/* Schrittweite in ms */


#ifdef __cplusplus
extern "C" {
#endif

void GameInit();
int32_t GameUpdate();

void Do_New();
void DrawComplete();
void ScorePoints(int points);
void DisplayHighScore();

#ifdef __cplusplus
}
#endif

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

typedef uint8_t  Ptr[H_STEPS * V_STEPS];
#define NIL_POINTER		NULL

typedef int BOOL;
#define Boolean BOOL

#define BELL()
#define GWorldEntry()
#define GWorldExit()
#define GWorldExitFlyer()
#define GWorldExitEater()
#define GWorldExitRunner()


//extern uint8_t GetUsbBuffer(char *buffer, uint8_t maxLength);

