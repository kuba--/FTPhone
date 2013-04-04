/**
   @file Interface.hxx
   @author Kuba Podgórski
   @brief Interface header
*/

#ifndef INTERFACE_HXX
#define INTERFACE_HXX

#include <ncurses.h>

#define VERSION          0.1
#define VERSION_STR     "ver. 0.1"

#define APP_NAME        "FTPhone"

#define FULL_APP_NAME   ((string)(APP_NAME) + (string)"  " + (string)(VERSION_STR)).c_str()

// @brief Macros with collors definitions
//-----------------------------------------------------------------------------
#define WHITE_ON_BLUE     1 
#define WHITE_ON_GREEN    2
#define WHITE_ON_RED      3
#define WHITE_ON_BLACK    4

#define YELLOW_ON_BLUE    5

#define CYAN_ON_BLUE      6
#define CYAN_ON_BLACK     7

#define BLUE_ON_GREEN     8

#define GREEN_ON_BLACK    9

#define BLACK_ON_YELLOW  10

#define MAGENTA_ON_BLACK 11

#define RED_ON_WHITE     12
#define RED_ON_BLACK     13


// \brief Help Window
//-----------------------------------------------------------------------------
#define HELP_WIN_X       4
#define HELP_WIN_Y       1
#define HELP_WIN_W      70 
#define HELP_WIN_H       7

// \brief Playing Window
//-----------------------------------------------------------------------------
#define PLAY_WIN_X       5 ///< playing window start x
#define PLAY_WIN_Y      10 ///< playing windows start y
#define PLAY_WIN_W      70 ///< playing window width
#define PLAY_WIN_H       7 ///< playing window height

// \brif Recording Window
//-----------------------------------------------------------------------------
#define REC_WIN_X        5 ///< recording window start x
#define REC_WIN_Y       18 ///< recording window start y
#define REC_WIN_W       70 ///< recording window width
#define REC_WIN_H        7 ///< recording window height


#define NO_VISIBILITY    0 ///< without cursor char

#define STATUS_GET      -3 
#define STATUS_PLAY     -2 
#define STATUS_SLEEP    -1
#define STATUS_STOP      0
#define STATUS_REC       1


/**
   @class TInterface: creating interaface; 
   responsible for key events
*/
class TInterface 
{
public:

	
	/**
	   Constructor
	*/  
	TInterface( const char *app_name,         ///< full application name
				const char *my_name,          ///< my user name 
				const char *my_friend_name,   ///< my friend user name
				const char *server_name,      ///< ftp server host name
				const char *remote_dir        ///< remote directory where we can shared files
		);
  
	/**
	   Destructor
	*/
	virtual ~TInterface();

	/**
	   Set Windows Status
	*/
	int SetStatus(int Status);

	/**
	   Set Windows @param Win Action @param Action
	   @brief information about getting/recording files
	*/
	int TInterface::SetAction(WINDOW *Win, const char *Action);	

	/**
	   Get pointer to recording window
	   @see pRecWin
	   @see SetAction
	   @return pRecWin if exist else (WINDOW *)NULL
	 */
	WINDOW *getRecWin( void );

	/**
	   Get pointer to playing window
	   @see pPlayWin
	   @see SetAction
	   @return pPlayWin if exist else (WINDOW *)NULL
	 */
	WINDOW *getPlayWin( void );


protected:

	WINDOW *pHelpWin;     ///< Help WINDOW
	WINDOW *pPlayWin;     ///< Playing WINDOW
	WINDOW *pQPlayWin;    ///< Quasi Playing WINDOW
	WINDOW *pRecWin;      ///< Recording WINDOW
	WINDOW *pQRecWin;     ///< Quasi Recording WINDOW

	const char *HotKeys;  ///< Header with hot keys

	/**
	   Back to console
	*/
	void Back2Console();

	/**
	   Fill whole window @param Win char @param C
	*/
	void FillWindow(WINDOW *Win, const char C);

};
#endif
