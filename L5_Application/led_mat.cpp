/*
 * led_mat.cpp
 *
 *  Created on: Apr 28, 2018
 *      Author: Supradeep-PC
 */

#include "FreeRTOS.h"
#include "LPC17xx.h"
#include"led_mat.hpp"
#include <stdint.h>
#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "stdio.h"
#include "stdbool.h"
#include "printf_lib.h"
#include "string.h"
#include "stdlib.h"
///


/*
GPIO R1(P1_19);
GPIO G1(P1_20);
GPIO B1(P1_22);
GPIO R2(P1_23);
GPIO G2(P1_28);
GPIO B2(P1_29);
GPIO A(P1_30);
GPIO B(P1_31);
GPIO C(P2_0);
GPIO D(P2_1);
GPIO E(P2_2);
GPIO clk(P2_3);
GPIO lat(P2_4);
GPIO oe(P2_5);
 */

GPIO R1(P1_20);
GPIO G1(P2_7);
GPIO B1(P1_22);
GPIO R2(P1_23);
GPIO G2(P2_6);
GPIO B2(P1_28);

GPIO A(P1_29);
GPIO B(P2_5);
GPIO C(P2_0);
GPIO D(P2_4);
GPIO E(P1_19);

GPIO clk(P2_1);
GPIO lat(P2_3);
GPIO oe(P2_2);

GPIO dir(P0_30);

uint8_t matrixbuff[MATRIX_NROWS][MATRIX_HEIGHT];


/*
 ** Initialize pins as output/input, high/low.
 */
void ledMat::displayInit()
{
    R1.setAsOutput();
    G1.setAsOutput();
    B1.setAsOutput();
    R2.setAsOutput();
    G2.setAsOutput();
    B2.setAsOutput();
    A.setAsOutput();
    B.setAsOutput();
    C.setAsOutput();
    D.setAsOutput();
    E.setAsOutput();
    clk.setAsOutput();
    lat.setAsOutput();
    oe.setAsOutput();
    dir.setAsOutput();

    R1.setLow();
    G1.setLow();
    B1.setLow();
    R2.setLow();
    G2.setLow();
    B2.setLow();
    A.setLow();
    B.setLow();
    C.setLow();
    D.setLow();
    E.setLow();
    clk.setLow();
    lat.setLow();
    oe.setLow();
    dir.setHigh();
    //    R1.setHigh();
    //    G1.setHigh();
    //    B1.setHigh();
    //    R2.setHigh();
    //    G2.setHigh();
    //    B2.setHigh();
    //    A.setHigh();
    //    B.setHigh();
    //    C.setHigh();
    //    D.setHigh();
    //    E.setHigh();
    //    clk.setHigh();
    //    lat.setHigh();
    //    oe.setHigh();
    //    dir.setHigh();
    //
    memset(matrixbuff, 0, MATRIX_HEIGHT * MATRIX_NROWS);

    //directionSem = xSemaphoreCreateBinary();
}


/*
 ** Function to glow a pixel.
 */
void ledMat::drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if ((x < 0) || (x > 63))
        return;
    if ((y < 0) || (y > 63))
        return;

    calculateMatrixPosition(&x, &color);

    //   if ((matrixbuff[x][y] & color) != color)
    matrixbuff[x][y] |= color;
}


/*
 ** Function to clear a pixel.
 */
void ledMat::clearPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if ((x < 0) || (x > 63))
        return;
    if ((y < 0) || (y > 63))
        return;

    calculateMatrixPosition(&x, &color);

    matrixbuff[x][y] &= ~(color);
}


/*
 ** Function to clear any particular section of the display or  entire display or .
 ** Can clear any particular color(s) in a section/entire display.
 ** To clear all colors from entire display, pass color as "cyan".
 */
void ledMat::clearDisplay(uint8_t color)
{
    for(uint8_t row = 0; row < 32; row++)
    {
        for(uint8_t col = 0; col < 64; col++)
        {
            clearPixel(row, col, color);
        }
    }
}

/*
 ** Function to update the display matrix.
 */
void ledMat::updateDisplay()
{
    for(uint8_t row = 0; row < 32; row++)
    {
        for(uint8_t col = 0; col < 64; col++)
        {

            if(matrixbuff[row][col] & 0x1)
            {
                B1.setHigh();
            }
            else
            {
                B1.setLow();
            }
            if(matrixbuff[row][col] & 0x2)
            {
                G1.setHigh();
            }
            else
            {
                G1.setLow();
            }
            if(matrixbuff[row][col] & 0x4)
            {
                R1.setHigh();
            }
            else
            {
                R1.setLow();
            }
            if(matrixbuff[row][col] & 0x8)
            {
                B2.setHigh();
            }
            else
            {
                B2.setLow();
            }
            if(matrixbuff[row][col] & 0x10)
            {
                G2.setHigh();
            }
            else
            {
                G2.setLow();
            }
            if(matrixbuff[row][col] & 0x20)
            {
                R2.setHigh();
            }
            else
            {
                R2.setLow();
            }

            clk.setHigh();
            clk.setLow();
        }

        oe.setHigh();
        lat.setHigh();

        A.setLow();
        B.setLow();
        C.setLow();
        D.setLow();
        E.setLow();

        if(row & 0x1)
        {
            A.setHigh();
        }

        if(row & 0x2)
        {
            B.setHigh();
        }

        if(row & 0x4)
        {
            C.setHigh();
        }

        if(row & 0x8)
        {
            D.setHigh();
        }

        if(row & 0x10)
        {
            E.setHigh();
        }

        lat.setLow();
        oe.setLow();
    }
}

/*
 ** Function to bring down the display by one pixel.
 */
void ledMat::moveDisplayDown()
{
    static uint8_t count = 0;
    uint8_t x, y;

    // From row 1 to row 31.
    for (x=1; x<31; x++)
    {
        // Column 7 to column 53.
        for (y=7; y < 53; y++)
        {
            // If less than col 52, then bring down.
            if (y < 52)
            {
                matrixbuff[x][y] = matrixbuff[x][y+1];
            }

            // If col = 52 (top most play area col), then clear.
            if (y == 52)
            {
                clearPixel(x, y, 63);

                count = (x == 9) ? count+1 : count;

                /*
                 ** For rows 10 & 21, print dotted lane lines.
                 ** Matching with dotted line function.
                 */
                if (x == 10 || x == 21)
                {

                    if (count == 4 || count == 5)
                    {
                        clearPixel(x, y, 63);
                        count = (count == 4) ? 0 : count;
                    }

                    else
                    {
                        drawPixel(x, y,cyan);
                    }
                }
            }
        }
    }
}


/*
 ** Function to check if the car hit the obstacle or no.
 ** Get the x coordinates of both cars from the Master module.
 */
/*
void ledMat::checkHit(uint8_t x1, uint8_t x2)
{
    uint8_t temp;
    for (int i=x1; i <= (x1+3); i++)
    {
        temp = matrixbuff[i][7];
        if ( (temp & 0x07) == 5)
        {
            LD.setNumber(x1);
            LE.on(1);
            break;
        }
        if ( (temp & 0x07) == 3)
        {
            LD.setNumber(x1);
            LE.on(2);
        }
        else
        {
            LE.off(1);
            LE.toggle(4);
            LD.setNumber(x1);
        }
    }
    for (int i=x2; i <= (x2+3); i++)
    {
        temp = matrixbuff[i][7];
        if ( (temp & 0x38) == 40)
        {
            LD.setNumber(x2);
            LE.on(1);
            break;
        }
        if ( (temp & 0x38) == 24)
        {
            LD.setNumber(x2);
            LE.on(2);
        }
        else
        {
             LE.off(1);
             LE.toggle(4);
             LD.setNumber(x1);
        }
    }
}
 */


/*
 ** Function to check if Player 1 car hit the obstacle or no.
 ** Get the x coordinate of the car from the Master module.
 ** Return an enum, "Collision".
 */
Collision ledMat::checkHitPlayer1(uint8_t x1)
{
    uint8_t temp;
    Collision mycarCollision;

    /*
     ** Car's width ranges from x to x+3.
     ** Check for obstacle for all values in this range.
     */
    for (int i=x1; i <= (x1+3); i++)
    {
        // Get the matrix cell value just above the car.
        temp = matrixbuff[i][7];

        // Checking for square obstacle (to be collected).
        if ( (temp & 0x07) == yellow1)
        {
            mycarCollision = hit_square;
            break;
            //return mycarCollision;
        }

        // Checking for triangular obstacle (to be avoided).
        if ( (temp & 0x07) == skyblue1)
        {
            mycarCollision = hit_triangle;
            break;
            //return mycarCollision;
        }

        else
        {
            mycarCollision = no_collide;
        }
    }

    return mycarCollision;
}


/*
 ** Function to check if Player 2 car hit the obstacle or no.
 ** Get the x coordinate of the car from the Master module.
 ** Return an enum, "Collision".
 */
Collision ledMat::checkHitPlayer2(uint8_t x2)
{
    uint8_t temp;
    Collision mycarCollision;


    for (int i=x2; i <= (x2+3); i++)
    {
        temp = matrixbuff[i][7];

        if ( (temp & 0x38) == yellow2)
        {
            mycarCollision = hit_square;
            break;
            //return mycarCollision;
        }

        if ( (temp & 0x38) == skyblue2)
        {
            mycarCollision = hit_triangle;
            break;
            //return mycarCollision;

        }

        else
        {
            mycarCollision=no_collide;
        }
    }
    return mycarCollision;
}

void ledMat::drawCrash(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2)
{
    //drawLine(x, y+4, x+3, y+4, color1);
    drawLine(x, y+5, x+3, y+5, color1);
    drawPixel(x, y+6, color2);  drawLine(x+1, y+6, x+2, y+6, color1); //drawPixel(x+3, y+6, color2);
    drawPixel(x+1, y+7, color2); drawPixel(x+2, y+7, color2);
    drawPixel(x, y+8, color2);

    if (x>1)
    {
        //drawPixel(x-1, y+5, color2);
        //drawPixel(x-3, y+6, color2);
        //drawPixel(x-2, y+7, color2);
    }
    if (x<27)
    {
        //drawPixel(x+4, y+5, color2);
        //drawPixel(x+6, y+6, color2);
        //drawPixel(x+5, y+7, color2);
    }

}


/*
 ** Function to draw line between 2 points.
 */
