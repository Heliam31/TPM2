// ECE 476 Final Projec: Autonomous stability system for a co-axial Helicopter
// Note: This code has been edited for better
//created Friday, April 7th
// last updated april 21st
// last updated april 23rd
// last updated May 3rd


#include <stdlib.h>
#include <stdio.h>

int main();

void _start(void) {
	main();
	while(1);
}



// MATH STUFF
// Code Credit for fixed point code: Bruce Land http://www.nbb.cornell.edu/neurobio/land/
//****************************************************************************************

//===The fixed macros=========================================
//#define int2fix(a)   (((int)(a))<<8)            //Convert char to fix

// flight duration
#define milsec 350      // used for our "seconds" counter used for
#define NBSEC 100

// replace the inclusion
//#include "data.c"
int valsX[NBSEC], valsY[NBSEC], valsZ[NBSEC], valsGyro[NBSEC];


#define minimum 0       // minimum PWM value
#define maxticks 100    // used for PWM
#define maximum 100     // maximum PWM value
#define Smax 60         // Maximum PWM value for stability motors
#define Sinc 256        // Stability Motor PWM increment

// Define sensor channels
#define GYRO_CHANNEL 0
#define AROMX_CHANNEL 1
#define AROMY_CHANNEL 2
#define AROMZ_CHANNEL 3


// Define Helicopter states
#define GROUND 0
#define LANDING 1
#define TAKEOFF 2
#define HOVER 3
#define LEFT 4
#define RIGHT 5
#define FORWARD 6
#define BACKWARD 7

#define ACCX 0
#define ACCY 1
#define ACCZ 2
#define GYRO 3

#define int2fix(a) (((int) (a)) << 8)
#define fix2int(a)  ((signed char)((a)>>8))    //Convert fix to char


// start off on the ground if we live in reality
char heliState = GROUND;

//used for timing of PWM
int ticks = 0;
int marks = 0;
int msec = 0;
int sec = 0;

// make motor speeds fixed point for greater accuracy
int topRotorSpeed = 0;
int bottomRotorSpeed = 0;
int motorStable1 = 0;
int motorStable2 = 0;
int motorStable3 = 0;

// used sometimes
int i = 0;
char k;
char temp;

// ADC variables
char Ain = 0;
char Ainlast = 0;
char first = 0;

// Gyroscope
char gyroCalibrate = 0;  // calibration
char gyro[32];           // buffer
char lastG = 0;          // not used

// X accelerometer
char aromXCalibrate = 0;        // calibration
char aromX[128];                // buffer

// Y accelerometer
char aromYCalibrate = 0;        // calibration
char aromY[128];                // buffer


// Z accelerometer
char aromZCalibrate = 0;        // calibration
char aromZ[128];                // buffer

char currentChannel = GYRO_CHANNEL;

char PORTC[5]; // PORT C to control motors

char USART_TXB;	// to transmit log to the ground

void updatePWM(void);
int updateADC(void);
void calibrateGyro(void);
void calibrateArom(void);
char FixFilter(int length_log, char *f);
char FixFilter32(char f[32]);
char FixFilter64(char f[64]);
char FixFilter128(char f[128]);
char FixFilter256(char f[256]);
void recordSample(int length_log, char *f, char new_sample);
char aromZfilter(char f[256]);

int X_index=0;
int Y_index=0;
int Z_index=0;
int Gyro_index=0;

//functions that simulate getting values from sensors
//-------------------
int getAccX(){
  return valsX[X_index++];
}

int getAccY(){
  return valsY[Y_index++];
}

int getAccZ(){
  return valsZ[Z_index++];
}

int getGyro(){
  return valsGyro[Gyro_index++];
}

int getValue(int channel){
  if(currentChannel == GYRO_CHANNEL)
    return getGyro();
  else if(currentChannel == currentChannel)
	return getAccX();
  else if(currentChannel == AROMY_CHANNEL)
    return getAccY();
  else if(currentChannel == AROMZ_CHANNEL)
    return getAccZ();
  else
	return 0;
}

//-------------------
// Dump function for debug
#ifdef DEBUG
void dump(){
  printf("temps = %d\t",sec);
  printf("vitesse M1 = %d\t",motorStable1);
  printf("vitesse M2 = %d\t",motorStable2);
  printf("vitesse M3 = %d\t",motorStable3);
  printf("vitesse top = %d\t",topRotorSpeed);
  printf("vitesse bottom = %d\n ",bottomRotorSpeed);
}
#endif


// ------------------
//
char FixFilter(int length_log, char *f){
  int sum = 0;
  int length = 1 << length_log;
  for(i = 0; i < length; i++){
    sum = sum + f[i];
  }
  // divide by length
  sum = sum >> length_log;
  return sum;
}


