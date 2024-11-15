#include "ws2811.h"
#include "main.h"
#include "string.h"
#include "stm32f0xx_hal_spi.h"

#define BITS (4)
#define BYTES_PER_LED (3)
#define BUFF_SIZE (NUMBER_LEDS*BYTES_PER_LED*WS2811_BUFF_LEN)
extern SPI_HandleTypeDef hspi1;

static uint8_t ws2811_bkup_buffer[BUFF_SIZE];
static uint8_t ws2811_buffer[BUFF_SIZE];

static volatile uint32_t spi_index = 0;

ws2811_ret_E ws2811_tx(led_code_S (*led_timeseries)[NUMBER_LEDS], uint32_t n) {
	for (uint32_t i = 0; i < n; i++) {
		for (uint32_t j = 0; j < NUMBER_LEDS; j++) {
			ws2811_bkup_buffer[i * NUMBER_LEDS*BYTES_PER_LED + j*BYTES_PER_LED + 0] = led_timeseries[i][j].red;
			ws2811_bkup_buffer[i * NUMBER_LEDS*BYTES_PER_LED + j*BYTES_PER_LED + 1] = led_timeseries[i][j].green;
			ws2811_bkup_buffer[i * NUMBER_LEDS*BYTES_PER_LED + j*BYTES_PER_LED + 2] = led_timeseries[i][j].blue;
		}
	}
	return WS2811_RET_OK;
}

static volatile uint8_t spi_d[BITS];
static volatile uint8_t output_buff[BYTES_PER_LED*NUMBER_LEDS*BITS+3];
ws2811_ret_E ws2811_trigger(void) {
	for(uint32_t i = 0;i < sizeof(output_buff);i++) {
		output_buff[i] = 0;
	}
	uint8_t* tmp = &ws2811_buffer[spi_index*BYTES_PER_LED*NUMBER_LEDS];
	uint32_t byte_i = 0;
	uint8_t bit_i = 7;
	for(uint32_t i = 0;i < BYTES_PER_LED*NUMBER_LEDS;i++) {
		uint8_t mask = 0x80;
		for(uint8_t j = 0; j < 8; j++) {
			uint8_t bit = mask & tmp[i];
			if (bit == 0) {
				spi_d[0] = 0;
				spi_d[1] = 0;
				spi_d[2] = 0;
				spi_d[3] = 1;
			} else {
				spi_d[0] = 0;
				spi_d[1] = 0;
				spi_d[2] = 1;
				spi_d[3] = 1;
			}
			for(uint32_t k = 0; k < BITS; k++) {
				output_buff[byte_i] = output_buff[byte_i] | (spi_d[k] << bit_i);
				if (bit_i == 0) {
					bit_i = 8;
					byte_i++;
				}
				bit_i--;
			}
			mask = mask >> 1;
		}
	}
	uint32_t last_i = sizeof(output_buff)-1;
	output_buff[last_i-2] = 0x00;
	output_buff[last_i-1] = 0x00;
	output_buff[last_i] = 0x00;
	HAL_SPI_Transmit_IT(&hspi1, output_buff, BYTES_PER_LED*NUMBER_LEDS*BITS+3);

	spi_index++;
	if (spi_index == WS2811_BUFF_LEN) {
		spi_index = 0;
		memcpy(ws2811_buffer, ws2811_bkup_buffer, BUFF_SIZE);
	}
	return WS2811_RET_OK;	
}
