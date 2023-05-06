#include <avr/io.h>
#include <util/setbaud.h>

static void usart_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static void usart_transmit(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
}

static unsigned char usart_receive(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

static void usart_send_line(const char *s)
{
	for (; *s; s++)
		usart_transmit(*s);
	usart_transmit('\r');
	usart_transmit('\n');
}

int main(void)
{
	unsigned char c;

	usart_init();	
	usart_send_line("AT");
	usart_send_line("AT+CWMODE=1");
	usart_send_line("AT+RST");
	usart_send_line("AT+CWLAP");

	for (;;) {
	}

	return 0;
}
