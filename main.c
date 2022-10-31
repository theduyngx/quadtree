/*
 * Main program with a variety of uses:
 * 1. manual inputs to manually construct the quad tree, as well as
 *    other operations, namely insertion and searches.
 * 2. pass arguments from terminal (stdin).
 * The outer square covering all points will be referred to as o.s.
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "read.h"

/* program's entry */
int main(int argc, char** argv) {
    /* case 1: no argument is passed (using purely human inputs) */
    if (argc <= 1)
        return manual_input();

    /* case 2: terminal, or text file passed as argument */
    /// WIP
    else if (argc < MIN_ARGS) {
        fprintf(stderr, "Insufficient arguments!\n");
        exit(EXIT_FAILURE);
    }

    // passing bottom left, top right positions to form o.s
    point_t *bL = init_point(strtold(argv[1], NULL), strtold(argv[2], NULL));
    point_t *tR = init_point(strtold(argv[3], NULL), strtold(argv[4], NULL));
    qtnode_t* tree = init_tree(init_square(bL, tR));
    print_tree(tree);
    return 1;
}
