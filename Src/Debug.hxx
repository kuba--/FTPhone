/**
   @file Debug.hxx
   @brief Some debug macros
*/

#ifndef DEBUG_HXX
#define DEBUG_HXX

#include <errno.h>
extern int errno;

#include <iostream>

/**
   @see TRACE 
   @a STR : stream
   @a VAR : stream
   @example Base.cxx
*/
#ifdef DO_DEBUG

#define TRACE( STR ) { \
                       std::cerr << __LINE__ << " : "; \
                       std::cerr << STR;  \
                       std::cerr << endl; \
                       perror(__FILE__); \
                     }

#define LOCV( VAR ) { \
                      std::cerr << "["; \
                      std::cerr << __FILE__ << " : " << __LINE__ << "]=="; \
                      std::cerr << VAR << endl; \
                    }


#else

#define TRACE( STR ) { ; }

#define LOCV( VAR ) { ; }

#endif

#endif
