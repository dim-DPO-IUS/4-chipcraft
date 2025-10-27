#include "graphics.h"
//============================================================
void ViewQuad(byte i, byte j, byte mycolor)
{
    /*-i и -j - создают промежутки между клетками (spacing)
    +1 (CELL_BORDER_OFFSET) - отступ от края для заливки внутри обводки
    -2 (CELL_BORDER_SIZE) - уменьшение размера заливки чтобы оставалась обводка
    -1 (CELL_CLEAR_OFFSET_Y) - специальное смещение при очистке (возможно баг или особенность)*/

    if (j < VISIBLE_FIELD_START)
        return;

    // Вычисляем позицию клетки с учетом отступов между клетками
    // -j создает промежутки между клетками
    byte wy = SmeY + (j - VISIBLE_FIELD_START) * razmer - j;
    // -i создает промежутки между клетками
    byte wx = SmeX + i * razmer - i;

    if (mycolor != 0) {
        // Рисуем клетку с цветом: обводка + заливка
        tft.drawRect(wx, wy, razmer, razmer, BLACK);
        tft.fillRect(wx + CELL_BORDER_OFFSET,
                     wy + CELL_BORDER_OFFSET,
                     razmer - CELL_BORDER_SIZE,
                     razmer - CELL_BORDER_SIZE,
                     MasCol[mycolor]);
    } else {
        // Очищаем клетку (заливаем белым)
        tft.fillRect(wx + CELL_BORDER_OFFSET,
                     wy + CELL_CLEAR_OFFSET_Y,
                     razmer - CELL_BORDER_SIZE,
                     razmer,
                     WHITE);
    }
}
//============================================================
void ViewStacan(void)
{
    char myStr2[5];
    tft.setTextColor(RED);
    tft.setTextSize(1);

    // Update score
    if (OldScore != Score || FlNew) {
        tft.fillRect(SCORE_VALUE_X,
                     SCORE_VALUE_Y,
                     SCORE_VALUE_WIDTH,
                     SCORE_VALUE_HEIGHT,
                     WHITE);
        tft.setCursor(SCORE_VALUE_X,
                      SCORE_VALUE_Y);
        sprintf(myStr2, "%05d", Score);
        tft.print(myStr2);
        OldScore = Score;
    }

    // Update record
    if (OldRecord != Record || FlNew) {
        tft.setCursor(TOP_VALUE_X,
                      TOP_VALUE_Y);
        sprintf(myStr2, "%05d", Record);
        tft.fillRect(TOP_VALUE_X,
                     TOP_VALUE_Y,
                     SCORE_VALUE_WIDTH,
                     SCORE_VALUE_HEIGHT,
                     WHITE);
        tft.print(myStr2);
        OldRecord = Record;
    }

    // Update level
    if (OldLevel != Level || FlNew) {
        tft.fillRect(LEVEL_VALUE_X,
                     LEVEL_VALUE_Y,
                     LEVEL_VALUE_WIDTH,
                     LEVEL_VALUE_HEIGHT,
                     WHITE);
        tft.setCursor(LEVEL_VALUE_X,
                      LEVEL_VALUE_Y);
        tft.setTextSize(TEXT_SIZE_LARGE);
        tft.print(Level);
        OldLevel = Level;
    }

    FlNew = false;

    // Draw game field
    for (byte j = 3; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            if (MasSt[i][j] != MasOld[i][j]) {
                ViewQuad(i, j, MasSt[i][j]);
            }
        }
    }

    tft.drawFastHLine(FIELD_LEFT_X,
                      FIELD_BOTTOM_Y,
                      FIELD_BOTTOM_WIDTH,
                      BLACK);

    // Update old state
    for (byte j = 3; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            MasOld[i][j] = MasSt[i][j];
        }
    }
}

void ClearMas(byte MasStx[MaxX][MaxY])
{
    for (byte j = 0; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            MasStx[i][j] = 0;
        }
    }
}
//============================================================