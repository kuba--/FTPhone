#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <sys/soundcard.h>

#define DSPDEV "/dev/dsp"
#define MIXDEV "/dev/mixer"

int opendsp(int bits,unsigned long rate,int stereo, int channels, int mode);

