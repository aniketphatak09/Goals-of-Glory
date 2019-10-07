/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 *          FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 *          @see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "FreeRTOS.h"
#include "LPC17xx.h"
#include "tasks.hpp"
#include "examples/examples.hpp"
#include <stdint.h>
#include "io.hpp"
#include "gpio.hpp"
#include "utilities.h"
#include "stdio.h"
#include "stdbool.h"
#include "printf_lib.h"
#include "led_mat.hpp"
#include "lpc_rit.h"
#include "nrf_stream.hpp"

ledMat mat;
int Y_HIT = 0;

char ballPath[4][57][2];

#define Player1_Tx_Address 250
#define Player2_Tx_Address 210


/*
 ** Task to update LED Matrix.
 */
void UpdateDisplay(void *p)
{
    while(1) {
        mat.drawPlayer2Scoreboard(62,59);
        mat.drawPlayer1Scoreboard(62,1);
        //mat.drawStartScreen(48,0);
        mat.updateDisplay();
        vTaskDelay(2);
    }
}

/*
 ** Update LED Matrix every 1ms by calling an interrupt.
 ** Currently, not using this. Using run task.
 */
void interruptToDisplay(void)
{
    mat.updateDisplay();
}

void animateBall()
{
    int x, y;
    int swing_var = 0;
    int y_var = 32 - Y_HIT;
    int direction_change_interval = X_START / abs(y_var);
    int swing_change_interval = 0;
    y = 32;

    swing_change_interval = direction_change_interval / 2;

    while (mat.ballBlocked) {

    }
    for(x = X_START; x >= 17; x--)
    {
        if (!mat.ballBlocked) {
            if ((x % direction_change_interval) == 0) {
                if (y_var < 0) {
                    y--;
                } else if (y_var > 0) {
                    y++;
                }
            }
            if ((x % swing_change_interval) == 0) {
                swing_change_interval++;
                if (y_var < 0) {
                    swing_var += 1;
                } else if (y_var >= 0) {
                    swing_var -= 1;
                }
            }

            //mat.drawBall(i, 30 - swing_var, BALL_COLOR);
            mat.drawBall(x, y + swing_var, BALL_COLOR);

            //           mat.drawGoalkeeper(3, 30, GOALKEEPER_COLOR);
            mat.drawGoalPost(GOALPOST, mat.goalpost_color);
            vTaskDelay(DELAY);
            //            mat.clearDisplay(blue);
            //            mat.clearDisplay(red);
            //            mat.clearDisplay(green);
            mat.clearBall(x + 1, y + swing_var , BALL_COLOR);

            //vTaskDelay(1);
        }
    }

    for(x = 16; x >= 2; x--) {
        if (!mat.ballBlocked) {
            if ((x % direction_change_interval) == 0) {
                if (y_var < 0) {
                    y--;
                } else if (y_var >= 0) {
                    y++;
                }
            }
            if ((x % swing_change_interval) == 0) {


                if (y_var < 0) {
                    swing_var -= 1;
                } else if (y_var > 0) {
                    swing_var += 1;
                }
            }

            //mat.drawBall(i, 30 -swing_var, BALL_COLOR);
            mat.drawBall(x, y + swing_var, BALL_COLOR);
            //            mat.drawGoalkeeper(3, 30, GOALKEEPER_COLOR);
            mat.drawGoalPost(GOALPOST, mat.goalpost_color);
            vTaskDelay(DELAY);
            //            mat.clearDisplay(blue);
            //            mat.clearDisplay(red);
            //            mat.clearDisplay(green);
            mat.clearBall(x, y + swing_var, BALL_COLOR);

            //vTaskDelay(1);
        }
        //            else {
        //                mat.clearBall(X_START, y, BALL_COLOR);
        //            }
    }

}

void ControlGoalkeeper(void *p)
{
    //int x = 3, y = 30;
    //int x = GOALKEEPER_X_MAX, y = 40;
    int x = 4, y = 30;
    mesh_packet_t GKPkt;
    int timeout_ms = 500;

    while (1)
    {
        if (wireless_get_rx_pkt(&GKPkt, timeout_ms) && (Player2_Tx_Address == GKPkt.nwk.src))
        {
            printf("G\n");
            wireless_flush_rx();
            if(!(memcmp(GKPkt.data, "right", 5)))
            {
                if(y != GOALKEEPER_Y_MAX)
                {
                    y += 3;
                }
            }
            else if(!(memcmp(GKPkt.data, "left", 4)))
            {
                if(y != GOALKEEPER_Y_MIN)
                {
                    y -= 3;
                }
            }
            else if(!(memcmp(GKPkt.data, "up", 2)))
            {
                if(x != GOALKEEPER_X_MIN)
                {
                    x -= 3;
                }
            }
            else if(!(memcmp(GKPkt.data, "down", 4)))
            {
                if(x != GOALKEEPER_X_MAX)
                {
                    x += 3;
                }
            }
        }
        mat.drawGoalkeeper(x, y, GOALKEEPER_COLOR);
        mat.drawGoalPost(GOALPOST, mat.goalpost_color);
    }
    vTaskDelay(500);
}

