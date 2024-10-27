/*
 * Program: mycache.c
 * Purpose: simulate a cache with a given trace file
 * Author:  Mohammad Shaker Asir
 * Date:    12/4/2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
/*
 * Function: main
 * Purpose: simulate a cache with a given trace file
 *
 * Parameters:
 *    argc: int showing the number of arguments
 *    *argv: pointer pointing to the location of the arguments
 *
 * prints: prints out the results of the cache simulation the number of hits, misses, and evictions
 *         and prints out the debug information if v is set to 1
 */
void main(int argc, char *argv[]) {
    int s, E, b; //initialize the parameters
    char *trace_file; //initialize the trace file
    int v = 0;
    int opt;

    while ((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) { // get all parameters from command line
        switch (opt) {                        
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                v = 1;
                break;
        }
    }
    int S = 1 << s;
    int B = 1 << b;
    int tag = 64 - (s + b);
   
    typedef struct {     //Structure for a line, has valid bit, tag, and timestamp
        int valid;
        unsigned long long int tag;
        int timestamp;
    } lines_struct;

    typedef struct {     //Structure for a set , points to array of lines
        lines_struct *lines;
    } sets_struct;

    typedef struct {     //Structure for a cache,  points to an array of sets
        sets_struct *sets;
    } cache_struct;

    cache_struct cache;                  //initiallize a cache
    cache.sets = malloc(S * sizeof ( sets_struct ) ); //allocate memory for the cache
    for ( int i = 0; i < S; i++ ){ // for each set
        cache.sets[i].lines = malloc( sizeof ( lines_struct ) * E ); //allocate memory for the sets
    }

    //beginning of the cache simulation
    int hit_count = 0;
    int miss_count = 0;
    int eviction_count = 0;

    FILE *trace = fopen(trace_file, "r"); //open the trace file

    char operation;
    int size;
    int time = 0;
    unsigned long long int address;
    int hit = 0;
    int eviction = 0;
    int empty = -1; // index of the line in the set that is empty
    while (fscanf(trace, " %c %llx,%d", &operation, &address, &size) == 3) { //read in the trace file one line at a time
        int toEvict = 0;
        unsigned long long int address_tag = address >> (s + b); //calculate the address tag
        unsigned long long int setIndex = (address << tag) >> (tag + b); //calculate the set index by getting rid of the tag bits and then shifting right to remove the block bits
        sets_struct set = cache.sets[setIndex]; //get the set from the cache
        int lowestTime = set.lines[0].timestamp; //set the lowest time to the first line's timestamp

        for(int i = 0; i < E; i++){ //for each line in the set
            if(set.lines[i].valid == 1){ //if the line is valid
                if(set.lines[i].tag == address_tag){ //if the tag matches the address tag then it is a hit
                    hit = 1;
                    hit_count++;
                    set.lines[i].timestamp = time; //update the timestamp
                }
                else if(set.lines[i].timestamp < lowestTime){ //if the timestamp is lower than the lowest time then it is the line to evict
                    lowestTime = set.lines[i].timestamp;
                    toEvict = i;
                }
            }
            else if (empty == -1){ //if the line is not valid then it is empty
                empty = i; //set the empty index to the current index
            }
        }
        if(hit == 0){ //if there is no hit then it is a miss
            miss_count++; //increment the miss count
            if(empty > -1){ //if there is an empty line then fill it with the address tag and update the timestamp
                set.lines[empty].valid = 1;
                set.lines[empty].tag = address_tag;
                set.lines[empty].timestamp = time;
                time++;
            }
            else if(empty < 0){ //if there is no empty line then evict the line with the lowest timestamp
                eviction = 1;
                set.lines[toEvict].tag = address_tag;
                set.lines[toEvict].timestamp = time;
                time++;
                eviction_count++;
            }
        }

        if(operation == 'M'){ //when the operation is m we add a hit after already computed if it was a hit or miss or eviction
            hit_count++;
        }

        if (v == 1){ //if verbose is on then print out the debugg information
            printf("%c ", operation);
            printf("%llx,%d", address, size);
            if (hit == 1) {
                printf(" hit");
            }
            if (hit != 1) {
                printf(" miss ");
            }
            if (eviction == 1) {
                printf(" eviction ");
            }
            if (operation == 'M') { //if the operation is m, it is a hit so we print hit again
                printf(" hit ");
            }
            printf("\n");
        }
        empty = -1;
        hit = 0;
        eviction = 0;
    }
    fclose(trace); // close the trace file
    free(cache.sets); //free the cache
    printf("hits:%d misses:%d evictions:%d\n", hit_count, miss_count, eviction_count); //print out the results      
}


