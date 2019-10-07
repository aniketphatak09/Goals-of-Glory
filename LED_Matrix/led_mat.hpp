/*
 * led_mat.hpp
 *
 *  Created on: Apr 28, 2018
 *      Author: Supradeep-PC
 */

#ifndef LED_MAT_HPP_
#define LED_MAT_HPP_

#define MATRIX_HEIGHT 64
#define MATRIX_WIDTH 64
#define MATRIX_NROWS 32

#define GOALPOST 9
#define GOALKEEPER_X_MIN 3  // Depth Min
#define GOALKEEPER_X_MAX 27 - 7 //// Depth Max
#define GOALKEEPER_Y_MIN (GOALPOST) // Width Max
#define GOALKEEPER_Y_MAX (MATRIX_WIDTH - 1) - GOALKEEPER_Y_MIN  // Width Max
#define GOALKEEPER_COLOR yellow1
#define  BALL_COLOR skyblue1
#define GOALPOST_COLOR cyan1

#define DELAY 10
#define X_START 58
//#define Y_HIT 5

#include "stdint.h"
#include "semphr.h"

#define swap(a, b) { int16_t t = a; a = b; b = t; }

typedef enum{
   no_collide,
   hit_square,
   hit_triangle,
}Collision;

/*
uint8_t black = 0;
uint8_t blue1 = 2;
uint8_t blue2 = 16;
uint8_t blue = 18;
uint8_t cyan1 = 7;
uint8_t cyan2 = 56;
uint8_t cyan = 63;
uint8_t green1 = 1;
uint8_t green2 = 8;
uint8_t green = 9;
uint8_t purple1 = 6;
uint8_t purple2 = 48;
uint8_t purple = 54;
uint8_t red1 = 4;
uint8_t red2 = 32;
uint8_t red = 36;
uint8_t skyblue1 = 3;
uint8_t skyblue2 = 24;
uint8_t skyblue = 27;
uint8_t yellow1 = 5;
uint8_t yellow2 = 40;
uint8_t yellow = 45;
*/


typedef enum
{
    black = 0,

    cyan1 = 7,
    cyan2 = 56,
    cyan = 63,

    blue1 = 1,
    blue2 = 8,
    blue = 9,

    green1 = 2,
    green2 = 16,
    green = 18,

    red1 = 4,
    red2 = 32,
    red = 36,

    yellow1 = 6,    // Red, Green
    yellow2 = 48,
    yellow = 54,

    skyblue1 = 3,   // Green, Blue
    skyblue2 = 24,
    skyblue = 27,

    purple1 = 5,    // Blue, Red
    purple2 = 40,
    purple = 45

}color_codes;

typedef enum {
    pending,
    goal,
    noGoal
}score_status;

class ledMat
{
public:

    ledMat();
    void displayInit();

    void drawGameName(uint8_t x, uint8_t y);
    void drawStartScreen(uint8_t x, uint8_t y);
    void drawPlayerReady(uint8_t x, uint8_t y, uint8_t color);
    void drawCountDown(uint8_t x, uint8_t y);
    void drawStartWord(uint8_t x, uint8_t y);

    void drawFrame();

    void drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void clearPixel(uint8_t x, uint8_t y, uint8_t color);
    void clearDisplay(uint8_t color);

    void updateDisplay();
    void moveDisplayDown();

    void drawCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void clearCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void moveCar(bool leftRight);

    void drawLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t color);
    void clearLine(uint8_t x, uint8_t y, uint8_t x1, uint8_t y1, uint8_t color);
    void drawLineDotted(uint8_t x, uint8_t w, uint8_t color);

    void drawDigit(uint8_t x, uint8_t y, uint8_t value, uint8_t color);
    void drawNumbers(uint8_t x, uint8_t y, uint8_t value, uint8_t color);
    void clearNumbers(uint8_t x, uint8_t y, uint8_t color);
    void drawTime(uint8_t x, uint8_t y, uint8_t value, uint8_t color1, uint8_t color2);
    void clearTime(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);

    void drawLetter(uint8_t x, uint8_t y, char alpha, uint8_t color);
    void drawText(uint8_t x, uint8_t y, uint8_t color);

    void drawObstacleSquare(uint8_t x, uint8_t y, uint8_t s, uint8_t color, uint8_t color1);
    void drawObstacleCircle(uint8_t x, uint8_t y, uint8_t color);
    void drawObstacleTriangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color1,uint8_t color2);


    //void checkHit(uint8_t x1, uint8_t x2);

    Collision checkHitPlayer1(uint8_t x);
    Collision checkHitPlayer2(uint8_t x);
    //void drawCrash(uint8_t x, uint8_t y, uint8_t color);
    void drawCrash(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2);
    void drawWinner(uint8_t x, uint8_t y, char player_number);
    void drawDraw(uint8_t x, uint8_t y);
    void drawGoalPost(uint8_t dist_left, uint8_t color);
    void drawBall(uint8_t x, uint8_t y, uint8_t color);
    void clearBall(uint8_t x, uint8_t y, uint8_t color);
    void drawGoalkeeper(uint8_t x, uint8_t y, uint8_t color);
    void clearGoalkeeper(uint8_t x, uint8_t y, uint8_t color);
    void drawGoalkeeperWithBall(uint8_t x, uint8_t y, uint8_t color);
    void clearGoalkeeperWithBall(uint8_t x, uint8_t y, uint8_t color);
    void resetShoot();
    void resetGame();
    void drawPlayer1Scoreboard(uint8_t x, uint8_t y);
    void drawPlayer2Scoreboard(uint8_t x, uint8_t y);
    void updateScore(uint8_t player, uint8_t shoot_num, score_status status);
    void drawArrow(uint8_t x,uint8_t y,uint8_t color );
    void clearArrow(uint8_t x,uint8_t y,uint8_t color );
    void drawGoalKeeperDemoPose1(uint8_t x, uint8_t y,uint8_t color);
    void drawGoalKeeperDemoPose2(uint8_t x, uint8_t y,uint8_t color);
    void clearGoalKeeperDemoPose1(uint8_t x, uint8_t y,uint8_t color);
    void clearGoalKeeperDemoPose2(uint8_t x, uint8_t y,uint8_t color);

    bool ballBlocked;
    //uint8_t ball_x, ball_y;
    uint8_t goalkeeper_x = 3, goalkeeper_y = 30;
    unsigned char goalpost_color = GOALPOST_COLOR;
    //SemaphoreHandle_t directionSem;
private:
    score_status p1_score[5];
    score_status p2_score[5];

    void calculateMatrixPosition(uint8_t *x, uint8_t *color);
    void drawScoreGoal(uint8_t x, uint8_t y,uint8_t color);
    void drawScoreNoGoal(uint8_t x, uint8_t y);
    void drawScorePending(uint8_t x, uint8_t y);
};



#endif /* LED_MAT_HPP_ */
