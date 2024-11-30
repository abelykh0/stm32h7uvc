/* Hey emacs, make this use -*- Mode: C++; tab-width: 4 -*- */
#ifndef XONIX_H
#define XONIX_H

#include <xonix/platform.h>

/*
 * xonix global definitions
 *
 * xonix.h,v 1.39 1995/09/08 09:51:30 j Exp
 */

/*
 * Copyright (c) 1995
 *   Torsten Schoenitz <torsten_schoenitz@bonnie.heep.sax.de>
 *   Joerg Wunsch <joerg_wunsch@uriah.heep.sax.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE DEVELOPERS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE DEVELOPERS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef USE_X11
#include <X11/Xos.h>		/* Boolean et al. */
#include <X11/Intrinsic.h>
#endif

#ifdef USE_MAC
#include <Types.h>          /* Basistypendeklarationen.                     */
#include <Memory.h>							     
#include <QuickDraw.h>      /* Interface-Datei zu den QuickDraw-Datenstruk- */
                            /* turen und Routinen.                          */
#include <QDOffscreen.h>    /* Wegen der hintergruendigen GWorld-Geschichten*/
#include <Fonts.h>          /* Interface-Datei des Font-Managers (wird vom  */
                            /* Window-Manager benoetigt).                   */
#include <Windows.h>        /* Interface-Datei des Window-Managers (hier 	*/
                            /* wird u.a. GetNewWindow deklariert).          */
#include <Events.h>         /* Interface-Datei zu den Routinen und Daten-   */
                            /* strukturen des Event-Managers.               */
#include <ToolUtils.h>      /* ToolBox-Utilities (z.B. HiWord, LoWord).     */
#include <Menus.h>          /* Interface-Datei des Menu-Managers.           */
#include <Desk.h>           /* Desk-Manager (Kompatibilitaet zu System 6.x +*/
                            /* Finder in bezug auf Schreibtischprogramme).  */
#include <TextEdit.h>       /* Interface-Datei des TextEdit-Managers (wird  */
                            /* vom Dialog-Manager verwendet).               */
#include <Dialogs.h>        /* Interface-Datei des Dialog-Managers.         */
#include <StandardFile.h>   /* Einsprungadressen bzw. Datenstrukturen von   */
                            /* SFGetFile und SFPutFile.                     */
#endif /* USE_MAC */

/* Typedefs.*/

struct point {
  short h, v;
};

typedef struct point myPoint;

struct rectangle {
	short top, left, bottom, right;
};

typedef struct rectangle myRect;

#ifdef USE_MAC
struct keyboardPrefs {
	char	leftKey, rightKey, upKey, downKey, pauseKey;
};

typedef struct keyboardPrefs kbPrefs, *kbPrefsPtr, **kbPrefsHandle;

struct highScoreEntry {
	Str255	name;
	int		score;
	int		level;
};

typedef struct highScoreEntry hgScoreEntry, *hgScoreEntryPtr, 
							**hgScoreEntryHandle;
#endif /* USE_MAC */
							
struct Player   {
  int       x, y;                           /* Koordinaten der Figur        */
  int       sx, sy;                         /* Groesse der Figur            */
  int       dx, dy;                         /* Schrittweite der Figur       */
  myRect	rr;								/* Redraw-Rechteck				*/
};

typedef struct Player Player;

struct Segment	{
	int		y;
	int		xl;
	int		xr;
	int		dy;
};

typedef struct Segment mySegment;

enum attribute  {
            EMPTY       = 0x00,             /* Ungefuellt                   */
            WAY         = 0x01,             /* Spur des Runners             */
            TESTFILLED  = 0x02,             /* Probefuellung                */
            FILLED      = 0x04,             /* Endgueltig gefuellt          */
            BORDER      = 0x08,             /* Rahmen                       */
            RUNNER      = 0x80,             /* Spielfigur                   */
            FLYER       = 0x40,             /* Flieger                      */
            EATER       = 0x20              /* Fresser                      */
            };


/* Andere Konstanten. */

/* Die Spieldimensionen werden durch die "Fresser"-Groesse bestimmt.        */
/* H_STEPS und V_STEPS sollten multipliziert mit EATER_SIZE jeweils         */
/* etwa 600 bzw. 400 ergeben                                                */
									     
#define EATER_SIZE          4               /* Groesse der "Fresser"        */
#define RATIO               2               /* Faktor von Eater zu Runner   */
//#define H_STEPS             150             /* Horizontal moegl. Positionen	*/
//#define V_STEPS             100             /* Vertikal moegl. Positionen 	*/

#define PERCENT_THRESHOLD	75				/* Fuellstand - Schwelle		*/
#define LEVEL_BONUS_FACTOR	100.0        /* Bonus fuer geschafftes Level */
#define LOOP_FACTOR			10.0			/* Faktor fuer Punktberechnung	*/

#define EATER_STEP          1                  /* Schrittweite der "Fresser"*/
#define FLYER_SIZE          RATIO*EATER_SIZE   /* Groesse der "Flieger"     */
#define FLYER_STEP          RATIO              /* Schrittweite der "Flieger"*/
#define RUNNER_SIZE         RATIO*EATER_SIZE   /* Groesse der Spielfigur    */
#define RUNNER_STEP         RATIO              /* Schrittweite der Spielfig.*/
#define FIELD_WIDTH         H_STEPS*EATER_SIZE /* Breite des Spielfeldes    */
#define FIELD_HEIGHT        V_STEPS*EATER_SIZE /* Hoehe des Spielfeldes     */

