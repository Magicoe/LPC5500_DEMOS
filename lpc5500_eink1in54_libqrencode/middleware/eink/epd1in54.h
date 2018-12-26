/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __EINK1IN54_H__
#define __EINK1IN54_H__


// Display resolution
#define EPD_WIDTH                                   200
#define EPD_HEIGHT                                  200

// Display orientation
#define ROTATE_0                                      0
#define ROTATE_90                                     1
#define ROTATE_180                                    2
#define ROTATE_270                                    3

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR                               1

// EPD1IN54 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

extern const uint8_t lut_full_update[];
extern const uint8_t lut_partial_update[];

typedef struct EINK_t {
    uint32_t       width;
    uint32_t       height;
    const uint8_t* lut;
    uint8_t*       image;
    uint8_t        rotate;
} EINK;

/* Hardware operating functions */
/* public functions */
extern uint8_t  eink_init(EINK* epd, const uint8_t* lut, uint8_t* image_buffer);
extern void     eink_reset(void);
extern void     eink_sleep(void);

extern void     eink_frame_set(EINK* epd, uint32_t x, uint32_t y);
extern void     eink_frame_clear(EINK* epd, uint8_t color);
extern void     eink_frame_display(EINK* epd);

extern void     eink_paint_clr(EINK* epd, uint8_t colored);

extern uint8_t* eink_get_image(EINK* epd);
extern void     eink_set_width(EINK* epd, uint32_t width);
extern uint32_t eink_get_height(EINK* epd);
extern void     eink_set_height(EINK* epd, uint32_t height);
extern uint8_t  eink_get_rotate(EINK* epd);
extern void     eink_set_rotate(EINK* epd, uint8_t rotate);

extern void     eink_disp_image(EINK* epd, uint8_t* data);

#endif /* __EINK1IN54_H__ */

