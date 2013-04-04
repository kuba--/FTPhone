/**
   @file Base.cxx   
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>   
   @brief Sources of base class 

   $Header: /home/cvs/FTPhone/Src/Base.cxx,v 1.32 2004/02/25 22:45:22 kuba Exp $
*/



#include "Base.hxx"

#include <getopt.h>

#define DEFAULT_MY_NAME "kuba"

// #define DEFAULT_FRIEND_NAME "agatag"
// #define DEFAULT_FTP_SERVER "dischaos.com"

#define DEFAULT_FRIEND_NAME "cunix"
#define DEFAULT_FTP_SERVER "alan.umcs.lublin.pl"

#define DEFAULT_REMOTE_DIR "/tmp/"

#define PUT_MODE -1
#define GET_MODE  1


/*
******************************************************************************
*                               P U B L I C
******************************************************************************
*/

TBase::TBase(int Argc, char *Argv[])
	:sMyName(NULL),
	 sMyPass(NULL),
	 sFriendName(NULL),
	 sFtpServerName(NULL),
	 sRemoteDir(NULL),
	 ulGetCnt(0),
	 ulPutCnt(0),
	 pIFace(NULL),
	 getNetbuf(NULL),
	 putNetbuf(NULL)
{
	GetOpt(Argc, Argv);
	FtpInit();
}
//----------------------------------------------------------------------------


TBase::~TBase()
{  
  

	if ( pIFace ) {  
		delete pIFace;
	}
  

	if ( getNetbuf ) {
		FtpQuit( getNetbuf );
		getNetbuf = NULL;
	}

	if ( putNetbuf ) {
		FtpQuit( putNetbuf );
		putNetbuf = NULL;
	}

	fprintf(stdout, "\tQUIT [remove remote and history files]\n");  
	fprintf(stdout, "\tQUIT [press ^C]\n");

	sys_tools::CreateTemplate(sFtpServerName, sMyName, sMyPass);
	sys_tools::ClearAll(sRemoteDir);
  
	if ( sMyName) free(sMyName);
	if ( sMyPass) free(sMyPass);
	if ( sFriendName) free(sFriendName);
	if ( sFtpServerName) free(sFtpServerName);
	if ( sRemoteDir) free(sRemoteDir);  
}
//----------------------------------------------------------------------------


void TBase::Run()
{
	TRACE("R U N");
	signal(SIGINT, SigHandler);
 
	if ( Login() ) { 
		
// 		sys_tools::CreateTemplate(sFtpServerName, sMyName, sMyPass);
// 		sys_tools::ClearAll(sRemoteDir);

		pIFace = new TInterface( FULL_APP_NAME, 
								 sMyName, 
								 sFriendName, 
								 sFtpServerName, 
								 sRemoteDir );   		
		pid_t pid;
		int status;

		switch(pid = fork()) {

		case -1: 
			TRACE("fork()");      
			break;
      
		case 0:     
			Play();
			break;
    
		default:      
			Record();
			kill(pid, SIGINT);
			waitpid(pid, &status, WUNTRACED);
			break;
		}   
	}
	return;
}
//----------------------------------------------------------------------------

/*
******************************************************************************
*                               P R O T E C T E D
******************************************************************************
*/

void TBase::GetOpt(int Argc, char *Argv[])
{
	int opt;

	while( (opt = getopt(Argc, Argv, "m:f:s:d:h")) != -1 ) 
    {
		switch(opt) {
		case 'm':
			sMyName = strdup(optarg);      
			break;
		case 'f':
			sFriendName = strdup(optarg);
			break;
		case 's':
			sFtpServerName = strdup(optarg);
			break;
		case 'd':
			sRemoteDir = strdup(optarg);
			break;
		case 'h':
			Usage(Argv[0]);
			exit(EXIT_FAILURE);
			break;
		case ':': // without value   
			break;     
		default: // unknown opt
			Usage(Argv[0]);
			exit(EXIT_FAILURE);
		}
    }
  
	if ( ! sMyName ) sMyName = strdup(DEFAULT_MY_NAME);
	if ( ! sFriendName ) sFriendName = strdup(DEFAULT_FRIEND_NAME);
	if ( ! sFtpServerName ) sFtpServerName = strdup(DEFAULT_FTP_SERVER);
	if ( ! sRemoteDir ) sRemoteDir = strdup(DEFAULT_REMOTE_DIR);    
}
//----------------------------------------------------------------------------