void ControlBall(void *p)
{
    //    int8_t shoot_num = -1;
    mesh_packet_t ballPkt;
    int timeout_ms = 500;

    while(1)
    {
        if (wireless_get_rx_pkt(&ballPkt, timeout_ms) && (Player1_Tx_Address == ballPkt.nwk.src))
        {
            wireless_flush_rx();
            printf("R1\n");
            if(!(memcmp(ballPkt.data, "right", 5)))
                Y_HIT = 5;
            else if(!(memcmp(ballPkt.data, "left", 4)))
                Y_HIT = 55;
            else if(!(memcmp(ballPkt.data, "straight", 7)))
                Y_HIT = 64-32;
            else
                continue;

            // Screen needed when ball is blocked


            //            wireless_send(250, mesh_pkt_ack, "Send Data", strlen("Send Data"), 0);

            //xSemaphoreTake(mat.directionSem, portMAX_DELAY);

            if (!mat.ballBlocked) {
                mat.drawPixel(X_START, 32, green1);
                mat.clearPixel(X_START, 32, green1);
                mat.drawBall(X_START, 32, BALL_COLOR);
            }
            animateBall();
            if (!mat.ballBlocked) {
                mat.goalpost_color = green1;
                //                shoot_num++;
                //                mat.updateScore(1, shoot_num, goal);
                //                if (4 == shoot_num) {
                //                    shoot_num = -1;
                //                    mat.resetGame();
                //                }

            }
            if (mat.ballBlocked) {
                //            printf("1");
                //mat.drawPixel(X_START, 32, green1);
                mat.goalpost_color = red1;
                //                mat.updateScore(1, shoot_num, noGoal);
                //                if (4 == shoot_num) {
                //                    shoot_num = -1;
                //                    mat.resetGame();
                //                }
            }
            mat.clearDisplay(cyan);
            mat.drawGoalPost(GOALPOST, mat.goalpost_color);
            delay_ms(1000);
            mat.resetShoot();
            mat.drawGoalkeeper(mat.goalkeeper_x, mat.goalkeeper_y, GOALKEEPER_COLOR);
        }
        vTaskDelay(500);
    }
}

/*
 ** Task to update LED Matrix.
 */
void showStartCondition(void *p)
{

    mat.drawStartScreen(48,0);

    while(1)
    {
        vTaskDelay(1);
    }
}

void showCountDown(void *p)
{

    while(1)
    {
        mat.drawCountDown(2, 30);
        vTaskDelay(10);
    }
}

int main(void)
{

    mat.displayInit();
    wireless_init();
    wireless_service();
    scheduler_add_task(new wirelessTask(PRIORITY_MEDIUM));

    //showStartCondition(NULL);
    //    mat.drawGameName(10,40);


    //mat.drawStartCondition(2,40);           // To display start condition for the game.
    //
    //    mat.drawWinner(8, 40, '2');
    //    mat.drawDraw(20, 36);
    //
    //
    // Task to check sensor values and show Ball movement animation.
    xTaskCreate(ControlBall, "Control Ball", 1024, NULL, PRIORITY_MEDIUM, NULL);
    // Task to show Ball movement animation.
    xTaskCreate(ControlGoalkeeper, "Control Goalkeeper", 1024, NULL, PRIORITY_MEDIUM, NULL);
    //    xTaskCreate(Player2Ready, "Display car", 1024, NULL, 3, NULL);     // Task to show Player 2 ready.
    //
    //    xTaskCreate(showCountDown, "Start the game", 1024, NULL, 3, NULL);
    //    mat.drawStartWord(10,30);

    //xTaskCreate(UpdateGoalkeeperPosition, "Update Goalkeeper Position", 1024, NULL, PRIORITY_LOW, NULL);
    xTaskCreate(UpdateDisplay, "Update LED Matrix", 1024, NULL, PRIORITY_HIGH, NULL);

    //    mat.drawFrame();
    //    mat.drawLineDotted(10, 3, cyan);
    //    mat.drawLineDotted(21, 3, cyan);
    //    mat.drawLine(1, 53, 30, 53, purple);
    //    mat.drawLine(1, 52, 30, 52, purple);
    //
    //    mat.drawLine(31, 0, 31, 63, purple1);    // Row: 31 & 63
    //    mat.drawLine(0, 0, 0, 63, purple2);    // Row: 31 & 63

    //    mat.drawObstacleSquare(12, 40, 3, yellow1, skyblue1);


    //    xTaskCreate(showStartCondition, "Start the game", 1024, NULL, 3, NULL);
    //    xTaskCreate(showScores, "Display score numbers", 1024, NULL, 3, NULL);
    //    xTaskCreate(showObstacleTriangle, "Display obstacles", 1024, NULL, 3, NULL);
    //    xTaskCreate(showObstacleSquare, "Display obstacles", 1024, NULL, 3, NULL);
    //    xTaskCreate(showDown, "Bring down", 1024, NULL, 3, NULL);

    //     tbd: Update LED matrix every 1ms by calling rit interrupt. Not being used atm.
    //    rit_enable(interruptToDisplay, 1);
    scheduler_start();
    //    vTaskStartScheduler();

    return -1;
}
