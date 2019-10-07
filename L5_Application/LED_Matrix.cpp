/*
 * LEDMatrixGPIO.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: Kevin
 */

#include "LED_Matrix.hpp"
#include "io.hpp"
#include <inttypes.h>

//frame data for our start screen
uint32_t startScreenImg[32][3] =
{{0, 0, 0}, {0,0,0}, {0x0, 0x0, 0x55000}, {0,0,0xAA800},
        {0,0,0xAA800}, {0,0,0xAA800}, {0,0,0xAA800}, {0,0,0x80800},
        {0,0,0x140800}, {0,0,0x140800}, {0,0,0x100800}, {0,0,0xFF000},
        {0,0,0}, {0x3C000000,0x0000003C,0}, {0x43800000,0x00003FC2,0}, {0x4C400000,0x00004032,0},
        {0x47800000,0x3FE2,0}, {0x40400000,0x4002,0}, {0x47800000,0x3FE2,0}, {0x40400000,0x0202,0},
        {0x47800000,0x01E2,0}, {0x40400000,0x202,0}, {0x3F800000,0x1FC,0}, {0,0,0},
        {0,0,0}, {0x7DF7BEF8,0,0x7DF7BEF8}, {0x4044A220,0,0x4044A220}, {0x7C47AE20,0,0x7C47AE20},
        {0x0444A420,0,0x0444A420}, {0x7C44A220,0,0x7C44A220}, {0,0,0}, {0,0,0}
};

//frame data for stars for our game over screen
uint32_t gameOverStarsImg[32][3] =
{{0x1010, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
};

//each index corresponds to one of five rows to show 'round' at a set x position
uint32_t roundImg[5] = {0x38C94B80, 0x25296A40, 0x39295A40, 0x29294A40, 0x24C64B80};

//5x5
uint8_t pImg[5] = {0x6, 0x5, 0x6, 0x4, 0x4};
uint8_t zeroImg[5] = {0x4, 0xA, 0xA, 0xA, 0x4};
uint8_t oneImg[5] = {0x4, 0xC, 0x4, 0x4, 0xE};
uint8_t twoImg[5] = {0x7, 0x1, 0x7, 0x4, 0x7};
uint8_t threeImg[5] = {0x7, 0x1, 0x7, 0x1, 0x7};
uint16_t goImg[5] = {0x3E7D, 0x2045, 0x2745, 0x2244, 0x3E7D};
//actions
uint16_t leftRockImg[10] = {0xF0, 0x10E, 0x101, 0x11E, 0x101, 0x11E, 0x101, 0x11E, 0x101, 0xFE};
uint16_t rightRockImg[10] = {0x1E, 0xE1, 0x101, 0xF1, 0x101, 0xF1, 0x101, 0xF1, 0x101, 0xFE};
uint16_t paperImg[10] = {0xAA, 0x155, 0x155, 0x155, 0x155, 0x101, 0x281, 0x281, 0x201, 0x1FE};
uint16_t leftScissorsImg[10] = {0x1E00, 0x21FE, 0x2601, 0x21FE, 0x2001, 0x21FE, 0x2020, 0x23C0, 0x2020, 0x1FC0};
uint16_t rightScissorsImg[10] = {0x1E, 0x1FE1, 0x2019, 0x1FF1, 0x2001, 0x1FF1, 0x101, 0xF1, 0x101, 0xFE};

//5x7 images
uint8_t p57Img[7] = {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10};
uint8_t one57Img[7] = {0x4, 0xC, 0x14, 0x4, 0x4, 0x4, 0x1F};
uint8_t two57Img[7] = {0x1F, 0x01, 0x01, 0x1F, 0x10, 0x10, 0x1F};
uint8_t w57Img[7] = {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x1F};
uint8_t i57Img[7] = {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1};
uint8_t n57Img[7] = {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11};
uint8_t s57Img[7] = {0x1F, 0x10, 0x10, 0x1F, 0x01, 0x1, 0x1F};
uint8_t y57Img[7] = {0x11, 0xA, 0x4, 0x4, 0x4, 0x4, 0x4};
uint8_t o57Img[7] = {0xE, 0x11, 0x11, 0x11, 0x11, 0x11, 0xE};
uint8_t u57Img[7] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xE};
uint8_t c57Img[7] = {0xF, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF};
uint8_t k57Img[7] = {0x12, 0x14, 0x18, 0x10, 0x18, 0x14, 0x12};
uint8_t ex57Img[7] = {0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x1};
//5x5 images
uint8_t g5Img[5] = {0x1F, 0x10, 0x17, 0x11, 0x1F};
uint8_t a5Img[5] = {0x1F, 0x11, 0x1F, 0x11, 0x11};
uint8_t m5Img[5] = {0x11, 0x1B, 0x15, 0x11, 0x11};
uint8_t e5Img[5] = {0xF, 0x8, 0xE, 0x8, 0xF};
uint8_t o5Img[5] = {0xE, 0x11, 0x11, 0x11, 0xE};
uint8_t v5Img[5] = {0x11, 0x11, 0x11, 0xA, 0x4};
uint8_t r5Img[5] = {0x1F, 0x11, 0x17, 0x12, 0x11};
uint8_t d5Img[5] = {0x1E, 0x11, 0x11, 0x11, 0x1E};
uint8_t w5Img[5] = {0x11, 0x11, 0x15, 0x15, 0x1F};

//arrow
uint8_t arrowImg[3] = {0x2, 0x3, 0x2};

