#ifndef SPI_H
#define SPI_H

// SPI modes
#define     SPI_MODE1       0x00        // CPOL = 0, CPHA = 0
#define     SPI_MODE2       0x01        // CPOL = 0, CPHA = 1
#define     SPI_MODE3       0x02        // CPOL = 1, CPHA = 0
#define     SPI_MODE4       0x03        // CPOL = 1, CPHA = 1

// SPI settings struct
typedef struct {
    /**
     * @brief SPI master output pin (MOSI/SOMI/SDO/DO/DON/SO/MRSR) number
     */
    uint8_t     mosi;

    /**
     * @brief SPI master input pin (MISO/SIMO/SDI/DI/DIN/SI/MTST) number
     */
    uint8_t     miso;

    /**
     * @brief SPI clock pin (SCLK/CLK/SCK) number
     */
    uint8_t     clk;

    /**
     * @brief SPI chip select pin (SS/CS/SYNC/STE/CSN/CSB) number
     */
    uint8_t     cs;
	
	/**
	 * @brief The clock frequency
	 */
	size_t		speed;

    /**
     * @brief Phase parameter (CPHA)
     */
    uint8_t     cpha : 1;

    /**
     * @brief polarity parameter (CPOL)
     */
    uint8_t     cpol : 1;
} spi_t;

typedef SPI *spi_t;

/**
 * @brief Initialize SPI bus and create related SPI object for access
 * to the bus
 *
 * @param mosi  		The number of the digital MOSI-pin
 * @param miso  		The number of the digital MISO-pin
 * @param clk   		The number of the digital SCLK-pin
 * @param cs    		The number of the digital CS-pin
 * @param mode  		The SPI mode (one of the constants SPI_MODEx)
 * @param clock_speed	The clock frequency (default 1 MHz)
 *
 * @return The created SPI object for the access to the SPI bus
 */
SPI spi_init(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t mode, size_t clock_speed = 1000000);

/**
 * @brief Create new SPI object based on pins older SPI, with
 * new CS-pin
 */
SPI spi_init(SPI spi, uint8_t cs);

/**
 * @brief Send the data to the SPI bus
 *
 * @param spi   The SPI object
 * @param data  The data buffer
 * @param size  The size of the data buffer
 *
 * @return
 */
void spi_write(SPI spi, void* data, size_t size);

void spi_read();

#endif // SPI_H
