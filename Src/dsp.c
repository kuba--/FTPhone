/*
  $Header: /home/cvs/FTPhone/Src/dsp.c,v 1.7 2004/02/09 00:11:09 kuba Exp $
*/

#include "dsp.h"

void dsperr(char *s) {
  fprintf(stderr, s);
  perror(s);
  exit(1);
}


int opendsp(int bits,unsigned long rate,int stereo, int channels, int mode)
{
  int f;

  f=open(DSPDEV,mode,0);
  if(!f) dsperr("Can't open audio device " DSPDEV ".\n");
  /* ioctl(f, SNDCTL_DSP_RESET, 0); */
  
  if(ioctl( f, SNDCTL_DSP_SAMPLESIZE,&bits) < 0 )
    dsperr("Can't set bitrate.\n");
  
  if(ioctl(f,SNDCTL_DSP_STEREO, &stereo) < 0 )
    dsperr("Can't set mono/stereo mode.\n");
  
  if( ioctl(f, SNDCTL_DSP_CHANNELS, &channels) < 0 )
    dsperr("Can't set channels mode.\n");
  
  if(ioctl(f,SNDCTL_DSP_SPEED,&rate)<0)
    dsperr("Can't set audio sampling rate.\n");
  
  /* ioctl(f, SNDCTL_DSP_SYNC, 0); */
  
  /* int p = 1 << SOUND_MIXER_MIC; */
  
  /*   int mix_fd = open(MIXDEV, O_WRONLY, 0); */
  /*   if ( ioctl(mix_fd, SOUND_MIXER_WRITE_RECSRC, &p) < 0 ) */
  /*     dsperr("Can't open " MIXDEV ".\n"); */
  /*   close(mix_fd); */
  return f;
}