void ledMat::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }

    else
    {
        ystep = -1;
    }

    for (; x0<=x1; x0++)
    {
        if (steep)
        {
            drawPixel(y0, x0, color);
        }

        else
        {
            drawPixel(x0, y0, color);
        }

        err -= dy;

        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

/*
 ** Function to clear line between 2 points.
 */
void ledMat::clearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }

    else
    {
        ystep = -1;
    }

    for (; x0<=x1; x0++)
    {
        if (steep)
        {
            clearPixel(y0, x0, color);
        }

        else
        {
            clearPixel(x0, y0, color);
        }

        err -= dy;

        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

/*
 ** Function to display dotted lanes.
 */
void ledMat::drawLineDotted(uint8_t x, uint8_t w, uint8_t color)
{
    for (int i = 1; i < 52; )
    {
        drawLine(x, i, x, i+w-1, color);
        i +=5;
    }
}


/*
 ** Function to display the border grid and center partition.
 */
void ledMat::drawFrame()
{
    drawLine(0, 0, 0, 63, purple);      // Row: 0 & 32
    //drawLine(0, 0, 0, 63, purple1);
    //drawLine(0, 0, 0, 53, purple2);

    drawLine(31, 0, 31, 63, purple);    // Row: 31 & 63
    //drawLine(31, 0, 31, 53, purple1);
    //drawLine(31, 0, 31, 63, purple2);

    drawLine(0, 0, 31, 0, purple);      // Column: 0
    drawLine(1, 53, 30, 53, purple);      // Column: 0
    drawLine(0, 63, 31, 63, purple);    // Column: 63
}


/*
 ** Function to display circular obstacle.
 ** Not using this for the moment.
 */
void ledMat::drawObstacleCircle(uint8_t x, uint8_t y, uint8_t color)
{
    drawPixel(x, y+2, color);
    drawPixel(x-1, y+1, color); drawPixel(x, y+1, color); drawPixel(x+1, y+1, color);
    drawPixel(x-2, y, color); drawPixel(x-1, y, color);   drawPixel(x, y, color);   drawPixel(x+1, y, color);   drawPixel(x+2, y, color);
    drawPixel(x-1, y-1, color); drawPixel(x, y-1, color); drawPixel(x+1, y-1, color);
    drawPixel(x, y-2, color);

}


/*
 ** Function to draw triangular obstacle.
 */
void ledMat::drawObstacleTriangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color1, uint8_t color2)
{
    // To get hollow triangle.
    drawLine(x, y, x+w, y, color1);    // Bottom row
    drawPixel(x, y+1, color1);
    drawPixel(x+w, y+1, color1);
    drawPixel(x+1, y+2, color1);
    drawPixel(x+w-1, y+2, color1);
    drawPixel(x+w/2, y+3, color1);

    // To fill the triangle.
    drawLine(x+1, y+1, x+w-1, y+1, color2);
    drawPixel(x+2, y+2, color2);

}


/*
 ** Function to draw square obstacle.
 */
void ledMat::drawObstacleSquare(uint8_t x, uint8_t y, uint8_t s, uint8_t color, uint8_t color1)
{
    //drawPixel(x+1, y,color1);  drawPixel(x+2,y, color1);   drawPixel(x+3, y, color1);  drawPixel(x+4, y, color1);
    //drawPixel(x+1,y+1,color1); drawPixel(x+2,y+1,color1);  drawPixel(x+3,y+1,color1);  drawPixel(x+4,y+1,color1);
    //drawPixel(x+1,y+2,color1); drawPixel(x+2,y+2,color1);  drawPixel(x+3,y+2,color1);  drawPixel(x+4,y+2,color1);
    //drawPixel(x+1,y+3,color1); drawPixel(x+2,y+3,color1);  drawPixel(x+3,y+3,color1);  drawPixel(x+4,y+3,color1);
    //drawPixel(x+1,y+4,color1); drawPixel(x+2,y+4,color1);  drawPixel(x+3,y+4,color1);  drawPixel(x+4,y+4,color1);

    drawLine(x, y, x+s, y, color);
    drawLine(x, y, x, y+s, color);
    drawLine(x+s, y, x+s, y+s, color);
    drawLine(x, y+s, x+s, y+s, color);
    drawPixel(x+1,y+1, color1);
    drawPixel(x+2, y+1, color1);
    drawPixel(x+1,y+2, color1);
    drawPixel(x+2, y+2, color1);
}


/*
 ** Function to display car.
 */
void ledMat::drawCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2)
{
    drawPixel(x, y,color1);                                                 drawPixel(x+3, y, color1);
    drawPixel(x+1,y+1,color1);  drawPixel(x+2,y+1,color1);
    drawPixel(x+1,y+2,color1);  drawPixel(x+2,y+2,color1);
    drawPixel(x+1,y+3,color1);  drawPixel(x+2,y+3,color1);
    drawPixel(x,y+4,color1);                                                drawPixel(x+3,y+4,color1);

}


/*
 ** Function to clear car's last position.
 */
void ledMat::clearCar(uint8_t x, uint8_t y, uint8_t color1, uint8_t color2)
{
    for(uint8_t i = x; i <= (x+3); i++)
    {
        for(uint8_t j = y; j <= (y+4); j++)
        {
            clearPixel(i, j, color1);
            //clearPixel(i, j, color2);     // If we are using 2 colors for car, clear other color too if using.
        }
    }
}


void ledMat::drawDigit(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color)
{
    //swap(x0, y0);

    if(value == 0)
    {
        drawPixel(x0, y0+1, color);
        drawPixel(x0, y0+2, color);
        drawPixel(x0, y0+3, color);
        drawPixel(x0, y0+4, color);
        drawPixel(x0, y0+5, color);
        drawPixel(x0, y0+6, color);
        drawPixel(x0+1, y0, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+1, color);
        drawPixel(x0+4, y0+2, color);
        drawPixel(x0+4, y0+3, color);
        drawPixel(x0+4, y0+4, color);
        drawPixel(x0+4, y0+5, color);
        drawPixel(x0+4, y0+6, color);
    }

    else if(value == 1)
    {
        //        drawPixel(x0+1, y0, color);
        //        drawPixel(x0+1, y0+6, color);
        //        drawPixel(x0+2, y0, color);
        //        drawPixel(x0+2, y0+1, color);
        //        drawPixel(x0+2, y0+2, color);
        //        drawPixel(x0+2, y0+3, color);
        //        drawPixel(x0+2, y0+4, color);
        //        drawPixel(x0+2, y0+5, color);
        //        drawPixel(x0+2, y0+6, color);
        //        drawPixel(x0+2, y0+7, color);
        //        drawPixel(x0+3, y0, color);

        drawLine(x0,y0,x0+4,y0,color);
        drawPixel(x0+1,y0-1,color);
        drawLine(x0+4,y0-1,x0+4,y0+1,color);
    }

    else if(value == 2)
    {
        //        drawPixel(x0, y0, color);
        //        drawPixel(x0, y0+1, color);
        //        drawPixel(x0, y0+2, color);
        //        drawPixel(x0, y0+6, color);
        //        drawPixel(x0+1, y0, color);
        //        drawPixel(x0+1, y0+3, color);
        //        drawPixel(x0+1, y0+7, color);
        //        drawPixel(x0+2, y0, color);
        //        drawPixel(x0+2, y0+3, color);
        //        drawPixel(x0+2, y0+7, color);
        //        drawPixel(x0+3, y0, color);
        //        drawPixel(x0+3, y0+4, color);
        //        drawPixel(x0+3, y0+7, color);
        //        drawPixel(x0+4, y0, color);
        //        drawPixel(x0+4, y0+1, color);
        //        drawPixel(x0+4, y0+5, color);
        //        drawPixel(x0+4, y0+6, color);
        drawPixel(x0+1,y0,color);
        drawPixel(x0,y0+1,color);drawPixel(x0,y0+2,color);
        drawPixel(x0+1,y0+3,color);
        drawPixel(x0+2,y0+2,color);
        drawPixel(x0+3,y0+1,color);
        drawPixel(x0+4,y0,color);
        drawPixel(x0+4,y0+1,color);
        drawPixel(x0+4,y0+2,color);
        drawPixel(x0+4,y0+3,color);

    }

    else if(value == 3)
    {
        //        drawPixel(x0, y0+1, color);
        //        drawPixel(x0, y0+6, color);
        //        drawPixel(x0+1, y0, color);
        //        drawPixel(x0+1, y0+4, color);
        //        drawPixel(x0+1, y0+7, color);
        //        drawPixel(x0+2, y0, color);
        //        drawPixel(x0+2, y0+4, color);
        //        drawPixel(x0+2, y0+7, color);
        //        drawPixel(x0+3, y0, color);
        //        drawPixel(x0+3, y0+4, color);
        //        drawPixel(x0+3, y0+7, color);
        //        drawPixel(x0+4, y0+1, color);
        //        drawPixel(x0+4, y0+2, color);
        //        drawPixel(x0+4, y0+3, color);
        //        drawPixel(x0+4, y0+5, color);
        //        drawPixel(x0+4, y0+6, color);
        drawPixel(x0,y0+1,color);drawPixel(x0+1,y0+2,color);
        drawPixel(x0+1,y0,color);drawPixel(x0+1,y0+3,color);
        drawPixel(x0+2,y0+2,color);
        drawPixel(x0+3,y0,color);drawPixel(x0+3,y0+3,color);
        drawPixel(x0+4,y0+1,color);drawPixel(x0+4,y0+2,color);

    }

    else if(value == 4)
    {
        drawPixel(x0, y0+2, color);
        drawPixel(x0, y0+3, color);
        drawPixel(x0, y0+4, color);
        drawPixel(x0+1, y0+2, color);
        drawPixel(x0+1, y0+5, color);
        drawPixel(x0+2, y0+2, color);
        drawPixel(x0+2, y0+6, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+1, color);
        drawPixel(x0+3, y0+2, color);
        drawPixel(x0+3, y0+3, color);
        drawPixel(x0+3, y0+4, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+2, color);
    }

    else if(value == 5)
    {
        drawPixel(x0, y0+1, color);
        drawPixel(x0, y0+5, color);
        drawPixel(x0, y0+6, color);
        drawPixel(x0, y0+7, color);
        drawPixel(x0+1, y0, color);
        drawPixel(x0+1, y0+4, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+4, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+4, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+1, color);
        drawPixel(x0+4, y0+2, color);
        drawPixel(x0+4, y0+3, color);
        drawPixel(x0+4, y0+7, color);
    }

    else if(value == 6)
    {
        drawPixel(x0, y0+1, color);
        drawPixel(x0, y0+2, color);
        drawPixel(x0, y0+3, color);
        drawPixel(x0, y0+4, color);
        drawPixel(x0, y0+5, color);
        drawPixel(x0, y0+6, color);
        drawPixel(x0+1, y0, color);
        drawPixel(x0+1, y0+4, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+4, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+4, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+1, color);
        drawPixel(x0+4, y0+2, color);
        drawPixel(x0+4, y0+3, color);
        drawPixel(x0+4, y0+6, color);
    }

    else if(value == 7)
    {
        drawPixel(x0, y0+7, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+1, color);
        drawPixel(x0+2, y0+2, color);
        drawPixel(x0+2, y0+3, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0+4, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+5, color);
        drawPixel(x0+4, y0+6, color);
        drawPixel(x0+4, y0+7, color);
    }

    else if(value == 8)
    {
        drawPixel(x0, y0+1, color);
        drawPixel(x0, y0+2, color);
        drawPixel(x0, y0+3, color);
        drawPixel(x0, y0+5, color);
        drawPixel(x0, y0+6, color);
        drawPixel(x0+1, y0, color);
        drawPixel(x0+1, y0+4, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+4, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+4, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+1, color);
        drawPixel(x0+4, y0+2, color);
        drawPixel(x0+4, y0+3, color);
        drawPixel(x0+4, y0+5, color);
        drawPixel(x0+4, y0+6, color);
    }

    else if(value == 9)
    {
        drawPixel(x0, y0+1, color);
        drawPixel(x0, y0+4, color);
        drawPixel(x0, y0+5, color);
        drawPixel(x0, y0+6, color);
        drawPixel(x0+1, y0, color);
        drawPixel(x0+1, y0+3, color);
        drawPixel(x0+1, y0+7, color);
        drawPixel(x0+2, y0, color);
        drawPixel(x0+2, y0+3, color);
        drawPixel(x0+2, y0+7, color);
        drawPixel(x0+3, y0, color);
        drawPixel(x0+3, y0+3, color);
        drawPixel(x0+3, y0+7, color);
        drawPixel(x0+4, y0+1, color);
        drawPixel(x0+4, y0+2, color);
        drawPixel(x0+4, y0+3, color);
        drawPixel(x0+4, y0+4, color);
        drawPixel(x0+4, y0+5, color);
        drawPixel(x0+4, y0+6, color);
    }
}



