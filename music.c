/**
 * @author Jackson Jenkins
 * @version 12/2/2018
 */
#include "music.h"
#include <msp430.h>

// Definition of the notes' frequecies in Hertz.
#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

#define b3 247
#define d3 146
#define f3 174
#define g3 196
#define a3 220
#define aSH3 233
#define c4 261
#define cSH4 277
#define d4 293
#define dSH4 311
#define e4 329
#define f4 349
#define fSH4 370
#define g4 392
#define gSH4 415
#define a4 440
#define aSH4 466
#define b4 493
#define d5 587
#define c5 523
#define cSH5 554

#define SIXTEENTH 124
#define EIGHT (SIXTEENTH * 2)
#define DOTTED_EIGHT (EIGHT + SIXTEENTH)
#define QUARTER (EIGHT * 2)

#define MUSIC_PIN 0x02

void music_init()
{
    P1DIR |= MUSIC_PIN; // DATA line outputs from p1.0
}

static void start()
{
    P2DIR &=~(0x02 | 0x04);
}

static void stop()
{
    P2DIR |= (0x02 | 0x04);
}

void delay_ms(unsigned int ms)
{
    unsigned int i;
    for (i = 0; i <= ms; i++)
       __delay_cycles(1000L*16L); // multiplied by 16 due to 16 MHz
}

void delay_us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i<= us/2; i++)
       __delay_cycles(1*16L); // multiplied by 16 due to 16 MHz
}

// Generates a square wave of desired frequency to generate sound
void beep(unsigned int note, unsigned int duration)
{
    int i;
    long delay = (long)((10000L*62L)/note);  //This is the semiperiod of each note.
    long time = (long)(((long)(duration)*100L*16L)/(delay*2));  //This is how much time we need to spend on the note.
    for (i=0;i<time;i++)
    {
        P1OUT |= MUSIC_PIN;     //Set P1.2...
        delay_us(delay);   //...for a semiperiod...
        P1OUT &= ~MUSIC_PIN;    //...then reset it...
        delay_us(delay);   //...for the other semiperiod.
    }
    delay_ms(20); //Add a little delay to separate the single notes
}

//This is the Imperial March code.
//As you can see, there are lots of beeps at different frequencies and durations, and some delays to separate the various bits of this wonderful song.
void playImperialMarch()
{
	_BIC_SR(GIE);
	start();

    beep(a, 500);
    beep(a, 500);
    beep(a, 500);
    beep(f, 350);
    beep(cH, 150);
    beep(a, 500);
    beep(f, 350);
    beep(cH, 150);
    beep(a, 650);

    delay_ms(150);
    //end of first bit

    beep(eH, 500);
    beep(eH, 500);
    beep(eH, 500);
    beep(fH, 350);
    beep(cH, 150);
    beep(gS, 500);
    beep(f, 350);
    beep(cH, 150);
    beep(a, 650);

    delay_ms(150);
    //end of second bit...

    beep(aH, 500);
    beep(a, 300);
    beep(a, 150);
    beep(aH, 400);
    beep(gSH, 200);
    beep(gH, 200);
    beep(fSH, 125);
    beep(fH, 125);
    beep(fSH, 250);

    delay_ms(250);

    beep(aS, 250);
    beep(dSH, 400);
    beep(dH, 200);
    beep(cSH, 200);
    beep(cH, 125);
    beep(b, 125);
    beep(cH, 250);

    delay_ms(250);

    beep(f, 125);
    beep(gS, 500);
    beep(f, 375);
    beep(a, 125);
    beep(cH, 500);
    beep(a, 375);
    beep(cH, 125);
    beep(eH, 650);

    //end of third bit... (Though it doesn't play well)
    //let's repeat it

    beep(aH, 500);
    beep(a, 300);
    beep(a, 150);
    beep(aH, 400);
    beep(gSH, 200);
    beep(gH, 200);
    beep(fSH, 125);
    beep(fH, 125);
    beep(fSH, 250);

    delay_ms(250);

    beep(aS, 250);
    beep(dSH, 400);
    beep(dH, 200);
    beep(cSH, 200);
    beep(cH, 125);
    beep(b, 125);
    beep(cH, 250);

    delay_ms(250);

    beep(f, 250);
    beep(gS, 500);
    beep(f, 375);
    beep(cH, 125);
    beep(a, 500);
    beep(f, 375);
    beep(cH, 125);
    beep(a, 650);
    //end of the song
    stop();
    _BIS_SR(GIE);
}

void playMeglovania()
{
	_BIC_SR(GIE);
	start();
	// https://pianoletternotes.blogspot.com/2017/10/megalovania-undertale-theme.html
	// 1
	beep(d4,SIXTEENTH);
	beep(d4,SIXTEENTH);
	beep(d5,EIGHT);
	beep(a4,DOTTED_EIGHT);
	beep(gSH4,EIGHT);
	beep(g4,EIGHT);
	beep(f4,EIGHT);
	beep(d4,SIXTEENTH);
	beep(f4,SIXTEENTH);
	beep(g4,SIXTEENTH);
	beep(c4,SIXTEENTH);
	beep(c4,SIXTEENTH);
	beep(d5,EIGHT);
	beep(a4,DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);

	// 2
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f, SIXTEENTH);
	beep(g, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(d5, EIGHT);

	// 3
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);

	// 4
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(c4, SIXTEENTH);
	beep(c4, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);

	// 5
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(d4, SIXTEENTH);

	// 6
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(c4, SIXTEENTH);
	beep(c4, SIXTEENTH);
	beep(d4, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);

	// 7
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);

	// 8
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);

	// 9
	beep(c4, SIXTEENTH);
	beep(c4, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(b3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);

	// 10
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(aSH3, SIXTEENTH);
	beep(d5, EIGHT);
	beep(a4, DOTTED_EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, SIXTEENTH);
	beep(f4, EIGHT);
	beep(f4, SIXTEENTH);
	beep(f4, EIGHT);

	// 11
	beep(f4, EIGHT);
	beep(f4, EIGHT);
	beep(d4, EIGHT);
	beep(d4, QUARTER);
	beep(d4, EIGHT);
	beep(f4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(f4, EIGHT);
	beep(g4, EIGHT);
	beep(gSH4, EIGHT);
	beep(g4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(d4, SIXTEENTH);
	beep(f4, SIXTEENTH);
	beep(g4, QUARTER);

	// 12
	beep(f4, EIGHT);
	beep(f4, SIXTEENTH);
	beep(f4, EIGHT);
	beep(g4, EIGHT);
	beep(gSH4, EIGHT);
	beep(a4, EIGHT);
	beep(c5, EIGHT);
	beep(a4, QUARTER);
	beep(d5, EIGHT);
	beep(d5, EIGHT);
	beep(d5, SIXTEENTH);
	beep(a4, SIXTEENTH);
	beep(d5, SIXTEENTH);
	beep(c5, QUARTER*4);

	stop();
	_BIS_SR(GIE);
}

void playDubstepFart()
{
	_BIC_SR(GIE);
	start();
	beep(16, QUARTER);
	beep(32, QUARTER);
	beep(23, QUARTER);
	beep(40, QUARTER);
	beep(23, EIGHT);
	beep(16, EIGHT);
	beep(50, QUARTER);
	stop();
	_BIS_SR(GIE);
}

