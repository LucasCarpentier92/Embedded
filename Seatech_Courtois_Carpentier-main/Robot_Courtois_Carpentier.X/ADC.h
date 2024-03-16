#ifndef ADC_H
void InitADC1(void);
void ADC1StartConversionSequence();
void ADCClearConversionFinishedFlag(void);
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
#endif