/*
 * read.c is specifically designed to read manual inputs only,
 * it is not used for reading text files as inputs on terminal.
 * The program is designed so that user can input commands and
 * data as instructed, and will be terminated also as instructed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "math.h"
#include "queue.h"
#include "read.h"

/* switch-cases for manual queries */
#define PRINT 0      // case print tree
#define POINT 1      // case point search
#define RANGE 2      // case range search
#define ERROR (-1)   // case error input
#define LEAVE (-2)   // case leave mode
#define CLOSE (-10)  // case stop program

/* valid manual inputs during queries */
#define PRINT_STR "print"
#define POINT_STR "point"
#define RANGE_STR "range"
#define CLOSE_STR "close"
#define LEAVE_STR "leave"

/* printing header for case 1 (no argument passed) - manual input */
void print_header(char* str);
/* check if a string is a proper number or not */
int verify_num(char* str);
/* check which type of queries being instructed */
int check_query(char* str);
/* point search operation during query */
void point_search_query(node_t* tree, char* str, long double* pos);
/* range search operation during query */
void range_search_query(node_t* tree, char* str, long double* pos);

/* manual input's entry; called when program runs with no arguments in terminal */
int manual_input() {
    // initializing some properties
    long double value; int i = 0;
    long double pos[MIN_ARGS];
    char* str = (char*) malloc(sizeof(char)*MAX_DIGIT);
    char* token;

    /* o.s initialization */
    print_header("Initializing tree");
    printf("Enter bottom left x and y coordinates of outer square here, ");
    printf("separated by a newline\n");
    // loop till finishes processing 2 positions of the o.s
    while (i < MIN_ARGS) {
        fgets(str, MAX_DIGIT, stdin);
        token = strtok(str, " ");
        // if input is not a number
        if (!verify_num(token)) {
            fprintf(stderr, "ERROR: input must be a number in proper form!\n");
            continue;
        }
        // check coordinate validity
        value = strtold(str, NULL);
        if (i >= 2 && value <= pos[i-2]) {
            fprintf(stderr,"ERROR: top right coordinates must be larger than bottom left!\n");
            i = (i%2) ? i-1 : i;
            continue;
        }
        // store coordinates
        pos[i] = value;
        i++;
    }

    /* tree initialization */
    long double xL = pos[0], xR = pos[2], yB = pos[1], yT = pos[3];
    point_t *bL = init_point(xL, yB);
    point_t *tR = init_point(xR, yT);
    node_t* tree = init_tree(init_square(bL, tR));

    /* insertion */
    print_header("Insertion");
    int stop = 0, pnt_fin = 0; i = 0;
    printf("Please enter x-y coordinates of points to be inserted, ");
    printf("separated by a newline:\n");
    printf("(To stop insertion, input any non-digit characters.)\n");
    // not until stop condition is satisfied
    while (!stop) {
        printf("Point %d:\n", (int) floor(i/2.0) + 1);
        while (!pnt_fin) {
            fgets(str, MAX_DIGIT, stdin);
            str = strtok(str, " ");
            // stop condition is when a non-number is inputted
            if (!verify_num(str)) {
                stop = 1;
                break;
            }
            // point must be in o.s
            value = strtold(str, NULL);
            if (i % 2 && (value > yT || value < yB)) {
                fprintf(stderr,"ERROR: y-coordinate must be in outer square!\n");
                i--;
                continue;
            }
            if (i % 2 == 0 && (value < xL || value > xR)) {
                fprintf(stderr,"ERROR: x-coordinate must be in outer square!\n");
                continue;
            }
            // storing x,y coordinates accordingly
            if (i % 2) {
                pos[1] = value;
                pnt_fin = 1;
            }
            else pos[0] = value;
            i++;
        }
        // insert to tree
        if (!stop) insert(tree, init_point(pos[0], pos[1]));
        pnt_fin = 0;
    }

    /* queries */
    print_header("Queries");
    int query; stop = 0;
    while (!stop) {
        printf("To show the entire tree structure, enter \"print\";\n");
        printf("To initiate point search query, enter \"point\";\n");
        printf("To initiate range search query, enter \"range\";\n");
        printf("To stop the program, enter \"close\";\n");
        fgets(str, MAX_DIGIT, stdin);
        query = check_query(str);
        switch (query) {
            // invalid query
            case ERROR:
            case LEAVE:
                fprintf(stderr,"ERROR: invalid input!\n");
                break;
            // print tree
            case PRINT:
                print_tree(tree);
                break;
            // point search
            case POINT:
                point_search_query(tree, str, pos);
                break;
            // range search
            case RANGE:
                range_search_query(tree, str, pos);
                break;
            // stop program
            case CLOSE:
                printf("Stopping program...");
                stop = 1;
        }
        printf("\n");
    }

    // cleaning up
    fflush(stdin);
    free(str);
    free_tree(tree);
    return 0;
}