/*
 ** Function to display numbers on the screen.
 */
void ledMat::drawNumbers(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color)
{
    uint8_t num[2] = {};

    num[0] = value / 10;
    num[1] = value % 10;

    for(uint8_t i = 0; i < 2; i++)
    {
        x0 = (i == 1)? x0+7 : x0;

        drawDigit(x0, y0, num[i], color);
    }
}


void ledMat::drawLetter(uint8_t x, uint8_t y, char alpha, uint8_t color)
{
    if (alpha == 'a' || alpha == 'A')
    {


        /*drawPixel(x+2, y+4, color);
        drawPixel(x+1, y+3, color);                             drawPixel(x+3, y+3, color);
        drawPixel(x, y+2, color);                                                             drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+1, y+1, color); drawPixel(x+2, y+1, color); drawPixel(x+3, y+1, color); drawPixel(x+4, y+1, color);
        drawPixel(x, y, color); */
        drawPixel(x, y+1, color);drawPixel(x, y+4, color);drawPixel(x, y+2, color);drawPixel(x, y+3, color);
        drawPixel(x+1, y, color);drawPixel(x+1, y+1, color);drawPixel(x+1, y+2, color);drawPixel(x+1, y+3, color);drawPixel(x+1, y+4, color);drawPixel(x+1, y+5, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+1, color);drawPixel(x+2, y+4, color);drawPixel(x+2, y+5, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+1, color);drawPixel(x+3, y+2, color);drawPixel(x+3, y+3, color);drawPixel(x+3, y+4, color);drawPixel(x+3, y+5, color);
        drawPixel(x+4, y, color);drawPixel(x+4, y+1, color);drawPixel(x+4, y+2, color);drawPixel(x+4, y+3, color);drawPixel(x+4, y+4, color);drawPixel(x+4, y+5, color);
        drawPixel(x+5, y, color);drawPixel(x+5, y+1, color);drawPixel(x+5, y+4, color);drawPixel(x+5, y+5, color);
        drawPixel(x+6, y, color);drawPixel(x+6, y+1, color);drawPixel(x+6, y+4, color);drawPixel(x+6, y+5, color);
    }

    if (alpha == 'b' || alpha == 'B')
    {
        drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color);
        drawPixel(x, y+3, color); drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+4, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);
    }

    if (alpha == 'c' || alpha == 'C')
    {
        drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color); //drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);
        drawPixel(x, y+2, color);
        drawPixel(x, y+1, color);
        drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color); //drawPixel(x+4, y, color);
    }

    if (alpha == 'd' || alpha == 'D')
    {
        drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color);
        drawPixel(x, y+3, color); drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+4, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);
    }

    if (alpha == 'e' || alpha == 'E')
    {
        drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color); drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color);
        drawPixel(x, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color); drawPixel(x+4, y, color);
    }

    if (alpha == 'f' || alpha == 'F')
    {
        drawPixel(x, y, color);   drawPixel(x, y+1, color);   drawPixel(x, y+2, color);   drawPixel(x, y+3, color);   drawPixel(x, y+4, color);
        drawPixel(x+1, y, color); drawPixel(x+1, y+1, color); drawPixel(x+1, y+2, color); drawPixel(x+1, y+3, color); drawPixel(x+1, y+4, color);
        drawPixel(x+2, y, color); drawPixel(x+2, y+1, color);
        drawPixel(x+3, y, color); drawPixel(x+3, y+1, color); drawPixel(x+3, y+2, color); drawPixel(x+3, y+3, color);
        drawPixel(x+4, y, color); drawPixel(x+4, y+1, color);drawPixel(x+4, y+2, color); drawPixel(x+4, y+3, color);
        drawPixel(x+5, y, color); drawPixel(x+5, y+1, color);
        drawPixel(x+6, y, color); drawPixel(x+6, y+1, color);

    }
    if (alpha ==  'G') {
        //drawPixel(x, y+2, color);
        drawPixel(x, y+1, color);drawPixel(x, y+2, color);drawPixel(x, y+3, color);drawPixel(x, y+4, color);drawPixel(x, y+5, color);
        drawPixel(x+1, y, color);drawPixel(x+1, y+1, color);drawPixel(x+1, y+2, color);drawPixel(x+1, y+3, color);drawPixel(x+1, y+4, color);drawPixel(x+1, y+5, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+1, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+1, color);drawPixel(x+3, y+3, color);drawPixel(x+3, y+4, color);drawPixel(x+3, y+5, color);
        drawPixel(x+4, y, color);drawPixel(x+4, y+1, color);drawPixel(x+4, y+4, color);drawPixel(x+4, y+5, color);
        drawPixel(x+5, y, color);drawPixel(x+5, y+1, color);drawPixel(x+5, y+2, color);drawPixel(x+5, y+3, color);drawPixel(x+5, y+4, color);drawPixel(x+5, y+5, color);
        drawPixel(x+6, y+1, color);drawPixel(x+6, y+2, color);drawPixel(x+6, y+3, color);drawPixel(x+6, y+4, color);drawPixel(x+6, y+5, color);
    }
    if (alpha == 'g') {
        //drawPixel(x, y+2, color);
        drawPixel(x, y+1, color);drawPixel(x, y+2, color);drawPixel(x, y+3, color);
        drawPixel(x+1, y, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+2, color);drawPixel(x+2, y+3, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+3, color);
        drawPixel(x+4, y+1, color);drawPixel(x+4, y+2, color);drawPixel(x+4, y+3, color);
    }

    if (alpha =='h' || alpha =='H')
    {
        drawPixel(x, y+4, color);                                       drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);                                       drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color); drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+4, y+1, color);
        drawPixel(x, y, color);  drawPixel(x+4, y, color);
    }

    if (alpha == 'i' || alpha == 'I')
    {
        drawPixel(x+2, y+4, color);
        //drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color); drawPixel(x+4, y+4, color);
        drawPixel(x+2, y+3, color);
        drawPixel(x+2, y+2, color);
        drawPixel(x+2, y+1, color);
        //drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color); drawPixel(x+4, y, color);
        drawPixel(x+2, y, color);
    }

    if (alpha == 'l' || alpha == 'L')
    {
        drawPixel(x, y, color);drawPixel(x, y+1, color);//drawPixel(x, y+2, color);
        drawPixel(x+1, y, color);drawPixel(x+1, y+1, color);//drawPixel(x+1, y+2, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+1, color);//drawPixel(x+2, y+2, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+1, color);//drawPixel(x+3, y+2, color);
        drawPixel(x+4, y, color);drawPixel(x+4, y+1, color);//drawPixel(x+4, y+2, color);drawPixel(x+4, y+3, color);drawPixel(x+4, y+4, color);drawPixel(x+4, y+5, color);
        drawPixel(x+5, y, color);drawPixel(x+5, y+1, color);drawPixel(x+5, y+2, color);drawPixel(x+5, y+3, color);drawPixel(x+5, y+4, color);//drawPixel(x+5, y+5, color);
        drawPixel(x+6, y, color);drawPixel(x+6, y+1, color);drawPixel(x+6, y+2, color);drawPixel(x+6, y+3, color);drawPixel(x+6, y+4, color);//drawPixel(x+6, y+5, color);
        /*drawPixel(x, y+4, color);
        drawPixel(x, y+3, color);
        drawPixel(x, y+2, color);
        drawPixel(x, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color); drawPixel(x+4, y, color);*/
    }

    if (alpha =='n' || alpha =='N')
    {
        drawPixel(x, y+4, color);                               drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);  drawPixel(x+1, y+3, color);  drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+2, y+2, color);   drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color);  drawPixel(x+3, y+1, color);  drawPixel(x+4, y+1, color);
        drawPixel(x, y, color);  drawPixel(x+4, y, color);
    }

    /*if (alpha == 'o' || alpha == 'O')
    {
        drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color);
        drawPixel(x, y+3, color); drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+4, y+1, color);
        drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);
    }*/
    if (alpha == 'O')
    {
        drawPixel(x, y+1, color);drawPixel(x, y+2, color);drawPixel(x, y+3, color);drawPixel(x, y+4, color);drawPixel(x, y+5, color);
        drawPixel(x+1, y+1, color); drawPixel(x+1, y+5, color);drawPixel(x+1, y, color);drawPixel(x+1, y+2, color);drawPixel(x+1, y+3, color);drawPixel(x+1, y+4, color);drawPixel(x+1, y+6, color);
        drawPixel(x+2, y+0, color);drawPixel(x+2, y+1, color);drawPixel(x+2, y+5, color);drawPixel(x+2, y+6, color);
        drawPixel(x+3, y+0, color);drawPixel(x+3, y+1, color);drawPixel(x+3, y+5, color);drawPixel(x+3, y+6, color);
        drawPixel(x+4, y+0, color);drawPixel(x+4, y+1, color);drawPixel(x+4, y+5, color);drawPixel(x+4, y+6, color);
        drawPixel(x+5, y+1, color);drawPixel(x+5, y+5, color);drawPixel(x+5, y, color);drawPixel(x+5, y+2, color);drawPixel(x+5, y+3, color);drawPixel(x+5, y+4, color);drawPixel(x+5, y+6, color);
        drawPixel(x+6, y+1, color);drawPixel(x+6, y+2, color);drawPixel(x+6, y+3, color);drawPixel(x+6, y+4, color);drawPixel(x+6, y+5, color);

    }

    if (alpha == 'p' || alpha == 'P')
    {
        //        drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color);
        //        drawPixel(x, y+3, color);                                                               drawPixel(x+4, y+3, color);
        //        drawPixel(x, y+2, color); drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color);
        //        drawPixel(x, y+1, color);
        //        drawPixel(x, y, color);



        drawPixel(x, y+1, color);
        drawPixel(x, y+2, color);
        drawPixel(x+1, y+3, color);
        drawPixel(x+2, y+2, color);
        drawPixel(x+2, y+1, color);
        drawLine(x,y,x+4,y,color);

        //        //Bigger P
        //
        //           drawPixel(x, y+1, color);
        //                  drawPixel(x, y+2, color);
        //                  drawPixel(x+1, y+3, color);
        //                  drawPixel(x+2, y+3, color);
        //                  drawPixel(x+3, y+2, color);
        //                  drawPixel(x+3, y+1, color);
        //                  drawLine(x,y,x+4,y,color);
        //

    }

    if (alpha == 'r' || alpha == 'R')
    {
        drawPixel(x, y+1, color); drawPixel(x, y+2, color); drawPixel(x, y+3, color);drawPixel(x, y+4, color);
        drawPixel(x+1, y, color);drawPixel(x+1, y+1, color); drawPixel(x+1, y+2, color); drawPixel(x+1, y+3, color);drawPixel(x+1, y+4, color);drawPixel(x+1, y+5, color);                                                               drawPixel(x+4, y+3, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+1, color);drawPixel(x+2, y+4, color);drawPixel(x+2, y+5, color); drawPixel(x+4, y+3, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+1, color);drawPixel(x+3, y+2, color);drawPixel(x+3, y+3, color);drawPixel(x+3, y+4, color);
        drawPixel(x+4, y, color);drawPixel(x+4, y+1, color);drawPixel(x+4, y+2, color);drawPixel(x+4, y+3, color);
        drawPixel(x+5, y, color);drawPixel(x+5, y+1, color);drawPixel(x+5, y+3, color);drawPixel(x+5, y+4, color);
        drawPixel(x+6, y, color);drawPixel(x+6, y+1, color);drawPixel(x+6, y+5, color);drawPixel(x+6, y+4, color);
    }

    if (alpha == 's' || alpha == 'S')
    {
        drawPixel(x, y+1, color);drawPixel(x, y+2, color);drawPixel(x, y+3, color);drawPixel(x, y+4, color);
        drawPixel(x+1, y, color);drawPixel(x+1, y+1, color);drawPixel(x+1, y+2, color);drawPixel(x+1, y+3, color);drawPixel(x+1, y+4, color);
        drawPixel(x+2, y, color);drawPixel(x+2, y+1, color);
        drawPixel(x+3, y, color);drawPixel(x+3, y+2, color);drawPixel(x+3, y+3, color);drawPixel(x+3, y+1, color);
        drawPixel(x+4, y+1, color);drawPixel(x+4, y+2, color);drawPixel(x+4, y+3, color);drawPixel(x+4, y+4, color);
        drawPixel(x+5, y+3, color);drawPixel(x+5, y+4, color);
        drawPixel(x+6, y, color);drawPixel(x+6, y+1, color);drawPixel(x+6, y+2, color);drawPixel(x+6, y+3, color);drawPixel(x+6, y+4, color);
        drawPixel(x+7, y, color);drawPixel(x+7, y+2, color);drawPixel(x+7, y+3, color);drawPixel(x+7, y+1, color);
        /*drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color); drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);
        drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color);
        drawPixel(x+4, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);*/
    }

    if (alpha == 't' || alpha == 'T')
    {
        drawPixel(x, y+4, color); drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color); drawPixel(x+4, y+4, color);
        drawPixel(x+2, y+3, color);
        drawPixel(x+2, y+2, color);
        drawPixel(x+2, y+1, color);
        drawPixel(x+2, y, color);
    }

    if (alpha == 'w' || alpha == 'W')
    {
        drawPixel(x, y+4, color);                                       drawPixel(x+4, y+4, color);
        drawPixel(x, y+3, color);                                       drawPixel(x+4, y+3, color);
        drawPixel(x, y+2, color); drawPixel(x+2, y+2, color);  drawPixel(x+4, y+2, color);
        drawPixel(x, y+1, color); drawPixel(x+1, y+1, color);drawPixel(x+3, y+1, color);drawPixel(x+4, y+1, color);
        drawPixel(x, y, color);  drawPixel(x+4, y, color);
    }

    if (alpha == 'y' || alpha == 'Y')
    {
        //        drawPixel(x, y+4, color);                           drawPixel(x+4, y+4, color);
        //        drawPixel(x+1, y+3, color); drawPixel(x+3, y+3, color);
        //        drawPixel(x+2, y+2, color);
        //        drawPixel(x+2, y+1, color);
        //        drawPixel(x+2, y, color);

        drawPixel(x,y,color);drawPixel(x,y+1,color);drawPixel(x,y+6,color);drawPixel(x,y+7,color);
        drawPixel(x+1,y+1,color);drawPixel(x+1,y+2,color);drawPixel(x+1,y+6,color);drawPixel(x+1,y+5,color);

        drawPixel(x+2,y+2,color);drawPixel(x+2,y+4,color);drawPixel(x+2,y+3,color);drawPixel(x+2,y+5,color);
        drawPixel(x+3,y+3,color);drawPixel(x+3,y+4,color);
        drawPixel(x+4,y+3,color);drawPixel(x+4,y+4,color);
        drawPixel(x+5,y+3,color);drawPixel(x+5,y+4,color);
        drawPixel(x+6,y+3,color);drawPixel(x+6,y+4,color);

    }

    if (alpha == '1')
    {
        drawPixel(x+2, y+4, color);
        drawPixel(x+1, y+3, color); drawPixel(x+2, y+3, color);
        drawPixel(x+2, y+2, color);
        drawPixel(x+2, y+1, color);
        drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);

    }

    if (alpha == '2')
    {
        drawPixel(x, y+4, color);drawPixel(x+1, y+4, color); drawPixel(x+2, y+4, color); drawPixel(x+3, y+4, color);
        drawPixel(x+4, y+3, color);
        drawPixel(x+1, y+2, color); drawPixel(x+2, y+2, color); drawPixel(x+3, y+2, color);
        drawPixel(x, y+1, color);
        drawPixel(x, y, color); drawPixel(x+1, y, color); drawPixel(x+2, y, color); drawPixel(x+3, y, color);  drawPixel(x+4, y, color);
    }
}