void TBase::Usage(const char *AppName)
{
	printf("\n%s [-m my user name] [-f friend user name] [-s ftp server address] [-d remote dir]\n", 
		   AppName);
	printf("default values:\n");
	printf("\tmy user name on ftp server: %s\n",       DEFAULT_MY_NAME);
	printf("\tfriend user name on ftp server: %s\n",   DEFAULT_FRIEND_NAME);  
	printf("\tftp server address: %s\n", DEFAULT_FTP_SERVER);
	printf("\tremote dir: %s\n",         DEFAULT_REMOTE_DIR);
	printf("\n\neg.:%s -m kuba -f john -s ftp.server.net -d /tmp/\n\n", AppName);
}
//----------------------------------------------------------------------------


#define MAX_PASS_LEN 32

bool TBase::Login()
{
	bool IsOK(false);
  
	fprintf(stdout, "Logging in...\n\tPassword requested by %s for user '%s'.\n\n", sFtpServerName, sMyName);
	sMyPass = (char *) malloc((MAX_PASS_LEN + 1)* sizeof(char));

	if ( IsOK = ( strncpy(sMyPass, getpass("Password: "), MAX_PASS_LEN) ) != NULL ) {
    
		IsOK = FtpConnect( sFtpServerName, &putNetbuf ) && FtpLogin(sMyName, sMyPass, putNetbuf);
		if ( IsOK ) {
			FtpConnect(sFtpServerName, &getNetbuf ) && FtpLogin(sMyName, sMyPass, getNetbuf);
			FtpOptions(FTPLIB_CONNMODE, FTPLIB_PORT, getNetbuf);  
		}
	}

	if( ! IsOK ) 
		fprintf(stdout, "Login\t[ F A I L E D ]\n");    
	else 
		fprintf(stdout, "Login\t[ O K ]\n");    

	return IsOK;
}
//----------------------------------------------------------------------------


bool TBase::TransferFile( const char *local_path, const char *remote_path, const int ftp_mode )
{
	bool IsOK(false);
  
	if ( ftp_mode == PUT_MODE ) {      
		TRACE("PUT_MODE");
		
		if ( IsOK = FtpPut( local_path, TRANSFER_TMP_FILE(remote_path), 
							FTPLIB_BINARY, putNetbuf ) != 0 ) 
		{
			IsOK = FtpRename(TRANSFER_TMP_FILE(remote_path), remote_path, putNetbuf) == 1;
			
			TRACE("FtpPut:OK:");
		}
		else TRACE("FtpPut:FAILED:");
    
	}
	else if ( ftp_mode == GET_MODE ) {
		TRACE("GET_MODE");

		TRACE("FtpAccess -> (" << remote_path << ")");
		netbuf *Data = NULL;
		do {	
			IsOK = FtpAccess(remote_path, FTPLIB_FILE_READ, FTPLIB_BINARY, getNetbuf, &Data) == 1;
			if ( ! IsOK ) { pIFace->SetStatus(STATUS_SLEEP); perror("*"); sleep(IDLE_SEC); }
		} while( ! IsOK );
		FtpClose(Data);  
		TRACE("FtpAccess <- ");

		pIFace->SetStatus(STATUS_GET);
		pIFace->SetAction( pIFace->getPlayWin(), remote_path );		
		if ( IsOK = FtpGet( local_path, remote_path, FTPLIB_BINARY, getNetbuf ) != 0 ) {
			TRACE("FtpGet:OK:");
		}
		else TRACE("FtpGet:FAILED:");
		pIFace->SetAction( pIFace->getPlayWin(), (const char *) NULL);
	}
	TRACE("Transfer File return:" << IsOK);
	return IsOK;
}
//----------------------------------------------------------------------------


void TBase::Gsm2Wav(const char *file_name)
{
	perror("G S M 2 W A V");

	struct stat st;
	LONGINT i, j;
  
	gsm GsmHndl;
	gsm_frame GsmBuf;
	gsm_signal GsmSig[160];
  
	wavfile  WavIn; ///< gsm file
	int Dsp;        ///< DSP DEVICE
	
	if ( lstat(file_name, &st) == -1 ) {
		TRACE("stat()"); 
	}
	else {
    
		WavIn.f=fileopen((char *)file_name, "r", NULL);
    
		while( ! (Dsp = opendsp(DEFBITS, DEFRATE, STEREO, DEFBYTES, O_WRONLY)) ) 
			;
		
		if ( !(GsmHndl = gsm_create()) ) {
			TRACE("gsm_create()");	    
		}
		else {
      
			for( i=0; ! feof(WavIn.f); i++) {
				j = fread(GsmBuf, sizeof(GsmBuf), 1, WavIn.f);
	  
				if( j != 1 ||  gsm_decode(GsmHndl, GsmBuf, GsmSig) < 0) {
					TRACE("Error decoding input file.\n");	    
				}
				else {
					write(Dsp, &GsmSig, 320);
					fsync(Dsp);
				}	         
			}
		}

		close(Dsp);
		fclose(WavIn.f);    
	}
  
	return ;
}
//----------------------------------------------------------------------------