/* printing header in manual input case */
void print_header(char* str) {
    int i, len = (int)strlen(str)+2;
    printf("\n+");
    for (i=0; i < len; i++) printf("%c", '-');
    printf("+\n| %s |\n+", str);
    for (i=0; i < len; i++) printf("%c", '-');
    printf("+\n");
}

/* check if a string is a number or not; used to verify human inputs
 * coordinates are valid
 */
int verify_num(char* str) {
    int decimal = 0;
    int len = (int)strlen(str);
    // if it's an empty string
    if (len == 0) return 0;
    len = (len > 1) ? len-1 : 1;
    for (int i=0; i < len; i++) {
        // check if string is proper double
        if (str[i] == '.') {
            if (decimal) return 0;
            decimal = 1;
        }
        else if (!isdigit(str[i])) return 0;
        i++;
    }
    return 1;
}

/* check which type of queries is being instructed by user; depending
 * on what user enters ("point", "range", "print", "leave", "close",
 * or otherwise).
 */
int check_query(char* str) {
    int len = (int)strlen(str);
    // if it's an empty string
    len = (len > 1) ? len-1 : 1;
    if (len != strlen(POINT_STR)) return ERROR;
    // conditions
    int notPoint = 0, notRange = 0, notPrint = 0,
        notClose = 0, notLeave = 0;
    // if at any point the 2 strings differ, then it isn't that mode
    for (int i=0; i < len; i++) {
        if (str[i] != POINT_STR[i])
            notPoint = 1;
        if (str[i] != RANGE_STR[i])
            notRange = 1;
        if (str[i] != PRINT_STR[i])
            notPrint = 1;
        if (str[i] != CLOSE_STR[i])
            notClose = 1;
        if (str[i] != LEAVE_STR[i])
            notLeave = 1;
    }
    // has to be either one of these modes
    return  (!notPrint) ? PRINT : (!notPoint) ? POINT :
            (!notRange) ? RANGE : (!notClose) ? CLOSE :
            (!notLeave) ? LEAVE : ERROR;
}

/* point search query; used when user has entered "point" mode, which
 * will accept 2 number arguments at a time as x,y-coordinates of the
 * queried point
 */
void point_search_query(node_t* tree, char* str, long double* pos) {
    // square coordinates
    long double xL = tree->square->bottom_left->x;
    long double xR = tree->square->top_right->x;
    long double yB = tree->square->bottom_left->y;
    long double yT = tree->square->top_right->y;
    // queried point position
    printf("Point search initiated.\nTo exit, enter \"leave\"\n");
    int i = 0; long double value;
    while (1) {
        fgets(str, MAX_DIGIT, stdin);
        str = strtok(str, " ");
        // stop condition is when "leave" is entered
        if (check_query(str) == LEAVE) {
            printf("Leaving point search mode...\n");
            break;
        }
        // point must be in o.s, if not enter point coordinates again
        value = strtold(str, NULL);
        if (i % 2 && (value > yT || value < yB)) {
            fprintf(stderr,"ERROR: y-coordinate must be within outer square!\n");
            i--;
            continue;
        }
        if (i % 2 == 0 && (value < xL || value > xR)) {
            fprintf(stderr,"ERROR: x-coordinate must be within outer square!\n");
            continue;
        }
        // storing x,y coordinates accordingly
        if (i % 2) {
            pos[1] = value;
            search_pt(tree, init_point(pos[0], pos[1]));
        }
        else pos[0] = value;
        i++;
    }
}

/* range search query; used when user has entered "range" mode, which
 * accepts 4 number arguments at a time as a pair of x-y coordinates
 * of the bottom left and top right points of the queried square
 */
void range_search_query(node_t* tree, char* str, long double* pos) {
    // square coordinates
    long double xL = tree->square->bottom_left->x;
    long double xR = tree->square->top_right->x;
    long double yB = tree->square->bottom_left->y;
    long double yT = tree->square->top_right->y;
    // queried point position
    printf("Range search initiated.\nTo exit, enter \"leave\"\n");
    int i = 0; long double value;
    while (1) {
        fgets(str, MAX_DIGIT, stdin);
        str = strtok(str, " ");
        // stop condition is when "leave" is entered
        if (check_query(str) == LEAVE) {
            printf("Leaving point search mode...\n");
            break;
        }
        // point must be in o.s
        value = strtold(str, NULL);
        if (i % 2 && (value > yT || value < yB)) {
            printf("ERROR: y-coordinate must be within outer square!\n");
            i--;
            continue;
        }
        if (i % 2 == 0 && (value < xL || value > xR)) {
            printf("ERROR: x-coordinate must be within outer square!\n");
            continue;
        }
        if (i >= 2 && value <= pos[i-2]) {
            printf("ERROR: top right coordinates must be larger than bottom left!\n");
            i = (i%2) ? i-1 : i;
            continue;
        }
        // store coordinates
        pos[i] = value;
        i++;
        // creating the square for range search once finishes the x-y pair
        if (i == MIN_ARGS) {
            i = 0;
            search_range(tree, init_square(init_point(pos[0], pos[1]),
                                                   init_point(pos[2], pos[3])));
        }
    }
}
