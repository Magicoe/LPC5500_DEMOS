/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"
#include "stdint.h"
#include "epd1in54.h"

/**
 *  @brief: Fully Update EINK content command sequence
 */
const uint8_t lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

/**
 *  @brief: Partial Update EINK content command sequence
 */
const uint8_t lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 *  @brief: Send Data by SPI format function
 */
void eink_spisend(unsigned char data)
{
    uint32_t i, j;
    GPIO_PinWrite(GPIO, EINK_CK_PORT, EINK_CK_PIN, 0);               /* Set CLK signal as default status */
    GPIO_PinWrite(GPIO, EINK_CS_PORT, EINK_CS_PIN, 0);               /* Set CS  signal as low to select the eink panel */
    for(i=0; i<8; i++) {
        if(data&0x80) {
            GPIO_PinWrite(GPIO, EINK_MO_PORT, EINK_MO_PIN, 1); 
        }
        else {
            GPIO_PinWrite(GPIO, EINK_MO_PORT, EINK_MO_PIN, 0); 
        }
        for(j=0; j<100; j++);
        GPIO_PinWrite(GPIO, EINK_CK_PORT, EINK_CK_PIN, 1);           /* Set CLK signal as high to transfer data bit on spi*/
        for(j=0; j<100; j++);
        GPIO_PinWrite(GPIO, EINK_CK_PORT, EINK_CK_PIN, 0);           /* Set CLK signal as low  to finished data bit transfer*/          
        data = data<<1;
    }
    
    GPIO_PinWrite(GPIO, EINK_CS_PORT, EINK_CS_PIN, 1);               /* Set CS  signal as high to release the pin select eink panel */
}

/**
 *  @brief: Delay
 */
void eink_delayms(uint32_t delaytime)
{  // 1ms
    uint32_t i, j;
    for(i=0; i<delaytime; i++)
    for(j=0; j<10000; j++);
}

/**
 *  @brief: Send Command to EINK pannel
 */
void eink_sendcmd(uint8_t command)
{
    GPIO_PinWrite(GPIO, EINK_DC_PORT, EINK_DC_PIN, 0);    /* Set DC signal as low to command mode */
    eink_spisend(command);                                /* Send Data */
}

/**
 *  @brief: Send Data to EINK pannel
 */
void eink_senddat(uint8_t data)
{
    GPIO_PinWrite(GPIO, EINK_DC_PORT, EINK_DC_PIN, 1);    /* Set DC signal as high to data mode */
    eink_spisend(data);                                   /* Send Data */
}

/**
 *  @brief: Wait EINK to IDLE status
 */
void eink_idlewait(void)
{
    while(GPIO_PinRead(GPIO, EINK_BY_PORT, EINK_BY_PIN) == 1)        /* Check BUSY signal pin is hold */
    {
        eink_delayms(100);                                           /* Delay for a while */
    }      
}

/**
 *  @brief: Reset EINK Panel
 */
void eink_reset(void)
{
    GPIO_PinWrite(GPIO, EINK_RT_PORT, EINK_RT_PIN, 0);          /* Set Reset as low to reset eink panel */ 
    eink_delayms(200);
    GPIO_PinWrite(GPIO, EINK_RT_PORT, EINK_RT_PIN, 1);          /* Set Reset as high, release reset status */ 
    eink_delayms(200);    
}

/**
 *  @brief: set the look-up tables
 */
