// --------------------------- Config Start --------------------------------
// Randomly slowdown animation. Nice in principle, but not when debugging performance
#define RANDOMSLOWME

// Control whether pattern order is random from the start or after one pass
//#define MIXIT_AFTER_FIRST_PASS

// write the pattern number in upper left (makes more sense on higher res displays)
//#define SHOW_PATTERN_NUM

// Some pattern transitions look weird without a clear in between
//#define CLEAR_BETWEEN_PATTERNS

// This allows a selection of only my favourite patterns.
// Comment this out to get all the patterns -- merlin
// #define BESTPATTERNS
#ifdef BESTPATTERNS
// 82 and 89 are similar     55 and 102 are similar
uint8_t bestpatterns[] = {
3, 4, 7, 8, 10, 11, 12, 14, 16, 17, 18, 19, 20, 21, 25, 26, 55, 58, 59, 61, 67, 69, 70, 72, 73, 77, 79, 80, 81, 82, 84, 86, 87, 89, 90, 94, 96, 98, 99, 101, 102, 103, 104, 105, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 118, 119, 120, 121, 122, 124, 126, 128, 130, 131, 132, 134, 139, 141, 144, 147, 148, 149, 151, 155, 158, 159, 
};
#define numbest           sizeof(bestpatterns)
#define lastpatindex numbest
#else
#define lastpatindex mpatterns
#endif

// By default audio support is on for what's expected to be teensy.
#define TME_AUDIO
#ifdef ARDUINOONPC
#undef TME_AUDIO
#endif
#ifdef ESP32
#undef TME_AUDIO
#endif

#ifdef TME_AUDIO
#include <EasyTransfer.h>// used for exchange with 2nd arduino for audio processing
#endif

// --------------------------- Config End ----------------------------------


// Enable LEDMATRIX support in neomatrix_config
#define LEDMATRIX
#include "neomatrix_config.h"
#define zeds ledmatrix

// Aurora includes
#include "matrix.h"
#include "Effects.h"
#include "Drawable.h"
#include "Boid.h"
#include "Attractor.h"
#include "Geometry.h"
#include "PatternAttract.h"
PatternAttract attract;
#include "PatternBounce.h"
PatternBounce bounce;
#include "PatternCube.h"
PatternCube cube;
#include "PatternFlock.h"
PatternFlock flock;
#include "PatternFlowField.h"
PatternFlowField flowfield;
#include "PatternIncrementalDrift.h"
PatternIncrementalDrift incrementaldrift;
#include "PatternIncrementalDrift2.h"
PatternIncrementalDrift2 incrementaldrift2;
#include "PatternPendulumWave.h"
PatternPendulumWave pendulumwave;
#include "PatternRadar.h"
PatternRadar radar;
#include "PatternSpiral.h"
PatternSpiral spiral;
#include "PatternSpiro.h"
PatternSpiro spiro;
#include "PatternSwirl.h"
PatternSwirl swirl;
#include "PatternWave.h"
PatternWave wave;

AuroraDrawable* items[] = {
  &attract,//0
  &bounce,//1
  &cube,//2
  &flock,//3
  &flowfield,//4
  &incrementaldrift,//5
  &incrementaldrift2,//6
  &pendulumwave,//7
  &cube,//8  was radar
  &spiral,//9
  &spiro,//10
  &swirl,//11
  &wave,//12
};
AuroraDrawable *patternz;


int8_t item = random(13);

#define MIDLX               (MATRIX_WIDTH/2)
#define MIDLY               (MATRIX_HEIGHT/2)
#define mpatterns           (190)// max number of patterns
#define TIMING              90//seconds per pattern
#define LATCH               23
#define  BallCount          30

#ifdef TME_AUDIO
EasyTransfer ETin;
#endif
struct RECEIVE_DATA_STRUCTURE {
  byte laudio[17];
  byte raudio[17];
  float scale;
  boolean bighit;
  byte peakL;
  byte peakR;
};
RECEIVE_DATA_STRUCTURE music;

byte pattern = 112;//this picks the pattern to start with...
byte slowest = 4, fastest = 22, cool,  bpm[32], targetfps = 10, adjunct = 3, sizzer;
byte sparky = 80, ccc, xxx, yyy, dot = 3, radius2, rr, gg, bb;
byte  maxiaud, pointy,  hue, steper,  xblender, hhowmany, blender = 120, radius3,  quietcount, ccoolloorr,  h = 0;
byte dot2 = 6, sdot, dot3 = 1, phew, lender = 128, xsizer, ysizer, xx,  yy, flipme = 1, shifty = 4,  poffset, wind = 2, fancy , sinewidth;
byte mstep, LLaudio[64], RRaudio[64], inner, bfade = 3, kind[MATRIX_WIDTH * 3];

