# ~~SPI library~~ (in development)

Implementation of the SPI protocol on digital I/O pins for various hardware plattforms.

## Example

```
#include <spi.h>

#define SPI_BUF_SIZE		2

...
SPI spi = spi_init(9, 10, 11, 12, SPI_MODE1);
...
uint8_t databuf[SPI_BUF_SIZE];

spi_write(spi, databuf, SPI_BUF_SIZE);
```