void LEDMatrixGPIO::init(){
    addrC = new GPIO(P2_0);
    clk = new GPIO(P2_1);
    oe = new GPIO(P2_2);
    latch = new GPIO(P2_3);
    addrD = new GPIO(P2_4);
    addrB = new GPIO(P2_5);
    g2 = new GPIO(P2_6);
    g1 = new GPIO(P2_7);

    addrA = new GPIO(P1_29);
    b2 = new GPIO(P1_28);
    r2 = new GPIO(P1_23);
    b1 = new GPIO(P1_22);
    r1 = new GPIO(P1_20);
    addrE = new GPIO(P1_19);
    //select data lines

    //configure dirs
    addrA->setAsOutput(); addrB->setAsOutput(); addrC->setAsOutput();
    addrD->setAsOutput();
    addrE->setAsOutput();
    latch->setAsOutput(); oe->setAsOutput();
    clk->setAsOutput(); r1->setAsOutput(); g1->setAsOutput();
    b1->setAsOutput(); r2->setAsOutput(); g2->setAsOutput();
    b2->setAsOutput();

    //set default states
    addrA->setLow(); addrB->setLow(); addrC->setLow();
    addrD->setLow();
    addrE->setLow();
    latch->setLow(); oe->setHigh();
    clk->setLow();  r1->setLow();  g1->setLow();
    b1->setLow(); r2->setLow(); g2->setLow();
    b2->setLow();
    puts("init complete");
}

void LEDMatrixGPIO::setEn(bool set)
{
    (set) ? oe->setHigh() : oe->setLow();
}

void LEDMatrixGPIO::setLatch(bool set)
{
    (set) ? latch->setHigh() : latch->setLow();
}