#define WINDOW_START_X      20
#define WINDOW_START_Y      40
#define MAX_FLYER           10
#define MAX_EATER           4


#ifdef USE_X11
typedef unsigned char * Ptr;
#define NIL_POINTER			NULL
#define STEP_TIME           50		/* Schrittweite in ms */
#define BELL() XBell(dpy, 50)
#endif

#ifdef USE_MAC
#define KEY char
#define BELL() RunnerDieSound()
#endif

/* ------------------------------------------------------------------------ */
/* Deklaration der globalen Variablen.                                      */
/* ------------------------------------------------------------------------ */

extern Boolean      gQuit,                  /* Wenn gQuit auf true gesetzt  */
                                            /* wird, terminiert die Appli-	*/
                                            /* kation.                      */
                    gRun,                   /* Ist gRun auf true gesetzt,	*/
                                            /* laeuft das Spiel, sonst sind */
                                            /* Einstellungen usw. moeglich  */
                    gPause,					/* Spiel unterbrochen           */
					gEndOfGame;				/* Runner sind alle 			*/
extern int          gFlyerCount;            /* Anzahl der Flieger           */
									    
extern Player       gFlyer[MAX_FLYER];      /* Alle Flieger                 */

extern int          gEaterCount;            /* Anzahl der Fresser           */
									    
extern Player       gEater[MAX_EATER];      /* Alle Fresser                 */
									    
extern Player       gMyRunner;              /* Spielfigur                   */
									    
extern Ptr          gMyStatusArea;          /* Status-Area                  */

extern unsigned     gHighScore;	            /* Punktestand                  */
extern int			gLevel;					/* Es geht mit Level 1 los		*/

#ifdef USE_MAC

extern GWorldPtr    gMyGWorld;              /* Offscreen-Area               */

extern WindowPtr    gTheCWindow;            /* Das Window fuer alles        */

extern Rect         gWorldRect;

#endif /* USE_MAC */

#ifdef USE_X11

extern Display *dpy;

#endif /* USE_X11 */


/* ------------------------------------------------------------------------ */
/* Funktionen								     							*/
/* ------------------------------------------------------------------------ */

extern void Animate (void);
extern void Do_Event (void);
extern void DrawRunnerToGWorld (void);
extern void DrawWayToGWorld (void);
extern void DrawEmptyToGWorld (int xPos, int yPos);
extern void DrawFlyerToGWorld (int xPos, int yPos);
extern void DrawEaterToGWorld (int xPos, int yPos);
extern void DrawFilledToGWorld (int xPos, int yPos);
extern void DrawSmallFilledToGWorld (int xPos, int yPos);
extern void DrawCompleteBorder (void);
extern void SetRunner (int x, int y);
extern void ClearWay (void);
extern Boolean FillUp (int x, int y);
extern void ExitXonix (int number);
extern void ScorePercentage (int num);
extern void ScoreLevel (int num);
extern void ScoreRunner (int num);
extern void ShowHighScore (void);

#ifdef USE_MAC

#define NIL_POINTER 0L

#define GWorldEntry() \
  GDHandle    oldGD; \
  GWorldPtr   oldGW; \
  GetGWorld (&oldGW, &oldGD); \
  LockPixels (gMyGWorld -> portPixMap); \
  SetGWorld (gMyGWorld, NIL_POINTER)

#define GWorldExit() \
  SetGWorld (oldGW, oldGD); \
  SetPort (gTheCWindow); \
  CopyBits ((BitMap*)*gMyGWorld -> portPixMap, &gTheCWindow -> portBits, \
	    &gWorldRect, \
	    &gWorldRect, srcCopy, 0L); \
  UnlockPixels (gMyGWorld -> portPixMap)

#define GWorldExitFlyer() \
  SetGWorld (oldGW, oldGD); \
  SetPort (gTheCWindow); \
  for (i = 0; i < gFlyerCount; i ++) \
  	CopyBits ((BitMap*)*gMyGWorld -> portPixMap, &gTheCWindow -> portBits, \
	    &gFlyer[i].rr, \
	    &gFlyer[i].rr, srcCopy, 0L); \
  UnlockPixels (gMyGWorld -> portPixMap)

#define GWorldExitEater() \
  SetGWorld (oldGW, oldGD); \
  SetPort (gTheCWindow); \
  for (i = 0; i < gEaterCount; i ++) \
  	CopyBits ((BitMap*)*gMyGWorld -> portPixMap, &gTheCWindow -> portBits, \
	    &gEater[i].rr, \
	    &gEater[i].rr, srcCopy, 0L); \
  UnlockPixels (gMyGWorld -> portPixMap)

#define GWorldExitRunner() \
  SetGWorld (oldGW, oldGD); \
  SetPort (gTheCWindow); \
  CopyBits ((BitMap*)*gMyGWorld -> portPixMap, &gTheCWindow -> portBits, \
	    &gMyRunner.rr, \
	    &gMyRunner.rr, srcCopy, 0L); \
  UnlockPixels (gMyGWorld -> portPixMap)

#endif /* USE_MAC */

#ifdef USE_X11

#define GWorldEntry()
#define GWorldExit()
#define GWorldExitFlyer()
#define GWorldExitEater()
#define GWorldExitRunner()

#endif /* USE_X11 */

#ifdef __FreeBSD__

#define rand() random()
#define srand(x) srandom(x)

#endif /* __FreeBSD */

#endif /* XONIX_H */
