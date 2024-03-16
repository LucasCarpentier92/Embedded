#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <time.h>
#include "ChipConfig.h"
#include  "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "adc.h"
#include "main.h"
#include "UART.h"
#include "libpic30.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "UART_Protocol.h"

unsigned char stateRobot;
int number;

int getRandom() {
    srand(time(NULL));
    number = rand() % 3;
    return number;
}

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 5000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(18, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-18, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_BACKWARD:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_BACKWARD_EN_COURS;

        case STATE_BACKWARD_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}



unsigned char nextStateRobot = 0;
unsigned int sensorState = 0b00000;

unsigned int getSensorState() {
    sensorState = 0;
    if (robotState.distanceTelemetreExGauche < 23)
        sensorState |= 0b10000;
    if (robotState.distanceTelemetreGauche < 26)
        sensorState |= 0b01000;
    if (robotState.distanceTelemetreCentre < 38)
        sensorState |= 0b00100;
    if (robotState.distanceTelemetreDroit < 26)
        sensorState |= 0b00010;
    if (robotState.distanceTelemetreExDroit < 23)
        sensorState |= 0b00001;

    return sensorState;
}

int num;

void SetNextRobotStateInAutomaticMode() {
    sensorState = getSensorState();

    switch (sensorState) {
        case 0b11110:
        case 0b11101:
        case 0b11010:
        case 0b11001:
        case 0b10110:
        case 0b10010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;

        case 0b11100:
        case 0b11000:
        case 0b10100:
        case 0b01100:
        case 0b01000:
        case 0b00100:
        case 0b10000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;

        case 0b10001:
        case 0b00000:
            nextStateRobot = STATE_AVANCE;
            break;

        case 0b00111:
        case 0b00110:
        case 0b00101:
        case 0b00011:
        case 0b00010:
        case 0b00001:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;

        case 0b10111:
        case 0b10011:
        case 0b01111:
        case 0b01101:
        case 0b01011:
        case 0b01001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;

        case 0b11111:
        case 0b11011:
        case 0b10101:
        case 0b01110:
        case 0b01010:
            num = getRandom();
            if (num == 0) {
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            } else if (num == 1) {
                nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            } else {
                nextStateRobot = STATE_BACKWARD;
            }
            break;
    }

    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }
}

int ADCValue0;
int ADCValue1;
int ADCValue2;
int ADCValue3;
int ADCValue4;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();
    InitUART();

    /****************************************************************************************************/
    // Configuration des entr?es sorties
    /****************************************************************************************************/
    InitIO();

    //initialisation du timer23
    InitTimer23();
    //initialisation du timer1
    InitTimer1();
    //initialisation du timer4
    InitTimer4();
    // initialisation des moteur
    InitPWM();
    // initialiser ADC
    InitADC1();

    

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExDroit = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExGauche = 34 / volts - 5;
               
            int function = 0x0030;
            unsigned char payload = ADCGetResult();
            int payloadLength = sizeof(ADCGetResult());
            UartEncodeAndSendMessage(function, payload, payloadLength);
            __delay32(40000000);

            if (robotState.distanceTelemetreExDroit < 15 || robotState.distanceTelemetreDroit < 15)
                LED_ORANGE = 1;
            else
                LED_ORANGE = 0;

            if (robotState.distanceTelemetreCentre < 30 || robotState.distanceTelemetreGauche < 15 || robotState.distanceTelemetreDroit < 15)
                LED_BLEUE = 1;
            else
                LED_BLEUE = 0;

            if (robotState.distanceTelemetreGauche < 15 || robotState.distanceTelemetreExGauche < 15)
                LED_BLANCHE = 1;
            else
                LED_BLANCHE = 0;



        }

        //Lescture du buffer de réception et renvoi
        int i;
        for (i = 0; i < CB_RX1_GetDataSize(); i++) {
            unsigned char c = CB_RX1_Get();
            SendMessage(&c, 1);
        }

        
        // Envoi d'un message formaté
        //unsigned char payload[] = {'B', 'o', 'n', 'j', 'o', 'u', 'r'};
        //UartEncodeAndSendMessage(0x0080, sizeof (payload), payload);
        //__delay32(4000000);

    }

    return 0;
}



