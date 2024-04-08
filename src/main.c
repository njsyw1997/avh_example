#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>

#include "uart.h"

extern void UartStdOutInit(void);

void main()
{
    UartStdOutInit();
    printf("Hello World\n");
    // UartPutc('a');
    // fputc('b', stdout);
}