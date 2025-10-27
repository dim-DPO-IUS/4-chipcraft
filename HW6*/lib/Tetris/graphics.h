#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "config.h"
#include "game_state.h"

// Display functions
void ViewQuad(byte i, byte j, byte mycolor);
void ViewStacan(void);
void ClearMas(byte MasStx[MaxX][MaxY]);

#endif