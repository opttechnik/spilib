# ~~SPI library~~ (in development)

Implementation of the SPI protocol on digital I/O pins for various hardware plattforms.

## Example

### RedPitaya
```
#include <spi.h>

#define SPI_BUF_SIZE		2

...
SPI spi = spi_init(9, 10, 11, 12, SPI_MODE1);
...
uint8_t databuf[SPI_BUF_SIZE];

spi_write(spi, databuf, SPI_BUF_SIZE);
```

### Arduino
```
#include <spi.h>

#define MISO_PIN	8
#define MOSI_PIN	9
#define SCLK_PIN	10
#define SYNC_PIN	11

#define SPI_FREQ	1000000		// 1 MHz clock frequency

SPI spi;

uint16_t some_data;

uint8_t some_buf[4];
...

void setup() {
	spi = spi_init(MOSI_PIN, MISO_PIN, SCLK_PIN, SYNC_PIN, MSBFIRST, SPI_FREQ);
}

void loop() {
	...
	spi_write(spi, &some_data, sizeof(some_data));
	...
}

```

### Teensy
```
#include <spi.h>
```