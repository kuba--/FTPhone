#----------------------------------------------------------------------------- 	
# $Header: /home/cvs/FTPhone/Src/Makefile,v 1.14 2004/01/31 15:32:48 kuba Exp $
#		
# @file Makefile
# @brief This is Makefile for GNU/Linux; if you don't have it's pitty
# @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
# 
#-----------------------------------------------------------------------------
# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.
#-----------------------------------------------------------------------------

# MAKRA
OBJSUFF = .o
BASEDIR = ./
GSMDIR  = gsm-1.0-pl10/
GSMLIBDIR = $(GSMDIR)lib/

CC = gcc
CPP = g++
CFLAGS =  -Wall -O2 -I $(BASEDIR) 
LFLAGS =  -lncurses -L $(GSMLIBDIR) -lgsm -lm 

ALLOBJECTS = dsp$(OBJSUFF) wave$(OBJSUFF) ftplib$(OBJSUFF) Interface$(OBJSUFF) SysTools$(OBJSUFF)\
Base$(OBJSUFF)  Main$(OBJSUFF)  

EXENAME = ../a.out

all: GSM $(EXENAME)

GSM:
	cd $(GSMDIR) && $(MAKE)
	cd ..


$(EXENAME): $(ALLOBJECTS)
	$(CPP) $(CFLAGS) $(ALLOBJECTS)  -o $(EXENAME)  $(LFLAGS)


Interface$(OBJSUFF): Interface.cxx Interface.hxx
	$(CPP) $(CFLAGS) Interface.cxx -c -o Interface$(OBJSUFF)


SysTools$(OBJSUFF): SysTools.cxx SysTools.hxx Debug.hxx
	$(CPP) $(CFLAGS) SysTools.cxx -c -o SysTools$(OBJSUFF)


Base$(OBJSUFF): Base.cxx Base.hxx Interface.hxx SysTools.hxx Debug.hxx dsp.h wave.h ftplib.h defaults.h
	$(CPP) $(CFLAGS) Base.cxx -DDO_DEBUG -c -o Base$(OBJSUFF)


Main$(OBJSUFF): Main.cxx Base.hxx
	$(CPP) $(CFLAGS) Main.cxx -DSHOW_HEADER  -c -o Main$(OBJSUFF)


dsp$(OBJSUFF): dsp.c dsp.h
	$(CPP) $(CFLAGS)  dsp.c -c -o dsp$(OBJSUFF)

wave$(OBJSUFF): wave.c wave.h
	$(CPP) $(CFLAGS)  wave.c -c -o wave$(OBJSUFF)

ftplib$(OBJSUFF): ftplib.c ftplib.h
	$(CC) $(CFLAGS)  ftplib.c -c -o ftplib$(OBJSUFF)

doc:
	doxygen

clean:
	rm -f $(EXENAME)
	rm -f *$(OBJSUFF) *~
	cd $(GSMDIR) && $(MAKE) clean
	cd ..
