/**
   @file SysTools.hxx   
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
   @brief This file contains some functions based on System function
*/
/*   
   $Log: SysTools.hxx,v $
   Revision 1.5  2004/02/09 00:11:20  kuba
   *** empty log message ***

*/

#ifndef SYS_TOOLS_HXX
#define SYS_TOOLS_HXX

extern char **environ;
extern char *optarg;
extern int optind, opterr, optopc;

#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define FTP_COMMAND( F )  ( "ftp -n < " + (string) F ).c_str()
#define FTP_TEMPLATE_FILE "ftp.cfg"
#define FTP_MDELETE_FILE  "ftp.del"

#define HISTORY_DIR "History/"

namespace sys_tools 
{
  
  //   int System( string cmd );
  
  /**
     Create template file for FTP_COMMAND
  */
  bool CreateTemplate( const char *server, const char *my_name, const char *my_pass );


  /**
     Delete template file with FTP COMMANDS
  */
  bool DeleteTemplate(  );


  /**
     Delete remote files ( all files from server directory @dir with extension @param ext )
     @return TRUE everything OK else FALSE
  */
  bool DeleteRemoteFiles( const char *remote_dir, const char *ext );

  /**
    Delete all files from directory @see HISTORY_DIR with extension @param ext 
  */
  int ClearHistory( const char *ext = "*.gsm" );

  /**
     Clearing - DeleteTemplate, DeleteRemoteFiles, ClearHistory
  */
  bool ClearAll( const char *remote_dir, const char *ext = "*.gsm" );

  /**
     Catch SIGINT (^C) signal 
  */
  void SigHandler( int arg );
};

#endif
