/*
 * A simple quadtree program that can be implemented to hold any kind of data,
 * not just points, as long as the footpath data itself contains the data
 * structure point.
 * The structure follows the "first come first serves" principle, where the
 * point to be inserted first is one of higher priority; meaning any point
 * inserted afterwards must be evaluated on the points inserted prior.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

#define LEVEL_PRINT_LIM 10000   // printing limit (based on tree's level)

/* checking intersection onesidedly, meaning full intersection should check
 * r1 relative to r2 and r2 relative to r1
 */
int oneside_intersect_check(square_t* r1, square_t* r2);

/* range search operation passing found to check whether found any points or not */
void search_range_check(node_t* tree, square_t* rectangle, int* found);

/* split the node into 4 branches - initializing 4 child nodes */
void split(node_t* node);

/* recursively check whether point to be inserted can be inserted to a quadrant
 * of the bounding square in question or not; if not then continue splitting
 */
void split_insert(node_t* root, point_t* point);

/* insert the point into a specified quadrant of the node */
node_t* insert_quadrant(node_t* node, point_t* point, enum quadrant q);

/* get the midpoints of the square, where point at (xMid, yMid) is the center */
void get_midpoints(square_t* square, long double* xMidPass, long double* yMidPass);

/* helper function printing out node */
void print_node(node_t* node, int level);

/* print in-order, along with the level; LEVEL_PRINT_LIM is specified to limit the
 * number of levels being printed
 */
void print_level_order(node_t* tree, int level);


/* initialize a point, based on x, y coordinates */
point_t* init_point(long double x, long double y) {
    point_t* point = (point_t*) malloc (sizeof(point_t));
    point->x = x;
    point->y = y;
    return point;
}

/* initialize a square, based on 2 points - bottom left and top right */
square_t* init_square(point_t* bottomL, point_t* topR) {
    square_t* square = (square_t*) malloc (sizeof(square_t));
    square->bottom_left = bottomL;
    square->top_right = topR;
    return square;
}

/* initialize a tree node from a square */
node_t* init_tree(square_t* square) {
    node_t* node = (node_t*) malloc (sizeof(node_t));
    assert(node);
    node->square = square;
    node->point = NULL;
    node->ne = NULL;
    node->nw = NULL;
    node->se = NULL;
    node->sw = NULL;
    return node;
}

/* insert a data point to the tree */
void insert(node_t* tree, point_t* point) {
    // base case - root node
    if (tree->sw == NULL)
        split_insert(tree, point);
    else {
        // finding the right quadrant
        enum quadrant q = determine_quad(tree->square, point);
        return  (q == sw) ? insert(tree->sw, point) :
                (q == se) ? insert(tree->se, point) :
                (q == nw) ? insert(tree->nw, point) :
                insert(tree->ne, point);
    }
}

/* split the root node until the 2 points aren't in the same quadrant
 * to then insert
 */
void split_insert(node_t* root, point_t* point) {
    // if root does not yet have a point, assign it with a point
    if (root->point == NULL) {
        root->point = point;
        return;
    }
    // the 2 points must not be the same (avoiding infinite recursion)
    assert(!point_cmp(root->point, point));
    // split root into 4 branches, check which quadrant
    split(root);
    enum quadrant qroot = determine_quad(root->square, root->point);
    enum quadrant qpoint = determine_quad(root->square, point);
    node_t* child = insert_quadrant(root, root->point, qroot);
    // existing point in different quadrant or not
    if (qroot == qpoint) split_insert(child, point);
    else insert_quadrant(root, point, qpoint);
}

/* split the node, used when insertion reaches leaf node */
void split(node_t* node) {
    // make sure node is a leaf node
    assert(node->sw == NULL);
    // top left and bottom right positions of node's square
    point_t* topLeft = node->square->bottom_left;
    point_t* topRight = node->square->top_right;
    long double xLeft = topLeft->x, xRight = topRight->x;
    long double yBottom = topLeft->y, yTop = topRight->y;
    // midpoint and center positions of node's square
    long double xMid, yMid;
    get_midpoints(node->square, &xMid, &yMid);
    point_t* midLeft = init_point(xLeft, yMid);
    point_t* midTop = init_point(xMid, yTop);
    point_t* midRight = init_point(xRight, yMid);
    point_t* midBottom = init_point(xMid, yBottom);
    point_t* center = init_point(xMid, yMid);
    // split node to 4 child nodes
    node->nw = init_tree(init_square(midLeft, midTop));
    node->ne = init_tree(init_square(center, topRight));
    node->sw = init_tree(init_square(topLeft, center));
    node->se = init_tree(init_square(midBottom, midRight));
}

/* insert to a node based on specified quadrant */
node_t* insert_quadrant(node_t* node, point_t* point, enum quadrant q) {
    assert(node->nw); assert(point);
    node_t** child = (node_t**) malloc (sizeof(node_t*));
    *child = (q == nw) ? node->nw : (q == ne) ? node->ne : (q == sw) ? node->sw : node->se;
    (*child)->point = point;
    return *child;
}

/* point searching in the tree */
void search_pt(node_t* tree, point_t* point) {
    // base case - root node
    if (tree->nw == NULL) {
        if (tree->point != NULL && point_cmp(tree->point, point))
            printf("The point (%Lf, %Lf) has been found.\n", tree->point->x, tree->point->y);
        else printf("Point not found!\n");
        return;
    }
    // otherwise, find the right quadrant
    enum quadrant q = determine_quad(tree->square, point);
    return (q == nw) ? search_pt(tree->nw, point) :
           (q == ne) ? search_pt(tree->ne, point) :
           (q == sw) ? search_pt(tree->sw, point) :
           search_pt(tree->se, point);
}

