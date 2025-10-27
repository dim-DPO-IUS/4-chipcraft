#include "config.h"
#include "game_logic.h"
#include "game_state.h"
#include "graphics.h"
#include "input.h"

/* ====== GLOBAL VARIABLES DEFINITION ====== */
// Game state
byte MasSt[MaxX][MaxY], MasTmp[MaxX][MaxY], MasOld[MaxX][MaxY];
byte Level = 1, OldLevel, tr, flfirst = 1;
byte fignext[3];

// Score and timing
uint16_t MasCol[] = {WHITE, BLACK, RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN};
unsigned long TimeOfPush, Counter, Score = 0, TScore = 0;
unsigned long Record = 0, OldRecord, OldScore, myrecord;
word tempspeed;

// Flags and controls
bool fl, Demo = true, myfl = false, Arbeiten = false, FlNew, FlZ = false;
int8_t VAL, Mp, x, y;

// Movement directions
int8_t mmm[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

// Level speeds
word MasSpeed[MaxLevel] = {500, 450, 400, 350, 300, 250, 200, 100};

/* ====== HARDWARE OBJECTS DEFINITION ====== */
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
Encod_er encoder(pin_CLK, pin_DT, pin_SW);

/* ====== SETUP ====== */
void setup()
{
    // Отладка
    Serial.begin(9600);
    Serial.println("Game started");
    //-----------------------------

    randomSeed(analogRead(0));

    // Display initialization
    tft.init(SCR_WD, SCR_HT);
    tft.fillScreen(WHITE);

    // UI text
    tft.setCursor(LEVEL_TEXT_X, LEVEL_TEXT_Y);
    tft.setTextColor(BLACK);
    tft.setTextWrap(true);
    tft.setTextSize(TEXT_SIZE_SMALL);
    tft.print("LEVEL");

    tft.setCursor(NEXT_TEXT_X, NEXT_TEXT_Y);
    tft.print("NEXT");

    tft.setCursor(SCORE_TEXT_X, SCORE_TEXT_Y);
    tft.print("SCORE");

    tft.setCursor(TOP_TEXT_X, TOP_TEXT_Y);
    tft.print("TOP");

    FlNew = true;
    ViewStacan();
    GetNext();

    // Timer and hardware setup
    Timer1.initialize(interval);
    Timer1.attachInterrupt(timerInterrupt, interval);

    pinMode(pin_DT, INPUT);
    pinMode(pin_CLK, INPUT);
    pinMode(pin_SW, INPUT);
    digitalWrite(pin_SW, HIGH);

    delay(100);
    encoder.timeLeft = 0;
    encoder.timeRight = 0;

    // Левая граница
    tft.drawFastVLine(SmeX - FIELD_BORDER_WIDTH,
                      FIELD_TOP_Y,
                      GUIDES_HEIGHT,
                      BLACK);
    // Правая граница
    tft.drawFastVLine(SmeX + FIELD_BOTTOM_WIDTH + FIELD_BORDER_WIDTH,
                      FIELD_TOP_Y,
                      GUIDES_HEIGHT,
                      BLACK);
    // Нижняя граница
    tft.drawFastHLine(SmeX - FIELD_BORDER_WIDTH,
                      FIELD_TOP_Y + GUIDES_HEIGHT,
                      FIELD_BOTTOM_WIDTH + FIELD_BORDER_WIDTH * 2,
                      BLACK);
}

/* ====== MAIN LOOP ====== */
void loop()
{
    if (Demo) {
        ProcDemo();
    } else {
        if (Arbeiten) {
            mydelay(tempspeed);
            figmove(0, 1);
        } else if (mypush() == ShortPush) {
            NewGame();
        }
    }
}