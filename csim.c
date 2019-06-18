/*
Name:Xinqi Jin
loginID:2016010524
*/
#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

typedef struct Node* PNode; 
typedef struct Node { // Node of the double linked list.
    int valid; // The valid bit.
    char tag[64]; // The tag bits.
    PNode prev;
    PNode next;
} Node;
typedef struct { // Declare the double linked list data structure.
    PNode head;
    PNode tail;
    int size;
} DoubleLinkedList;

struct globalArgs_t {
    /* The struct will be used to store some global arguments. */
    int s;                      /* -s option */
    int E;                      /* -E option */
    int b;                      /* -b option */
    const char* tracefile;      /* -t option */
    int verbosity;              /* -v option */
} globalArgs;

static const char* optString="vs:E:b:t:";

int main( int argc,char* argv[] ) {	

    /* Initialize globalArgs before we get to work. */
    globalArgs.s = 0;
    globalArgs.E = 0;
    globalArgs.b = 0;
    globalArgs.tracefile = NULL;
    globalArgs.verbosity = 0;    /* false */

    /* Process the arguments with getopt(), then 
     * populate globalArgs.
     */
    int opt = getopt( argc, argv, optString );
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
        
        opt = getopt( argc, argv, optString );
    }

    //int t = 64 - globalArgs.b - globalArgs.s; //size of Tag bits

    /* Declare three variables used to store the result. */
    int hits = 0;
    int misses = 0;
    int evictions = 0;

    /* Parse the trace file line by line. */
    FILE *fp = fopen( globalArgs.tracefile, "r" );
    char buf[1024];
    DoubleLinkedList list; // Use a double linked list to simulate the cache.
    list.head = ( PNode )malloc( sizeof( Node ) );
    list.tail = ( PNode )malloc( sizeof( Node ) );
    list.head->prev = NULL;
    list.head->next = list.tail;
    list.tail->prev = list.head;
    list.tail->next = NULL;
    list.size = 0;
    char address[64];
    while ( fgets( buf, sizeof(buf), fp ) != NULL ) {
        if( buf[0] == ' ' ){
            
            int pos = 3; // position of the comma in this line of the trace file
            while ( buf[pos]!= ',' ) {
                pos ++;
            }

            memset( address, 0, sizeof(char)*64);
            int pos2 = 60;
            for( int i = pos - 1 ; i > 2 ; i-- ) {
                switch ( buf[i] ) {
                    case '0':
                        break;
                    case '1':
                        address[ pos + 3 ] = 1;
                        break;                       
                    case '2':
                        address[ pos + 2 ] = 1;
                        break;
                    case '3':
                        address[ pos + 2 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;
                    case '4':
                        address[ pos + 1 ] = 1;
                        break;
                    case '5':
                        address[ pos + 1 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;
                    case '6':
                        address[ pos + 1 ] = 1;
                        address[ pos + 2 ] = 1;
                        break;
                    case '7':
                        address[ pos + 1 ] = 1;
                        address[ pos + 2 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;   
                    case '8':
                        address[ pos ] = 1;
                        break;
                    case '9':
                        address[ pos ] = 1;
                        address[ pos + 3 ] = 1;
                        break;                       
                    case 'a':
                        address[ pos ] = 1;
                        address[ pos + 2 ] = 1;
                        break;
                    case 'b':
                        address[ pos ] = 1;
                        address[ pos + 2 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;
                    case 'c':
                        address[ pos ] = 1;
                        address[ pos + 1 ] = 1;
                        break;
                    case 'd':
                        address[ pos ] = 1;
                        address[ pos + 1 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;
                    case 'e':
                        address[ pos ] = 1;
                        address[ pos + 1 ] = 1;
                        address[ pos + 2 ] = 1;
                        break;
                    case 'f':
                        address[ pos ] = 1;
                        address[ pos + 1 ] = 1;
                        address[ pos + 2 ] = 1;
                        address[ pos + 3 ] = 1;
                        break;
                    default:
                        break;                                                                                                                                                                                       

                }
            }

            PNode pointer = list.head->next;
            int cached = 0;
            while ( pointer != list.tail ) {
                if( pointer->valid ) {
                    cached=pos2;
                    pos2=cached;
                }
                pointer = pointer->next;
            }
        }
    }


    printSummary( hits, misses, evictions );
    return 0;
}