void ledMat::drawGameName(uint8_t x, uint8_t y)
{
    uint8_t color1 = red1;
    uint8_t color2 = red2;

    drawLetter(14, 2, 'P', cyan1);
    drawLetter(14, 53, 'P', cyan1);

    drawLetter(x, y, 'G', red1);
    drawLetter(x, y+7, 'O', red1);
    drawLetter(x, y+15, 'A', red1);
    drawLetter(x, y+22, 'L', red1);
    drawLetter(x, y+28, 'S', red1);
    drawLine(x+7, y, x+7, y+33, skyblue1);

    drawLetter(x+10, y+12, 'O', red1);
    drawLetter(x+10, y+20, 'F', red1);
    drawLine(x+17, y+12, x+17, y+24, skyblue1);

    drawLetter(x+20, y, 'G', red1);
    drawLetter(x+20, y+7, 'L', red1);
    drawLetter(x+20, y+13, 'O', red1);
    drawLetter(x+20, y+21, 'R', red1);
    drawLetter(x+20, y+27, 'Y', red1);
    drawLine(x+27, y, x+27, y+33, skyblue1);

    //    drawLetter(x+1, y, 'd', red);     // 10+1, 40
    //    drawLetter(x+8, y, 'e', color1);
    //    drawLetter(x+15, y, 'a', cyan);
    //    drawLetter(x-10, y, 't', cyan2);
    //    drawLetter(x-3, y, 'h', color2);
    //
    //    drawLetter(x-10, y-8, 'r', cyan);
    //    drawLetter(x-3, y-8, 'a', cyan1);
    //    drawLetter(x+4, y-8, 'c', cyan2);
    //    drawLetter(x+11, y-8, 'e', color2);

}

ledMat::ledMat()
{
    ballBlocked = false;

    for (int i = 0; i < 5; i++) {
        p1_score[i] = pending;
        p2_score[i] = pending;
    }
}

void ledMat::updateScore(uint8_t player, uint8_t shoot_num, score_status status)
{
    //shoot_num++;

    switch(player) {
        case 1:
            p1_score[shoot_num] = status;
            break;
        case 2:
            p2_score[shoot_num] = status;
            break;
    }
}

void ledMat::drawScoreGoal(uint8_t p, uint8_t q,uint8_t color_goal)
{
    //    uint8_t color_goal = green2;

    drawLine(p,q+1,p,q+2, color_goal);
    drawLine(p-1,q,p-1,q+3, color_goal);
    drawLine(p-2,q,p-2,q+3, color_goal);
    drawLine(p-3,q+1,p-3,q+2, color_goal);
}