/* determine which quadrant the point belongs to */
enum quadrant determine_quad(square_t* square, point_t* point) {
    long double x = point->x, y = point->y;
    long double xR = square->top_right->x, yT = square->top_right->y;
    long double xL = square->bottom_left->x, yB = square->bottom_left->y;
    long double xMid = (xR + xL)/2.0, yMid = (yT + yB)/2.0;
    // point must be in square
    assert(x >= xL && x <= xR && y >= yB && y <= yT);
    // return quadrant
    return  (x < xMid && y >= yMid) ? nw :
            (x >= xMid && y > yMid) ? ne :
            (x <= xMid && y < yMid) ? sw :
            se;
}

/* range search operation + check if any point is found within range */
void search_range_check(node_t* tree, square_t* rectangle, int* found) {
    // base case - root node
    if (tree->nw == NULL) {
        if (tree->point != NULL && in_sq(rectangle, tree->point)) {
            printf("Range search: (%Lf, %Lf)\n", tree->point->x, tree->point->y);
            *found = 1;
        }
        return;
    }
    // otherwise, find which quadrant rectangle intersects with
    if (rectangle_intersect(tree->nw->square, rectangle))
        search_range_check(tree->nw, rectangle, found);
    if (rectangle_intersect(tree->ne->square, rectangle))
        search_range_check(tree->ne, rectangle, found);
    if (rectangle_intersect(tree->sw->square, rectangle))
        search_range_check(tree->sw, rectangle, found);
    if (rectangle_intersect(tree->se->square, rectangle))
        search_range_check(tree->se, rectangle, found);
}

/* range search all valid points in tree */
void search_range(node_t* tree, square_t* rectangle) {
    int found = 0;
    search_range_check(tree, rectangle, &found);
    if (!found)
        printf("Range search: no point found!\n");
}

/* rectangle intersection check; call intersect_check both ways (r1 relative
 * to r2 and, likewise the other way around); used for range search
 */
int rectangle_intersect(square_t* r1, square_t* r2) {
    return (oneside_intersect_check(r1, r2) || oneside_intersect_check(r2, r1));
}

/* helper function checking rectangle intersection (only checks onesidedly) */
int oneside_intersect_check(square_t* r1, square_t* r2) {
    long double xRight1 = r1->top_right->x;
    long double yBottom1 = r1->bottom_left->y, yTop1 = r1->top_right->y;
    long double xLeft2 = r2->bottom_left->x, xRight2 = r2->top_right->x;
    long double yBottom2 = r2->bottom_left->y, yTop2 = r2->top_right->y;
    return ((xRight1 >= xLeft2 && xRight1 <= xRight2 &&
            ((yBottom1 >= yBottom2 && yBottom1 <= yTop2) ||
            yTop1 >= yBottom2 && yTop1 <= yTop2)) || yTop1 <= yTop2 && yBottom1 >= yBottom2);
}

/* check whether a point is within a square, or range, or not; used for range search */
int in_sq(square_t* square, point_t* point) {
    return (point->x >= square->bottom_left->x && point->x <= square->top_right->x &&
            point->y >= square->bottom_left->y && point->y <= square->top_right->y);
}

/* get the midpoint coordinates */
void get_midpoints(square_t* square, long double* xMidPass, long double* yMidPass) {
    long double xR = square->top_right->x, yT = square->top_right->y;
    long double xL = square->bottom_left->x, yB = square->bottom_left->y;
    long double xMid = (xR + xL)/2.0, yMid = (yT + yB)/2.0;
    (*xMidPass) = xMid;
    (*yMidPass) = yMid;
}

/* point comparison: check if 2 points lie in the same exact location */
int point_cmp(point_t* p1, point_t* p2) {
    return (p1->x == p2->x && p1->y == p2->y);
}

/* print the entire tree using level-order traversal */
void print_tree(node_t* tree) {
    print_node(tree, 0);
    print_level_order(tree, 0);
}

// print a single node
void print_node(node_t* node, int level) {
    long double xMid, yMid;
    get_midpoints(node->square, &xMid, &yMid);
    if (level <= LEVEL_PRINT_LIM) {
        printf("Square's center (at level %d):\t(%.5Lf, %.5Lf)\n", level, xMid, yMid);
    }
}

// print levels of a tree recursively
void print_level_order(node_t* tree, int level) {
    if (tree->nw == NULL) {
        if (tree->point != NULL) {
            print_node(tree, level);
            printf("   The point in this root is:\t(%.5Lf, %.5Lf)\n",
                   tree->point->x, tree->point->y);
        }
        return;
    }
    if (level == LEVEL_PRINT_LIM) printf("\t...\n   < many more >\n");
    print_level_order(tree->nw, level+1);
    print_level_order(tree->ne, level+1);
    print_level_order(tree->sw, level+1);
    print_level_order(tree->se, level+1);
}

/* free the entire tree structure recursively */
void free_tree(node_t* tree) {
    if (tree == NULL) return;
    free_tree(tree->nw);
    free_tree(tree->ne);
    free_tree(tree->sw);
    free_tree(tree->se);
    tree->square->top_right = NULL;
    tree->square->bottom_left = NULL;
    tree->point = NULL;
    free(tree->square);
    tree->square = NULL;
    free(tree);
    tree = NULL;
}
