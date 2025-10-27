#include "game_logic.h"

/*Анализ текущей игровой логики:

1. Система фигур
Сейчас игра использует только вертикальные линии из 3 блоков:

#define FIGURE_SIZE 3
byte fignext[3];

Проблема: Это не классический Тетрис, где 7 различных тетрамино.

2. Механика вращения
Текущее "вращение" - это просто циклическая перестановка цветов:

// Это не настоящее вращение фигуры!
byte temp = MasSt[x][y + ROTATION_TOP_INDEX];
MasSt[x][y + ROTATION_TOP_INDEX] = MasSt[x][y + ROTATION_BOTTOM_INDEX];
// ...

3. Система подсчета очков
Упрощенная система без классических бонусов за:
    Очистку нескольких линий одновременно
    "Т-spin"
    Комбо

4. Физика игры
Отсутствуют:
    Удержание фигуры (Hold)
    Предпросмотр следующих фигур (более чем 1)
    Wall kick (отталкивание от стен при вращении)

*/

//============================================================
void NewGame(void)
{
    // Draw vertical guides (вертикальные направляющие линии)
    for (byte i = 0; i < VERTICAL_GUIDES_COUNT; i++) {
        tft.drawFastVLine(SmeX + razmer * i - i,
                          GUIDES_TOP_Y,
                          GUIDES_HEIGHT,
                          BLACK);
    }

    // // Отладочная информация
    // tft.setCursor(10, 20);
    // tft.setTextColor(BLUE);
    // tft.print("NewGame SmeX:");
    // tft.print(SmeX);

    // Initialize comparison buffer with special value
    for (byte j = VISIBLE_FIELD_START; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            MasOld[i][j] = COMPARISON_BUFFER_INIT_VALUE;
        }
    }

    // Reset game state
    Score = INITIAL_SCORE;
    FlNew = true;
    OldScore = Score;
    ClearMas(MasSt);
    Arbeiten = true;
    GetNext();
    Counter = INITIAL_COUNTER;
    Level = INITIAL_LEVEL;
    tempspeed = MasSpeed[0];
    Record = myrecord;
    ViewStacan();
}
//============================================================
void gameover(void)
{
    Arbeiten = false;

    // // Отладочная информация
    // tft.setCursor(10, 10);
    // tft.setTextColor(RED);
    // tft.print("SmeX:");
    // tft.print(SmeX);
    // tft.print(" GX:");
    // tft.print(GAME_OVER_X);

    // Game over screen
    tft.drawRect(GAME_OVER_X,
                 GAME_OVER_Y,
                 GAME_OVER_WIDTH,
                 GAME_OVER_HEIGHT,
                 BLACK);
    tft.fillRect(GAME_OVER_X + GAME_OVER_BORDER_OFFSET,
                 GAME_OVER_Y + GAME_OVER_BORDER_OFFSET,
                 GAME_OVER_WIDTH - GAME_OVER_BORDER_OFFSET * 2,
                 GAME_OVER_HEIGHT - GAME_OVER_BORDER_OFFSET * 2,
                 WHITE);

    tft.setCursor(GAME_OVER_TEXT_X,
                  GAME_OVER_TEXT1_Y);
    tft.setTextSize(GAME_OVER_TEXT_SIZE);
    tft.setTextColor(RED);
    tft.print("GAME");

    tft.setCursor(GAME_OVER_TEXT_X,
                  GAME_OVER_TEXT2_Y);
    tft.print("OVER");

    if (Score > myrecord) {
        myrecord = Score;
    }
}
//============================================================
void ProcDemo(void)
{
    Score = 0;
    GetNext();

    // Fill with random blocks for demo
    for (byte j = 3; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            MasSt[i][j] = random(6) + 2;
        }
    }

    ViewStacan();
    mydelay(1000);

    if (!Demo)
        return;

    MyScore();
    if (Record < Score) {
        Record = Score;
    }
}
//============================================================
void GetNext(void)
{
    byte dx = NEXT_FIGURE_X;
    byte dy = NEXT_FIGURE_Y;
    x = INITIAL_FIGURE_X;
    y = INITIAL_FIGURE_Y;

    // Сначала генерируем новые цвета для следующей фигуры
    for (byte i = 0; i < FIGURE_SIZE; i++) {
        fignext[i] = random(NumCol) + COLOR_OFFSET;

        // Отрисовываем превью с новыми цветами
        tft.drawRect(dx, dy + razmer * i - i, razmer, razmer, BLACK);
        tft.fillRect(dx + CELL_BORDER_OFFSET,
                     dy + razmer * i - i + CELL_BORDER_OFFSET,
                     razmer - CELL_BORDER_SIZE,
                     razmer - CELL_BORDER_SIZE,
                     MasCol[fignext[i]]);
    }

    // Затем размещаем текущую фигуру в игровом поле
    if (!Demo) {
        for (byte i = 0; i < FIGURE_SIZE; i++) {
            MasSt[x][i] = fignext[i]; // Используем уже новые цвета
        }

        Counter++;
        if (Counter == NextLevel) {
            Counter = 0;
            Level++;
            if (Level > MaxLevel) {
                Level = MaxLevel;
            }
        }
        tempspeed = MasSpeed[Level - 1];
    }
}
//============================================================
bool figmove(int dx, int dy)
{
    bool fff = false;

    // Wall collision
    if (x + dx < MIN_FIELD_X || x + dx > MAX_FIELD_X) {
        return false;
    }

    // Horizontal movement
    if (dx != 0) {
        // Serial.print("figmove horizontal: dx=");
        // Serial.println(dx);
        // Check if the rightmost block of the figure can move horizontally
        if (MasSt[x + dx][y + dy + FIGURE_BOTTOM_OFFSET] == EMPTY_CELL) {
            return true;
        } else {
            return false;
        }
    }

    // Vertical movement (falling)
    if (dy > 0) {
        // Check if reached bottom or another block
        if (y + dy + FIGURE_BOTTOM_OFFSET > MAX_FIELD_Y ||
            MasSt[x + dx][y + dy + FIGURE_BOTTOM_OFFSET] > EMPTY_CELL) {

            if (y < GAMEOVER_HEIGHT_THRESHOLD) {
                gameover();
            } else {
                fff = true;
            }
        } else {
            // Move figure down - shift all blocks
            for (byte i = 0; i < FIGURE_SIZE; i++) {
                MasSt[x][y + FIGURE_BOTTOM_OFFSET - i + dy] = MasSt[x][y + FIGURE_BOTTOM_OFFSET - i];
            }
            MasSt[x][y] = EMPTY_CELL;
            y = y + dy;
        }

        if (fff) {
            MyScore();
            GetNext();
        }
        ViewStacan();
    }

    return true;
}
//============================================================
/*Логика подсчета очков:
Меньше 8 блоков: Score + TScore (простая сумма)
8 и более блоков: Score + TScore + (TScore - 8) * 2 (бонус за большие цепочки)
Например:
5 блоков = 5 очков
8 блоков = 8 + (8-8)*2 = 8 очков
10 блоков = 10 + (10-8)*2 = 14 очков*/
void MyScore(void)
{
    TScore = 0;
    while (FindFull()) {
        // Calculate score with bonus for large chains
        if (TScore > BONUS_THRESHOLD) {
            Score = Score + TScore + (TScore - BONUS_BASE) * BONUS_MULTIPLIER;
        } else {
            Score = Score + TScore;
        }
        // Play sound effect
        if (!Demo) {
            analogWrite(pin_Speaker, 5); // Sound
        }
        // Update display and add delay for visual feedback
        ViewStacan();
        analogWrite(pin_Speaker, SOUND_OFF);
        FlZ = true;
        mydelay(SCORE_DISPLAY_DELAY);
    }
    FlZ = false;
}
//============================================================
/*Алгоритм функции:
    Поиск цепочек по каждому цвету
    Помечаем блоки для удаления
    Визуальная обратная связь с задержкой
    Применяем гравитацию - блоки падают вниз
*/
bool FindFull(void)
{
    bool res = false;

    // Check for each color
    for (byte color = MIN_COLOR_INDEX; color <= MAX_COLOR_INDEX; color++) {
        ClearMas(MasTmp);

        // First pass: count neighbors for each cell
        for (byte j = VISIBLE_FIELD_START; j < MaxY; j++) {
            for (byte i = 0; i < MaxX; i++) {
                if (MasSt[i][j] == color) {
                    Sos(i, j, 1);
                }
            }
        }

        // Second pass: mark potential chains
        for (byte j = VISIBLE_FIELD_START; j < MaxY; j++) {
            for (byte i = 0; i < MaxX; i++) {
                if (MasTmp[i][j] >= MIN_NEIGHBORS_FOR_CHAIN) {
                    Sos(i, j, 2);
                }
            }
        }

        // Third pass: propagate chain marks
        do {
            fl = false;
            for (byte j = VISIBLE_FIELD_START; j < MaxY; j++) {
                for (byte i = 0; i < MaxX; i++) {
                    if (MasTmp[i][j] > 0) {
                        Sos(i, j, 3);
                    }
                }
            }
        } while (fl);

        // Remove marked blocks
        for (byte j = VISIBLE_FIELD_START; j < MaxY; j++) {
            for (byte i = 0; i < MaxX; i++) {
                if (MasTmp[i][j] == MARKED_FOR_REMOVAL) {
                    MasSt[i][j] = REMOVED_CELL_VALUE;
                    TScore++;
                }
            }
        }
    }

    // Visual feedback for chain removal
    if (TScore >= MIN_SCORE_FOR_DISPLAY) {
        ViewStacan();
        FlZ = true;
        mydelay(CHAIN_REMOVE_DELAY);
    }

    // Apply gravity - make blocks fall
    for (byte j = GRAVITY_START_INDEX; j < MaxY; j++) {
        for (byte i = 0; i < MaxX; i++) {
            while (MasSt[i][MaxY - 1 - j] == REMOVED_CELL_VALUE) {
                for (byte k = 0; k < MaxY - GRAVITY_OFFSET - j; k++) {
                    MasSt[i][MaxY - 1 - k - j] = MasSt[i][MaxY - 2 - k - j];
                }
                res = true;
            }
        }
    }

    return res;
}
//============================================================
/*Это вспомогательная функция для поиска соседей.
Режимы работы функции:
Mode 1: Подсчет количества соседей того же цвета
Mode 2: Помечание цепочек (если у соседа достаточно соседей)
Mode 3: Распространение пометок цепочек на соседние блоки
*/
void Sosed(int i, int j, int dx, int dy, byte mode)
{
    int nx = i + dx;
    int ny = j + dy;

    // Check if neighbor is within bounds and has the same color
    if (nx >= MIN_FIELD_X_INDEX && ny >= MIN_FIELD_Y_INDEX &&
        nx < MaxX && ny < MaxY &&
        MasSt[nx][ny] == MasSt[i][j]) {

        switch (mode) {
        case 1:
            // Mode 1: Count neighbors
            MasTmp[i][j]++;
            break;
        case 2:
            // Mode 2: Mark chains (if either cell has enough neighbors)
            if (MasTmp[nx][ny] >= MIN_NEIGHBORS_FOR_CHAIN ||
                MasTmp[i][j] > CHAIN_MARK_VALUE) {
                MasTmp[nx][ny] = CHAIN_MARK_VALUE;
                MasTmp[i][j] = CHAIN_MARK_VALUE;
            }
            break;
        case 3:
            // Mode 3: Propagate chain marks
            if (MasTmp[nx][ny] == CHAIN_MARK_VALUE &&
                MasTmp[i][j] != CHAIN_MARK_VALUE) {
                MasTmp[i][j] = CHAIN_MARK_VALUE;
                fl = true;
            }
            break;
        }
    }
}
//============================================================
/*Это вспомогательная функция, которая упрощает проверку всех соседних клеток вокруг текущей позиции (i, j).
Это простая функция-обертка для проверки всех четырех направлений
Функция Sos() просто вызывает Sosed() для всех четырех направлений из массива mmm*/
void Sos(int i, int j, byte mode)
{
    for (byte direction = 0; direction < DIRECTIONS_COUNT; direction++) {
        Sosed(i, j, mmm[direction][0], mmm[direction][1], mode);
    }
}
//============================================================
/*Логика функции:
Обработка энкодера - повороты влево/вправо
Выход из демо-режима - любое действие запускает игру
Горизонтальное движение - с проверкой возможности перемещения
Вращение цветов - циклический сдвиг цветов фигуры
Быстрое падение - ускорение падения при долгом нажатии*/
void mydelay(int md)
{
    unsigned long starttime = millis();

    while (millis() - starttime < md) {
        VAL = NO_MOVEMENT;
        Mp = mypush();

        // Handle encoder rotation
        if (encoder.timeRight != 0) {
            VAL = ENCODER_RIGHT;
            encoder.timeRight = 0;
            //--------------------
            // Serial.print("Encoder RIGHT: ");
            // Serial.println(VAL);
            //--------------------
        } else if (encoder.timeLeft != 0) {
            VAL = ENCODER_LEFT;
            encoder.timeLeft = 0;
            //--------------------
            // Serial.print("Encoder LEFT: ");
            // Serial.println(VAL);
            //--------------------
        }

        if (!digitalRead(pin_SW)) {
            VAL = NO_MOVEMENT;
        }

        // Demo mode handling - start new game on any input
        if ((VAL != NO_MOVEMENT || Mp != NotPush) && Demo) {
            Demo = false;
            NewGame();
        }

        // Move figure horizontally
        if (VAL != NO_MOVEMENT && figmove(VAL, 0) && !FlZ) {
            // Move all blocks of the figure horizontally
            for (byte i = 0; i < FIGURE_SIZE; i++) {
                MasSt[x + VAL][y + i] = MasSt[x][y + i];
                MasSt[x][y + i] = EMPTY_CELL;
            }
            ViewStacan();

            // Clear the trail above the moved figure
            if (MasSt[x][y + BOTTOM_BLOCK_CHECK_OFFSET] == EMPTY_CELL) {
                tft.drawFastHLine(SmeX + x * razmer - x + CELL_BORDER_OFFSET,
                                  SmeY + y * razmer - y - BOTTOM_BLOCK_CHECK_OFFSET,
                                  razmer - CELL_BORDER_SIZE,
                                  WHITE);
            }
            x = x + VAL;
        }

        // Rotate colors (short push) - циклический сдвиг цветов
        if (Mp == ShortPush && !FlZ) {
            byte temp = MasSt[x][y + ROTATION_TOP_INDEX];
            MasSt[x][y + ROTATION_TOP_INDEX] = MasSt[x][y + ROTATION_BOTTOM_INDEX];
            MasSt[x][y + ROTATION_BOTTOM_INDEX] = MasSt[x][y + ROTATION_MIDDLE_INDEX];
            MasSt[x][y + ROTATION_MIDDLE_INDEX] = temp;
            ViewStacan();
        }

        // Fast drop (long push)
        if (Mp == LongPush && !FlZ) {
            tempspeed = FAST_DROP_SPEED;
        }
    }
}
//============================================================