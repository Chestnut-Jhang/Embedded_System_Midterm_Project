#include "mbed.h"

#include "parallax.h"

#include "uLCD_4DGL.h"

#include "TCS3200.h"



Serial pc(USBTX, USBRX);

DigitalIn pin8(D8);

PwmOut pin12(D12);

PwmOut pin13(D13);

I2C m_i2c(D14,D15);

uLCD_4DGL uLCD(D1, D0, D2);

TCS3200 color(D11, D6, D5, D3, A1);

Serial xbee(D9, D7);

char m_addr = 0x90;

float TempDetect();

void reply_messange(char *, char *);

void check_addr(char *, char *);

long map (long, long, long, long, long);

int main() {

    int cycle = 0;

    int number = 0;

    float temp = 0;

    long red, green, blue, clear;

    int i = 0;

    char xbee_reply[3];



    pc.baud(9600);

    pc.printf("Before start\r\n");



    parallax_encoder encoder0(pin8);

    encoder_ticker.attach(&encoder_control, .01);



    parallax_servo servo0(pin12);

    parallax_servo servo1(pin13);

    servo_ticker.attach(&servo_control, .5);



    color.SetMode(TCS3200::SCALE_100);



    xbee.baud(9600);
    
    xbee.printf("+++");

    xbee_reply[0] = xbee.getc();

    xbee_reply[1] = xbee.getc();

    if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){

        pc.printf("enter AT mode.\r\n");

        xbee_reply[0] = '\0';

        xbee_reply[1] = '\0';

    }

    xbee.printf("ATMY 0x48\r\n");

    reply_messange(xbee_reply, "setting MY : 0x48");

    xbee.printf("ATDL 0x47\r\n");

    reply_messange(xbee_reply, "setting DL : 0x47");

    xbee.printf("ATWR\r\n");

    reply_messange(xbee_reply, "write config");

    xbee.printf("ATMY\r\n");

    check_addr(xbee_reply, "MY");

    xbee.printf("ATDL\r\n");

    check_addr(xbee_reply, "DL");

    xbee.printf("ATCN\r\n");

    reply_messange(xbee_reply, "exit AT mode");

    xbee.getc();

    // start

    pc.printf("start\r\n");

    char buf[100] = {0};
    
    
    
    servo0 = 0;

    servo1 = 0;

    while(1) {

        encoder0.reset();

        wait(0.1);

        //pc.printf("encoder = %f\r\n", (float)encoder0);

        if (temp < 29){
        
            if (number % 2 == 0){
            
                while ((float)encoder0 < 2){
                    
                    //encoder0.reset();
                    
                    //wait(0.1);
                    
                    pc.printf("encoder = %f\r\n", (float)encoder0);
                    
                    servo1 = 10;

                    //wait(2.5);

                }

                //wait(0.1);

            }

            else if (number % 2 == 1){

                while ((float)encoder0 < 4){

                    pc.printf("encoder = %f\r\n", (float)encoder0);

                    servo1 = 10;

                }

                wait(0.1);

            }

            encoder0.reset();

            servo1 = 0;

            cycle++;

            pc.printf("encoder_spe = %f\r\n", (float)encoder0);

            pc.printf("cycle = %d\r\n", cycle);

            //pc.printf("number = %d\r\n", number);

            if (number % 2 == 0 && cycle == 6){
                
                number++;

                cycle = 0;

                servo0 = 20;

                servo1 = 0;

                wait(1);

                servo0 = 0;

                wait(0.1);

            }

            else if (number % 2 == 1 && cycle == 4){

                number++;
                
                cycle = 0;

                servo0 = -20;

                servo1 = 0;

                wait(1);

                servo0 = 0;

                wait(0.1);

            }

            // if (cycle == 1){

            //     number++;

            //     pc.printf("number = %d\r\n", number);

            //     if (number % 2 == 1){

            //         cycle = 0;

            //         servo0 = 20;

            //         servo1 = 0;

            //         wait(1);

            //         servo0 = 0;

            //         wait(0.1);

            //     }
            //     else if (number % 2 == 0){

            //         cycle = 0;

            //         servo0 = -20;

            //         servo1 = 0;

            //         wait(1);

            //         servo0 = 0;

            //         wait(0.1);
                    
            //     }

            // }

            servo1 = 0;

            wait(1);
        }

        temp = TempDetect();

        uLCD.printf("Temprature = %f\r\n", temp);

        wait(0.1);

        red = color.ReadRed();

        blue = color.ReadBlue();

        green = color.ReadGreen();

        clear = color.ReadClear();

        //red = map (red, , , 255, 0);

        //blue = map (blue, , , 255, 0);

        //green = map (green, , , 255, 0);

        uLCD.printf("(Red, Blue, Green) = (%2d, %2d, %2d)\r\n", red, blue, green);

        wait(0.1);

        //xbee.printf("%f\r\n", temp);

        xbee.printf("%2d\r\n", red); //

        xbee.printf("(Red, Blue, Green) = (%2d, %2d, %2d)\r\n", red, blue, green);

        wait(0.1);

    }

}



