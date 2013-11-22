<pre>
OOOOOOO 	OOOOOO  	OOOOOO  	OOO     	        	        	        	
 OO   O 	O OO O  	 OO  OO 	 OO     	        	        	        	
 OO O   	  OO    	 OO  OO 	 OO OO  	 OOOO   	OOOOO   	 OOOO   	
 OOOO   	  OO    	 OOOOO  	 OOO OO 	OO  OO  	OO  OO  	OO  OO  	
 OO O   	  OO    	 OO     	 OO  OO 	OO  OO  	OO  OO  	OOOOOO  	
 OO     	  OO    	 OO     	 OO  OO 	OO  OO  	OO  OO  	OO      	
OOOO    	 OOOO   	OOOO    	OOO  OO 	 OOOO   	OO  OO  	 OOOO   	ver. 0.1
</pre>

FTPhone - It's a simple console application. It emulates walky-talky. 
If you have an access on ftp server and read/write permission for 
any remote directory on this server, and this same permission has your friend,
you can try to talk using this program ;-)

[1] Compile:
	./make.sh

[2] Run with following parameters:
	
	sh ftphone.sh {-m my user name} {-f friend user name} {-s ftp server address} {-d remote dir}

default values:
	my user name on the ftp server: kuba
	friend's user name on the ftp server: john
	ftp server's address: ftp.server.net
	remote dir, where application will store files : /tmp


eg.:.	sh ftphone.sh  -m kuba -f john -s ftp.server.net -d /tmp

