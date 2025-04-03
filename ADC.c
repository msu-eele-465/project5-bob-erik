#include <msp430.h>

unsigned int ADC_Value;

int main(void){

WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

PM5CTL0 &= ~LOCKLPM5; // Turn on GPIO

// configure ports
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

__enable_interrupt();

while(1){
    ADCCTL0 |= ADCENC | ADCSC; // enable and start conversion

    while((ADCIFG & ADCIFG0) == 0); // wait for conversion to complete

    ADC_Value = ADCMEM0; // read ADC result

    if(ADC_Value > 361){ // if (A8 > 3v)
        P1OUT |= BIT0; // LED1 = ON (red) PORTS
        P6OUT &= ~BIT6; // LED = OFF PORTS
    } else { // if (A8 < 3v)
        P1OUT &= ~BIT0; // LED1 = OFF
        P6OUT |= BIT6; // LED2 = ON (green)
    }
}
return 0;
}

if(ADC_Start == 1){ // asserts every .33 seconds
    data_cnt = 0;
    ADC_Start = 0; // resets
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

        for(i=0; i<press; i++){
            total = total + average[i];
        }

        ave_c = (total/press)*100; // convert to celcius average
        ave_k = ave_c+27315; // get kelvin average
        sprintf(test,"%d", ave_c); // input celcius into array
        sprintf(kelvin, "%d", ave_k); // input kelvin into array
        sprintf(p, "%d", press); // input press into array
        strcat(test, kelvin);
        strcat(test, p); // array should be populated
        total = 0;

    }

}

// interrupt

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    ADC_Value = ADCMEM0; // Read ADC Value

    if(ADC_Value > 3613){ // If (A8 > 3v)
       // P1OUT |= BIT0; // LED1 = ON (red)
       // P6OUT &= ~BIT6; // LED2=OFF
    } else { // IF (A8 < 3v)
        P1OUT &= ~BIT0; //LED1 = OFF
        P6OUT |= BIT6; //LED2 = ON (green)
    }
}

/*
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    ADC_Value = ADCMEM0; // get ADC value
}
    */

