/*
 * Debug mode - when terminal argument "debug" is passed as the first
 * argument (after ./program), debug mode is activated. This mode simply
 * executes pre-defined test cases and check for its correctness.
 */

#include <stdio.h>
#include "queue.h"
#include "read.h"
#include "debug.h"

/* debug mode's entry program */
int debug_mode() {
    /**
     * Tree
     */
    point_t *bL = init_point(0, 0);
    point_t *tR = init_point(20, 20);
    qtnode_t *tree = init_tree(init_square(bL, tR));
    // define the points
    point_t *p1 = init_point(5, 3);
    point_t *p2 = init_point(6, 3);
    point_t *p3 = init_point(2, 2);
    point_t *p4 = init_point(9, 4);
    point_t *p5 = init_point(12, 15);
    point_t *p6 = init_point(1, 13);
    point_t *p7 = init_point(13, 16);
    point_t *p8 = init_point(5.145687234, 3.415234565);
    point_t *p9 = init_point(5.145687234521, 3.415234256565);
    // insertion
    insert(tree, p1);
    insert(tree, p2);
    insert(tree, p3);
    insert(tree, p4);
    insert(tree, p5);
    insert(tree, p6);
    insert(tree, p7);
    insert(tree, p8);
    insert(tree, p9);
    // print tree
    printf("\n+-----------------------+\n");
    printf(  "|    Printing tree:     |");
    printf("\n+-----------------------+\n");
    print_tree(tree);
    printf("\n");

    /**
     * Point search
     */
    printf("\n+-----------------------+\n");
    printf(  "|     Point search:     |");
    printf("\n+-----------------------+\n");
    search_pt(tree, p1);
    search_pt(tree, p2);
    search_pt(tree, p3);
    search_pt(tree, p4);
    search_pt(tree, p5);
    search_pt(tree, p6);
    search_pt(tree, p7);
    search_pt(tree, p8);
    search_pt(tree, p9);
    printf("(19,19) is not in the square -> result: ");
    search_pt(tree, init_point(19, 19));
    printf("\n");

    /**
     * Range search
     */
    point_t *p20 = init_point(1, 1);
    point_t *p21 = init_point(5, 5);
    square_t *sq0 = init_square(p20, p21);
    point_t *p22 = init_point(3.12345667, 2.456723556);
    point_t *p23 = init_point(9.998762, 7.89);
    square_t *sq1 = init_square(p22, p23);
    point_t *p24 = init_point(12, 2);
    point_t *p25 = init_point(18, 14);
    square_t *sq2 = init_square(p24, p25);
    printf("\n+-----------------------+\n");
    printf(  "|     Range search:     |");
    printf("\n+-----------------------+");
    printf("\nsquare [bL (%.2Lf, %.2Lf), tR (%.2Lf, %.2Lf)]\n",
           p20->x, p20->y, p21->x, p21->y);
    search_range(tree, sq0);
    printf("\nsquare [bL (%.2Lf, %.2Lf), tR (%.2Lf, %.2Lf)]\n",
           p22->x, p22->y, p23->x, p23->y);
    search_range(tree, sq1);
    printf("\nsquare [bL (%.2Lf, %.2Lf), tR (%.2Lf, %.2Lf)]\n",
           p24->x, p24->y, p25->x, p25->y);
    search_range(tree, sq2);

    /**
     * Freeing memory
     */
    free_tree(tree);
    printf("\n");

    /// Checking rectangle intersection
    /*
    // case 1
    point_t* p7 = init_point(1, 3);
    point_t* p8 = init_point(5, 5);
    point_t* p9 = init_point(5, 0.5);
    point_t* p10 = init_point(7, 5);
    // intersect by only 1 point (5,3)
    square_t* sq0 = init_square(p7, p8);
    printf("intersect case 0 (by 1 point): %d\n", rectangle_intersect(sq0, init_square(p9, p10)));

    // case 2
    point_t* p11 = init_point(3, 7);
    point_t* p12 = init_point(2, 1);
    point_t* p13 = init_point(5, 5);
    square_t* sq1 = init_square(p7, p11);
    square_t* sq2 = init_square(p12, p13);
    printf("intersect case 2: %d\n", rectangle_intersect(sq1, sq2));

    // case 3
    point_t* p14 = init_point(3,3);
    point_t* p15 = init_point(6,8);
    square_t* sq3 = init_square(p14, p15);
    printf("intersect case 3 (line intersection): %d\n", rectangle_intersect(sq1, sq3));

    // case 4
    point_t* p16 = init_point(0,4);
    point_t* p17 = init_point(5,6);
    square_t* sq4 = init_square(p16, p17);
    printf("intersect case 4 (cross intersection): %d\n", rectangle_intersect(sq1, sq4));

    // case 5 - inside
    point_t* p18 = init_point(1.5,4);
    point_t* p19 = init_point(2.5,6);
    square_t* sq5 = init_square(p18, p19);
    printf("intersect case 5 (rectangle within rectangle): %d\n", rectangle_intersect(sq1, sq5));

    // case 6
    point_t* p22 = init_point(2,5);
    point_t* p23 = init_point(6,8);
    square_t* sq6 = init_square(p22, p23);
    printf("intersect case 6: %d\n", rectangle_intersect(sq1, sq6));

    // case non-intersection
    point_t* p20 = init_point(6, 1);
    point_t* p21 = init_point(8, 4);
    square_t* sqn = init_square(p20, p21);
    printf("no interesection: %d\n", rectangle_intersect(sqn, sq0));
    */
    return 0;
}