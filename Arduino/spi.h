#ifndef SPI_H
#define SPI_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// SPI mode
#define     SPI_MODE1       0x00        // CPOL = 0, CPHA = 0
#define     SPI_MODE2       0x01        // CPOL = 0, CPHA = 1
#define     SPI_MODE3       0x02        // CPOL = 1, CPHA = 0
#define     SPI_MODE4       0x03        // CPOL = 1, CPHA = 1

/**
 * SPI class
 */
class SPI {
public:
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
	SPI(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t mode,
			uint8_t order = MSBFIRST, size_t clock_speed = 1000000UL);

	/**
	 * @brief Create a new SPI object based on pins former SPI, with
	 * new CS-pin
	 *
	 * @param spi	The SPI object
	 * @param cs	The number of new CS-pin

	 * @return The new created SPI object
	 */
	SPI(const SPI& spi, uint8_t cs);

	/**
	 * @brief Send the data to the SPI bus
	 *
	 * @param spi   The SPI object
	 * @param data  The data buffer
	 * @param size  The size of the data buffer
	 *
	 * @return
	 */
	void write(const void* data, size_t size);

	/**
	 * @brief Receive data from the SPI bus
	 *
	 * @param buffer	The buffer to store readed data
	 * @param size		Number of bytes to read
	 */
	void read(void* buffer, size_t size);

private:
	/**
	 * @brief SPI master output pin (MOSI/SOMI/SDO/DO/DON/SO/MRSR) number
	 */
	uint8_t mosi;

	/**
	 * @brief SPI master input pin (MISO/SIMO/SDI/DI/DIN/SI/MTST) number
	 */
	uint8_t miso;

	/**
	 * @brief SPI clock pin (SCLK/CLK/SCK) number
	 */
	uint8_t clk;

	/**
	 * @brief SPI chip select pin (SS/CS/SYNC/STE/CSN/CSB) number
	 */
	uint8_t cs;

	/**
	 * @brief The clock frequency
	 */
	size_t speed;

	/**
	 * @brief Clock phase (CPHA)
	 */
	uint8_t cpha :1;

	/**
	 * @brief Clock polarity (CPOL)
	 */
	uint8_t cpol :1;

	/**
	 * @brief Byte order
	 */
	uint8_t order :1;

	/**
	 * @brief Initialize digital pin
	 *
	 * @param pin_number    The number of the pin
	 * @param dir           Pin direction (INPUT/OUTPUT, default OUTPUT)
	 * @param lvl           Initial level of the pin (LOW/HIGH, default HIGH)
	 */
	void initPin(uint8_t pin_number, uint8_t dir = OUTPUT,
			uint8_t lvl = HIGH) const;

	/**
	 * Suspend execution for a given time
	 *
	 * Note: The real suspend time will be aliquot of the clock-time
	 * of the board
	 *
	 * @param time	The suspend time in nanoseconds
	 */
	void sleep_ns(size_t time) const;
};

#endif // SPI_H
