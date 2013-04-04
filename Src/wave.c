/* $Header: /home/cvs/FTPhone/Src/wave.c,v 1.6 2004/01/06 01:31:54 kuba Exp $ 
 * $Log: wave.c,v $
 * Revision 1.6  2004/01/06 01:31:54  kuba
 * *** empty log message ***
 *
 * Revision 1.5  2004/01/06 01:30:14  kuba
 * changes in 57, 78 line WAV_MONO -> WAV_STEREO
 *
 */

#include "wave.h"


#define MAXFILES 32

int verbose=0;
char *PRG=0;

FILE *openfile[MAXFILES];
int openfiles = 0;


const char *ERRMSG[ERRMSGS] =
{
  "Terminated without errors",
  "Terminated due to previous errors",
  "Invalid time specification",
  "Invalid frequency specification",
  "Invalid parameter",
  "Invalid Wave-File",
  "Unsupported Wave-File-Format",
  "Read Error",
  "Write Error",
  "Out of Memory",
  "Invalid syntax",
  "Too many arguments"
};


void readheader(wavfile *w)
{
  LONG l;
  LONG lriff=0,llist=0;
  /*   LONG lfmt=0; */
  /*   LONG ldata=0; */
  LONG param[7];
  
  fgetlong(&l,w->f); 
  if(l!=WAV_RIFF) errexit(ERR_FORMAT);
  fgetlong(&lriff,w->f);
  fgetlong(&l,w->f);
  if(l!=WAV_WAVE) errexit(ERR_FORMAT);
  fgetlong(&l,w->f);
  if(l==WAV_LIST)
  {
    fgetlong(&llist,w->f);
    if(fseek(w->f,llist,1)) errexit(ERR_READ);
    fgetlong(&l,w->f);
  };
  if(l!=WAV_FMT) errexit(ERR_FORMAT);
  if(fread(param,4,7,w->f)!=7 || ferror(w->f)) errexit(ERR_READ);
  if(param[0]!=16 || param[5]!=WAV_DATA) errexit(ERR_FORMAT);
  /* if(param[1]!=WAV_MONO) errexit(ERR_UNSUPP); */
  w->rate=param[2];
  w->bytes=(param[4] & 0xffff);
  if(w->bytes>sizeof(LONGINT)) errexit(ERR_UNSUPP);
  w->bits=(param[4] >> 16);
  w->n=(param[6]/w->bytes);
  w->i=0;
  w->overflow=w->underflow=0;
}

void writeheader(wavfile *w)
{
  LONG l;
  LONG h[11];

  l=w->n*w->bytes;
  h[0]=WAV_RIFF;
  h[1]=l+36;
  h[2]=WAV_WAVE;
  h[3]=WAV_FMT;
  h[4]=16;
  /*   h[5]=WAV_MONO; */
  h[5]=WAV_STEREO;
  h[6]=w->rate;
  h[7]=w->rate*w->bytes;
  h[8]=(w->bits<<16)|w->bytes;
  h[9]=WAV_DATA;
  h[10]=l;
  if(fwrite(h,4,11,w->f)!=11 || ferror(w->f)) errexit(ERR_READ);
  w->i=0;
  w->overflow=w->underflow=0;
}  

void wavstdin(wavfile *w)
{
  w->f=stdin;
  readheader(w);
}

void wavfin(wavfile *w,char *n)
{
  w->f=fileopen(n,"rb",".wav");
  readheader(w);
}

void wavstdout(wavfile *w)
{
  w->f=stdout;
  writeheader(w);
}

void wavfout(wavfile *w,char *n)
{
  w->f=fileopen(n,"wb",".wav");
  writeheader(w);
}

FILE *fileopen(char *n,char *mod,char *ext)
{
  FILE *f;
  char s[128];

  f=fopen(n,mod);
  if(!f)
  {
    if(ext)
    {
      strcpy(s,n);
      strcat(s,ext);
      if(!(f=fopen(s,mod)))
      {
        fprintf(stderr,"%s: Can neither open file %s nor %s\n",PRG,n,s);
        errexit(ERR_PREV);
      };
    } else {
      fprintf(stderr,"%s: Cannot open file %s\n",PRG,n);
      errexit(ERR_PREV);
    };
  };
  openfile[openfiles]=f;
  openfiles++;
  return f;
} 

