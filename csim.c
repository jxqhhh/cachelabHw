/*
Name:Xinqi Jin
loginID:2016010524
*/
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

struct globalArgs_t {
    int s;                      /* -s option */
    int E;                      /* -E option */
    int b;                      /* -b option */
    const char* tracefile;      /* -t option */
    int verbosity;              /* -v option */
} globalArgs;

static const char* optString="vs:E:b:t:";

int main( int argc,char* argv[] ) {	
    int opt = 0;

    /* Initialize globalArgs before we get to work. */
    globalArgs.s = 0;
    globalArgs.E = 0;
    globalArgs.b = 0;
    globalArgs.tracefile = NULL;
    globalArgs.verbosity = 0;    /* false */

    /* Process the arguments with getopt(), then 
     * populate globalArgs.
     */
    opt=getopt( argc, argv, optString );
    while( opt != -1) {
        switch( opt ) {
            case 's':
                globalArgs.s = atoi( optarg );
                break;
            case 'E':
                globalArgs.E = atoi( optarg );
                break;
            case 'b':
                globalArgs.b = atoi( optarg );
                break;
            case 't':
                globalArgs.tracefile = optarg;
                break;
            case 'v':
                globalArgs.verbosity = 1;
                break;
            default:
                break;
        }
        
        opt=getopt( argc, argv, optString );
    }

    printSummary( globalArgs.s, globalArgs.E, globalArgs.b );
    return 0;
}
