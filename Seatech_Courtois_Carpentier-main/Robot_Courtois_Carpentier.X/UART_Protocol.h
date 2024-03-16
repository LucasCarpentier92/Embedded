#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <stdint.h>

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartDecodeMessage(unsigned char c);

void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);

#endif /* UART_PROTOCOL_H */
