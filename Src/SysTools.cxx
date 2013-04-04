/**
   @file SysTools.cxx   
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
   @brief Sources of system tools/functions
*/

#include "SysTools.hxx"

#include "Debug.hxx"

#include <unistd.h>

#include <string>
#include <fstream>

using namespace std;

/**
   my_system from man system
   this same like system(const char *), but better ;-)
   @return 
   -1: perror("fork()"); 
   0: if @param  cmd == NULL; 
   status getting from waitpid
*/
int System( std::string cmd )
{
	pid_t pid, status;
  
	if ( cmd != "" ) {
    
		switch( pid = fork() ) {
		case -1: 
			TRACE("fork");
			return -1;
			break;
   
		case 0:
			char *argv[4];
			argv[0] = "sh";
			argv[1] = "-c";
			argv[2] = (char *)cmd.c_str();
			argv[3] = NULL;
			execve("/bin/sh", argv, environ);
			exit(127);
			break;
      
		default:
			do {
				if (waitpid(pid, &status, 0) == -1) {
					if (errno != EINTR) {
						TRACE("return -1");
						return -1;
					}
				} else {
					TRACE("return " << status);
					return status;
				}
			} while(1);
		}
    
	}
	TRACE("return 0");
	return 0;
}
//----------------------------------------------------------------------------

// NAMESPACE SYS_TOOLS
//----------------------------------------------------------------------------
namespace sys_tools 
{

	bool CreateTemplate( const char *server, const char *my_name, const char *my_pass )
	{
		bool IsOK(false);
		string buf = (string)"open " + (string)server + (string)"\n\nuser " + (string)my_name + (string)"  " + (string)my_pass ;
		buf += "\n\nbinary\n\n";
    
		int fd = creat(FTP_TEMPLATE_FILE, 0600);
		if ( ! (IsOK = write(fd, buf.c_str(), buf.size()) != -1) ) {
			TRACE("write()");
		}
		close(fd);

		return IsOK;
	}
//----------------------------------------------------------------------------

	
	bool DeleteTemplate( ) 
	{
		bool IsOK(false); 
		IsOK = (unlink(FTP_TEMPLATE_FILE) == 0) && (unlink(FTP_MDELETE_FILE) == 0);
  
		if ( ! IsOK ) {
			TRACE("unlink()");
		}
		return IsOK;  
	}
//----------------------------------------------------------------------------


	bool DeleteRemoteFiles( const char *remote_dir, const char *ext = "*.gsm" )
	{
		bool IsOK(false);
		if ( IsOK = System("cp -f " + (string)FTP_TEMPLATE_FILE + " " + (string)FTP_MDELETE_FILE) >= 0 ) {
    
			ofstream Out(FTP_MDELETE_FILE, ios::out | ios::app);
			Out << "cd " << remote_dir << endl << endl;
			Out << "mdelete " << ext << endl << endl;
			Out << "bye" << endl << endl;
			Out.close();

			IsOK = System( FTP_COMMAND(FTP_MDELETE_FILE) ) >= 0;
		}
		LOCV("return " << IsOK);
		return IsOK;
	}
//----------------------------------------------------------------------------


	int ClearHistory( const char *ext /*= "*.gsm"*/ )
	{
		int ret = System("rm -fr " + (string)HISTORY_DIR + (string)ext);  

		LOCV("ret:" << ret);
		return ret;
	}
//----------------------------------------------------------------------------


	bool ClearAll( const char *remote_dir, const char *ext /* = "*.gsm"*/ )
	{
		return  DeleteRemoteFiles(remote_dir, ext) && DeleteTemplate() && ClearHistory(ext) > 0 ;  
	}
//----------------------------------------------------------------------------

//____________________________________________________________________________
/**
   Catch SIGINT (^C) signal 
*/
	void SigHandler( int arg )
	{
  
		TRACE("S I G I N T");
		DeleteTemplate();
		exit(EXIT_SUCCESS);
	}
//----------------------------------------------------------------------------
};
//----------------------------------------------------------------------------