//change GPIO address lines to select correct row. 32x32 LED Matrix has multiplexed rows,
//so addr[3:0] = 0000 will select row 0 and 16, 0001 with row 1 and 17 etc
void LEDMatrixGPIO::selRow(uint8_t row)
{
    switch(row)
    {
        case 0: //00000
        case 32:
        {
            addrA->setLow(); addrB->setLow();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 1: //00001
        case 33:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 2: //00010
        case 34:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 3: //00011
        case 35:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 4: //00100
        case 36:
        {
            addrA->setLow(); addrB->setLow();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 5: //00101
        case 37:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 6: //00110
        case 38:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 7: //00111
        case 39:
        {
            addrA->setLow(); addrB->setLow();
            addrC->setHigh(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 8: //01000
        case 40:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 9: //01001
        case 41:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 10: //01010
        case 42:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setLow(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 11: //01011
        case 43:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setLow(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 12: //01100
        case 44:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 13: //01101
        case 45:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setHigh(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 14: //01110
        case 46:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setHigh(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 15: //01111
        case 47:
        {
            addrA->setLow(); addrB->setHigh();
            addrC->setHigh(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 16: //10000
        case 48:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 17: //10001
        case 49:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setLow(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 18: //10010
        case 50:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setLow(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 19: //10011
        case 51:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setLow(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 20: //10100
        case 52:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 21: //10101
        case 53:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setHigh(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 22: //10110
        case 54:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setHigh(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 23: //10111
        case 55:
        {
            addrA->setHigh(); addrB->setLow();
            addrC->setHigh(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 24: //11000
        case 56:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 25: //11001
        case 57:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setLow(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 26: //11010
        case 58:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setLow(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 27: //11011
        case 59:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setLow(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        case 28: //11100
        case 60:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setHigh(); addrD->setLow();
            addrE->setLow();
            break;
        }

        case 29: //11101
        case 61:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setHigh(); addrD->setLow();
            addrE->setHigh();
            break;
        }

        case 30: //11110
        case 62:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setHigh(); addrD->setHigh();
            addrE->setLow();
            break;
        }

        case 31: //11111
        case 63:
        {
            addrA->setHigh(); addrB->setHigh();
            addrC->setHigh(); addrD->setHigh();
            addrE->setHigh();
            break;
        }

        default:
        {
            puts("selRow out of bounds");
            return;
            break;
        }
    }
}

//just for testing with patterned data
void LEDMatrixGPIO::fillBuffer()
{
//    //all 3 colors for all 32 rows are filled so should be white
//    for(uint8_t i = 0; i < 64; i++)
//    {
//        frameBuffer[i][0] = 0xFFFFFFFFFFFF1FFF; //r
//        frameBuffer[i][1] = 0xFFFFFFFFFFFF1FFF; //g
//        frameBuffer[i][2] = 0xFFFFFFFFFFFF1FFF; //b
//        //printf("Row %i Data: %" PRIu32 "  %" PRIu32 "  %" PRIu32 "\n", i, frameBuffer[i][0], frameBuffer[i][1], frameBuffer[i][2]);
//    }

    //all 3 colors for all 32 rows are filled so should be white
    for(uint8_t i = 0; i < 64; i++)
    {
        frameBuffer[i][0] = 0x00000000000000FF; //r
        frameBuffer[i][1] = 0x00000000000000FF; //g
        frameBuffer[i][2] = 0x00000000000000FF; //b
        //printf("Row %i Data: %" PRIu32 "  %" PRIu32 "  %" PRIu32 "\n", i, frameBuffer[i][0], frameBuffer[i][1], frameBuffer[i][2]);
    }
}

//should fill selected row with FFh data for all 3 colors, so a white line
void LEDMatrixGPIO::drawLine(uint8_t row)
{
    matrixGPIO.setEn(true); //turn off display
    matrixGPIO.setLatch(false);
    matrixGPIO.selRow(row); //set select lines
    r1->setHigh(); g1->setHigh(); b1->setHigh();
    //shift in FF for 3 top half shift registers
    for(uint8_t i = 0; i < 64; i++)
    {
        clk->setHigh(); clk->setLow(); //possible delay
    }
    matrixGPIO.setLatch(true); //move shift register contents into parallel output register
    matrixGPIO.setEn(false); //turn on display
}

//Supposed to draw a horizontal line going down vertically
//only used to test smooth top to bottom animation with GPIO bus
void LEDMatrixGPIO::verticalLineAnimation()
{   //need to initially fill registers with FFh
    matrixGPIO.setEn(true);
    matrixGPIO.setLatch(false);
    matrixGPIO.fillBuffer();
    //i iterates through rows, j iterates through "columns/pixels" in each row
    for(int i = 0; i < 32; i++)
    {
        matrixGPIO.setEn(true);
        matrixGPIO.setLatch(false);
        //this inner loop will shift MSB of frameBuffer[i] into corresponding shift register
        for(int j = 64; j > 0; j--)
        {
            ((frameBuffer[i][0] >> j) & 1) ? r1->setHigh() : r1->setLow(); //shift in r1 data with msb getting shifted in first
            ((frameBuffer[i][1] >> j) & 1) ? g1->setHigh() : g1->setLow();
            ((frameBuffer[i][2] >> j) & 1) ? b1->setHigh() : b1->setLow();
            clk->setHigh(); clk->setLow();  //shift in all 3 color bits at once
        }
        //at this point, all 3 shift registers should be filled with corresponding row data in frameBuffer
        matrixGPIO.selRow(i); //select corresponding row pair
        matrixGPIO.setLatch(true); //shift down to output register
        matrixGPIO.setEn(false); //display rows
        delay_ms(1); //using delay_us instead of delay_ms so vTaskDelay call is avoided
    }
    r1->setLow(); g1->setLow(); b1->setLow(); //don't want leftover data getting shifted into top half regs as we move to bottom half
    for(int i = 32; i < 64; i++)
    {
        matrixGPIO.setEn(true);
        matrixGPIO.setLatch(false);
        //this inner loop will shift MSB of frameBuffer[i] into corresponding shift register
        for(int j = 63; j >= 0; j--)
        {
            ((frameBuffer[i][0] >> j) & 1) ? r2->setHigh() : r2->setLow(); //shift in r1 data with msb getting shifted in first
            ((frameBuffer[i][1] >> j) & 1) ? g2->setHigh() : g2->setLow();
            ((frameBuffer[i][2] >> j) & 1) ? b2->setHigh() : b2->setLow();
            clk->setHigh(); clk->setLow();  //shift in all 3 color bits at once
        }
        //at this point, all 3 shift registers should be filled with corresponding row data in frameBuffer
        matrixGPIO.selRow(i);
        matrixGPIO.setLatch(true);
        matrixGPIO.setEn(false);
        delay_ms(1); //using delay_us instead of delay_ms so vTaskDelay call is avoided
    }
    r2->setLow(); g2->setLow(); b2->setLow(); //set to default states
}


//designed to loop through frameBuffer and shift data for paired rows
void LEDMatrixGPIO::updateDisplay()
{
    //only need to iterate up to 32 because we're doing 2 rows at a time
    for(int i = 0; i < 32; i++)
    {
        matrixGPIO.setEn(true);
        matrixGPIO.setLatch(false);
        matrixGPIO.selRow(i); //will select i and i+16 rows at same time
        //this inner loop will shift MSB of frameBuffer[i] into corresponding shift register
        for(int j = 64; j > 0; j--)
        {
            ((frameBuffer[i][0] >> j) & 1) ? r1->setHigh() : r1->setLow(); //shift in r1 data with msb getting shifted in first
            ((frameBuffer[i][1] >> j) & 1) ? g1->setHigh() : g1->setLow(); //g1 data
            ((frameBuffer[i][2] >> j) & 1) ? b1->setHigh() : b1->setLow(); //b1 data

            //bottom half for corresponding row; rows 0/16, 1/17 etc
            ((frameBuffer[i + 32][0] >> j) & 1) ? r2->setHigh() : r2->setLow(); //r2
            ((frameBuffer[i + 32][1] >> j) & 1) ? g2->setHigh() : g2->setLow(); //g2
            ((frameBuffer[i + 32][2] >> j) & 1) ? b2->setHigh() : b2->setLow(); //b2
            clk->setHigh(); clk->setLow();  //shift in all 3 color bits at once for top half/bottom half regs
        }
        //at this point, all 3 shift registers should be filled with corresponding row data in frameBuffer
        matrixGPIO.setEn(true); //turn off display before updating output registers
        matrixGPIO.setLatch(true); //push shift register contents down to output registers
        matrixGPIO.setEn(false); //turn on display
//        delay_us(250); //let the row display for a small time
    }
}

//just sets corresponding bits in frameBuffer; doesn't actually do overhead to actually display pixel
//Assumes that updateDisplay is being run repetitively
void LEDMatrixGPIO::drawPixel(uint8_t x, uint8_t y, uint8_t c)
{
    //basic position validation
    if((x < 0) || (x > 63) || (y < 0) || (y > 63)) return;
    x = 63 - x; //small offset. For example, if (x,y) = (0, 1) then we want rowData for row 1 to have the 31st bit set.
    //set rgb pixel data according to provided color data
    (c & 1) ? frameBuffer[y][0] |= (1 << x) : frameBuffer[y][0] &= ~(1 << x); //if c[0](LSB) is set, then set r bit
    (c >> 1) ? frameBuffer[y][1] |= (1 << x) : frameBuffer[y][1] &= ~(1 << x); //if c[1] is set, then set g bit
    (c >> 2) ? frameBuffer[y][2] |= (1 << x) : frameBuffer[y][2] &= ~(1 << x); //if c[2] (MSB) is set, then set b bit
}

void LEDMatrixGPIO::drawStartScreen()
{
    //TODO: hook up PCB's START switch to Port 0 or 2 external interrupt and have ISR give a semaphore that will exit this loop
    //while(!SW.getSwitch(1))
    //{
    //copy startScreenImg into our frameBuffer that will be drawn by updateDisplay
    memcpy(frameBuffer, startScreenImg, sizeof(frameBuffer));
    //dot animation
    frameBuffer[29][0] &= ~(0xF);
    frameBuffer[29][1] &= ~(0xF);
    frameBuffer[29][2] &= ~(0xF);
    delay_ms(25);
    frameBuffer[29][0] |= (0x8);
    frameBuffer[29][1] |= (0x8);
    frameBuffer[29][2] |= (0x8);
    delay_ms(25);
    frameBuffer[29][0] |= (0xC);
    frameBuffer[29][1] |= (0xC);
    frameBuffer[29][2] |= (0xC);
    delay_ms(25);
    frameBuffer[29][0] |= (0xE);
    frameBuffer[29][1] |= (0xE);
    frameBuffer[29][2] |= (0xE);
    delay_ms(150);
    for(int i = 25; i < 30; i++)
    {
        frameBuffer[i][0] &= ~(0xFFFFFFFF);
        frameBuffer[i][1] &= ~(0xFFFFFFFF);
        frameBuffer[i][2] &= ~(0xFFFFFFFF);
    }
    //draw 1p and 2p
    for(int i = 25; i < 30; i++)
    {
        frameBuffer[i][0] |= (oneImg[i-25] << 24);
        frameBuffer[i][0] |= (pImg[i-25] << 21);
        frameBuffer[i][2] |= (twoImg[i-25] << 7);
        frameBuffer[i][2] |= (pImg[i-25] << 3);
    }
    //draw arrow
    for(int i = 26; i < 29; i++)
    {
        (onePlayer) ? frameBuffer[i][0] |= (arrowImg[i-26] << 29) : frameBuffer[i][0] |= (arrowImg[i-26] << 11);
        (onePlayer) ? frameBuffer[i][1] |= (arrowImg[i-26] << 29) : frameBuffer[i][1] |= (arrowImg[i-26] << 11);
        (onePlayer) ? frameBuffer[i][2] |= (arrowImg[i-26] << 29) : frameBuffer[i][2] |= (arrowImg[i-26] << 11);
    }
    delay_ms(250);

    //}
}

void LEDMatrixGPIO::drawScoreboard(uint8_t roundNum, uint8_t leftScore, uint8_t rightScore)
{
    printf("left: %d, right: %d\n", leftScore, rightScore);
    if(roundNum < 0 || roundNum > 5) return;
    if(leftScore < 0 || leftScore > 3) return;
    if(rightScore < 0 || rightScore > 3) return;

    clearScreen();

    //round animation
    for(int i = 27; i >= 0; i--)
    {
        frameBuffer[i][0] = frameBuffer[i][1] = frameBuffer[i][2] = roundImg[0];
        frameBuffer[i+1][0] = frameBuffer[i+1][1] = frameBuffer[i+1][2] = roundImg[1];
        frameBuffer[i+2][0] = frameBuffer[i+2][1] = frameBuffer[i+2][2] = roundImg[2];
        frameBuffer[i+3][0] = frameBuffer[i+3][1] = frameBuffer[i+3][2] = roundImg[3];
        frameBuffer[i+4][0] = frameBuffer[i+4][1] = frameBuffer[i+4][2] = roundImg[4];
        delay_ms(10);
        //don't clear round when it hits top of screen
        if(i != 0)
        {
            frameBuffer[i][0] = frameBuffer[i][1] = frameBuffer[i][2] = 0;
            frameBuffer[i+1][0] = frameBuffer[i+1][1] = frameBuffer[i+1][2] = 0;
            frameBuffer[i+2][0] = frameBuffer[i+2][1] = frameBuffer[i+2][2] = 0;
            frameBuffer[i+3][0] = frameBuffer[i+3][1] = frameBuffer[i+3][2] = 0;
            frameBuffer[i+4][0] = frameBuffer[i+4][1] = frameBuffer[i+4][2] = 0;
        }
    }

    //draw round num
    updateRoundNum(roundNum);

    //save temp
    uint32_t tempRoundImg[5] = {frameBuffer[0][0], frameBuffer[1][0], frameBuffer[2][0], frameBuffer[3][0], frameBuffer[4][0]};

    //blink round
    for(int i = 0; i < 10; i++)
    {
        frameBuffer[0][0] = frameBuffer[0][1] = frameBuffer[0][2] = 0;
        frameBuffer[1][0] = frameBuffer[1][1] = frameBuffer[1][2] = 0;
        frameBuffer[2][0] = frameBuffer[2][1] = frameBuffer[2][2] = 0;
        frameBuffer[3][0] = frameBuffer[3][1] = frameBuffer[3][2] = 0;
        frameBuffer[4][0] = frameBuffer[4][1] = frameBuffer[4][2] = 0;
        delay_ms(10);
        frameBuffer[0][0] = frameBuffer[0][1] = frameBuffer[0][2] = tempRoundImg[0];
        frameBuffer[1][0] = frameBuffer[1][1] = frameBuffer[1][2] = tempRoundImg[1];
        frameBuffer[2][0] = frameBuffer[2][1] = frameBuffer[2][2] = tempRoundImg[2];
        frameBuffer[3][0] = frameBuffer[3][1] = frameBuffer[3][2] = tempRoundImg[3];
        frameBuffer[4][0] = frameBuffer[4][1] = frameBuffer[4][2] = tempRoundImg[4];
        delay_ms(10);
    }

    //clear round title
    frameBuffer[0][0] = frameBuffer[0][1] = frameBuffer[0][2] = 0;
    frameBuffer[1][0] = frameBuffer[1][1] = frameBuffer[1][2] = 0;
    frameBuffer[2][0] = frameBuffer[2][1] = frameBuffer[2][2] = 0;
    frameBuffer[3][0] = frameBuffer[3][1] = frameBuffer[3][2] = 0;
    frameBuffer[4][0] = frameBuffer[4][1] = frameBuffer[4][2] = 0;



    //draw P1 and P2, vertical line
    frameBuffer[0][0] |= (0x3 << 15); frameBuffer[0][1] |= (0x3 << 15); frameBuffer[0][2] |= (0x3 << 15);
    frameBuffer[6][0] |= (0x3 << 15); frameBuffer[6][1] |= (0x3 << 15); frameBuffer[6][2] |= (0x3 << 15);
    for(int i = 1; i < 6; i++)
    {
        frameBuffer[i][0] |= (pImg[i-1] << 29);
        frameBuffer[i][0] |= (oneImg[i-1] << 24);
        frameBuffer[i][2] |= (pImg[i-1] << 4);
        frameBuffer[i][2] |= (twoImg[i-1]);
        frameBuffer[i][0] |= (0x3 << 15);
        frameBuffer[i][1] |= (0x3 << 15);
        frameBuffer[i][2] |= (0x3 << 15);
    }
    frameBuffer[7][0] |= (0xFFFFFFFF); frameBuffer[7][1] |= (0xFFFFFFFF); frameBuffer[7][2] |= (0xFFFFFFFF);
    frameBuffer[8][0] |= (0xFFFFFFFF); frameBuffer[8][1] |= (0xFFFFFFFF); frameBuffer[8][2] |= (0xFFFFFFFF);


    drawScore(leftScore, rightScore);


    delay_ms(250);
    drawCountdown();

}

void LEDMatrixGPIO::updateRoundNum(uint8_t num)
{
    //basic validation
    if(num < 0 || num > 5) return;

    switch(num)
    {
        case 1:
            frameBuffer[0][0] |= (0x4 << 1); frameBuffer[0][1] |= (0x4 << 1); frameBuffer[0][2] |= (0x4 << 1);
            frameBuffer[1][0] |= (0xC << 1); frameBuffer[1][1] |= (0xC << 1); frameBuffer[1][2] |= (0xC << 1);
            frameBuffer[2][0] |= (0x4 << 1); frameBuffer[2][1] |= (0x4 << 1); frameBuffer[2][2] |= (0x4 << 1);
            frameBuffer[3][0] |= (0x4 << 1); frameBuffer[3][1] |= (0x4 << 1); frameBuffer[3][2] |= (0x4 << 1);
            frameBuffer[4][0] |= (0xE << 1); frameBuffer[4][1] |= (0xE << 1); frameBuffer[4][2] |= (0xE << 1);
            break;
        case 2:
            frameBuffer[0][0] |= (0xE << 1); frameBuffer[0][1] |= (0xE << 1); frameBuffer[0][2] |= (0xE << 1);
            frameBuffer[1][0] |= (0x2 << 1); frameBuffer[1][1] |= (0x2 << 1); frameBuffer[1][2] |= (0x2 << 1);
            frameBuffer[2][0] |= (0xE << 1); frameBuffer[2][1] |= (0xE << 1); frameBuffer[2][2] |= (0xE << 1);
            frameBuffer[3][0] |= (0x8 << 1); frameBuffer[3][1] |= (0x8 << 1); frameBuffer[3][2] |= (0x8 << 1);
            frameBuffer[4][0] |= (0xE << 1); frameBuffer[4][1] |= (0xE << 1); frameBuffer[4][2] |= (0xE << 1);
            break;
        case 3:
            frameBuffer[0][0] |= (0xE << 1); frameBuffer[0][1] |= (0xE << 1); frameBuffer[0][2] |= (0xE << 1);
            frameBuffer[1][0] |= (0x2 << 1); frameBuffer[1][1] |= (0x2 << 1); frameBuffer[1][2] |= (0x2 << 1);
            frameBuffer[2][0] |= (0xE << 1); frameBuffer[2][1] |= (0xE << 1); frameBuffer[2][2] |= (0xE << 1);
            frameBuffer[3][0] |= (0x2 << 1); frameBuffer[3][1] |= (0x2 << 1); frameBuffer[3][2] |= (0x2 << 1);
            frameBuffer[4][0] |= (0xE << 1); frameBuffer[4][1] |= (0xE << 1); frameBuffer[4][2] |= (0xE << 1);
            break;
        case 4:
            frameBuffer[0][0] |= (0xA << 1); frameBuffer[0][1] |= (0xA << 1); frameBuffer[0][2] |= (0xA << 1);
            frameBuffer[1][0] |= (0xA << 1); frameBuffer[1][1] |= (0xA << 1); frameBuffer[1][2] |= (0xA << 1);
            frameBuffer[2][0] |= (0xE << 1); frameBuffer[2][1] |= (0xE << 1); frameBuffer[2][2] |= (0xE << 1);
            frameBuffer[3][0] |= (0x2 << 1); frameBuffer[3][1] |= (0x2 << 1); frameBuffer[3][2] |= (0x2 << 1);
            frameBuffer[4][0] |= (0x2 << 1); frameBuffer[4][1] |= (0x2 << 1); frameBuffer[4][2] |= (0x2 << 1);
            break;
        case 5:
            frameBuffer[0][0] |= (0xE << 1); frameBuffer[0][1] |= (0xE << 1); frameBuffer[0][2] |= (0xE << 1);
            frameBuffer[1][0] |= (0x8 << 1); frameBuffer[1][1] |= (0x8 << 1); frameBuffer[1][2] |= (0x8 << 1);
            frameBuffer[2][0] |= (0xE << 1); frameBuffer[2][1] |= (0xE << 1); frameBuffer[2][2] |= (0xE << 1);
            frameBuffer[3][0] |= (0x2 << 1); frameBuffer[3][1] |= (0x2 << 1); frameBuffer[3][2] |= (0x2 << 1);
            frameBuffer[4][0] |= (0xE << 1); frameBuffer[4][1] |= (0xE << 1); frameBuffer[4][2] |= (0xE << 1);
            break;
        default:
            break;
    }
}

void LEDMatrixGPIO::clearScreen()
{
    //size of one element * # of rows * # of columns
    memset(frameBuffer, 0, (sizeof(frameBuffer[0][0])) * 3 * 64);
}

void LEDMatrixGPIO::drawCountdown()
{
    frameBuffer[32][1] = (threeImg[0] << 15);
    frameBuffer[33][1] = (threeImg[1] << 15);
    frameBuffer[34][1] = (threeImg[2] << 15);
    frameBuffer[35][1] = (threeImg[3] << 15);
    frameBuffer[36][1] = (threeImg[4] << 15);
    updateDisplay();
    delay_us(1000 * 1000);
    frameBuffer[32][1] = (twoImg[0] << 15);
    frameBuffer[33][1] = (twoImg[1] << 15);
    frameBuffer[34][1] = (twoImg[2] << 15);
    frameBuffer[35][1] = (twoImg[3] << 15);
    frameBuffer[36][1] = (twoImg[4] << 15);
    updateDisplay();
    delay_us(1000 * 1000);
    frameBuffer[32][1] = (oneImg[0] << 15);
    frameBuffer[33][1] = (oneImg[1] << 15);
    frameBuffer[34][1] = (oneImg[2] << 15);
    frameBuffer[35][1] = (oneImg[3] << 15);
    frameBuffer[36][1] = (oneImg[4] << 15);
    updateDisplay();
    delay_us(1000 * 1000);
    frameBuffer[32][1] = (zeroImg[0] << 15);
    frameBuffer[33][1] = (zeroImg[1] << 15);
    frameBuffer[34][1] = (zeroImg[2] << 15);
    frameBuffer[35][1] = (zeroImg[3] << 15);
    frameBuffer[36][1] = (zeroImg[4] << 15);
    updateDisplay();
    delay_us(1000 * 1000);
    frameBuffer[32][1] = frameBuffer[33][1] = frameBuffer[34][1] = frameBuffer[35][1] = frameBuffer[36][1] = 0;
}

void LEDMatrixGPIO::drawAction(LEDMatrixGPIO::Action left, LEDMatrixGPIO::Action right)
{
    switch(left)
    {
        case Rock:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][0] |= (leftRockImg[i] << 22);
            }
            break;
        case Paper:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][0] |= (paperImg[i] << 22);
            }
            break;
        case Scissors:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][0] |= (leftScissorsImg[i] << 17);
            }

            break;
        default:
            break;
    }

    switch(right)
    {
        case Rock:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][2] |= (rightRockImg[i] << 1);
            }
            break;
        case Paper:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][2] |= (paperImg[i] << 1);
            }
            break;
        case Scissors:
            for(int i = 0; i < 10; i++)
            {
                frameBuffer[i+20][2] |= (rightScissorsImg[i] << 1);
            }
            break;
        default:
            break;
    }
    delay_ms(500);
}

void LEDMatrixGPIO::clearActions()
{
    for(int i = 20; i < 30; i++)
    {
        frameBuffer[i][0] = frameBuffer[i][1] = frameBuffer[i][2] = 0;
    }
}
//use case is to draw the score for players & cpu
//implementation will be a switch on num up to a certain limit
//will either be on left or right depending on left flag
void LEDMatrixGPIO::drawScore(uint8_t leftScore, uint8_t rightScore)
{
    //small validation
    if(leftScore > 3 || leftScore < 0) return;
    if(rightScore > 3 || rightScore < 0) return;

    //clear scores before drawing
    for(int i = 1; i < 6; i++)
    {
        frameBuffer[i][0] &= ~(0x1F << 17);
        frameBuffer[i][1] &= ~(0x1F << 17);
        frameBuffer[i][2] &= ~(0x1F << 17);
        frameBuffer[i][0] &= ~(0x1F << 10);
        frameBuffer[i][1] &= ~(0x1F << 10);
        frameBuffer[i][2] &= ~(0x1F << 10);
    }

    switch(leftScore)
    {
        case 0:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][0] |= (zeroImg[i-1] << 17);
            }
            break;
        case 1:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][0] |= (oneImg[i-1] << 17);
            }
            break;
        case 2:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][0] |= (twoImg[i-1] << 18);
            }
            break;
        case 3:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][0] |= (threeImg[i-1] << 18);
            }
            break;
        default:

            break;
    }

    switch(rightScore)
    {
        case 0:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][2] |= (zeroImg[i-1] << 10);
            }
            break;
        case 1:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][2] |= (oneImg[i-1] << 10);
            }
            break;
        case 2:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][2] |= (twoImg[i-1] << 11);
            }
            break;
        case 3:
            for(int i = 1; i < 6; i++)
            {
                frameBuffer[i][2] |= (threeImg[i-1] << 11);
            }
            break;
        default:

            break;
    }
}

