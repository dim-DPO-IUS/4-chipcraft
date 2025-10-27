#ifndef CONFIG_H
#define CONFIG_H

#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <Arduino_ST7789_Fast.h>
#include <EEPROM2.h>
#include <Encod_er.h>
#include <SPI.h>
#include <TimerOne.h>
//============================================================
// Display pins
#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 7
#define SCR_WD 240
#define SCR_HT 240

// Encoder and speaker pins
#define pin_CLK 2
#define pin_DT 4
#define pin_SW 3

#define pin_Speaker 5

//============================================================
// ИГРОВОЕ ПОЛЕ:
//============================================================
#define razmer 12 // Размер одной клетки в пикселях
// Размер:
// 10×12=120px × 20×12=240px (в 240х240 идеально вписывается!)
#define MaxX 10 // Ширина поля в клетках
// 20 видимых + 3 невидимых сверху (VISIBLE_FIELD_START)
#define MaxY 23 // Высота поля в клетках (20 - классический Тетрис)
// Начало видимой области поля: сколько первых строк не отображать
#define VISIBLE_FIELD_START 3
//
#define SmeX 10 // 10 (Klop:3) Смещение игрового поля по X
#define SmeY 5  // 0 (Klop:5) Смещение игрового поля по Y
// Границы игрового поля // Draw game borders
#define FIELD_TOP_Y 2
#define FIELD_BORDER_WIDTH 1

// Game field border constants
// X-координата левого края игрового поля
#define FIELD_LEFT_X SmeX
// Y-координата нижней границы игрового поля
#define FIELD_BOTTOM_Y (SmeY + (MaxY - VISIBLE_FIELD_START) * razmer - (MaxY - VISIBLE_FIELD_START))
// ширина нижней границы (рассчитывается из размера клеток и их количества)
#define FIELD_RIGHT_X (razmer * MaxX - MaxX + FIELD_LEFT_X)
// Alternative calculation for bottom border width:
#define FIELD_BOTTOM_WIDTH (razmer * MaxX - MaxX)
// или можно использовать:
// #define FIELD_BOTTOM_WIDTH (FIELD_RIGHT_X - FIELD_LEFT_X)
//============================================================
// Правая информационная панель
//============================================================
#define INFO_PANEL_X 150 // Базовая X-координата правого блока (бывший LL)
#define INFO_PANEL_Y_OFFSET 20  // Новое смещение по Y

// Относительные позиции внутри правой панели
#define NEXT_TEXT_X INFO_PANEL_X
#define NEXT_TEXT_Y 5
#define NEXT_FIGURE_X INFO_PANEL_X + 4 // 106 в оригинале относительно LL=97
#define NEXT_FIGURE_Y 16

#define LEVEL_TEXT_X INFO_PANEL_X
#define LEVEL_TEXT_Y 61
#define LEVEL_VALUE_X INFO_PANEL_X + 10 // 107 в оригинале относительно LL=97
#define LEVEL_VALUE_Y 73

#define SCORE_TEXT_X INFO_PANEL_X
#define SCORE_TEXT_Y 100
#define SCORE_VALUE_X INFO_PANEL_X
#define SCORE_VALUE_Y 113

#define TOP_TEXT_X INFO_PANEL_X + 5
#define TOP_TEXT_Y 135
#define TOP_VALUE_X INFO_PANEL_X
#define TOP_VALUE_Y 148
//============================================================
// Game Over screen constants - делаем относительными к полю
//============================================================
#define GAME_OVER_RELATIVE_X 4  // Смещение от левого края поля
#define GAME_OVER_RELATIVE_Y 40 // Смещение от верхнего края поля
#define GAME_OVER_WIDTH 81      // Ширина окна
#define GAME_OVER_HEIGHT 50     // Высота окна
// Рассчитаем абсолютные координаты на основе положения поля
#define GAME_OVER_X (SmeX + GAME_OVER_RELATIVE_X) // X позиция окна Game Over
#define GAME_OVER_Y (SmeY + GAME_OVER_RELATIVE_Y) // Y позиция окна Game Over
// #define GAME_OVER_X 7  // X позиция окна Game Over
// #define GAME_OVER_Y 40 // Y позиция окна Game Over
// Смещение для внутреннего заполнения
#define GAME_OVER_BORDER_OFFSET 1
// текст
#define GAME_OVER_TEXT_X 25 // X позиция текста
// выровнять текст по вертикали центра окна
// Y позиция текста "GAME"
#define GAME_OVER_TEXT1_Y (GAME_OVER_Y + 7) // выравнивать здесь
// Y позиция текста "OVER" считается автоматически тоносительно Y позиция текста "GAME"
#define GAME_OVER_TEXT2_Y (GAME_OVER_TEXT1_Y + 20) // не трогать

#define GAME_OVER_TEXT_SIZE 2 // Размер текста

//============================================================
#define NumCol 6
#define MaxLevel 8
#define NextLevel 80
#define interval 250
#define DurationOfLongPush 350

//============================================================
// Button states
#define NotPush 0
#define ShortPush 1
#define LongPush 2

//============================================================
// Color definitions
#define BLACK 0x0000
#define RED 0x001F
#define BLUE 0xF800
#define GREEN 0x07E0
#define YELLOW 0x07FF
#define MAGENTA 0xF81F
#define CYAN 0xFFE0
#define WHITE 0xFFFF

