#include "spi.h"

//#define clockCyclesPerMicrosecond (F_CPU / 1000000L)

/**
 * @brief Initialize SPI bus
 *
 * @param mosi  		The number of the digital MOSI-pin
 * @param miso  		The number of the digital MISO-pin
 * @param clk   		The number of the digital SCLK-pin
 * @param cs    		The number of the digital CS-pin
 * @param mode  		The SPI mode (one of the constants SPI_MODEx)
 * @param order			The byte order
 * @param clock_speed	The clock frequency (default 1 MHz)
 *
 * @return The created SPI object for the access to the SPI bus
 */
SPI::SPI(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t mode,
		uint8_t order, size_t clock_speed) :
		mosi(mosi), miso(miso), clk(clk), cs(cs), order(order), speed(
				clock_speed), cpha(mode & 0x01), cpol((mode & 0x02) >> 1) {

	initPin(mosi);
	initPin(miso, INPUT);
	initPin(cs);
	initPin(clk, OUTPUT, cpol ? HIGH : LOW);
}

/**
 * @brief Create a new SPI object based on pins former SPI, with
 * new CS-pin
 *
 * @param spi	The SPI object
 * @param cs	The number of new CS-pin
 *
 * @return The new created SPI object
 */
SPI::SPI(const SPI& spi, uint8_t cs) :
		mosi(spi.mosi), miso(spi.miso), clk(spi.clk), cs(cs), order(spi.order), speed(
				spi.speed), cpha(spi.cpha), cpol(spi.cpol) {

	initPin(cs);
}

/**
 * @brief Initialize digital pin
 *
 * @param pin_number    The number of the pin
 * @param dir           Pin direction (INPUT/OUTPUT, default OUTPUT)
 * @param lvl           Initial level of the pin (LOW/HIGH, default HIGH)
 */
void SPI::initPin(uint8_t pin_number, uint8_t dir, uint8_t lvl) const {
	pinMode(pin_number, dir);
	if (dir == OUTPUT) {
		digitalWrite(pin_number, lvl);
	}
}

/**
 * Suspend execution for a given time
 *
 * Note: The real suspend time will be aliquot of the clock-time
 * of the board
 *
 * @param time	The suspend time in nanoseconds
 */
void SPI::sleep_ns(size_t time) const {
	double nop_time = 1000 / clockCyclesPerMicrosecond();// Time of the NOP-instruction in nanoseconds
	int nop_count = (time / nop_time) + 1;

	for (int i = 0; i < nop_count; i++) {
		__asm__("nop\n\t");
		//asm volatile("nop");
	}
}

/**
 * @brief Send the data to the SPI bus
 *
 * @param data  The data buffer
 * @param size  The size of the data buffer
 */
void SPI::write(const void* data, size_t size) {

	size_t ct = 10e8 / (speed * 4);

	digitalWrite(mosi, LOW);
	digitalWrite(clk, cpol);
	sleep_ns(ct);

	digitalWrite(cs, LOW);					// Start transmit (CS low)

	for (size_t i = 0; i < size; i++) {
		uint8_t b = ((uint8_t*) data)[i];

		for (int j = 0; j < 8; j++) {

			sleep_ns(ct);
			if (cpha == 0) {
				if (order == MSBFIRST) {
					digitalWrite(mosi, (b >> (7 - j)) & 0x01);
				} else {
					digitalWrite(mosi, (b >> j) & 0x01);
				}
			}

			sleep_ns(ct);
			digitalWrite(clk, !cpol);					// Clock front
			sleep_ns(ct);

			if (cpha == 1) {
				if (order == MSBFIRST) {
					digitalWrite(mosi, (b >> (7 - j)) & 0x01);
				} else {
					digitalWrite(mosi, (b >> j) & 0x01);
				}
			}

			sleep_ns(ct);
			digitalWrite(clk, cpol);					// Clock back
		}
	}

	sleep_ns(ct);
	digitalWrite(cs, HIGH);					// Stop transmit (CS high)
	sleep_ns(ct * 4);
}

/**
 * @brief Receive data from the SPI bus
 *
 * @param buffer	The buffer to store readed data
 * @param size		Number of bytes to read
 */
void SPI::read(void* buffer, size_t size) {
	size_t ct = 10e8 / (speed * 4);

	digitalWrite(mosi, LOW);
	digitalWrite(clk, cpol);
	sleep_ns(ct);

	digitalWrite(cs, LOW);					// Start transmit (CS low)

	for (size_t i = 0; i < size; i++) {

		((uint8_t*) buffer)[i] = 0;

		for (int j = 0; j < 8; j++) {

			sleep_ns(ct);
			digitalWrite(clk, !cpol);					// Clock front
			sleep_ns(ct);

			if (cpha == 0) {
				if (order == MSBFIRST) {
					((uint8_t*) buffer)[i] |= digitalRead(miso) << (7 - j);
				} else {
					((uint8_t*) buffer)[i] |= digitalRead(miso) << j;
				}
			}

			sleep_ns(ct);
			digitalWrite(clk, cpol);					// Clock back
			sleep_ns(ct);

			if (cpha == 1) {
				if (order == MSBFIRST) {
					((uint8_t*) buffer)[i] |= digitalRead(miso) << (7 - j);
				} else {
					((uint8_t*) buffer)[i] |= digitalRead(miso) << j;
				}
			}

			sleep_ns(ct);
		}
	}

	sleep_ns(ct);
	digitalWrite(cs, HIGH);					// Stop transmit (CS high)
	sleep_ns(ct * 4);
}