uint16_t raad;
int howmany, xhowmany, how, fcool[MATRIX_WIDTH], velo = 30 , pointyfix,  fpeed[MATRIX_WIDTH * 3] ;
int bigmax, directn = 1, quash = 5, quiet = 0, waiter = 7, level, levelfull;
int  fcolor[MATRIX_WIDTH * 3], fcountr[MATRIX_WIDTH * 3], heaty[MATRIX_WIDTH][MATRIX_HEIGHT], xvort[MATRIX_WIDTH * 3];
int yvort[MATRIX_WIDTH * 3], xpoffset[MATRIX_WIDTH * 3], fcount[MATRIX_WIDTH * 3], fvelo[MATRIX_WIDTH * 3];
unsigned int  counter, ringdelay, bringdelay, firedelay, hitcounter;

boolean gmusic =  false, flop[12],   nextsong = false;
boolean rimmer[MATRIX_WIDTH * 3], xbouncer[MATRIX_WIDTH * 3], ybouncer[MATRIX_WIDTH * 3];

boolean  audi = false, mixit = false, slowme = true;

unsigned long lasttest, lastmillis, dwell = TIMING * 200;

float  mscale = 1.4, fps = 30;
float radius, xslope[MATRIX_WIDTH * 3], yslope[MATRIX_WIDTH * 3], xfire[MATRIX_WIDTH * 3], yfire[MATRIX_WIDTH * 3], cangle, sangle;
float locusx, locusy, driftx, drifty, xcen, ycen, yangle, xangle, eeksangle, whyangle;
float Gravity = -9.81;
int StartHeighty = 1;
float Heighty[BallCount];
float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeighty );
float ImpactVelocity[BallCount];
float TimeSinceLastBounce[BallCount];
int   Positiony[BallCount], Positionx[BallCount];
long  ClockTimeSinceLastBounce[BallCount];
float Dampening[BallCount];
CRGBPalette16 thepal;
char readchar;

#ifdef SHOW_PATTERN_NUM
uint8_t print_width;
#endif

#include "Table_Mark_Estes_Impl.h"

void setup()
{
#ifdef TME_AUDIO
  Serial1.begin(57600);
  ETin.begin(details(music), &Serial1);
  // warning, the up to date matrix_setup also does serial.begin
  Serial.begin(57600);
  delay(2000);
  Serial.println("Reset");
  randomSeed(analogRead(1) - analogRead(2) + analogRead(5));  
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);    // sets the audio module hardware to off
#endif
  matrix_setup();

  driftx = random(3, MATRIX_WIDTH - 4);//set an initial location for the animation center
  drifty = random(3, MATRIX_HEIGHT - 4);// set an initial location for the animation center
  mstep = byte(256 / (MATRIX_WIDTH - 1)); //mstep is the step size to distribute 256 over an array the width of the matrix

  steper = random8(2, 8);// steper is used to modify h to generate a bigger color step on each move  early stuff not so common any more
  for (byte j = 0; j < 12; j++) {
    flop[j] = false;
    if (random8() < 128) flop[j] = true;
  }

  flop[1] = true;
  flop[6] = false;
  flop[1] = false;
  flop[2] = true;
  flop[3] = false;
  flop[5] = true;


  lastmillis = millis();
  lasttest = millis();
#ifdef TME_AUDIO
  digitalWrite(LATCH, HIGH);
#endif
  hue = random8();//get a starting point for the color progressions

  cangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation gives a float value between -1 and 1
  sangle = (sin8(random(25, 220)) - 128.0) / 128.0;//angle of movement for the center of animation in the y direction gives a float value between -1 and 1

  whatami();//this prints out the current status of stuff

  effects.leds = matrixleds;
  effects.Setup();
  matrix->show();
#ifdef TME_AUDIO
  digitalWrite(LATCH, LOW);
  delay(10);
  Serial.print("Scale: ");
  audioprocess();
  Serial.println(music.scale);
#endif

}

void loop()
{
  hue += 1;//increment the color basis
  h = hue;  //set h to the color basis
  checkshifty();
  // test for frame rate,  every 8 frames
  checktempo();

  checkfader();

  checkfancy();

  checkwind();

  if (slowme) slowmo();
  EVERY_N_SECONDS(11) {
    cangle = random(25, 85) / 100.0;
    sangle = random(25, 85) / 100.0;
    if (random8() > 128) cangle = -cangle;
    if (random8() > 128) sangle = -sangle;
    fcool[random(64)] = random (7, 25);
    fcool[random(64)] = random (6, 28);
    fcool[random(64)] = random (8, 25);
    fcool[random(64)] = random (6, 30);
    blender += random(4, 12);
    xblender -= random(4, 12);
  }

  runpattern();//go generate a updted screen
  counter++;//increment the counter which is used for many things
  // spititout();//map the 2d screen to the 8 outpus and push them out on my other matrix, not used here
  matrix->show();
  delay(waiter);//frame rate control
  if (Serial.available()) readchar = Serial.read(); else readchar = 0;
  if (readchar > 31 || millis() > lastmillis + dwell || nextsong) //when to change patterns
  {
    Serial.print(", Actual FPS: ");
    Serial.println (fps, 2);
    newpattern();
  }
}