void ledMat::drawScoreNoGoal(uint8_t p, uint8_t q)
{
    uint8_t color_nogoal = red2;

    drawLine(p,q,p-3,q+3,color_nogoal);
    drawLine(p,q+3,p-3,q,color_nogoal);
}

void ledMat::drawScorePending(uint8_t p, uint8_t q)
{
    uint8_t color = cyan2;
    drawLine(p,q+1,p,q+2,cyan1);
    drawPixel(p-1,q,cyan1);
    drawPixel(p-1,q+3,cyan1);
    drawPixel(p-2,q,cyan1);
    drawPixel(p-2,q+3,cyan1);
    drawLine(p-3,q+1,p-3,q+2,cyan1);
}


void ledMat::drawPlayer2Scoreboard(uint8_t x, uint8_t y)
{
    switch (p2_score[0]) {
        case pending:
            drawScorePending(x,y);
            break;
        case goal:
            drawScoreGoal(x,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x,y);
            break;
    }

    switch (p2_score[1]) {
        case pending:
            drawScorePending(x-5,y);
            break;
        case goal:
            drawScoreGoal(x-5,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-5,y);
            break;
    }

    switch (p2_score[2]) {
        case pending:
            drawScorePending(x-10,y);
            break;
        case goal:
            drawScoreGoal(x-10,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-10,y);
            break;
    }

    switch (p2_score[3]) {
        case pending:
            drawScorePending(x-15,y);
            break;
        case goal:
            drawScoreGoal(x-15,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-15,y);
            break;
    }

    switch (p2_score[4]) {
        case pending:
            drawScorePending(x-20,y);
            break;
        case goal:
            drawScoreGoal(x-20,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-20,y);
            break;
    }
}

void ledMat::drawPlayer1Scoreboard(uint8_t x, uint8_t y)
{
    switch (p1_score[0]) {
        case pending:
            drawScorePending(x,y);
            break;
        case goal:
            drawScoreGoal(x,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x,y);
            break;
    }

    switch (p1_score[1]) {
        case pending:
            drawScorePending(x-5,y);
            break;
        case goal:
            drawScoreGoal(x-5,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-5,y);
            break;
    }

    switch (p1_score[2]) {
        case pending:
            drawScorePending(x-10,y);
            break;
        case goal:
            drawScoreGoal(x-10,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-10,y);
            break;
    }

    switch (p1_score[3]) {
        case pending:
            drawScorePending(x-15,y);
            break;
        case goal:
            drawScoreGoal(x-15,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-15,y);
            break;
    }

    switch (p1_score[4]) {
        case pending:
            drawScorePending(x-20,y);
            break;
        case goal:
            drawScoreGoal(x-20,y,green1);
            break;
        case noGoal:
            drawScoreNoGoal(x-20,y);
            break;
    }
}

void ledMat::resetShoot()
{
    goalpost_color = GOALPOST_COLOR;
    clearGoalkeeper(goalkeeper_x, goalkeeper_y, GOALKEEPER_COLOR);
    ballBlocked = false;
}

void ledMat::resetGame()
{
    ballBlocked = false;

    for (int i = 0; i < 5; i++) {
        p1_score[i] = pending;
        p2_score[i] = pending;
    }
}

void ledMat::calculateMatrixPosition(uint8_t *x, uint8_t *color)
{
    if (*x > 31) {
        *x = *x - 32;
        switch(*color) {
            case blue1:
                *color = blue2;
                break;
            case cyan1:
                *color = cyan2;
                break;
            case green1:
                *color = green2;
                break;
            case purple1:
                *color = purple2;
                break;
            case red1:
                *color = red2;
                break;
            case skyblue1:
                *color = skyblue2;
                break;
            case yellow1:
                *color = yellow2;
                break;

        }
    }
}

void ledMat::drawGoalPost(uint8_t dist_left, uint8_t color)
{
    uint8_t dist_right = 63 - dist_left;

    //Left Boundry line
    drawLine(10, 0, 10, dist_left, GOALPOST_COLOR);
    // Right Boundry Line
    drawLine(10, dist_right, 10, 63, GOALPOST_COLOR);

    // Outer left goalpost
    drawLine(0, dist_left, 9, dist_left, color);
    // Inner left goalpost
    drawLine(2, (dist_left + 2), 8, (dist_left + 2), color);

    // Outer Right Goal Post
    drawLine(0, dist_right, 9, dist_right, color);

    // Inner Right Goal Post
    drawLine(2, (dist_right - 2), 8, (dist_right - 2), color);

    // Outer Top Goal Post
    drawLine(0, (dist_left + 1), 0, dist_right, color);

    // Inner Top Goalpost
    drawLine(2, (dist_left + 2), 2, (dist_right - 2), color);

    // Top Left Corner Pixel
    drawPixel(1, (dist_left + 1), color);
    // Top Right Corner Pixel
    drawPixel(1, (dist_right - 1), color);
    // Bottom Left Corner Pixel
    drawPixel(9, (dist_left + 1), color);
    // Bottom Right Corner Pixel
    drawPixel(9, (dist_right - 1), color);

    // Left Field Marking
    drawLine((GOALKEEPER_X_MAX + 1), 2, 11, 2, GOALPOST_COLOR);
    // Middle Field Marking
    drawLine((GOALKEEPER_X_MAX + 1), 2, (GOALKEEPER_X_MAX + 1), 61, GOALPOST_COLOR);
    // Right Field Marking
    drawLine(11, 61, (GOALKEEPER_X_MAX + 1), 61, GOALPOST_COLOR);
}

void ledMat::drawBall(uint8_t ball_x, uint8_t ball_y, uint8_t color)
{
    if ((ball_y - 2) >= goalkeeper_y && (ball_y - 2) <= (goalkeeper_y + 4)) {
        if ((ball_x - 1) >= goalkeeper_x && (ball_x - 1) <= (goalkeeper_x + 10)) {
            ballBlocked = true;
        }

    }
    if ((ball_y) >= goalkeeper_y && (ball_y) <= (goalkeeper_y + 4)) {
        if ((ball_x - 2) >= goalkeeper_x && (ball_x - 2) <= (goalkeeper_x + 10)) {
            ballBlocked = true;
        }

    }
    if ((ball_y + 1) >= goalkeeper_y && (ball_y + 1) <= (goalkeeper_y + 4)) {
        if ((ball_x - 2) >= goalkeeper_x && (ball_x - 2) <= (goalkeeper_x + 10)) {
            ballBlocked = true;
        }

    }
    /*if ((ball_y - 2) >= goalkeeper_y && (ball_y - 2) <= (goalkeeper_y + 4)) {
        if ((ball_x - 1) >= goalkeeper_x && (ball_x - 1) <= (goalkeeper_x + 10)) {
            ballBlocked = true;
        }

    }*/
    if ((ball_y + 2) >= goalkeeper_y && (ball_y + 2) <= (goalkeeper_y + 4)) {
        if ((ball_x - 1) >= goalkeeper_x && (ball_x - 1) <= (goalkeeper_x + 10)) {
            ballBlocked = true;
        }

    }
#if 0
#endif

    drawLine(ball_x-2, ball_y-1, ball_x+2, ball_y-1, color);
    drawLine(ball_x-2, ball_y, ball_x+2, ball_y, color);
    drawLine(ball_x-2, ball_y+1, ball_x+2, ball_y+1, color);
    // Horizontal Lines
    drawLine(ball_x-1, ball_y-2, ball_x-1, ball_y+2, color);
    drawLine(ball_x, ball_y-2, ball_x, ball_y+2, color);
    drawLine(ball_x+1, ball_y-2, ball_x+1, ball_y+2, color);
    /*
    drawPixel(ball_x+1, ball_y+4, color); drawPixel(ball_x+2, ball_y+4, color); drawPixel(ball_x+3, ball_y+4, color);
    drawPixel(ball_x, ball_y+3, color); drawPixel(ball_x+1, ball_y+3, color); drawPixel(ball_x+2, ball_y+3, color); drawPixel(ball_x+3, ball_y+3, color); drawPixel(ball_x+4, ball_y+3, color);
    drawPixel(ball_x, ball_y+2, color); drawPixel(ball_x+1, ball_y+2, color); drawPixel(ball_x+2, ball_y+2, color); drawPixel(ball_x+3, ball_y+2, color); drawPixel(ball_x+4, ball_y+2, color);
    drawPixel(ball_x, ball_y+1, color); drawPixel(ball_x+1, ball_y+1, color); drawPixel(ball_x+2, ball_y+1, color); drawPixel(ball_x+3, ball_y+1, color); drawPixel(ball_x+4, ball_y+1, color);
    drawPixel(ball_x+1, ball_y, color); drawPixel(ball_x+2, ball_y, color); drawPixel(ball_x+3, ball_y, color);
     */
}

void ledMat::clearBall(uint8_t ball_x, uint8_t ball_y, uint8_t color)
{
    // Vertical Lines
    clearLine(ball_x-2, ball_y-1, ball_x+2, ball_y-1, color);
    clearLine(ball_x-2, ball_y, ball_x+2, ball_y, color);
    clearLine(ball_x-2, ball_y+1, ball_x+2, ball_y+1, color);
    // Horizontal Lines
    clearLine(ball_x-1, ball_y-2, ball_x-1, ball_y+2, color);
    clearLine(ball_x, ball_y-2, ball_x, ball_y+2, color);
    clearLine(ball_x+1, ball_y-2, ball_x+1, ball_y+2, color);
    /*
    clearPixel(ball_x+1, ball_y+4, color); clearPixel(ball_x+2, ball_y+4, color); clearPixel(ball_x+3, ball_y+4, color);
    clearPixel(ball_x, ball_y+3, color); clearPixel(ball_x+1, ball_y+3, color); clearPixel(ball_x+2, ball_y+3, color); clearPixel(ball_x+3, ball_y+3, color); clearPixel(ball_x+4, ball_y+3, color);
    clearPixel(ball_x, ball_y+2, color); clearPixel(ball_x+1, ball_y+2, color); clearPixel(ball_x+2, ball_y+2, color); clearPixel(ball_x+3, ball_y+2, color); clearPixel(ball_x+4, ball_y+2, color);
    clearPixel(ball_x, ball_y+1, color); clearPixel(ball_x+1, ball_y+1, color); clearPixel(ball_x+2, ball_y+1, color); clearPixel(ball_x+3, ball_y+1, color); clearPixel(ball_x+4, ball_y+1, color);
    clearPixel(ball_x+1, ball_y, color); clearPixel(ball_x+2, ball_y, color); clearPixel(ball_x+3, ball_y, color);
     */
}

