OOOOOOO 	OOOOOO  	OOOOOO  	OOO     	        	        	        	
 OO   O 	O OO O  	 OO  OO 	 OO     	        	        	        	
 OO O   	  OO    	 OO  OO 	 OO OO  	 OOOO   	OOOOO   	 OOOO   	
 OOOO   	  OO    	 OOOOO  	 OOO OO 	OO  OO  	OO  OO  	OO  OO  	
 OO O   	  OO    	 OO     	 OO  OO 	OO  OO  	OO  OO  	OOOOOO  	
 OO     	  OO    	 OO     	 OO  OO 	OO  OO  	OO  OO  	OO      	
OOOO    	 OOOO   	OOOO    	OOO  OO 	 OOOO   	OO  OO  	 OOOO   	ver. 0.1

by Kuba Podgorski 



Hello, this is read me file for project FTPhone ver. 0.1

FTPhone - It's simple console application. It tries to emulate walky-talky. 
If you have an access to ftp server and read/write permission for 
any remote directory on this server, and this same permission har your friend,
you can try to talk to him using this program ;-)

It's very easy. 
[1] Compile program:
	./make.sh

[2] And if everything OK start program with correct parameters and that all ;)
	
	sh ftphone.sh [-m my user name] [-f friend user name] [-s ftp server address] [-d remote dir]

default values:
	my user name on ftp server: kuba
	friend user name on ftp server: john
	ftp server address: ftp.server.net
	remote dir, where we will share files : /tmp/


eg.:.	sh ftphone.sh  -m kuba -f john -s ftp.server.net -d /tmp/

