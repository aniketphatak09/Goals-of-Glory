/*
 * LED_Matrix.hpp
 *
 *  Created on: Nov 10, 2018
 *      Author: tahirlatif
 */

#ifndef LED_MATRIX_HPP_
#define LED_MATRIX_HPP_

#include <stdio.h>
#include "FreeRTOS.h"
//#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "string.h"
#include "singleton_template.hpp"

//extern uint32_t startScreenImg[32][3];

extern bool onePlayer;

class LEDMatrixGPIO : public SingletonTemplate<LEDMatrixGPIO>
{
public:
    enum Action
    {
        Rock = 0,
        Paper = 1,
        Scissors = 2
    };

    enum Player
    {
        Player1,
        Player2,
        Neither
    };

    void init();
    void setEn(bool set);
    void setLatch(bool set);
    void selRow(uint8_t row);
    void fillBuffer();
    void drawLine(uint8_t row);
    void verticalLineAnimation();
    void updateDisplay();
    void drawPixel(uint8_t x, uint8_t y, uint8_t c=7);
    void drawStartScreen();
    void drawScoreboard(uint8_t roundNum, uint8_t leftScore, uint8_t rightScore);
    void drawAction(LEDMatrixGPIO::Action left, LEDMatrixGPIO::Action right);
    void clearActions();
    void drawCountdown();
    void updateRoundNum(uint8_t num);
    void drawRoundEnd(LEDMatrixGPIO::Player winner);
    void clearScreen();
    void drawScore(uint8_t leftScore, uint8_t rightScore);
    void drawGameOver(Player matchWinner);
    void drawZero();
    void test();

private:
    LEDMatrixGPIO(){}
    friend class SingletonTemplate<LEDMatrixGPIO>;

    GPIO *addrA, *addrB, *addrC, *addrD, *addrE, *latch, *oe, *clk, *r1, *g1, *b1, *r2, *g2, *b2;
    uint64_t frameBuffer[64][3] = {{0}}; //`64 rows, 3 "color" columns, each entry is 32 bits for each row pixel
};


#endif /* LED_MATRIX_HPP_ */