void ledMat::drawGoalkeeper(uint8_t new_x, uint8_t new_y, uint8_t color)
{
    if (ballBlocked) {
        drawGoalkeeperWithBall(new_x, new_y, color);
        return;
    }

    // Clear Previous Goalkeeper Image
    clearGoalkeeper(goalkeeper_x, goalkeeper_y, color);
    // Check Goalkeeper horizontal movement range
    if (new_y > GOALKEEPER_Y_MIN && (new_y + 4) < GOALKEEPER_Y_MAX) {
        goalkeeper_y = new_y;
    }
    // Check Goalkeeper vertical movement range
    if (new_x > GOALKEEPER_X_MIN && (new_x + 10) < GOALKEEPER_X_MAX) {
        goalkeeper_x = new_x;
    }

    // Draw Goalkeeper Head Top
    drawPixel(goalkeeper_x, goalkeeper_y+1, color);
    drawPixel(goalkeeper_x, goalkeeper_y+2, color);
    drawPixel(goalkeeper_x, goalkeeper_y+3, color);
    // Draw Goalkeeper Head Left
    drawPixel(goalkeeper_x+1, goalkeeper_y, color);
    drawPixel(goalkeeper_x+2, goalkeeper_y, color);
    drawPixel(goalkeeper_x+3, goalkeeper_y+1, color);
    // Draw Goalkeeper Head Right
    drawPixel(goalkeeper_x+1, goalkeeper_y+4, color);
    drawPixel(goalkeeper_x+2, goalkeeper_y+4, color);
    drawPixel(goalkeeper_x+3, goalkeeper_y+3, color);
    // Draw Goalkeeper Neck
    drawPixel(goalkeeper_x+4, goalkeeper_y+2, color);

    // Draw Goalkeeper Body
    drawLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);

    // Draw Goalkeeper Left Hand
    drawPixel(goalkeeper_x+6, goalkeeper_y+3, color); drawPixel(goalkeeper_x+5, goalkeeper_y+4, color);
    //drawPixel(goalkeeper_x+4, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Right Hand
    drawPixel(goalkeeper_x+6, goalkeeper_y+1, color); drawPixel(goalkeeper_x+5, goalkeeper_y, color);
    //drawPixel(goalkeeper_x+4, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Left Leg
    drawPixel(goalkeeper_x+9, goalkeeper_y+3, color); drawPixel(goalkeeper_x+10, goalkeeper_y+4, color);
    //drawPixel(goalkeeper_x+12, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Right Leg
    drawPixel(goalkeeper_x+9, goalkeeper_y+1, color); drawPixel(goalkeeper_x+10, goalkeeper_y, color);
    //drawPixel(goalkeeper_x+12, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);
}
void ledMat::drawGoalKeeperDemoPose1(uint8_t goalkeeper_x, uint8_t goalkeeper_y,uint8_t color){
    //    // Draw Goalkeeper Head Top
        drawPixel(goalkeeper_x , goalkeeper_y+1, color);
        drawPixel(goalkeeper_x, goalkeeper_y+2, color);
        drawPixel(goalkeeper_x, goalkeeper_y+3, color);
        // Draw Goalkeeper Head Left
        drawPixel(goalkeeper_x+1, goalkeeper_y, color);
        drawPixel(goalkeeper_x+2, goalkeeper_y, color);
        drawPixel(goalkeeper_x+3, goalkeeper_y+1, color);
        // Draw Goalkeeper Head Right
        drawPixel(goalkeeper_x+1, goalkeeper_y+4, color);
        drawPixel(goalkeeper_x+2, goalkeeper_y+4, color);
        drawPixel(goalkeeper_x+3, goalkeeper_y+3, color);
        // Draw Goalkeeper Neck
        drawPixel(goalkeeper_x+4, goalkeeper_y+2, color);

        // Draw Goalkeeper Body
        drawLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);

        // Draw Goalkeeper Left Hand
        drawPixel(goalkeeper_x+6, goalkeeper_y+3, color); drawPixel(goalkeeper_x+5, goalkeeper_y+4, color);
        //drawPixel(goalkeeper_x+4, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

        // Draw Goalkeeper Right Hand
        drawPixel(goalkeeper_x+6, goalkeeper_y+1, color); drawPixel(goalkeeper_x+5, goalkeeper_y, color);
        //drawPixel(goalkeeper_x+4, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

        // Draw Goalkeeper Left Leg
        drawPixel(goalkeeper_x+9, goalkeeper_y+3, color); drawPixel(goalkeeper_x+10, goalkeeper_y+4, color);
        //drawPixel(goalkeeper_x+12, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

        // Draw Goalkeeper Right Leg
        drawPixel(goalkeeper_x+9, goalkeeper_y+1, color); drawPixel(goalkeeper_x+10, goalkeeper_y, color);
        //drawPixel(goalkeeper_x+12, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

}

void ledMat::clearGoalKeeperDemoPose1(uint8_t goalkeeper_x, uint8_t goalkeeper_y,uint8_t color){
    //    // Clear Goalkeeper Head Top
        clearPixel(goalkeeper_x, goalkeeper_y+1, color);
        clearPixel(goalkeeper_x, goalkeeper_y+2, color);
        clearPixel(goalkeeper_x, goalkeeper_y+3, color);
        // Clear Goalkeeper Head Left
        clearPixel(goalkeeper_x+1, goalkeeper_y, color);
        clearPixel(goalkeeper_x+2, goalkeeper_y, color);
        clearPixel(goalkeeper_x+3, goalkeeper_y+1, color);
        // clear Goalkeeper Head Right
        clearPixel(goalkeeper_x+1, goalkeeper_y+4, color);
        clearPixel(goalkeeper_x+2, goalkeeper_y+4, color);
        clearPixel(goalkeeper_x+3, goalkeeper_y+3, color);
        // clear Goalkeeper Neck
        clearPixel(goalkeeper_x+4, goalkeeper_y+2, color);

        // clear Goalkeeper Body
        clearLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);

        // clear Goalkeeper Left Hand
        clearPixel(goalkeeper_x+6, goalkeeper_y+3, color); clearPixel(goalkeeper_x+5, goalkeeper_y+4, color);
        //clearPixel(goalkeeper_x+4, goalkeeper_y+5, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

        // clear Goalkeeper Right Hand
        clearPixel(goalkeeper_x+6, goalkeeper_y+1, color); clearPixel(goalkeeper_x+5, goalkeeper_y, color);
        //clearPixel(goalkeeper_x+4, goalkeeper_y-1, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

        // clear Goalkeeper Left Leg
        clearPixel(goalkeeper_x+9, goalkeeper_y+3, color); clearPixel(goalkeeper_x+10, goalkeeper_y+4, color);
        //clearPixel(goalkeeper_x+12, goalkeeper_y+5, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

        // clear Goalkeeper Right Leg
        clearPixel(goalkeeper_x+9, goalkeeper_y+1, color); clearPixel(goalkeeper_x+10, goalkeeper_y, color);
        //clearPixel(goalkeeper_x+12, goalkeeper_y-1, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

}

void ledMat::drawGoalKeeperDemoPose2(uint8_t goalkeeper_x, uint8_t goalkeeper_y,uint8_t color)
{
//     Draw Goalkeeper Head Top
       drawPixel(goalkeeper_x+1, goalkeeper_y+1, color);
       drawPixel(goalkeeper_x+1, goalkeeper_y+2, color);
       drawPixel(goalkeeper_x+1, goalkeeper_y+3, color);
       // Draw Goalkeeper Head Left
       drawPixel(goalkeeper_x+2, goalkeeper_y, color);
       drawPixel(goalkeeper_x+3, goalkeeper_y, color);
       drawPixel(goalkeeper_x+4, goalkeeper_y+1, color);
       // Draw Goalkeeper Head Right
       drawPixel(goalkeeper_x+2, goalkeeper_y+4, color);
       drawPixel(goalkeeper_x+3, goalkeeper_y+4, color);
       drawPixel(goalkeeper_x+4, goalkeeper_y+3, color);
       // Draw Goalkeeper Neck
       drawPixel(goalkeeper_x+5, goalkeeper_y+2, color);

       // Draw Goalkeeper Body
       drawLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);

       // Draw Goalkeeper Left Hand
       drawPixel(goalkeeper_x+6, goalkeeper_y+3, color); drawPixel(goalkeeper_x+7, goalkeeper_y+4, color);
       //drawPixel(goalkeeper_x+4, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

       // Draw Goalkeeper Right Hand
       drawPixel(goalkeeper_x+6, goalkeeper_y+1, color); drawPixel(goalkeeper_x+7, goalkeeper_y, color);
       //drawPixel(goalkeeper_x+4, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

       // Draw Goalkeeper Left Leg
       drawPixel(goalkeeper_x+9, goalkeeper_y+3, color); drawPixel(goalkeeper_x+10, goalkeeper_y+3, color);
       //drawPixel(goalkeeper_x+12, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

       // Draw Goalkeeper Right Leg
       drawPixel(goalkeeper_x+9, goalkeeper_y+1, color); drawPixel(goalkeeper_x+10, goalkeeper_y+1, color);
       //drawPixel(goalkeeper_x+12, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);
}

