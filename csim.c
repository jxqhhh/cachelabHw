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

const int Hit = 1;
const int Miss = 2;
const int Unknown = -1;
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

    const int t = 64 - globalArgs.b - globalArgs.s; //size of Tag bits
    int num_of_Sets = 1; // #Sets = 2^s
    for( int i = 0; i < globalArgs.s; i++ ) {
        num_of_Sets *= 2;
    }

    /* Declare three variables used to store the result. */
    int hits = 0;
    int misses = 0;
    int evictions = 0;

    /* Parse the trace file line by line. */
    FILE *fp = fopen( globalArgs.tracefile, "r" );
    char buf[1024];
    DoubleLinkedList list[num_of_Sets]; // Use a double linked list array to simulate the cache.
    for( int i = 0; i < num_of_Sets; i++ ) {
        list[i].head = ( PNode )malloc( sizeof( Node ) );
        list[i].tail = ( PNode )malloc( sizeof( Node ) );
        list[i].head->prev = NULL;
        list[i].head->next = list[i].tail;
        list[i].tail->prev = list[i].head;
        list[i].tail->next = NULL;
        list[i].size = 0;
    }
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
                        address[ pos2 + 3 ] = 1;
                        break;                       
                    case '2':
                        address[ pos2 + 2 ] = 1;
                        break;
                    case '3':
                        address[ pos2 + 2 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;
                    case '4':
                        address[ pos2 + 1 ] = 1;
                        break;
                    case '5':
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;
                    case '6':
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        break;
                    case '7':
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;   
                    case '8':
                        address[ pos2 ] = 1;
                        break;
                    case '9':
                        address[ pos2 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;                       
                    case 'a':
                        address[ pos2 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        break;
                    case 'b':
                        address[ pos2 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;
                    case 'c':
                        address[ pos2 ] = 1;
                        address[ pos2 + 1 ] = 1;
                        break;
                    case 'd':
                        address[ pos2 ] = 1;
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;
                    case 'e':
                        address[ pos2 ] = 1;
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        break;
                    case 'f':
                        address[ pos2 ] = 1;
                        address[ pos2 + 1 ] = 1;
                        address[ pos2 + 2 ] = 1;
                        address[ pos2 + 3 ] = 1;
                        break;
                    default:
                        break;
                }
                pos2 -= 4;
            }

            int setIndex = 0;
            for ( int i = 63 - globalArgs.b; i >= t; i-- ) {
                setIndex = 2*setIndex + (int)(address[i]);
            }

            memset( address + t, 0, sizeof( char )*( 64 - t ) );

            PNode pointer = list[setIndex].head->next;
            int status = Unknown;
            while ( pointer != list[setIndex].tail ) {
                if ( pointer->valid ) {
                    if ( memcmp( pointer->tag, address, t ) ==0 ) { // Cache hit!
                        /* Adjust the double linked list 
                         * so that the latest-recently used node is the first node following the head node.
                         */
                        status = Hit;
                        PNode next = pointer->next;
                        PNode prev = pointer->prev;
                        prev->next = next;
                        next->prev = prev;
                        PNode first = list[setIndex].head->next;
                        list[setIndex].head->next = pointer;
                        pointer->prev = list[setIndex].head;
                        pointer->next = first;
                        first->prev = pointer;

                        // Update hits:
                        if( buf[1] == 'M' ) {
                            hits += 2;
                        } else {
                            hits ++;
                        }
                        break;
                    }

                }
                pointer = pointer->next;
            }
            if( status == Unknown ){
                if ( list[setIndex].size < globalArgs.E ) { //Cache miss and no eviction

                    //Update:
                    if( buf[1] == 'M' ) {
                        misses ++;
                        hits ++;
                        status = Hit;
                    } else {
                        misses ++;
                        status = Miss;
                    }

                    /* Adjust the double linked list 
                     * so that the missed cache will be insterted into the corresponding double linked list cache-simulator.
                     */
                    PNode pointer = ( PNode )malloc( sizeof( Node ) );
                    pointer->valid = 1;
                    memcpy( pointer->tag, address, 64 );
                    pointer->prev = list[setIndex].head;
                    pointer->next = pointer->prev->next;
                    list[setIndex].head->next = pointer;
                    pointer->next->prev = pointer;
                    list[setIndex].size ++;
                } else { // Cache miss and eviction

                    //Update:
                    if( buf[1] == 'M' ) {
                        misses ++;
                        hits ++;
                        evictions ++;
                        status = Hit;
                    } else {
                        misses ++;
                        evictions ++;
                        status = Miss;
                    }

                    PNode pointer=list[setIndex].tail->prev;
                    pointer->prev->next = list[setIndex].tail;
                    list[setIndex].tail->prev = pointer->prev;
                    memcpy( pointer->tag, address, 64 );
                    pointer->prev = list[setIndex].head;
                    pointer->next = pointer->prev->next;
                    list[setIndex].head->next = pointer;
                    pointer->next->prev = pointer;
                }
            }


        }
    }


    printSummary( hits, misses, evictions );
    return 0;
}
