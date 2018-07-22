// Morse Challenge Geocache v1 - 2018-07-22
// (c) 2018 Ernest Neijenhuis PA3HCM
//
// Dependencies:
// - SevenSeg: https://github.com/sigvaldm/SevenSeg
// - Servo: bundled with the Arduino IDE
//
// For more information please read the bundled readme.md.

#include <SevenSeg.h>
SevenSeg disp(1,2,3,4,5,6,7); // Vishay TDSx5150
const int numOfDigits=1;
int digitPins[numOfDigits]={0};

#include <Servo.h>
Servo doorlock;

void setup() {
    pinMode(0, OUTPUT);  // 7-segment display common anode
    pinMode(1, OUTPUT);  // 7-segment display cathode a
    pinMode(2, OUTPUT);  // 7-segment display cathode b
    pinMode(3, OUTPUT);  // 7-segment display cathode c
    pinMode(4, OUTPUT);  // 7-segment display cathode d
    pinMode(5, OUTPUT);  // 7-segment display cathode e
    pinMode(6, OUTPUT);  // 7-segment display cathode f
    pinMode(7, OUTPUT);  // 7-segment display cathode g
    pinMode(8, INPUT);   // Pushbutton to enter selected value
    pinMode(9, OUTPUT);  // PWM-signal to control door lock servo
    pinMode(10, INPUT);  // door sensor
    pinMode(13, OUTPUT); // Buzzer
    disp.setDigitPins(numOfDigits, digitPins);
    disp.setRefreshRate(25);
    disp.setTimer(2);
    randomSeed(analogRead(5));
    doorlock.attach(9);
}

int i=0;         // generic counter
int question=10;  // question number
int state=1;     // program state, 1=init
int answer=99;   // the given answer
int rightanswer; // the answer to be guessed

void loop() {
    switch(state) {
        case 1: // init
            disp.startTimer();
            doorlock.write(0); // ensure door is closed
            for(i=0;i<=9;i++) {
                disp.write(i);
                delay(100);
            }
            disp.stopTimer();
            disp.clearDisp();
            state=2;
            break;
        case 2: // read random number to guess
            rightanswer=random(9);
            state=3;
            break;
        case 3: // sound the morse code 3 times
            for(i=1;i<=3;i++) {
                delay(1000);
                morse(rightanswer, question);
                delay(1000);
            }
            disp.startTimer();
            state=4;
            break;
        case 4: // wait for answer
            answer=map(analogRead(A0), 0, 1023, 0, 9);
            disp.write(answer);
            if(digitalRead(8)) { // Button pushed!
                disp.stopTimer();
                disp.clearDisp();
                delay(200);
                state=5;
            }
            break;
        case 5: // evaluate the given answer
            if(answer==rightanswer) {
                question++;    // go toe the next level!
            } else {
                question--;    // wrong answer, one level down :-(
            }
            state=2;
            if(question>10) {
                state=6;       // you win!
            }
            if(question<1) {
                question=1;
            }
            break;
        case 6: // unlock the door
            doorlock.write(180); // open the door lock!
            tatatataaa();
            state=7;
            break;
        case 7: // wait for door to close
            break;
        case 8: // lock the door
            if(digitalRead(10)) {
                delay(250);
                doorlock.write(0); // close the door
                delay(1000);
                state=9;
            }
            break;
        case 9: // the end
            break;
        default:
            state=1;
    }
}

void dit(int dottime) {
    for(int i=1;i<dottime;i++) {
        digitalWrite(13, HIGH);
        delay(1);
        digitalWrite(13, LOW);
        delay(1);
    }
    delay(2*dottime);
}

void dah(int dottime) {
    for(int i=1;i<(dottime*3);i++) {
        digitalWrite(13, HIGH);
        delay(1);
        digitalWrite(13, LOW);
        delay(1);
    }
    delay(2*dottime);
}

void morse(int number, int question) {
    int dottime;
    switch(question) { // each level is 13% faster
        case 1:
            dottime=100;
            break;
        case 2:
            dottime=87;
            break;
        case 3:
            dottime=76;
            break;
        case 4:
            dottime=66;
            break;
        case 5:
            dottime=57;
            break;
        case 6:
            dottime=50;
            break;
        case 7:
            dottime=43;
            break;
        case 8:
            dottime=38;
            break;
        case 9:
            dottime=33;
            break;
        case 10:
            dottime=29;
            break;
        default:
            dottime=100;
    }
    switch(number) {
        case 0:
            dah(dottime); dah(dottime); dah(dottime); dah(dottime); dah(dottime);
            break;
        case 1:
            dit(dottime); dah(dottime); dah(dottime); dah(dottime); dah(dottime);
            break;
        case 2:
            dit(dottime); dit(dottime); dah(dottime); dah(dottime); dah(dottime);
            break;
        case 3:
            dit(dottime); dit(dottime); dit(dottime); dah(dottime); dah(dottime);
            break;
        case 4:
            dit(dottime); dit(dottime); dit(dottime); dit(dottime); dah(dottime);
            break;
        case 5:
            dit(dottime); dit(dottime); dit(dottime); dit(dottime); dit(dottime);
            break;
        case 6:
            dah(dottime); dit(dottime); dit(dottime); dit(dottime); dit(dottime);
            break;
        case 7:
            dah(dottime); dah(dottime); dit(dottime); dit(dottime); dit(dottime);
            break;
        case 8:
            dah(dottime); dah(dottime); dah(dottime); dit(dottime); dit(dottime);
            break;
        case 9:
            dah(dottime); dah(dottime); dah(dottime); dah(dottime); dit(dottime);
            break;
        default:
            break;
    }
}

void tatatataaa() {
    // probably the poorest code play some winning tune...
    for(i=1;i<15;i++) {
        digitalWrite(13, HIGH);
        delay(4);
        digitalWrite(13, LOW);
        delay(4);
    }
    delay(80);
    for(i=1;i<15;i++) {
        digitalWrite(13, HIGH);
        delay(4);
        digitalWrite(13, LOW);
        delay(4);
    }
    delay(80);
    for(i=1;i<15;i++) {
        digitalWrite(13, HIGH);
        delay(4);
        digitalWrite(13, LOW);
        delay(4);
    }
    delay(80);
    for(i=1;i<120;i++) {
        digitalWrite(13, HIGH);
        delay(3);
        digitalWrite(13, LOW);
        delay(3);
    }
}


ISR(TIMER2_COMPA_vect) {
    disp.interruptAction() ;
}
