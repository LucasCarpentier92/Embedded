#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_RX1.h"
#include "UART.h"


#define CBRX1_BUFFER_SIZE 128
int cbRx1Head;
int cbRx1Tail;
unsigned char cbRx1Buffer[CBRX1_BUFFER_SIZE];

void CB_RX1_Add(unsigned char value) {
    if (CB_RX1_GetRemainingSize() > 0) {
        cbRx1Buffer[cbRx1Head] = value;
        cbRx1Head++;
        if (cbRx1Head >= CBRX1_BUFFER_SIZE)
            cbRx1Head = 0;
    }
}

unsigned char CB_RX1_Get(void) {
    unsigned char value = cbRx1Buffer[cbRx1Head];
    cbRx1Head++;
    if (cbRx1Head >= CBRX1_BUFFER_SIZE)
        cbRx1Head = 0;
    return value;
}

unsigned char CB_RX1_IsDataAvailable(void) {
    if (cbRx1Head != cbRx1Tail)
        return 1;
    else
        return 0;
}



int CB_RX1_GetDataSize(void) {
    //return size of data stored in circular buffer
    int dataSize;
    if (cbRx1Head >= cbRx1Tail)
        return dataSize = cbRx1Head - cbRx1Tail;
    else
        return dataSize = CBRX1_BUFFER_SIZE - (cbRx1Tail - cbRx1Head);
}

int CB_RX1_GetRemainingSize(void) {
    //return size of remaining size in circular buffer
    return CBRX1_BUFFER_SIZE - CB_RX1_GetDataSize();

}