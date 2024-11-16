#ifndef WS2811_H
#define WS2811_H

#include "stdint.h"

#define NUMBER_LEDS (36)
#define WS2811_BUFF_LEN (2)

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} led_code_S;

typedef enum {
	WS2811_RET_OK = 0,
	WS2811_RET_FAILED = 1
} ws2811_ret_E;

ws2811_ret_E ws2811_tx(led_code_S (*led_timeseries)[NUMBER_LEDS], uint32_t n);
ws2811_ret_E ws2811_parse_buffer(uint8_t* led_data, uint8_t led_index, uint8_t num_leds, uint32_t t);
ws2811_ret_E ws2811_trigger(void);
#endif
