#include <xc.h>
#include "CB_TX1.h"

unsigned char UartCalculateChecksum(int msgFunction,
int msgPayloadLength, unsigned char* msgPayload)
{
unsigned char checksum = 0;
    checksum ^= (unsigned char )(0xFE);
    checksum ^= (unsigned char )(msgFunction);
    checksum ^= (unsigned char )(msgPayloadLength);

    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {

    unsigned char checksumSend = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

    unsigned char encodedMessage[msgPayloadLength + 6];
    int pos = 0;
    encodedMessage[pos++] = 0xFE;
    encodedMessage[pos++] = (unsigned char) (msgFunction >> 8);
    encodedMessage[pos++] = (unsigned char) (msgFunction >> 0);
    encodedMessage[pos++] = (unsigned char) (msgPayloadLength >> 8);
    encodedMessage[pos++] = (unsigned char) (msgPayloadLength >> 0);
    for (int i = 0; i < msgPayloadLength; i++) {
        encodedMessage[pos++] += msgPayload[i];
    }
    encodedMessage[pos++] = (unsigned char) (checksumSend);
    
    SendMessage(encodedMessage, pos);
}



//int msgDecodedFunction = 0;
//int msgDecodedPayloadLength = 0;
//unsigned char msgDecodedPayload[128];
//int msgDecodedPayloadIndex = 0;
//
//void UartDecodeMessage(unsigned char c) {
//    switch (rcvState) {
//        case StateReception.Waiting:
//            if (c == 0xFE) {
//                rcvState = StateReception.FunctionMSB;
//            }
//            break;
//
//        case StateReception.FunctionMSB:
//            msgDecodedFunction = c << 8;
//            rcvState = StateReception.FunctionLSB;
//            break;
//        case StateReception.FunctionLSB:
//            msgDecodedFunction += c;
//            rcvState = StateReception.PayloadLengthMSB;
//            break;
//        case StateReception.PayloadLengthMSB:
//            msgDecodedPayloadLength = c << 8;
//            rcvState = StateReception.PayloadLengthLSB;
//            break;
//        case StateReception.PayloadLengthLSB:
//            msgDecodedPayloadLength += c;
//            if (msgDecodedPayloadLength == 0)
//                rcvState = StateReception.CheckSum;
//            else if (msgDecodedPayloadLength >= 256)
//                rcvState = StateReception.Waiting;
//            else {
//                msgDecodedPayload = new byte[msgDecodedPayloadLength];
//                msgDecodedPayloadIndex = 0;
//                rcvState = StateReception.Payload;
//            }
//            break;
//        case StateReception.Payload:
//            msgDecodedPayload[msgDecodedPayloadIndex++] = c;
//            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
//                rcvState = StateReception.CheckSum;
//            break;
//        case StateReception.CheckSum:
//            calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
//
//            if (calculatedChecksum == c) {
//                Debug.WriteLine("Youpi");
//
//                ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
//            } else {
//                Debug.WriteLine("Bouh");
//            }
//            rcvState = StateReception.Waiting;
//
//            break;
//        default:
//            rcvState = StateReception.Waiting;
//            break;
//    }
//}
//
//void UartProcessDecodedMessage(int function,
//        int payloadLength, unsigned char* payload) {
//    switch (msgFunction) {
//        case 0x0080:
//            textBoxReception.Text = Encoding.Default.GetString(msgPayload);
//            break;
//
//        case 0x0020:
//            if (msgPayload[0] == 1) {
//                CheckboxLed1.IsChecked = true;
//            } else if (msgPayload[0] == 2)
//                CheckboxLed2.IsChecked = true;
//            else if (msgPayload[0] == 3)
//                CheckboxLed3.IsChecked = true;
//            break;
//
//        case 0x0030:
//            IRGauche.Text = "IRGauche :  " + msgPayload[0].ToString();
//            IRCentre.Text = "IRCentre :  " + msgPayload[1].ToString();
//            IRDroit.Text = "IRDroit :  " + msgPayload[2].ToString();
//            break;
//
//        case 0x0040:
//            VitesseD.Text = "VitesseD :  " + msgPayload[0].ToString();
//            VitesseG.Text = "VitesseG :  " + msgPayload[1].ToString();
//            break;
//    }
//}


//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/