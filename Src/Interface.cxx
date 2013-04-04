/**
   @file Interface.cxx
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
   @brief Interface sources
*/

#include "Interface.hxx"

/*
******************************************************************************
*                               P U B L I C
******************************************************************************
*/

TInterface::TInterface( const char *app_name, 
						const char *my_name, 
						const char *my_friend_name, 
						const char *server_name, 
						const char *remote_dir
	)
	:HotKeys("[R]ecord\t\t[S]top-record\t\t[Q]uit")
{

	initscr();             /* initialize the curses library */
	keypad(stdscr, true);  /* enable keyboard mapping */
	cbreak();              /* take input chars one at a time, no wait for */
	noecho();              /* don't echo input */
	nodelay(stdscr, true);

	if(has_colors()) {	
		start_color();  
    
		init_pair(WHITE_ON_BLUE, COLOR_WHITE, COLOR_BLUE);      
		init_pair(BLACK_ON_YELLOW, COLOR_WHITE, COLOR_YELLOW);  
		init_pair(WHITE_ON_GREEN, COLOR_WHITE, COLOR_GREEN);    
		init_pair(WHITE_ON_RED, COLOR_WHITE, COLOR_RED);        
		init_pair(YELLOW_ON_BLUE, COLOR_YELLOW, COLOR_BLUE);    
		init_pair(MAGENTA_ON_BLACK, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK);        
		init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);   
		init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE);       
		init_pair(CYAN_ON_BLUE, COLOR_CYAN, COLOR_BLUE);       
		init_pair(BLUE_ON_GREEN, COLOR_BLUE, COLOR_GREEN);     
		init_pair(CYAN_ON_BLACK, COLOR_CYAN, COLOR_BLACK);     
		init_pair(GREEN_ON_BLACK, COLOR_GREEN, COLOR_BLACK);   
    
//     wattron(stdscr, COLOR_PAIR(WHITE_ON_BLUE) | A_BOLD);
//     FillWindow(stdscr, ' '); 

	}  
  

	pHelpWin = subwin(stdscr, HELP_WIN_H, HELP_WIN_W, HELP_WIN_Y, HELP_WIN_X);


	touchwin(pHelpWin);
	mvwprintw(pHelpWin, 0, 1, "%s", app_name);
	wattron(pHelpWin, COLOR_PAIR(YELLOW_ON_BLUE) | A_BOLD);
	for(int i = 1; i < HELP_WIN_W; i++) mvwaddch(pHelpWin, 1, i, ' ');
  
	mvwprintw(pHelpWin, 1, 1, HotKeys);
	wattron(pHelpWin, COLOR_PAIR(CYAN_ON_BLACK) | A_BOLD);
  
	mvwprintw(pHelpWin, 3, 1, "my user name:       %s", my_name);
	mvwprintw(pHelpWin, 4, 1, "friend user name:   %s", my_friend_name);
	mvwprintw(pHelpWin, 5, 1, "ftp server address: %s", server_name);
	mvwprintw(pHelpWin, 6, 1, "remote directory:   %s", remote_dir);



	pQPlayWin = subwin(stdscr, PLAY_WIN_H, PLAY_WIN_W, PLAY_WIN_Y, PLAY_WIN_X);
	box(pQPlayWin, ACS_VLINE, ACS_HLINE); 
	touchwin(pQPlayWin);  
	wattron(pQPlayWin, COLOR_PAIR(GREEN_ON_BLACK) | A_BOLD);
	mvwprintw(pQPlayWin, 1, 2, ".: P L A Y I N G   W I N D O W :.");
	    
	pPlayWin = subwin(stdscr, PLAY_WIN_H - 2, PLAY_WIN_W - 2, PLAY_WIN_Y + 2, PLAY_WIN_X + 2);
	box(pPlayWin, ACS_VLINE, ACS_HLINE); 
	touchwin(pPlayWin);
	wattron(pPlayWin, COLOR_PAIR(CYAN_ON_BLACK) | A_BOLD);
	mvwprintw(pPlayWin, 2, 2,  "Status:");
	mvwprintw(pPlayWin, 3, 2,  "Action:");

	pQRecWin = subwin(stdscr,REC_WIN_H, REC_WIN_W, REC_WIN_Y, REC_WIN_X);
	box(pQRecWin, ACS_VLINE, ACS_HLINE); 
	touchwin(pQRecWin);
	wattron(pQRecWin, COLOR_PAIR(GREEN_ON_BLACK) | A_BOLD);
	mvwprintw(pQRecWin, 1, 2,  ".: R E C O R D I N G   W I N D O W :.");
  
	pRecWin = subwin(stdscr, REC_WIN_H - 2, REC_WIN_W - 2, REC_WIN_Y + 2, REC_WIN_X + 2);
	box(pRecWin, ACS_VLINE, ACS_HLINE);   
	touchwin(pRecWin);  
	wattron(pRecWin, COLOR_PAIR(CYAN_ON_BLACK) | A_BOLD);
	mvwprintw(pRecWin, 2, 2,  "Status:");
	mvwprintw(pRecWin, 3, 2,  "Action:");

	move(0, 0);
	curs_set(NO_VISIBILITY); 
	refresh();
}
//----------------------------------------------------------------------------