float TempDetect(){

    const char tempRegAddr = 0x00;

    m_i2c.write(m_addr, &tempRegAddr, 1); //Pointer to the temperature register



    char reg[2] = {0,0};

    m_i2c.read(m_addr, reg, 2); //Read



    unsigned short res = (reg[0] << 4) | (reg[1] >> 4);

    float temp =  (float) ((float)res * 0.0625);



    pc.printf("load temp %d,%d\r\n", reg[0], reg[1]);

    wait(1);

    return temp;

}



TCS3200::TCS3200(PinName S0, PinName S1, PinName S2, PinName S3, PinName OUT) : 
mS0(S0), mS1(S1), mS2(S2), mS3(S3), signal(OUT) 
{
    SetMode(SCALE_100);
    signal.rise(this,&TCS3200::HighTrigger);
    signal.fall(this,&TCS3200::LowTrigger);
}
 
long TCS3200::ReadRed() {
    mS2=0;                    
    mS3=0;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}
 
long TCS3200::ReadGreen() {
    mS2=1;                    
    mS3=1;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}
 
long TCS3200::ReadBlue() {
    mS2=0;                    
    mS3=1;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}
 
long TCS3200::ReadClear() {
    mS2=1;                    
    mS3=0;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}
 
void TCS3200::SetMode(TCS3200Mode mode) {
    if(mode == POWERDOWN) {         //TCS3200 in power down
        mS0 = 0;                     
        mS1 = 0;
    }
    else if(mode == SCALE_2) {      //Output frequency at 2% scaling
        mS0 = 0;                     
        mS1 = 1;
    }
    else if(mode == SCALE_20) {     //Output frequency at 20% scaling
        mS0 = 1;                     
        mS1 = 0;
    }
    else if(mode == SCALE_100) {    //Output frequency at 100% scaling
        mS0 = 1;                     
        mS1 = 1;
    }
    else {                          //default is POWERDOWN                
        mS0 = 0;                     
        mS1 = 0;
    }
}
 
void TCS3200::HighTrigger() {
    timer.start();
}
 
void TCS3200::LowTrigger() {
    timer.stop();
    pulsewidth = timer.read_us();
    timer.reset();
}



long map (long x, long in_min, long in_max, long out_min, long out_max){

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}



void reply_messange(char *xbee_reply, char *messange){

    xbee_reply[0] = xbee.getc();

    xbee_reply[1] = xbee.getc();

    xbee_reply[2] = xbee.getc();

    if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){

        pc.printf("%s\r\n", messange);

        xbee_reply[0] = '\0';

        xbee_reply[1] = '\0';

        xbee_reply[2] = '\0';

    }

}



void check_addr(char *xbee_reply, char *messenger){

    xbee_reply[0] = xbee.getc();

    xbee_reply[1] = xbee.getc();

    xbee_reply[2] = xbee.getc();

    pc.printf("%s = %c%c\r\n", messenger, xbee_reply[1], xbee_reply[2]);

    xbee_reply[0] = '\0';

    xbee_reply[1] = '\0';

    xbee_reply[2] = '\0';

}