void LEDMatrixGPIO::drawRoundEnd(LEDMatrixGPIO::Player winner)
{
    clearActions();
    //player 1 won the round
    if(winner == 0)
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 7; i++)
            {
                frameBuffer[i+10][0] |= (p57Img[i] << 17);
                frameBuffer[i+10][0] |= (one57Img[i] << 10);
                frameBuffer[i+18][0] |= (w57Img[i] << 21);
                frameBuffer[i+18][0] |= (i57Img[i] << 19);
                frameBuffer[i+18][0] |= (n57Img[i] << 13);
                frameBuffer[i+18][0] |= (s57Img[i] << 7);
            }
            delay_ms(25);
            //clear
            for(int x = 0; x < 7; x++)
            {
                frameBuffer[x+10][0] = 0;
                frameBuffer[x+18][0] = 0;
            }
            delay_ms(25);
        }

    }
    else if(winner == 1 && onePlayer) //cpu won round
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 7; i++)
            {
                frameBuffer[i+10][2] |= (c57Img[i] << 20);
                frameBuffer[i+10][2] |= (p57Img[i] << 14);
                frameBuffer[i+10][2] |= (u57Img[i] << 8);
                frameBuffer[i+18][2] |= (w57Img[i] << 21);
                frameBuffer[i+18][2] |= (i57Img[i] << 19);
                frameBuffer[i+18][2] |= (n57Img[i] << 13);
                frameBuffer[i+18][2] |= (s57Img[i] << 7);
            }
            delay_ms(25);
            //clear
            for(int x = 0; x < 7; x++)
            {
                frameBuffer[x+10][2] = 0;
                frameBuffer[x+18][2] = 0;
            }
            delay_ms(25);
        }

    }
    else if(winner == 1 && !onePlayer) //p2 won in a 2p match
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 7; i++)
            {
                frameBuffer[i+10][2] |= (p57Img[i] << 17);
                frameBuffer[i+10][2] |= (two57Img[i] << 10);
                frameBuffer[i+18][2] |= (w57Img[i] << 21);
                frameBuffer[i+18][2] |= (i57Img[i] << 19);
                frameBuffer[i+18][2] |= (n57Img[i] << 13);
                frameBuffer[i+18][2] |= (s57Img[i] << 7);
            }
            delay_ms(25);
            //clear
            for(int x = 0; x < 7; x++)
            {
                frameBuffer[x+10][2] = 0;
                frameBuffer[x+18][2] = 0;
            }
            delay_ms(25);
        }
    }
    else if(winner == 2)
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 5; i++)
            {
                frameBuffer[i+12][2] |= (d5Img[i] << 23);
                frameBuffer[i+12][2] |= (r5Img[i] << 17);
                frameBuffer[i+12][2] |= (a5Img[i] << 11);
                frameBuffer[i+12][2] |= (w5Img[i] << 5);
                frameBuffer[i+12][0] |= (d5Img[i] << 23);
                frameBuffer[i+12][0] |= (r5Img[i] << 17);
                frameBuffer[i+12][0] |= (a5Img[i] << 11);
                frameBuffer[i+12][0] |= (w5Img[i] << 5);
            }
            delay_ms(25);
            //clear
            for(int x = 0; x < 5; x++)
            {
                frameBuffer[x+12][2] = 0;
                frameBuffer[x+12][0] = 0;
            }
            delay_ms(25);
        }
    }

}

