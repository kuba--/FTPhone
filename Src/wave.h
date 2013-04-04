#ifndef _WAVE_H
#define _WAVE_H 1

#if defined (__GNUC__)
  #define _GCC_ 1		/* GNU Compiler (UNIX) */
#elif defined (__WATCOMC__)
  #define _WATCOM_ 1		/* Watcom Compiler (MSDOS) */
#else
  #define _GCC_ 1		/* default */ 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define BYTE unsigned char
#if USHRT_MAX==0xffff
#define WORD unsigned short int
#define INTEGER signed short int
#else
#define WORD unsigned int
#define INTEGER signed int
#endif
#define LONG unsigned long
#define LONGINT signed long 
#define REAL double
#define VAL  float

#define WAV_RIFF   0x46464952	/* "RIFF" */
#define WAV_WAVE   0x45564157	/* "WAVE" */
#define WAV_LIST   0x5453494c	/* "LIST" */
#define WAV_INFO   0x4f464e49	/* "INFO" */
#define WAV_ISFT   0x54465349	/* "ISFT" */
#define WAV_ICRD   0x44524349	/* "ICRD" */
#define WAV_FMT    0x20746d66	/* "fmt " */
#define WAV_DATA   0x61746164	/* "data" */
#define WAV_MONO   0x00010001	/* 0x0001 0x0001 */
#define WAV_STEREO 0x00020001	/* 0x0001 0x0002 */

#define dmp(x) fprintf(stderr,"[%d]",x)

#define U_NU     1
#define U_SEC    2
#define U_HZ     4
#define U_GAIN   8
#define U_PHASE 16
#define U_METER 32
#define U_PT    64
#define U_ALL  127

#define MAXAMPL 0.99999


#ifndef PI
  #define PI 3.14159265359
#endif


#define fgetbyte(b,f) if(fread(b,1,1,f)!=1) exit(ERR_READ)
#define fgetword(w,f) if(fread(w,2,1,f)!=1) exit(ERR_READ)
#define fgetlong(l,f) if(fread(l,4,1,f)!=1) exit(ERR_READ)

#define ERRMSGS 12

#define ERR_NONE    0
#define ERR_PREV    1
#define ERR_TIME    2
#define ERR_FREQ    3
#define ERR_PARAM   4
#define ERR_FORMAT  5
#define ERR_UNSUPP  6
#define ERR_READ    7
#define ERR_WRITE   8
#define ERR_MEM     9
#define ERR_SYNTAX 10
#define ERR_ARG    11

typedef struct
{
  FILE *f;
  LONG n;
  LONG i;
  LONG rate;
  LONG bits;
  LONG bytes;
  int  overflow;
  int  underflow;
} wavfile;


void readheader(wavfile *w);
void writeheader(wavfile *w);
void wavstdin(wavfile *w);
void wavfin(wavfile *w,char *n);
void wavstdout(wavfile *w);
void wavfout(wavfile *w,char *n);
FILE *fileopen(char *n,char *mod,char *ext);
LONG wavbinget(wavfile *w);
void wavbinput(wavfile *w, LONG l);
VAL wavget(wavfile *w);
void wavput(wavfile *w,VAL x);
int getparam(char *s,REAL *v,int msk);
REAL gettime(char *s);
REAL getfreq(char *s);
int stdopt(char c,char *h);
void setexitmsg(char *prg);
void errexit(int err);



#ifdef _WATCOM_
  int optind=0;
  char *optarg;
  char getopt(int argc,char **argv,char *opts)
  {
    char *p;
    char *q;
    char c;
  
    optind++;
    optarg=0;
    if(optind>=argc) return 127;
    p=argv[optind];
    if(p[0]=='-' && p[1])
    {
      p++;
      c=p[0];
      q=strchr(opts,c);
      if(q)
      {
        p++; q++;
        if(q[0] ==':')
        {
          if(!(p[0])) return ':';
          optarg=p;
          return c;
        };
        if(p[0]) return '?';
        return c;
      } else {
        return '?';
      };
    };
    return 127;
  };
#else
  extern int optind;
  extern char *optarg;
#endif

#ifdef _WATCOM_
  #define random() rand()
  #define rint(x) floor(x+0.5)
#endif

#ifdef _WATCOM_
  #include "wave.c"
#endif

#endif