//============================================================
// Display layout constants for ViewStacan()
#define SCORE_VALUE_WIDTH 30
#define SCORE_VALUE_HEIGHT 8
#define LEVEL_VALUE_WIDTH 30
#define LEVEL_VALUE_HEIGHT 20
#define TEXT_SIZE_SMALL 1
#define TEXT_SIZE_LARGE 2
//============================================================
// Y positions for score display (уже должны быть, но проверь)
#define SCORE_TEXT_Y 100
#define SCORE_VALUE_Y 113
#define LEVEL_TEXT_Y 61
#define LEVEL_VALUE_Y 73
#define TOP_TEXT_Y 135
#define TOP_VALUE_Y 148
//============================================================

//============================================================
// ViewQuad drawing constants
#define CELL_BORDER_OFFSET 1   // Смещение для обводки
#define CELL_BORDER_SIZE 2     // Размер обводки (вычитается)
#define CELL_CLEAR_OFFSET_Y -1 // Специальное смещение Y при очистке

//============================================================
// NewGame constants
#define VERTICAL_GUIDES_COUNT 8          // Количество вертикальных направляющих
#define COMPARISON_BUFFER_INIT_VALUE 255 // Значение инициализации буфера сравнения
#define INITIAL_LEVEL 1                  // Начальный уровень
#define INITIAL_SCORE 0                  // Начальный счет
#define INITIAL_COUNTER 0                // Начальное значение счетчика
#define GUIDES_TOP_Y 2                   // Y-координата линий
// высота линий// Автоматический расчет
#define VISIBLE_ROWS (MaxY - VISIBLE_FIELD_START)            // 20
#define GUIDES_HEIGHT (VISIBLE_ROWS * razmer - VISIBLE_ROWS) // 20 * 12 - 20 = 220
//============================================================
// GetNext constants
#define INITIAL_FIGURE_X 3    // Начальная X позиция новой фигуры
#define INITIAL_FIGURE_Y 0    // Начальная Y позиция новой фигуры
#define FIGURE_SIZE 3         // Размер фигуры (количество блоков)
#define COLOR_OFFSET 2        // Смещение для цветов (2-7 используются для цветов)
#define NEXT_FIGURE_SPACING 1 // Отступ между блоками в превью (-i)
//============================================================
// figmove constants
#define MIN_FIELD_X 0               // Минимальная X координата
#define MAX_FIELD_X (MaxX - 1)      // Максимальная X координата
#define MAX_FIELD_Y (MaxY - 1)      // Максимальная Y координата
#define FIGURE_BOTTOM_OFFSET 2      // Смещение для проверки нижнего блока фигуры
#define GAMEOVER_HEIGHT_THRESHOLD 3 // Высота, при достижении которой игра заканчивается
#define EMPTY_CELL 0                // Пустая клетка
//============================================================
// FindFull constants
#define MIN_COLOR_INDEX 2         // Минимальный индекс цвета (2-7 используются)
#define MAX_COLOR_INDEX 7         // Максимальный индекс цвета
#define MIN_NEIGHBORS_FOR_CHAIN 2 // Минимальное количество соседей для начала цепочки
#define MARKED_FOR_REMOVAL 3      // Значение для помеченных на удаление блоков
#define REMOVED_CELL_VALUE 1      // Значение удаленной клетки
#define MIN_SCORE_FOR_DISPLAY 1   // Минимальное количество очков для отображения
#define CHAIN_REMOVE_DELAY 500    // Задержка при удалении цепочки (мс)

// Gravity calculation constants
#define GRAVITY_START_INDEX 0
#define GRAVITY_OFFSET 2
//============================================================
// MyScore constants
#define BONUS_THRESHOLD 7        // Порог для получения бонуса
#define BONUS_MULTIPLIER 2       // Множитель бонуса
#define BONUS_BASE 8             // База для расчета бонуса
#define SOUND_VOLUME 5           // Громкость звука (0-255)
#define SOUND_OFF 0              // Выключение звука
#define SCORE_DISPLAY_DELAY 1000 // Задержка отображения счета (мс)
#define INITIAL_TSCORE 0         // Начальное значение TScore
//============================================================
// Sosed constants
#define MIN_FIELD_X_INDEX 0           // Минимальный индекс X
#define MIN_FIELD_Y_INDEX 0           // Минимальный индекс Y
#define CHAIN_PROPAGATION_THRESHOLD 1 // Порог для распространения цепочки
#define CHAIN_MARK_VALUE 3            // Значение для помеченных в цепочке
//============================================================
// Sos constants
#define DIRECTIONS_COUNT 4 // Количество направлений для проверки (вверх, вниз, влево, вправо)
//============================================================
// mydelay constants
#define ENCODER_RIGHT 1             // Значение поворота энкодера вправо
#define ENCODER_LEFT -1             // Значение поворота энкодера влево
#define NO_MOVEMENT 0               // Отсутствие движения
#define FAST_DROP_SPEED 50          // Скорость быстрого падения
#define ROTATION_TOP_INDEX 0        // Индекс верхнего блока для вращения
#define ROTATION_MIDDLE_INDEX 1     // Индекс среднего блока для вращения
#define ROTATION_BOTTOM_INDEX 2     // Индекс нижнего блока для вращения
#define BOTTOM_BLOCK_CHECK_OFFSET 3 // Смещение для проверки нижнего блока
//============================================================
//============================================================
//============================================================
//============================================================
//============================================================
//============================================================
//============================================================
#endif