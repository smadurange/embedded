#include <avr/io.h>
#include <util/delay.h>

static int x = 0b00100000;

int main(void)
{
	DDRB = x;
	for (;;) {
		PORTB = 0b00100000;
		_delay_ms(5000);
		PORTB = 0b00000000;
		_delay_ms(5000);
	}
	return 0;
}
