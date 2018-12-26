/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_gpio.h"

#include "clock_config.h"

/* EINK drivers include file */
#include "epd1in54.h"

/* QR Encode Library Include files */
#include "qrencode.h"
#include "qrspec.h"
#include "mqrspec.h"
#include "bitstream.h"
#include "qrinput.h"
#include "rsecc.h"
#include "split.h"
#include "mask.h"
#include "mmask.h"

/* QR Code Display Pixel prescaler Setting */
#define OUT_FILE_PIXEL_PRESCALER                           2 

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define COLORED                                            0
#define UNCOLORED                                          1

#define SPI_USE_GPIO                                       1

/* EINK GPIOs */
#define EINK_BY_PORT                                       1     
#define EINK_BY_PIN                                        8

#define EINK_RT_PORT                                       1
#define EINK_RT_PIN                                        5

#define EINK_DC_PORT                                       1
#define EINK_DC_PIN                                        1

#define EINK_CS_PORT                                       0
#define EINK_CS_PIN                                       26

#define EINK_CK_PORT                                       1
#define EINK_CK_PIN                                        3

#define EINK_MO_PORT                                       1
#define EINK_MO_PIN                                        2


#endif /* __MAIN_H__ */

