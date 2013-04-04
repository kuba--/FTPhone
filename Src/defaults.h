/**
   @todo : check these macros on other systems
   
   $Log: defaults.h,v $
   Revision 1.5  2004/02/09 00:11:00  kuba
   *** empty log message ***

   Revision 1.4  2004/01/05 00:31:18  kuba
   add STEREO and MONO macros
*/
#define DEFRATE  44100
#define DEFBITS  16
#define DEFBYTES 2 ///< stereo --------------------
//                                                |
#define DEFRATESTR  "44100Hz"  //                 |
#define DEFBITSSTR  "16"       //                 |
#define DEFBYTESSTR "2"        //                 |
//                                                |
#define DEFFONTSIZE 10         //                 |
#define DEFPLOTWIDTH 0.1       //                 |
//                                                |
#define STEREO   1 ///< we'd like 2 channels  <<--|
#define MONO     0 ///< 1 channel transmission