void ledMat::clearGoalKeeperDemoPose2(uint8_t goalkeeper_x, uint8_t goalkeeper_y,uint8_t color)
{
//     clear Goalkeeper Head Top
       clearPixel(goalkeeper_x+1, goalkeeper_y+1, color);
       clearPixel(goalkeeper_x+1, goalkeeper_y+2, color);
       clearPixel(goalkeeper_x+1, goalkeeper_y+3, color);
       // clear Goalkeeper Head Left
       clearPixel(goalkeeper_x+2, goalkeeper_y, color);
       clearPixel(goalkeeper_x+3, goalkeeper_y, color);
       clearPixel(goalkeeper_x+4, goalkeeper_y+1, color);
       // clear Goalkeeper Head Right
       clearPixel(goalkeeper_x+2, goalkeeper_y+4, color);
       clearPixel(goalkeeper_x+3, goalkeeper_y+4, color);
       clearPixel(goalkeeper_x+4, goalkeeper_y+3, color);
       // clear Goalkeeper Neck
       clearPixel(goalkeeper_x+5, goalkeeper_y+2, color);

       // clear Goalkeeper Body
       clearLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);

       // clear Goalkeeper Left Hand
       clearPixel(goalkeeper_x+6, goalkeeper_y+3, color); clearPixel(goalkeeper_x+7, goalkeeper_y+4, color);
       //clearPixel(goalkeeper_x+4, goalkeeper_y+5, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

       // clear Goalkeeper Right Hand
       clearPixel(goalkeeper_x+6, goalkeeper_y+1, color); clearPixel(goalkeeper_x+7, goalkeeper_y, color);
       //clearPixel(goalkeeper_x+4, goalkeeper_y-1, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

       // clear Goalkeeper Left Leg
       clearPixel(goalkeeper_x+9, goalkeeper_y+3, color); clearPixel(goalkeeper_x+10, goalkeeper_y+3, color);
       //clearPixel(goalkeeper_x+12, goalkeeper_y+5, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);

       // clear Goalkeeper Right Leg
       clearPixel(goalkeeper_x+9, goalkeeper_y+1, color); clearPixel(goalkeeper_x+10, goalkeeper_y+1, color);
       //clearPixel(goalkeeper_x+12, goalkeeper_y-1, color); // clearPixel(goalkeeper_x+3, goalkeeper_y, color);
}

void ledMat::drawGoalkeeperWithBall(uint8_t new_x, uint8_t new_y, uint8_t color)
{
    // Clear Previous Goalkeeper Image
    clearGoalkeeperWithBall(goalkeeper_x, goalkeeper_y, color);

    // Check Goalkeeper horizontal movement range
    if (new_y > GOALKEEPER_Y_MIN && (new_y + 4) < GOALKEEPER_Y_MAX) {
        goalkeeper_y = new_y;
    }
    // Check Goalkeeper vertical movement range
    if (new_x > GOALKEEPER_X_MIN && (new_x + 10) < GOALKEEPER_X_MAX) {
        goalkeeper_x = new_x;
    }

    // Draw Goalkeeper Head Top
    drawPixel(goalkeeper_x, goalkeeper_y+1, color);
    drawPixel(goalkeeper_x, goalkeeper_y+2, color);
    drawPixel(goalkeeper_x, goalkeeper_y+3, color);
    // Draw Goalkeeper Head Left
    drawPixel(goalkeeper_x+1, goalkeeper_y, color);
    drawPixel(goalkeeper_x+2, goalkeeper_y, color);
    //drawPixel(goalkeeper_x+3, goalkeeper_y+1, color);
    // Draw Goalkeeper Head Right
    drawPixel(goalkeeper_x+1, goalkeeper_y+4, color);
    drawPixel(goalkeeper_x+2, goalkeeper_y+4, color);
    //drawPixel(goalkeeper_x+3, goalkeeper_y+3, color);
    // Draw Goalkeeper Neck
    //drawPixel(goalkeeper_x+4, goalkeeper_y+2, color);

    // Draw Goalkeeper Body
    //drawLine(goalkeeper_x+5, goalkeeper_y+2, goalkeeper_x+8, goalkeeper_y+2, color);
    drawPixel(goalkeeper_x+8, goalkeeper_y+2, color);

    // Draw Ball in hands of Goalkeeper
    drawBall(goalkeeper_x+5, goalkeeper_y+2, black);
    drawBall(goalkeeper_x+5, goalkeeper_y+2, BALL_COLOR);
    // Draw Extra Left Hand Pixel
    drawPixel(goalkeeper_x+4, goalkeeper_y+5, color);
    // Draw Extra Right Hand Pixel
    drawPixel(goalkeeper_x+4, goalkeeper_y-1, color);

    // Draw Goalkeeper Left Hand
    //drawPixel(goalkeeper_x+6, goalkeeper_y+3, color); drawPixel(goalkeeper_x+5, goalkeeper_y+4, color);
    //drawPixel(goalkeeper_x+4, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Right Hand
    //drawPixel(goalkeeper_x+6, goalkeeper_y+1, color); drawPixel(goalkeeper_x+5, goalkeeper_y, color);
    //drawPixel(goalkeeper_x+4, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Left Leg
    drawPixel(goalkeeper_x+9, goalkeeper_y+3, color); drawPixel(goalkeeper_x+10, goalkeeper_y+4, color);
    //drawPixel(goalkeeper_x+12, goalkeeper_y+5, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);

    // Draw Goalkeeper Right Leg
    drawPixel(goalkeeper_x+9, goalkeeper_y+1, color); drawPixel(goalkeeper_x+10, goalkeeper_y, color);
    //drawPixel(goalkeeper_x+12, goalkeeper_y-1, color); // drawPixel(goalkeeper_x+3, goalkeeper_y, color);
}

void ledMat::clearGoalkeeper(uint8_t new_x, uint8_t new_y, uint8_t color)
{
    static uint8_t x = 3, y = 30;

    if (ballBlocked) {
        clearGoalkeeperWithBall(new_x, new_y, color);
        return;
    }

    // Check Goalkeeper horizontal movement range
    if (new_y > GOALKEEPER_Y_MIN && (new_y + 4) < GOALKEEPER_Y_MAX) {
        y = new_y;
    }
    // Check Goalkeeper vertical movement range
    if (new_x > GOALKEEPER_X_MIN && (new_x + 10) < GOALKEEPER_X_MAX) {
        x = new_x;
    }

    // Clear Goalkeeper Head Top
    clearPixel(x, y+1, color);
    clearPixel(x, y+2, color);
    clearPixel(x, y+3, color);
    // Clear Goalkeeper Head Left
    clearPixel(x+1, y, color);
    clearPixel(x+2, y, color);
    clearPixel(x+3, y+1, color);
    // Clear Goalkeeper Head Right
    clearPixel(x+1, y+4, color);
    clearPixel(x+2, y+4, color);
    clearPixel(x+3, y+3, color);
    // Clear Goalkeeper Neck
    clearPixel(x+4, y+2, color);

    // Clear Goalkeeper Body
    clearLine(x+5, y+2, x+8, y+2, color);

    // Clear Goalkeeper Left Hand
    clearPixel(x+6, y+3, color); clearPixel(x+5, y+4, color);
    //clearPixel(x+4, y+5, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Right Hand
    clearPixel(x+6, y+1, color); clearPixel(x+5, y, color);
    //clearPixel(x+4, y-1, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Left Leg
    clearPixel(x+9, y+3, color); clearPixel(x+10, y+4, color);
    //clearPixel(x+12, y+5, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Right Leg
    clearPixel(x+9, y+1, color); clearPixel(x+10, y, color);
    //clearPixel(x+12, y-1, color); // clearPixel(x+3, y, color);
}

void ledMat::clearGoalkeeperWithBall(uint8_t new_x, uint8_t new_y, uint8_t color)
{
    static uint8_t x = 3, y = 30;

    // Check Goalkeeper horizontal movement range
    if (new_y > GOALKEEPER_Y_MIN && (new_y + 4) < GOALKEEPER_Y_MAX) {
        y = new_y;
    }
    // Check Goalkeeper vertical movement range
    if (new_x > GOALKEEPER_X_MIN && (new_x + 10) < GOALKEEPER_X_MAX) {
        x = new_x;
    }

    // Clear Goalkeeper Head Top
    clearPixel(x, y+1, color);
    clearPixel(x, y+2, color);
    clearPixel(x, y+3, color);
    // Clear Goalkeeper Head Left
    clearPixel(x+1, y, color);
    clearPixel(x+2, y, color);
    clearPixel(x+3, y+1, color);
    // Clear Goalkeeper Head Right
    clearPixel(x+1, y+4, color);
    clearPixel(x+2, y+4, color);
    clearPixel(x+3, y+3, color);
    // Clear Goalkeeper Neck
    clearPixel(x+4, y+2, color);

    // Clear Goalkeeper Body
    clearLine(x+5, y+2, x+8, y+2, color);

    // Clear Ball in hands of Goalkeeper
    clearBall(x+5, y+2, BALL_COLOR);
    // Draw Extra Left Hand Pixel
    clearPixel(x+4, y+5, color);
    // Draw Extra Right Hand Pixel
    clearPixel(x+4, y-1, color);

    // Clear Goalkeeper Left Hand
    clearPixel(x+6, y+3, color); clearPixel(x+5, y+4, color);
    //clearPixel(x+4, y+5, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Right Hand
    clearPixel(x+6, y+1, color); clearPixel(x+5, y, color);
    //clearPixel(x+4, y-1, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Left Leg
    clearPixel(x+9, y+3, color); clearPixel(x+10, y+4, color);
    //clearPixel(x+12, y+5, color); // clearPixel(x+3, y, color);

    // Clear Goalkeeper Right Leg
    clearPixel(x+9, y+1, color); clearPixel(x+10, y, color);
    //clearPixel(x+12, y-1, color); // clearPixel(x+3, y, color);
}

/*void ledMat::drawCircle(uint8_t x, uint8_t y, uint8_t color)
{
    int i, j;

    drawPixel(x+1, y, color);  drawPixel(x+1, y+1, color); drawPixel(x+1, y+2, color);
    drawPixel(x, y+1, color);
    drawPixel(x+2, y+1, color);

}*/


