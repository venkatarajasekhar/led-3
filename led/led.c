/******************************************************************************

  Filename: led.c

    Simple test program for peripheral I/O sequentially blinks the LEDs

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <s3c2410-regs.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

int running = 1;

void done (int sig)
/*
    Signal handler to stop the program gracefully
*/
{
    running = 0;
}

int main(int argc, void *argv[])
{
    unsigned long led;
    int fd;
    S3C2410P_GPIO GPIOp;
/*
    Initialization
*/
    signal (SIGINT, done);  // set up signal handler
    fd = open ("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) FATAL;

    GPIOp = mmap (0, sizeof (S3C2410_GPIO), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x56000000);
    if (GPIOp == (void *) -1) FATAL;
    close (fd);

// Set up the LED bits
    write_reg (&GPIOp->GPBCON, 0x3FC00, GP_BIT(8, POUT) | GP_BIT(7, POUT) | GP_BIT(6, POUT) | GP_BIT(5, POUT));
    set_reg (&GPIOp->GPBDAT, 0x1E0);     // Set initially to "off"

    printf ("\nThe Embedded Linux Learning Kit from Intellimetrix\n");
/*
    Blink loop
*/
    while (running)
    {
        led = 0x20;
        do
        {
            clear_reg (&GPIOp->GPBDAT, led);
            sleep (1);
            set_reg (&GPIOp->GPBDAT, led);
            led <<= 1;
        }
        while (led < 0x200);
    }
    if (munmap (GPIOp, sizeof (S3C2410_GPIO)) == -1) FATAL;
    return 0;
}
