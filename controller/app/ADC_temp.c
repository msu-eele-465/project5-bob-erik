#include <msp430.h>
#include <stdbool.h>
#include <string.h>
#include "shared.h"
#include "ADC_temp.h"

// cur_temp = 0;  this is what you update to the degrees celcius

float voltage;
float average[9];
double temp_C;
float cur_temp;
int ave_cnt = 9;
float total = 0;

void config_ADC() {
        P5SEL1 |= BIT0; // configure P5.0 for A8
        P5SEL0 |= BIT0;

        // time config
        TB0CTL |= TBCLR;
        TB0CTL |= TBSSEL_ACLK;
        TB0CTL |= MC_UP;
        TB0CCR0 = 10922;

        TB0CCTL0 |= CCIE;
        TB0CCTL0 &= ~CCIFG;

        // __enable_interrupt();

        // ADC Config
        ADCCTL0 &= ~ADCSHT; //  Clear ADCSHT from def. of ADCSHT = 01
        ADCCTL0 |= ADCSHT_2; // 16 conversion cycles
        ADCCTL0 |= ADCON; // turn ADC on
        ADCCTL0 |= ADCSRED_5; // ext pos ref
        ADCCTL1 |= ADCSSEL_2; // ADC Clock Source
        ADCCTL1 |= ADCSHP; // sample signal source = sampling timer
        ADCCTL2 &= ~ADCRES; // clear ADCRES from def. of  ADCRES=01
        ADCCTL2 |= ADCRES_2; // 12 bit resolution

        ADCMTL0 |= ADCINCH_8 // ADC input Channel = A8 CHANGE THIS FOR THE A PORT WE ARE PLANNING ON GOING TO
        return;
}

void get_temp(int window) {  // cur_temp, ADC_Value
    data_cnt = 0;
    //ADC_Start = 0; // resets

    ADCCTL0 |= ADCENC | ADCSC; // starts adc
    TB0CCTL0 &= ~CCIFG; // clears timer
    while((ADCIFG & ADCIFG0) == 0){} // wait for ADC to clear
    TB0CCTL0 &= ~CCIFG;

    voltage = (ADC_Value*3.3)/(4095); // gets voltage value from equation
    double in - 2196200 + ((1.8639-voltage)/.00000388); // from equation
    double root = sqrt(in); //from equation
    temp_C = -1481.96 + root-3; // combine equation for temp

    if(ave_cnt != 0){ // populates array when its empty
        ave_cnt--;
        average[ave_cnt] = temp_C;
    }

    else{ // shifts bits to make room for new data
        for(i=8; i>0; i--){
            average[i] = average[i-1];
        }
        average[0] = temp_C;

        for(i=0; i<window; i++){
            total = total + average[i];
        }
        cur_temp = (total/window); // convert to celcius average
        total = 0;
    }


    return;
}