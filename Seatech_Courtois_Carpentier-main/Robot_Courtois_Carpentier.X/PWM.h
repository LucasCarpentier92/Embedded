#ifndef PWM_H
#define PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1
void InitPWM(void);
void PWMSetSpeed(float vitesseEnPourcents, int moteur);
void PWMSetSpeedConsigne(float vitesseEnPourcents, int moteur);
void PWMUpdateSpeed();

#endif
