#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "config.h"
#include "game_state.h"

// Core game functions
void NewGame(void);
void gameover(void);
void ProcDemo(void);

// Figure management
void GetNext(void);
bool figmove(int dx, int dy);

// Game mechanics
void MyScore(void);
bool FindFull(void);

// Helper functions
void Sosed(int i, int j, int dx, int dy, byte mode);
void Sos(int i, int j, byte mode);

// Utility
void mydelay(int md);

// Добавь эти объявления, так как они используются в game_logic.cpp
void ViewStacan(void);
void ClearMas(byte MasStx[MaxX][MaxY]);
byte mypush(void);
void analogWrite(uint8_t pin, int value);

#endif