// ------------------
// Records a new sample (in a buffer of length 2^length_log)
void recordSample(int length_log, char *f, char new_sample){
  int i;
  int length = 1 << length_log;
   for(i = 0; i < length-1; i++){
      f[i] = f[i+1];
    }
    f[length-1] = new_sample;
}


//-------------------
// Read sensor values from an input file
#if DEBUG
void loadSensorValues(){

  FILE *fp ;
  int i;

  if ((fp = fopen("generHelico.txt", "r")) == NULL)
    {
      fprintf(stderr, "Erreur à l'ouverture du fichier en mode r.\n\n");
      exit(1);
    }

  for( i=0;i<NBSEC*milsec;i++){
    fscanf(fp, "%d",&valsZ[i]);

    //fflush(NULL);
  }
  for( i=0;i<NBSEC*milsec;i++)
    fscanf(fp, "%d",&valsY[i]);

  for( i=0;i<NBSEC*milsec;i++)
    fscanf(fp, "%d",&valsX[i]);

  for( i=0;i<NBSEC*milsec;i++){

    fscanf(fp, "%d",&valsGyro[i]);
    //printf("%d \n",valsGyro[i]);
  }
}
#endif


//-------------------
// Controls behavior

int action(void) {

	// transmit log
	int i;
	union {
		struct {
			int top;
			int bottom;
		} data;
		char buf[8];
	} m;
	m.data.top = topRotorSpeed;
	m.data.bottom = bottomRotorSpeed;
	for(i = 0; i < 8; i++)
		USART_TXB = m.buf[i];

  // take off
  // increase the motor speeds quickly but not abruptly
  // so we dont screw up the gears

  if(sec <= 5){
    heliState = TAKEOFF;
    if(topRotorSpeed < int2fix(95))
      topRotorSpeed += 16;
    if(bottomRotorSpeed < int2fix(95))
      bottomRotorSpeed += 16;
  }

  // hover
  // We turn on the X, Y, Z accelerometer stability systems

  if(sec == 23) {
    heliState = HOVER;
  }

  // landing
  // We turn off all the stability accelerometers
  // and their respective motors for stable landing

  if(sec == 299){
    heliState = LANDING;
    motorStable1 = 0;
    motorStable2 = 0;
    motorStable3 = 0;
  }


  if(sec >= 300 && sec <= 400){
    if(topRotorSpeed > int2fix(50))
      topRotorSpeed -= 1;
    if(bottomRotorSpeed > int2fix(50))
      bottomRotorSpeed -= 1;
  }

  // grounded
  // We're done! Lets get into an infinite loop so
  // watchdog resets the MCU and is ready for the next flight

  if(sec >= 410){
    heliState = GROUND;
  }
  return 0;
}

void updatePWM() {
  int tempTick;
  tempTick = int2fix(ticks);

  // PWM top motor
  if (tempTick > topRotorSpeed)
    PORTC[0] = 0;
  else
    PORTC[0] = 1;

  // PWM second motor
  if (tempTick > bottomRotorSpeed)
    PORTC[1] = 0;
  else
    PORTC[1] = 1;

  // front motor (1)
  if(tempTick > fix2int(motorStable1))
    PORTC[2]= 0;
  else
    PORTC[2] = 1;

  // right motor (2)
  if(tempTick > motorStable2)
    PORTC[3] = 0;
  else
    PORTC[3] = 1;

  // left motor (3)
  if(tempTick > motorStable3)
    PORTC[4] = 0;
  else
    PORTC[4] = 1;

  //PWM reset etc
  if (ticks == maxticks){
    ticks = 0;
  }

}

//-------------------
// Compute motor speeds

void doGyroChannel(void) {
    // currently looking at the gyro channel
    //  clockwise : Ain greater than gyroCalibrate
    //  counter-clockwise : Ain is less than gyro calibrate
    // for clockwise : top less, bottom more
    // for counterclockwise : top more, bottom less
    // use of dead zone of +/- 1 from calibrate value
    recordSample(5, gyro, Ain);
    temp = FixFilter(5,gyro); // last 32 values


    // we had to use a small threshold since the gyro was a little too sensitive
    if (temp < (gyroCalibrate - 1)) {
      if(topRotorSpeed < int2fix(maximum))
	topRotorSpeed += 8;
      if(bottomRotorSpeed > int2fix(minimum))
	bottomRotorSpeed -= 8;
    }
    else
      if(temp > (gyroCalibrate + 1)) {
	if(topRotorSpeed > int2fix(minimum))
	  topRotorSpeed -= 8;
	if(bottomRotorSpeed< int2fix(maximum))
	  bottomRotorSpeed += 8;
      }
}