void LEDMatrixGPIO::drawGameOver(LEDMatrixGPIO::Player matchWinner)
{
    clearScreen();
    //do game over animation
    for(int j = 0; j < 10; j++)
    {
        for(int i = 0 ; i < 5; i++)
        {
            frameBuffer[i+10][1] |= (g5Img[i] << 22);
            frameBuffer[i+10][1] |= (a5Img[i] << 16);
            frameBuffer[i+10][1] |= (m5Img[i] << 10);
            frameBuffer[i+10][1] |= (e5Img[i] << 5);
            frameBuffer[i+17][1] |= (o5Img[i] << 22);
            frameBuffer[i+17][1] |= (v5Img[i] << 16);
            frameBuffer[i+17][1] |= (e5Img[i] << 11);
            frameBuffer[i+17][1] |= (r5Img[i] << 5);

        }
        delay_ms(25);
        for(int i = 0 ; i < 5; i++)
        {
            frameBuffer[i+10][1] &= ~(0xFFFFFFFF);
            frameBuffer[i+17][1] &= ~(0xFFFFFFFF);
        }
        delay_ms(25);
    }

    //draw if cpu won and it's a 1P match
    if(matchWinner != Player1 && onePlayer)
    {
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 7; j++)
            {
                frameBuffer[j+8][0] |= (y57Img[j] << 20);
                frameBuffer[j+8][0] |= (o57Img[j] << 14);
                frameBuffer[j+8][0] |= (u57Img[j] << 8);
                frameBuffer[j+17][0] |= (s57Img[j] << 23);
                frameBuffer[j+17][0] |= (u57Img[j] << 17);
                frameBuffer[j+17][0] |= (c57Img[j] << 11);
                frameBuffer[j+17][0] |= (k57Img[j] << 5);
                frameBuffer[j+17][0] |= (ex57Img[j] << 4);
            }
            delay_ms(25);

            //clear
            if(i < 14)
            {
                for(int x = 0; x < 7; x++)
                {
                    frameBuffer[x+8][2] = 0;
                    frameBuffer[x+17][2] = 0;
                }
                delay_ms(25);
            }

        }
    }
    else if(matchWinner == Player1 && onePlayer)
    {
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 7; j++)
            {
                frameBuffer[j+8][0] |= (y57Img[j] << 20);
                frameBuffer[j+8][0] |= (o57Img[j] << 14);
                frameBuffer[j+8][0] |= (u57Img[j] << 8);
                frameBuffer[j+17][0] |= (w57Img[j] << 18);
                frameBuffer[j+17][0] |= (i57Img[j] << 16);
                frameBuffer[j+17][0] |= (n57Img[j] << 10);
                frameBuffer[j+17][0] |= (ex57Img[j] << 8);
            }
            delay_ms(25);
            //clear
            if(i < 14)
            {
                for(int x = 0; x < 7; x++)
                {
                    frameBuffer[x+8][2] = 0;
                    frameBuffer[x+17][2] = 0;
                }
                delay_ms(25);
            }

        }
    }
    else if(matchWinner == Player1 && !onePlayer) //player1 wins and it's 2p game
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 7; i++)
            {
                frameBuffer[i+10][0] |= (p57Img[i] << 17);
                frameBuffer[i+10][0] |= (one57Img[i] << 10);
                frameBuffer[i+18][0] |= (w57Img[i] << 21);
                frameBuffer[i+18][0] |= (i57Img[i] << 19);
                frameBuffer[i+18][0] |= (n57Img[i] << 13);
                frameBuffer[i+18][0] |= (s57Img[i] << 7);
            }
            delay_ms(25);
            //clear
            if(j < 14)
            {
                for(int x = 0; x < 7; x++)
                {
                    frameBuffer[x+10][2] = 0;
                    frameBuffer[x+18][2] = 0;
                }
                delay_ms(25);
            }
        }
    }
    else //player2 wins and it's 2p game
    {
        for(int j = 0; j < 15; j++)
        {
            for(int i = 0; i < 7; i++)
            {
                frameBuffer[i+10][2] |= (p57Img[i] << 17);
                frameBuffer[i+10][2] |= (two57Img[i] << 10);
                frameBuffer[i+18][2] |= (w57Img[i] << 21);
                frameBuffer[i+18][2] |= (i57Img[i] << 19);
                frameBuffer[i+18][2] |= (n57Img[i] << 13);
                frameBuffer[i+18][2] |= (s57Img[i] << 7);
            }
            delay_ms(25);
            if(j < 14)
            {
                for(int x = 0; x < 7; x++)
                {
                    frameBuffer[x+10][2] = 0;
                    frameBuffer[x+18][2] = 0;
                }
                delay_ms(25);
            }
            //clear

        }
    }

}


