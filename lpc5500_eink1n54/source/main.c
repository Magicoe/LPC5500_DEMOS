/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"
#include "imagedata.h"

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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    gpio_pin_config_t output_config = { kGPIO_DigitalOutput, 1, };   /* Define the init structure for the output gpio pin*/
    gpio_pin_config_t input_config  = { kGPIO_DigitalInput,  1, };   /* Define the init structure for the input  gpio pin*/
    
    /* Step1 : Initialize EINK related GPIO */
    CLOCK_EnableClock(kCLOCK_Iocon);                                 /* Enables the clock for the I/O controller.: Enable Clock. */
    IOCON_PinMuxSet(IOCON, 1U,  2U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN2  is configured as PIO1_2  */
    IOCON_PinMuxSet(IOCON, 1U,  3U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN2  is configured as PIO1_3  */
    IOCON_PinMuxSet(IOCON, 0U, 26U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT0 PIN26 is configured as PIO0_26 */
    IOCON_PinMuxSet(IOCON, 1U,  1U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN1  is configured as PIO1_1  */
    IOCON_PinMuxSet(IOCON, 1U,  5U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN5  is configured as PIO1_5  */
    IOCON_PinMuxSet(IOCON, 1U,  8U, IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN);  /* PORT1 PIN8  is configured as PIO1_8  */
    
    GPIO_PortInit(GPIO, 0);                                          /* Enable GPIO0 */
    GPIO_PortInit(GPIO, 1);                                          /* Enable GPIO1 */
    GPIO_PinInit(GPIO, EINK_BY_PORT, EINK_BY_PIN, &input_config);    /* Intialize GPIO as input  */
    GPIO_PinInit(GPIO, EINK_RT_PORT, EINK_RT_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_DC_PORT, EINK_DC_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_CS_PORT, EINK_CS_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_CK_PORT, EINK_CK_PIN, &output_config);   /* Intialize GPIO as output */
    GPIO_PinInit(GPIO, EINK_MO_PORT, EINK_MO_PIN, &output_config);   /* Intialize GPIO as output */
    
    /* Step2 : Initialize EINK panel */
    if (eink_init(&g_EinkHandler, lut_full_update, g_EINKBuffer) != 0) {
        return -1;
    }
    
    /* Step3 : Initialize EINK draw paint data buffer */
    memset((void *)g_EINKBuffer, 0xFF, sizeof(g_EINKBuffer));        /* Clear data buffer */
    eink_disp_image(&g_EinkHandler, (uint8_t *)gImage_image);
    
    /* Step4 : Display on EINK */
    /* Set Display paint buffer with EINK panel */
    eink_frame_set(&g_EinkHandler, 0, 0);
    eink_frame_display(&g_EinkHandler);                              /* Display on EINK */

    while (1)
    {
        __WFI();
    }
}
