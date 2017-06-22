#include <time.h>
#include "spi.h"

#define MAX_NANO_TIME       999999999

/**
 * @brief Initialize pin on RedPitaya-board
 *
 * @param pin_number    The number of the pin
 * @param dir           Pin direction (in/out)
 * @param lvl           Initial level of the pin (low/high)
 */
void init_pin(uint8_t pin_number, RP dir = RP_OUT, RP lvl = RP_HIGH) {
    rp_DpinSetDirection(pin_number, dir);
    if (dir == RP_OUT) {
        rp_DpinSetState(pin_number, lvl);
    }
}

/**
 * @brief Suspend execution for a given nanoseconds
 *
 * @param time  The suspend time in nanoseconds
 *
 * @return 0 on successfully sleeping, otherwise one from constants:
 *          EFAULT  Problem with copying information from user space
 *          EINTR   The pause has been interrupted by a signal
 *          EINVAL  Invalid time value
 */
int sleep_ns(size_t time) {
    struct timespec t, t2;

    if (time > MAX_NANO_TIME) {
        t.tv_sec = time / MAX_NANO_TIME;
        time %= MAX_NANO_TIME;
    } else {
        t.tv_sec = 0;
    }

    t.tv_nsec = time;

    return nanosleep(&t, &t2);
}

/**
 * @brief Suspend execution for a given microseconds
 *
 * @param time The suspend time in microseconds
 *
 * @return 0 on successfully sleeping, otherwise one from constants:
 *			EINTR	Interrupted by a signal
 *			EINVAL	Not correct time value (>1000000)
 */
int sleep_ms(size_t time) {
	return usleep(time);
}

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
SPI spi_init(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t mode, size_t clock_speed = 1000000); {
    SPI spi = new SPI;

    spi->mosi = mosi;
    spi->miso = miso;
    spi->clk = clk;
    spi->cs = cs;
    spi->cpha = mode & 0x01;
    spi->cpol = (mode & 0x02) >> 1;
	spi->speed = clock_speed;

    init_pin(spi->mosi);
    init_pin(spi->miso, RP_IN);
    init_pin(spi->clk, spi->cpol ? RP_HIGH : RP_LOW);
    init_pin(spi->cs);

    return spi;
}

/**
 * @brief Create new SPI object based on pins older SPI, with
 * new CS-pin
 */
SPI spi_init(SPI spi, uint8_t cs) {
	SPI spi_r = new SPI;
	
	spi_r->mosi = spi->mosi;
	spi_r->miso = spi->miso;
	spi_r->clk = spi->clk;
	spi_r->cs = cs;
	spi_r->cpha = cpha;
	spi_r->cpol = cpol;
	spi_r->speed = spi->speed;
	
	init_pin(spi_r->cs);
	
	return spi;
}

/**
 * @brief Send the data to the SPI bus
 *
 * @param spi   The SPI object
 * @param data  The data buffer
 * @param size  The size of the data buffer
 *
 * @return
 */
void spi_write(SPI spi, void* data, size_t size) {
    rp_DpinSetState(spi->cs, RP_LOW);

    // ...

    rp_DpinSetState(spi->cs, RP_HIGH);
}