static void eink_setlut(EINK* epd, const uint8_t* lut)
{
    epd->lut = lut;
    eink_sendcmd(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < 30; i++) {
        eink_senddat(epd->lut[i]);
    } 
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
static void eink_setMemArea(EINK* epd, int x_start, int y_start, int x_end, int y_end)
{
    eink_sendcmd(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    eink_senddat((x_start >> 3) & 0xFF);
    eink_senddat((x_end >> 3) & 0xFF);
    eink_sendcmd(SET_RAM_Y_ADDRESS_START_END_POSITION);
    eink_senddat(y_start & 0xFF);
    eink_senddat((y_start >> 8) & 0xFF);
    eink_senddat(y_end & 0xFF);
    eink_senddat((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
static void eink_setMemPointer(EINK* epd, int x, int y)
{
    eink_sendcmd(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    eink_senddat((x >> 3) & 0xFF);
    eink_sendcmd(SET_RAM_Y_ADDRESS_COUNTER);
    eink_senddat(y & 0xFF);
    eink_senddat((y >> 8) & 0xFF);
    eink_idlewait();
}


/**
 *  @brief: Initialize EINK pannel.
 */
uint8_t eink_init(EINK* epd, const uint8_t* lut, uint8_t* image_buffer)
{  
    epd->width  = EPD_WIDTH;
    epd->height = EPD_HEIGHT;
    epd->lut    = lut;
    epd->image  = image_buffer;
    epd->rotate = ROTATE_0;                                          /* Set Defualt Rotate is 0 */
    /* EPD hardware init start */
    eink_reset();

    eink_sendcmd(DRIVER_OUTPUT_CONTROL);
    eink_senddat((EPD_HEIGHT - 1) & 0xFF);
    eink_senddat(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    eink_senddat(0x00);                     // GD = 0; SM = 0; TB = 0;
    eink_sendcmd(BOOSTER_SOFT_START_CONTROL);
    eink_senddat(0xD7);
    eink_senddat(0xD6);
    eink_senddat(0x9D);
    eink_sendcmd(WRITE_VCOM_REGISTER);
    eink_senddat(0xA8);                     // VCOM 7C
    eink_sendcmd(SET_DUMMY_LINE_PERIOD);
    eink_senddat(0x1A);                     // 4 dummy lines per gate
    eink_sendcmd(SET_GATE_TIME);
    eink_senddat(0x08);                     // 2us per line
    eink_sendcmd(DATA_ENTRY_MODE_SETTING);
    eink_senddat(0x03);                     // X increment; Y increment
    eink_setlut(epd, epd->lut);
    /* EPD hardware init end */

    return 0;
}


/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void eink_frame_set(EINK* epd, uint32_t x, uint32_t y)
{
    uint32_t i, j;
    uint32_t x_end;
    uint32_t y_end;

    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    epd->width &= 0xF8;
    if ( (x + epd->width) >= epd->width) {
        x_end = epd->width - 1;
    } else {
        x_end = x + epd->width - 1;
    }
    if ( (y + epd->height) >= epd->height) {
        y_end = epd->height - 1;
    } else {
        y_end = y + epd->height - 1;
    }
    eink_setMemArea(epd, x, y, x_end, y_end);
    eink_setMemPointer(epd, x, y);
    eink_sendcmd(WRITE_RAM);
    /* send the image data */
    for (j = 0; j < y_end - y + 1; j++) {
        for (i = 0; i < (x_end - x + 1) / 8; i++) {
            eink_senddat(epd->image[i + j * (epd->width / 8)]);
        }
    }
}

/**
*  @brief: clear the frame memory with the specified color.
*          this won't update the display.
*/
void eink_frame_clear(EINK* epd, uint8_t color)
{
    eink_setMemArea(epd, 0, 0, epd->width - 1, epd->height - 1);
    eink_setMemPointer(epd, 0, 0);
    eink_sendcmd(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < epd->width / 8 * epd->height; i++) {
        eink_senddat(color);
    }
}

/**
*  @brief: update the display
*          there are 2 memory areas embedded in the e-paper display
*          but once this function is called,
*          the the next action of SetFrameMemory or ClearFrame will 
*          set the other memory area.
*/
void eink_frame_display(EINK* epd)
{
    eink_sendcmd(DISPLAY_UPDATE_CONTROL_2);
    eink_senddat(0xC4);
    eink_sendcmd(MASTER_ACTIVATION);
    eink_sendcmd(TERMINATE_FRAME_READ_WRITE);
    eink_idlewait();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          You can use eink_init() to awaken
 */
void eink_sleep(void)
{
    eink_sendcmd(DEEP_SLEEP_MODE);
    eink_idlewait();
}


/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void eink_draw_abspixel(EINK* epd, uint32_t x, uint32_t y, uint8_t colored)
{
    if ( x >= epd->width || y >= epd->height ) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (colored) {
            epd->image[(x + y * epd->width) / 8] |= 0x80 >> (x % 8);
        } else {
            epd->image[(x + y * epd->width) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            epd->image[(x + y * epd->width) / 8] &= ~(0x80 >> (x % 8));
        } else {
            epd->image[(x + y * epd->width) / 8] |= 0x80 >> (x % 8);
        }
    }
}
/**
 *  @brief: clear the image
 */
void eink_paint_clr(EINK* epd, uint8_t colored)
{
    uint32_t x, y;
    for ( x = 0; x < epd->width; x++ ) {
        for ( y = 0; y < epd->height; y++ ) {
            eink_draw_abspixel(epd, x, y, colored);
        }
    }
}

/**
 *  @brief: Getters and Setters
 */
uint8_t* eink_get_image(EINK* epd)
{
    return epd->image;
}

uint32_t eink_get_width(EINK* epd)
{
    return epd->width;
}

void eink_set_width(EINK* epd, uint32_t width)
{
    epd->width = width;
}

uint32_t eink_get_height(EINK* epd)
{
    return epd->height;
}

void eink_set_height(EINK* epd, uint32_t height)
{
    epd->height = height;
}

uint8_t eink_get_rotate(EINK* epd)
{
    return epd->rotate;
}

void eink_set_rotate(EINK* epd, uint8_t rotate)
{
    epd->rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void eink_draw_pixel(EINK* epd, uint32_t x, uint32_t y, uint8_t colored)
{
    uint32_t point_temp;
    if (epd->rotate == ROTATE_0) {
        if( x >= epd->width || y >= epd->height ) {
            return;
        }
        eink_draw_abspixel(epd, x, y, colored);
    } else if (epd->rotate == ROTATE_90) {
        if( x >= epd->height || y >= epd->width ) {
          return;
        }
        point_temp = x;
        x = epd->width - y;
        y = point_temp;
        eink_draw_abspixel(epd, x, y, colored);
    } else if (epd->rotate == ROTATE_180) {
        if( x >= epd->width || y >= epd->height) {
          return;
        }
        x = epd->width - x;
        y = epd->height - y;
        eink_draw_abspixel(epd, x, y, colored);
    } else if (epd->rotate == ROTATE_270) {
        if( x >= epd->height || y >= epd->width) {
          return;
        }
        point_temp = x;
        x = y;
        y = epd->height - point_temp;
        eink_draw_abspixel(epd, x, y, colored);
    }
}

/**
 *  @brief: Display monochrome bitmap
 */
void eink_disp_image(EINK* epd, uint8_t* data)
{
    uint32_t x, y;
    uint32_t addr = 0;

    for (y = 0; y < epd->height; y++) {
        for (x = 0; x < (epd->width/8); x++) {//8 pixel =  1 byte
            addr = x + y * (epd->width/8);
            epd->image[addr] = data[addr];
        }
    }
}


/* END OF FILE */

