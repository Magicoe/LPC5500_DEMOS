/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_EINKBuffer[EPD_WIDTH * EPD_HEIGHT / 8];                    /* EINK display data buffer*/
EINK    g_EinkHandler;                                               /* EINK display controler handler */

QRcode *qrcode;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t x, y, xx, yy, i, j;
    uint8_t  PixelScaler = 0;
    
    gpio_pin_config_t output_config = { kGPIO_DigitalOutput, 1, };   /* Define the init structure for the output gpio pin*/
    gpio_pin_config_t input_config  = { kGPIO_DigitalInput,  1, };   /* Define the init structure for the input  gpio pin*/
    
    BOARD_BootClockFROHF96M();
    
    /* Initialize EINK related GPIO */
    CLOCK_EnableClock(kCLOCK_Iocon);                                 /* Enables the clock for the I/O controller.: Enable Clock. */
    IOCON_PinMuxSet(IOCON, EINK_BY_PORT, EINK_BY_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN2  is configured as PIO1_2  */
    IOCON_PinMuxSet(IOCON, EINK_RT_PORT, EINK_RT_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN2  is configured as PIO1_3  */
    IOCON_PinMuxSet(IOCON, EINK_DC_PORT, EINK_DC_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT0 PIN26 is configured as PIO0_26 */
    IOCON_PinMuxSet(IOCON, EINK_CS_PORT, EINK_CS_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN1  is configured as PIO1_1  */
    IOCON_PinMuxSet(IOCON, EINK_CK_PORT, EINK_CK_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN5  is configured as PIO1_5  */
    IOCON_PinMuxSet(IOCON, EINK_MO_PORT, EINK_MO_PIN, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN8  is configured as PIO1_8  */
    
    GPIO_PortInit(GPIO, 0);                                          /* Enable GPIO0 */
    GPIO_PortInit(GPIO, 1);                                          /* Enable GPIO1 */
    GPIO_PinInit(GPIO, EINK_BY_PORT, EINK_BY_PIN, &input_config);    /* Intialize GPIO as input  */
    GPIO_PinInit(GPIO, EINK_RT_PORT, EINK_RT_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_DC_PORT, EINK_DC_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_CS_PORT, EINK_CS_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_CK_PORT, EINK_CK_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_MO_PORT, EINK_MO_PIN, &output_config);   /* Intialize GPIO as output */
    
    /* Initialize EINK panel */
    if (eink_init(&g_EinkHandler, lut_full_update, g_EINKBuffer) != 0) {
        return -1;
    }
    
    /* Initialize EINK draw paint data buffer */
    memset((void *)g_EINKBuffer, 0xFF, sizeof(g_EINKBuffer));        /* Clear data buffer */

    /* QR Code Generate */
    qrcode= QRcode_encodeString("Hello 2019! Hello LPC55S69!", 2, QR_ECLEVEL_H, QR_MODE_8,0);
    
    /* Calculate QR code image scale size based on Panel Pixels */
    for(i=1; i<20; i++) {
        if( EPD_WIDTH < (qrcode->width * i) ) {                      /* Scaled image size is larger than Panel pixels */
            PixelScaler = i-1;
            i = 255;
        }
    }
    
    /* Generate a image and copy it into EINK frame Buffer */
    for(y=0; y<qrcode->width; y++) {
        for(x=0; x<qrcode->width; x++) {
            if(qrcode->data[y*qrcode->width+x]&0x01) {
                xx= (EPD_WIDTH  - (qrcode->width*PixelScaler))/2 + x*PixelScaler;
                yy= (EPD_HEIGHT - (qrcode->width*PixelScaler))/2 + y*PixelScaler;
                for(i=0; i<PixelScaler; i++) {
					for(j=0; j<PixelScaler; j++) {
                        g_EINKBuffer[(yy+j)*EPD_WIDTH/8 + (xx+i)/8] &= ~(0x80 >> ((xx+i)%8));
                    }
                }
            }
        }
    }
    /* free QRcode malloc */
    QRcode_free(qrcode);
    
    /* Display on EINK */
    eink_frame_set(&g_EinkHandler, 0, 0);
    eink_frame_display(&g_EinkHandler);                              /* Display on EINK */

    while (1)
    {
        __WFI();
    }
}
