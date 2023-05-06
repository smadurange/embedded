#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define TCNT0_VALUE 47
#define TCCR0B_VALUE 1 << CS01

#define TXD PD7
#define TXDDR DDRD
#define TXPORT PORTD

#define STOP_BIT 10

static volatile int tx = 0;
static volatile uint8_t bit = 0;
static volatile uint16_t data = 0;

static void swuart_init()
{
	TCNT0 = TCNT0_VALUE;
	TCCR0A = 0;
	TCCR0B = TCCR0B_VALUE;
	TIMSK0 = 1 << TOIE0;

	TXDDR |= (1 << TXD);
	TXPORT |= (1 << TXD);
}

static void swuart_send_byte(uint8_t byte)
{
	data = (1 << STOP_BIT) | ((uint16_t) byte << 1);
	tx = 1;
	while (tx)
		;
}

static void swuart_send_line(const char *s)
{
	for (; *s; s++)
		swuart_send_byte(*s);
	swuart_send_byte('\r');
	swuart_send_byte('\n');
}

int main(void)
{
	swuart_init();
	sei();
	
	swuart_send_line("Jabberwocky by Lewis Carroll");
	swuart_send_line("");

	swuart_send_line("'Twas brillig, and the slithy toves");
	swuart_send_line("   Did gyre and gimble in the wabe:");
	swuart_send_line("All mimsy were the borogoves,");
	swuart_send_line("   And the mome raths outgrabe.");

	swuart_send_line("");

	swuart_send_line("\"Beware the Jabberwock, my son!");
	swuart_send_line("   The jaws that bite, the claws that catch!");
	swuart_send_line("Beware the Jubjub bird, and shun");
	swuart_send_line("   The frumious Bandersnatch!\"");

	swuart_send_line("");

	swuart_send_line("He took his vorpal sword in hand;");
	swuart_send_line("   Long time the manxome foe he sought-");
	swuart_send_line("So rested he by the Tumtum tree");
	swuart_send_line("   And stood awhile in thought.");

	swuart_send_line("");

	swuart_send_line("And, as in uffish thought he stood,");
	swuart_send_line("   The Jabberwock, with eyes of flame,");
	swuart_send_line("Came whiffling through the tulgey wood,");
	swuart_send_line("   And burbled as it came!");

	swuart_send_line("");

	swuart_send_line("One, two! One, two! And through and through");
	swuart_send_line("   The vorpal blade went snicker-snack!");
	swuart_send_line("He left it dead, and with its head");
	swuart_send_line("   He went galumphing back.");

	swuart_send_line("");

	swuart_send_line("\"And hast thou slain the Jabberwock?");
	swuart_send_line("   Come to my arms, my beamish boy!");
	swuart_send_line("O frabjous day! Callooh! Callay!\"");
	swuart_send_line("   He chortled in his joy.");

	swuart_send_line("");

	swuart_send_line("'Twas brillig, and the slithy toves");
	swuart_send_line("   Did gyre and gimble in the wabe:");
	swuart_send_line("All mimsy were the borogoves,");
	swuart_send_line("   And the mome raths outgrabe.");

	swuart_send_line("");

	return 0;
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TCNT0_VALUE;

	if (!tx)
		return;

	TXPORT = (data >> bit++) & 1 ? TXPORT | (1 << TXD) : TXPORT & ~(1 << TXD);
	if (bit > STOP_BIT) {
		tx = 0;
		bit = 0;
	}
}
