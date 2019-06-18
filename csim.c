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
    DoubleLinkedList list;
    list.head = ( PNode )malloc( sizeof( Node ) );
    list.tail = ( PNode )malloc( sizeof( Node ) );
    list.head->prev = NULL;
    list.head->next = list.tail;
    list.tail->prev = list.head;
    list.tail->next = NULL;
    list.size = 0;
    while ( fgets( buf, sizeof(buf), fp ) != NULL ) {
        if( buf[0] == ' ' ){
            if(list.size==0){

            }
        }
    }


    printSummary( hits, misses, evictions );
    return 0;
}