void doAROMXChannel(void) {
    if(heliState == HOVER){
      recordSample(7, aromX, Ain);
      temp = FixFilter(7, aromX);

      if(temp < (aromXCalibrate)) {
	if(motorStable1 > 0)
	  motorStable1 -= Sinc*2;
	if(motorStable2 < int2fix(Smax))
	  motorStable2 += Sinc*2;
	if(motorStable3 < int2fix(Smax))
	  motorStable3 += Sinc*2;
      }
      else {
	if(temp > (aromXCalibrate + 1)) {
	  if(motorStable1 < int2fix(Smax))
	    motorStable1 += Sinc*2;
	  if(motorStable2 > 0)
	    motorStable2 -= Sinc*2;
	  if(motorStable3 > 0)
	    motorStable3 -= Sinc*2;
	}
	else {
	  if(motorStable1 > 0)
	    motorStable1 -= Sinc*2;
	}
      }
    }
}

void doAROMYChannel(void) {
    if(heliState == HOVER){
       recordSample(7, aromY, Ain);
       temp = FixFilter(7, aromY);
       if(temp < (aromYCalibrate)) {
	 if(motorStable2 < int2fix(Smax))
	   motorStable2 += Sinc*4;
	 if(motorStable3 > 0 )
	   motorStable3 -= Sinc*2;
       }
       else {
	 if(temp > (aromYCalibrate)) {
	   if(motorStable2 > 0 )
	     motorStable2 -= Sinc*2;
	   if(motorStable3 < int2fix(Smax))
	     motorStable3 += Sinc*4;
	 }
	 else {
	   if(motorStable2 > 0)
	     motorStable2 -= Sinc;
	   if(motorStable3 > 0)
	     motorStable3 -= Sinc;
	 }
       }
    }
}

void doAROMZChannel(void) {
    if(heliState == HOVER){
      recordSample(7,aromZ,Ain);
      temp = FixFilter(7,aromZ);
      // this sensor was a little touchy as one direction was more sensitive
      // and noisier than the other.  The threshold allowed us to help this as well
      // as prevent the helicopter from losing altitude which was more important than it
      // gaining too much altitude
      if(temp > (aromZCalibrate + 3)){
	if(topRotorSpeed > int2fix(minimum))
	  topRotorSpeed -= 2;
	if(bottomRotorSpeed > int2fix(minimum))
	  bottomRotorSpeed -= 2;
      }
      else
	if(temp == (aromZCalibrate)){
	  if(topRotorSpeed < int2fix(maximum - 3))
	    topRotorSpeed += 5;
	  if(bottomRotorSpeed < int2fix(maximum - 3))
	    bottomRotorSpeed += 5;
	}
    }
}

int updateADC(void){

  Ainlast = Ain;
  Ain=getValue(currentChannel);

  // each case occurs every fourth time
  if(currentChannel == GYRO_CHANNEL)
	doGyroChannel();
  else if(currentChannel == AROMX_CHANNEL)
  	doAROMXChannel();
  else if(currentChannel == AROMY_CHANNEL)
  	doAROMYChannel();

  // currently lloking at the Z axis accelerometer channel
  // when going up we have less g so would be lower
  // when going down we have more g so should be over
  else if(currentChannel == AROMZ_CHANNEL)
	doAROMZChannel();

  return 0;
}

void calibrateGyro(void){
  int i;
  for(i = 0; i < 32; i++){
    gyro[i]=getGyro();
  }
  gyroCalibrate = FixFilter(5,gyro);
}

void calibrateArom(void){
  for(i = 0; i < 128; i++){
    aromX[i] = getAccX();
  }
  aromXCalibrate = FixFilter(7,aromX);

  for(i = 0; i < 128; i++){
    aromY[i] = getAccY();
  }
  aromYCalibrate = FixFilter(7,aromY);

  for(i = 0; i < 128; i++){
    aromZ[i] = getAccZ();
  }
  aromZCalibrate = FixFilter(7,aromZ);

}

void timer_interrupt() {
  ticks++;
  marks++;
  if (marks == milsec){
    marks = 0;
    msec++;
 #ifdef DEBUG
      dump();
#endif
   if(msec == 10){
      msec = 0;
      sec++;

    }
  }
  updatePWM();
}

void doPWM(void) {
      msec++;
      if(msec == 10){
	    updatePWM();
	    msec = 0;
	    sec++;
#       ifdef DEBUG
          dump();
#       endif
      }
}

void stabilize() {

	for(currentChannel = GYRO_CHANNEL;
	currentChannel <= AROMZ_CHANNEL;
	currentChannel++) {

		updateADC();

		action();

		doPWM();

	}
	
}

int main(void){

#ifdef DEBUG
  loadSensorValues()();
#endif

  calibrateGyro();
  calibrateArom();


  // Lets a GO!

  sec = 0;

  while(1) {
    ticks++;
    marks++;
    if (marks == milsec){
      marks = 0;
      stabilize();     
    }
  }
  return 0;
}

