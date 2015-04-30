#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <bcm2835.h>
#include <unistd.h>
#include "dht22.h"

int data[100];

int readDHT(int pin, float* temperature, float* humidity) {
    if (!bcm2835_init())
        return 1;

    int counter = 0;
    int laststate = HIGH;
    int j=0;

    // Set GPIO pin to output
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(pin, HIGH);
    usleep(500000);  // 500 ms
    bcm2835_gpio_write(pin, LOW);
    usleep(20000);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    // wait for pin to drop?
    int cnt = 0;
    while (bcm2835_gpio_lev(pin) == 1) {
        cnt++;
        usleep(1);
        if (cnt == 1000)
        break;
    }

    // read data!
    for (int i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        while ( bcm2835_gpio_lev(pin) == laststate) {
            counter++;
            // nanosleep(1);        // overclocking might change this?
            if (counter == 1000)
            break;
        }
        laststate = bcm2835_gpio_lev(pin);
        if (counter == 1000) break;

        if ((i>3) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 200)
            data[j/8] |= 1;
            j++;
        }
    }

    bcm2835_close();

    #ifdef DEBUG
        printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x \n", j, data[0], data[1], data[2], data[3], data[4]);
    #endif

    if ((j >= 39) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
        float f, h;
        h = data[0] * 256 + data[1];
        h /= 10;

        f = (data[2] & 0x7F)* 256 + data[3];
        f /= 10.0;
        if (data[2] & 0x80)  f *= -1;
        #ifdef DEBUG
            printf("%.1f ; %.1f \n", f, h);
        #endif

        *temperature = f;
        *humidity = h;

        return 0;
    } else {
        #ifdef DEBUG
            printf("CRC error! \n");
            printf("Data (%d): 0x%x 0x%x 0x%x 0x%x 0x%x \n", j, data[0], data[1], data[2], data[3], data[4]);
        #endif

        return 1;
    }

    return 1;
}