TInterface::~TInterface()
{
	delwin(pHelpWin);
	delwin(pQPlayWin);
	delwin(pPlayWin);
	delwin(pQRecWin);
	delwin(pRecWin);
	Back2Console();
}
//----------------------------------------------------------------------------


WINDOW * TInterface::getRecWin( void )
{
	if ( pRecWin ) return pRecWin;
	return ((WINDOW *) NULL);
}
//----------------------------------------------------------------------------
WINDOW * TInterface::getPlayWin( void )
{
	if ( pPlayWin ) return pPlayWin;
	return ((WINDOW *) NULL);
}
//----------------------------------------------------------------------------

int TInterface::SetStatus(int Status)
{    
	switch(Status) {
	case STATUS_GET:
		wattron(pPlayWin, COLOR_PAIR(BLACK_ON_YELLOW) | A_BOLD);
		mvwprintw(pPlayWin, 2, 12,  "g.e.t.t.i.n.g    ");
		wnoutrefresh(pPlayWin);
		break;
	case STATUS_PLAY:
		wattron(pPlayWin, COLOR_PAIR(WHITE_ON_GREEN) | A_BOLD);
		mvwprintw(pPlayWin, 2, 12,  "p.l.a.y.i.n.g    ");
		wnoutrefresh(pPlayWin);
		break;
	case STATUS_SLEEP:
		wattron(pPlayWin, COLOR_PAIR(WHITE_ON_BLACK) | A_BOLD);
		mvwprintw(pPlayWin, 2, 12,  "s.l.e.e.p.i.n.g  ");
		wnoutrefresh(pPlayWin);
		break;
	case STATUS_STOP:  
		wattron(pRecWin, COLOR_PAIR(MAGENTA_ON_BLACK) | A_BOLD);
		mvwprintw(pRecWin, 2, 12,   "s.t.o.p          ");
		wnoutrefresh(pRecWin);
		break;
	case STATUS_REC:
		wattron(pRecWin, COLOR_PAIR(WHITE_ON_RED) | A_BOLD);
		mvwprintw(pRecWin, 2, 12,   "r.e.c.o.r.d.i.n.g");
		wnoutrefresh(pRecWin);
		break;
	default: ;
	}
	doupdate();
	return Status;
}
//----------------------------------------------------------------------------


int TInterface::SetAction(WINDOW *Win, const char *Action)
{    
	if ( ! Win ) return 0;
	
	wattron(Win, COLOR_PAIR(WHITE_ON_BLACK) | A_BOLD);
	for(int i = 12; i < Win->_maxx - 1; ++i)
		mvwaddch(Win, 3, i, ' ');
	
	if ( Action ) {		
		mvwprintw(Win, 3, 12,   Action);
	}
	wnoutrefresh(Win);
	
	return doupdate();
}
//----------------------------------------------------------------------------

/*
******************************************************************************
*                               P R O T E C T E D
******************************************************************************
*/

void TInterface::Back2Console() 
{
	clear();
	refresh();  
	erase();   
	endwin();
}
//----------------------------------------------------------------------------

void TInterface::FillWindow(WINDOW *Win, const char C = ' ')
{
	for(int i = 0; i < Win->_maxx - 1; ++i) {
		for(int j = 0; j < Win->_maxy - 1; ++j) {
			mvwaddch(Win, j, i, C);
		}
	}
	wrefresh(Win);
}
//----------------------------------------------------------------------------