LONG wavbinget(wavfile *w)
{
  LONG l=0;
  char b;
  LONG i;

  for(i=0; i< w->bytes; i++)
  {
    b=getc(w->f);
    if(b == EOF) errexit(ERR_READ);
    l|=(b&255)<<(8*i);
  };
  w->i++;
  return l;
}

void wavbinput(wavfile *w, LONG l)
{
  LONG v=l;
  LONG b,i;
  
  for(i=0;i<w->bytes;i++)
  {
    b=v&255; v=v>>8;
    if(putc(b,w->f)==EOF) errexit(ERR_WRITE);
  };
  w->i++;
}

VAL wavget(wavfile *w)
{
  VAL x;
  LONG l;
  LONG p = 1<<(w->bytes*8-1);

  l=wavbinget(w);
  if(w->bytes==1)
  {
    x=l/((VAL)p)-1.;
  } else {
    if(l&p)
    {
      l=p-(l^p);
      x=-(VAL)l/((VAL)p);
    } else {
      x=l/((VAL)p);
    };
  };
  return x;
}

void wavput(wavfile *w, VAL x)
{
  /* VAL y; */
  LONG l;
  LONG p = 1<<(w->bytes*8-1);

  if(x>=1.)
  {
    x=MAXAMPL;
    if(!w->overflow)
    {
      w->overflow=1;
      if(verbose) fprintf(stderr,"Overflow: maximum value set.\n");
    };
  }
  else if(x<-1.)
  {
    x=-MAXAMPL;
    if(!w->underflow)
    {
      w->underflow=1;
      if(verbose) fprintf(stderr,"Underflow: minimum value set.\n");
    };
  };
  l=(LONG) floor(w->bytes==1 ? x*p+p : x*p);
  wavbinput(w,l);
}

int getparam(char *s,REAL *v,int msk)
{
  char *p,*q;
  char c;
  REAL x;
  int u = 0;

  x=strtod(s,&p);
  if(p==s) errexit(ERR_PARAM);
  c=*p;
  p++;
  switch(c)
  {
    case 'K': x*=1024.; break;
    case 'k': x*=1000.; break;
    case 'd': x*=0.1  ; break;
    case 'c': x*=0.01 ; break;
    case 'm': x*=0.001; break;
    default : p--;
  };
  q=strstr(" s Hz hz h f B b % rad eg m pt ",p);
  if(!q && *p!=0) errexit(ERR_PARAM);
  switch(*q)
  {
  case ' ': u=U_NU;    break;
    case 's': u=U_SEC;   break;
    case 'H': ;
    case 'h': u=U_HZ;    break;
    case 'f': u=U_HZ;    x*=11025.;    break;
    case 'B': ;
    case 'b': u=U_GAIN;  x=pow(10.,x); break;
    case '%': u=U_GAIN;  x*=0.01;      break;
    case 'r': u=U_PHASE; x/=2*PI;      break;
    case 'e': u=U_PHASE; x/=36.;       break;
    case 'm': u=U_METER; break;
    case 'p': u=U_PT;    break;
    default : errexit(ERR_PARAM);
  };
  if( (u & msk) == 0 ) errexit(ERR_PARAM);
  *v=x;
  return u;
}

REAL gettime(char *s)
{
  REAL t;

  if(getparam(s,&t,U_ALL)!=U_SEC) errexit(ERR_TIME);
  return t;
}

REAL getfreq(char *s)
{
  REAL f;

  if(getparam(s,&f,U_ALL)!=U_HZ) errexit(ERR_TIME);
  return f;
};

int stdopt(char c,char *h)
{
  switch(c)
  {
    case 'v': verbose=1; return 0;
    case 'h': if(h) fprintf(stderr,h,PRG); errexit(0);
    case ':': ;
    case '?': if(h) fprintf(stderr,"Type %s -h for help.\n",PRG);
              errexit(ERR_PREV);
    case 127:
    case -1 : return 1;
  };
  return 0;
};

void setexitmsg(char *prg)
{
  PRG=prg;
};
  
void errexit(int err)
{
  int i;
  for(i=0;i<openfiles;i++) if(openfile[i]) fclose(openfile[i]);
  if(err>ERR_PREV || verbose) fprintf(stderr,"%s: %s.\n",PRG,ERRMSG[err]);
  exit(err);
}


