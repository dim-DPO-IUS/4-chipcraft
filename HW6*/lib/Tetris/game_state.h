#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "config.h"

// Global variables declaration
extern byte MasSt[MaxX][MaxY];
extern byte MasTmp[MaxX][MaxY];
extern byte MasOld[MaxX][MaxY];
extern byte fignext[3];
extern byte Level;
extern byte OldLevel, tr, flfirst;

extern uint16_t MasCol[];
extern unsigned long TimeOfPush, Counter, Score, TScore;
extern unsigned long Record, OldRecord, OldScore, myrecord;
extern word tempspeed;

extern bool fl, Demo, myfl, Arbeiten, FlNew, FlZ;
extern int8_t VAL, Mp, x, y;

extern int8_t mmm[4][2];
extern word MasSpeed[MaxLevel];

// Hardware objects
extern Arduino_ST7789 tft;
extern Encod_er encoder;

#endif