void ledMat::drawStartScreen(uint8_t x, uint8_t y)
{
    uint8_t color1 = cyan1;
    uint8_t color2 = cyan2;
    uint8_t color3= skyblue1;
    //draw player1&2
    drawDigit(14,8,1,color1);
    drawDigit(14,58,2,color1);

    // Draw (inner) Field outline
    drawLine(x,y,22,14,color3);
    drawLine(x,y+63,22,49,color3);
    drawLine(x-4,y,20,13,color3);
    // Draw (outer) Field outline
    drawLine(x-4,y+63,20, 50, color3);
    drawLine(22,14,22, 49,color3);
    drawLine(20,14,20, 49,color3);
    //select color cubes
    uint8_t x1=7;
    uint8_t y1=12;
    //    drawDigit(0,0,1,blue1);
    //    drawDigit(7,0,2,blue1);
    //    drawDigit(15,0,3,blue1);

        drawScoreGoal(x1,y1,blue1);
        drawScoreGoal(x1,y1+7,green1);
        drawScoreGoal(x1,y1+14,red1);
        drawScoreGoal(x1,y1+21,yellow1);
        drawScoreGoal(x1,y1+28,skyblue1);
        drawScoreGoal(x1,y1+35,purple1);



    //    drawScorePending(x1,y1+7);
    //    drawLine(x1+1,y1-1,x1-4,y1-1,cyan1);
    //    drawLine(x1-4,y1-1,x1-4,y1+4,cyan1);
    //    drawLine(x1+1,y1+4,x1-4,y1+4,cyan1);
    //    drawLine(x1+1,y1-1,x1+1,y1+4,cyan1);
    //    // Draw Left Roof
    //    drawLine(15, 0, 0, 10,color1);
    //    // Draw Right Roof
    //    drawLine(0, 53, 15, 63,color1);
    //    for (int i =0;i<20;i++ )
    //    {
    //        uint8_t color1=red1;
    //        uint8_t color2=green1;
    //        uint8_t color3=blue1;

    //        switch(i%5) {
    //            case 0:
    //                drawLine(i,0,i,63,color1);
    //                break;
    //            case 1:
    //                drawLine(i,0,i,63,black);
    //                break;
    //            case 2:
    //                drawLine(i,0,i,63,color2);
    //                break;
    //            case 3:
    //                drawLine(i,0,i,63,color3);
    //                break;
    //            case 4:
    //                drawLine(i,0,i,63,black);
    //                break;
    //        }
    // }

    //    for (int i =0;i<63;i++ )
    //    {
    //        uint8_t color1=red1;
    //        uint8_t color2=green1;
    //        uint8_t color3=blue1;

    //        switch(i%5) {
    //            case 0:
    //                drawLine(0,i,20,i,color1);
    //                break;
    //            case 1:
    //               // drawLine(0,i,20,i,black);
    //                break;
    //            case 2:
    //                drawLine(0,i,20,i,color2);
    //                drawLine(0,1,20,i,color3);
    //                break;
    //             case 3:
    //                 drawLine(0,i,20,i,color3);
    //                 drawLine(0,1,20,i,color3);
    //                 break;
    //            case 4:
    //                // drawLine(0,i,20,i,black);
    //                 break;

    //        }
    //    }

    drawGameName(32, 14);
    //    drawLetter(x, y, 'p', color1);
    //    drawLetter(x+6, y, 'r', color1);
    //    drawLetter(x+12, y, 'e', color1);
    //    drawLetter(x+18, y, 's', color1);
    //    drawLetter(x+24, y, 's', color1);
    //
    //    drawLetter(x, y, 's', color2);
    //    drawLetter(x+6, y, 'w', color2);
    //    //drawPixel(x+13,y+2, color2);
    //    //drawPixel(x+14,y+2, color2);
    //    //drawPixel(x+15,y+2, color2);
    //    //drawLetter(x+16, y, '1', color2);
    //    drawLetter(x+10, y, 'i', color2);
    //    drawLetter(x+14, y, 't', color2);
    //    drawLetter(x+19, y, 'c', color2);
    //    drawLetter(x+24, y, 'h', color2);
    //
    //    drawLetter(x+16, y-8, 't', color1);
    //    drawLetter(x+22, y-8, 'o', color1);
    //
    //    drawLetter(x-1, y-8, 's', color2);
    //    drawLetter(x+5, y-8, 't', color2);
    //    drawLetter(x+11, y-8, 'a', color2);
    //    drawLetter(x+17, y-8, 'r', color2);
    //    drawLetter(x+23, y-8, 't', color2);

}

void ledMat::drawPlayerReady(uint8_t x, uint8_t y, uint8_t color)
{
    drawLetter(x, y, 'r', color);
    drawLetter(x+6, y, 'e', color);
    drawLetter(x+12, y, 'a', color);
    drawLetter(x+18, y, 'd', color);
    drawLetter(x+24, y, 'y', color);
}

void ledMat::drawCountDown(uint8_t x, uint8_t y)
{
    uint8_t color1 = skyblue1;
    uint8_t color2 = skyblue2;

    drawDigit(x, y, 3, color1);
    drawPixel(x+6, y, color1);
    drawPixel(x+8, y, color1);
    drawPixel(x+10, y, color1);
    delay_ms(1000);


    drawDigit(x+16, y, 2, color1);
    drawPixel(x+22, y, color1);
    drawPixel(x+24, y, color1);
    drawPixel(x+26, y, color1);
    delay_ms(1000);


    drawDigit(x, y, 1, color2);
    drawPixel(x+6, y, color2);
    drawPixel(x+8, y, color2);
    drawPixel(x+10, y, color2);
    delay_ms(1000);

    drawDigit(x+16, y, 0, color2);
    drawPixel(x+22, y, color2);
    drawPixel(x+24, y, color2);
    drawPixel(x+26, y, color2);
    delay_ms(1000);
}


void ledMat::drawStartWord(uint8_t x, uint8_t y)
{
    uint8_t color1 = skyblue1;
    uint8_t color2 = skyblue2;

    drawLetter(x+1, y, 's', color1);
    drawLetter(x+8, y, 't', color1);
    drawLetter(x+15, y, 'a', color1);
    drawLetter(x-10, y, 'r', color2);
    drawLetter(x-3, y, 't', color2);
    drawPixel(x+4, y, color2);
    drawPixel(x+7, y, color2);
    drawPixel(x+10, y, color2);
}


void ledMat::drawWinner(uint8_t x, uint8_t y, char player_number)
{
    uint8_t color1;
    uint8_t color2;

    if (player_number == '1')
    {
        color1 = red1;
        color2 = red2;
    }
    else
    {
        color1 = blue1;
        color2 = blue2;
    }


    drawLetter(x, y, 'p', color1);
    drawLetter(x+6, y, 'l', color1);
    drawLetter(x+12, y, 'a', color1);
    drawLetter(x+18, y, 'y', color1);
    drawLetter(x-x, y, 'e', color2);
    drawLetter(x-x+6, y, 'r', color2);

    drawPixel(x-x+13, y+2, color2);
    drawPixel(x-x+14, y+2, color2);
    drawPixel(x-x+15, y+2, color2);

    drawLetter(x-x+17, y, player_number, color2);

    drawLetter(x+12, y-8, 'w', color1);
    drawLetter(x+18, y-8, 'i', color1);
    drawLetter(x-x, y-8, 'n', color2);
    drawLetter(x-x+6, y-8, 's', color2);
    drawLine(x-x+13, y-6, x-x+13, y-4, color2);
    drawPixel(x-x+13, y-8, color2);
}


void ledMat::drawDraw(uint8_t x, uint8_t y)
{
    uint8_t color1 = green1;
    uint8_t color2 = green2;

    drawLetter(x, y, 'd', color1);
    drawLetter(x+6, y, 'r', color1);
    drawLetter(x-x, y, 'a', color2);
    drawLetter(x-x+6, y, 'w', color2);
    drawLine(x-x+13, y+2, x-x+13, y+4, color2);
    drawPixel(x-x+13, y, color2);
}


void ledMat::drawTime(uint8_t x0, uint8_t y0, uint8_t value, uint8_t color1, uint8_t color2)
{
    uint8_t num[2] = {};
    uint8_t color;
    num[0] = value / 10;
    num[1] = value % 10;


    for(uint8_t i = 0; i < 2; i++)
    {
        x0 = (i == 1)? (x0 - 24) : x0;

        if (i==0)
            color = color1;
        if (i==1)
            color = color2;

        drawDigit(x0, y0, num[i], color);
    }
}


/*
 ** Function to clear time at specific location.
 */
void ledMat::clearTime(uint8_t x0, uint8_t y0, uint8_t color1, uint8_t color2)
{
    for(uint8_t i = 0; i < 5; i++)
    {
        for(uint8_t j = 0; j < 8; j++)
        {
            clearPixel(i+x0, j+y0, color1);
            clearPixel(i+(x0-24), j+y0, color2);
        }
    }
}


/*
 ** Function to clear numbers at specific location.
 **
 */
void ledMat::drawArrow(uint8_t x,uint8_t y,uint8_t color )
{
    //    drawPixel(x,y+2,color);
    //    drawPixel(x+1,y+1,color);drawPixel(x+1,y+3,color);
    //    drawPixel(x+2,y+1,color);drawPixel(x+2,y+2,color);drawPixel(x+2,y+3,color);
    //    drawPixel(x+2,y,color);drawPixel(x+2,y+4,color);
    //    drawPixel(x+3,y+2,color);drawPixel(x+1,y+2,color);
    //    drawPixel(x+4,y+2,color);drawPixel(x+5,y+2,color);
    //    drawPixel(x+73,y+2,color);drawPixel(x+6,y+2,color);
    drawLine(x,y+1,x+5,y+1,color);
    drawLine(x,y+2,x+5,y+2,color);
    drawPixel(x+1,y,color);
    drawPixel(x+1,y+3,color);
    drawPixel(x+2,y-1,color);
    drawPixel(x+2,y+4,color);
    //drawPixel(x,y,color);drawPixel(x+1,y+3,color);
    //drawPixel(x-1,y-1,color);drawPixel(x-1,y+4,color);

}

void ledMat::clearArrow(uint8_t x,uint8_t y,uint8_t color )
{
    //    clearPixel(x,y+2,color);
    //    clearPixel(x+1,y+1,color);clearPixel(x+1,y+3,color);
    //    clearPixel(x+2,y+1,color);clearPixel(x+2,y+2,color);clearPixel(x+2,y+3,color);
    //    clearPixel(x+2,y,color);clearPixel(x+2,y+4,color);
    //    clearPixel(x+3,y+2,color);clearPixel(x+1,y+2,color);
    //    clearPixel(x+4,y+2,color);clearPixel(x+5,y+2,color);
    //    clearPixel(x+73,y+2,color);clearPixel(x+6,y+2,color);
    clearLine(x,y+1,x+5,y+1,color);
    clearLine(x,y+2,x+5,y+2,color);
    clearPixel(x+1,y,color);
    clearPixel(x+1,y+3,color);
    clearPixel(x+2,y-1,color);
    clearPixel(x+2,y+4,color);
    //clearPixel(x,y,color);clearPixel(x+1,y+3,color);
    //clearPixel(x-1,y-1,color);clearPixel(x-1,y+4,color);

}

void ledMat::clearNumbers(uint8_t x0, uint8_t y0, uint8_t color)
{
    for(uint8_t i = 0; i < 5; i++)
    {
        for(uint8_t j = 0; j < 8; j++)
        {
            clearPixel(i+x0, j+y0, color);
            clearPixel(i+x0+7, j+y0, color);
        }
    }
}


