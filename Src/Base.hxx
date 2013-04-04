/**
   @file Base.hxx
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
   @brief Header for base class
   $Header: /home/cvs/FTPhone/Src/Base.hxx,v 1.22 2004/02/25 22:45:52 kuba Exp $
*/

#ifndef BASE_HXX
#define BASE_HXX


#ifndef _GNU_SOURCE 
#define _GNU_SOURCE
#endif

#include <ctype.h>

#include <signal.h>

extern "C"{
#include <gsm-1.0-pl10/inc/gsm.h>
};

#include <wave.h>
#include <defaults.h>
#include <dsp.h>

#include <ftplib.h>
#define _FTPLIB_NO_COMPAT

#include <Debug.hxx>
#include <SysTools.hxx>
#include <Interface.hxx>

/** @param TIMEOUT_MSEC (in mili seconds)
    @brief how long sleep between ftp-actions (getting, putting files) 
    @example Base.cxx
*/
#define TIMEOUT_MSEC       1000
#define IDLE_SEC           2



#include <string>
using namespace std;

using namespace sys_tools;

#define TRANSFER_FILE( PREFIX, N, FILE_NAME ) sprintf(FILE_NAME, "%s_%ld.gsm", PREFIX, N)
#define TRANSFER_TMP_FILE( FILE_NAME ) ((string)( FILE_NAME ) + (string)".tmp").c_str()

#define INFO( MIN, SEC, INFO_MSG) sprintf(INFO_MSG, "Playing - Total Time: %d min.   %d sec.", MIN, SEC)

/**
   @class TBase: based class for speakers and transfering files
*/
class TBase
{
public:
  
	/**
	   Constructor
	*/
	TBase(int Argc, char *Argv[]);
  

	/**
	   Destructor
	*/
	virtual ~TBase();
  

	/**
	   Run whole program
	*/
	void Run();


protected:
	// It's me
	char *sMyName;         ///< my user name on FTP server  
	char *sMyPass;         ///< my password on FTP server

	// I'd like to talk with....
	char *sFriendName;     ///< mine friend user name on FTP server

	// Our server
	char *sFtpServerName;  ///< FTP server address

	char *sRemoteDir;      ///< Remote dir (on FTP server) for exchanging voice files

	unsigned long ulGetCnt; ///< Getting file counter (how many files i've got from server)
	unsigned long ulPutCnt; ///< Putting file counter (how many files i've put on server)

	TInterface *pIFace;    ///< Interface pointer
  
	netbuf *getNetbuf;
	netbuf *putNetbuf;  
  


	/**
	   GetOpt works like getopt
	   filling @see sMyName, @see sFriendName, @see sFtpServerName, @see sRemoteDir
	*/
	void GetOpt(int Argc, char *Argv[]);


	/**
	   How does it works
	   @param AppName : application name
	*/
	void Usage(const char *AppName);
  

	/**
	   Login on FTP server
	   @return TRUE if success other FALSE
	*/
	virtual bool Login();


	/**
	   Transfer (put, get) File (gsm file)
	   @param local_path: local file to transfer
	   @param remote_file: name of remote file
	   @param ftp_mode: possibility values PUT_MODE( put file on server), 
	   GET_MODE( get file from server)
	   @return TRUE if everything OK ; else FALSE 
	*/
	virtual bool TransferFile( const char *local_path, const char *remote_path, const int ftp_mode );


	/**
	   Convert GSM file @param arg to WAV file
	*/
	void Gsm2Wav(const char *arg);
    
	/**
	   Get file .gsm and play it
	*/
	virtual void Play( void );
  

	/**
	   Convert WAV file (creating from /dev/dsp) to GSM file @param arg
	*/
	void Wav2Gsm(const char *arg);

	/**
	   Record file .gsm and put it on FTP server
	*/
	virtual void Record( void );

};
#endif