//use case is to draw the score for players & cpu
//implementation will be a switch on num up to a certain limit
//will either be on left or right depending on left flag
void LEDMatrixGPIO::drawZero()
{
    //clear scores before drawing
    for(int i = 1; i < 6; i++)
    {
        frameBuffer[i][0] &= ~(0x1F << 17);
        frameBuffer[i][1] &= ~(0x1F << 17);
        frameBuffer[i][2] &= ~(0x1F << 17);
        frameBuffer[i][0] &= ~(0x1F << 10);
        frameBuffer[i][1] &= ~(0x1F << 10);
        frameBuffer[i][2] &= ~(0x1F << 10);
    }

    for(int i = 1; i < 6; i++)
    {
        frameBuffer[i][0] |= (zeroImg[i-1] << 49);
    }
}

void LEDMatrixGPIO::test()
{
    matrixGPIO.setEn(true); //turn off display
    matrixGPIO.setLatch(false);
    matrixGPIO.clearScreen();

    addrA->setLow(); addrB->setLow();
    addrC->setLow(); addrD->setLow();
    addrE->setLow();

    r1->setHigh(); g1->setHigh(); b1->setHigh();
    //shift in FF for 3 top half shift registers
    for(uint8_t i = 0; i < 64; i++)
    {
        clk->setHigh(); clk->setLow(); //possible delay
    }
    matrixGPIO.setLatch(true); //move shift register contents into parallel output register
    matrixGPIO.setEn(false); //turn on display
}