void TBase::Play( void )
{
	TRACE("P L A Y");
   
  
	char get_file[256];

	string HistoryPath;
	string RemotePath;
  
  

	pIFace->SetStatus(STATUS_SLEEP);
  
	while( true ) {
		TRANSFER_FILE( sFriendName, ulGetCnt, get_file );
		HistoryPath =  string(HISTORY_DIR)   + string(get_file);
		RemotePath  =  string(sRemoteDir) + string(get_file); 
     
		if ( ! TransferFile( HistoryPath.c_str(), RemotePath.c_str(), GET_MODE) )
		{
			// 	 pIFace->SetStatus(STATUS_SLEEP);
		}
		else {
			// we GOT file ;)
			// let's play

			pIFace->SetStatus(STATUS_PLAY);			
			
			TRACE( "P ->" );      
			Gsm2Wav((char *)HistoryPath.c_str());
			TRACE( "<- P" );
       
			// wait for next msg
			// pIFace->SetStatus(STATUS_GET);
		}

		ulGetCnt++;
	}

	return;
}

//----------------------------------------------------------------------------


void TBase::Wav2Gsm(const char *arg)
{
	perror("W A V 2 G S M");
  
	bool stop(false);  
	char put_file[256];

	string HistoryPath;
	string RemotePath;
  
	gsm GsmHndl;
	gsm_frame GsmBuf;
	gsm_signal GsmSig[160];
  
	wavfile  WavIn;  ///< microfone
	wavfile  WavOut; ///< recorded file
  
	int Dsp; ///< DSP DEVICE

	WavIn.rate = DEFRATE;
	WavIn.n = INT_MAX;
	//WavIn.bytes = DEFBYTES;
  
	if ( ! (GsmHndl = gsm_create()) ) {
		TRACE("gsm_create()");
		stop = true;
		exit(EXIT_FAILURE);
	}

	//       we start recording !
	TRACE("opendsp ->");
      
	//       open DEVICE
	while( ! (Dsp = opendsp(DEFBITS, WavIn.rate, STEREO, DEFBYTES, O_RDONLY)) )
		;

	TRACE("<- opendsp");
  
	TRANSFER_FILE(sMyName, ulPutCnt, put_file);
	HistoryPath = string(HISTORY_DIR)   + string(put_file);
	RemotePath  = string(sRemoteDir) + string(put_file); 

	pIFace->SetAction( pIFace-> getRecWin(),  RemotePath.c_str());

	TRACE("fileopen ->");
	WavOut.f = fileopen((char *)HistoryPath.c_str(), "wb", NULL);  
	TRACE("<- fileopen");
  
	TRACE("gsm_encode ->");
	//       record WAV file
	for( WavIn.i = 0; ( toupper(getch()) != 'S' ) && ( WavIn.i < WavIn.n );  ) 
    {
		WavIn.i += read(Dsp, GsmSig, 320) / 2;
      
		// converting to GSM
		gsm_encode(GsmHndl, GsmSig, GsmBuf);
		fwrite((char *)GsmBuf, sizeof(GsmBuf), 1, WavOut.f);
    }   

	TRACE("<- gsm_encode");

	fclose(WavOut.f);
	close(Dsp);      
     
	gsm_destroy(GsmHndl);
	pIFace->SetAction( pIFace-> getRecWin(),  (const char *)NULL);	
	pIFace->SetStatus(STATUS_STOP);
  
	if ( ! TransferFile(HistoryPath.c_str(), RemotePath.c_str(), PUT_MODE) )
    {      
		TRACE("TransferFile():FAILED:");      
    }
	else {
		ulPutCnt++; 
	}
  
	return;
}

//----------------------------------------------------------------------------

void TBase::Record( void )
{
	TRACE("R E C O R D");

	bool stop(false);
	
	pIFace->SetStatus(STATUS_STOP);

	while( ! stop ) {
    
		switch( getch() ) {
		case 'r': case 'R':
			// Record file and put it on server
			TRACE("Rec ->");
			pIFace->SetStatus(STATUS_REC);
			Wav2Gsm(NULL);

			TRACE("<- Rec");
			break;
   
		case 's': case 'S':
			TRACE("Stop ->");
			// Stop recording
			pIFace->SetStatus(STATUS_STOP);
			TRACE("<- Stop");
			break;

		case 'q': case 'Q':
			// Quit
			TRACE("QUIT");

			stop = true;
			break;
		}

		usleep(100);
	}

	return;
}
//----------------------------------------------------------------------------
