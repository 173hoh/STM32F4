#include "gd32f4xx.h"
#include "systick.h"
#include "board.h"
#include "test.h"
#include "board_led.h"


#include "esp8266.h"
int main(void) {
    board_init();
		esp8266_init();
    test_init();

//	esp8266_peripheral_init();
//	esp8266_uart_send_byte('H');
	while (1) {
		
    delay_1ms(1000);
	}
}
