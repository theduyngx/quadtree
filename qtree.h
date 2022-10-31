/*
 * Header file for qtree, including:
 * Structures, archetypes and basic operations' prototypes of a quadtree.
 * To replace the data quadtree carries with, change it to a comprehensive
 * footpath that also contains a point structure.
 */

/** data and structures */

// quadrant
enum quadrant {sw,nw,ne,se};

// quadtree structure
typedef struct node qtnode_t;

// point, including x,y coordinates
typedef struct point {
    long double x;
    long double y;
} point_t;

// square containing bottom left and top right points
typedef struct square {
    point_t* bottom_left;
    point_t* top_right;
} square_t;

// a qtree node, which contains point, the square and 4 children
struct node {
    point_t* point;
    square_t* square;
    qtnode_t* nw;
    qtnode_t* ne;
    qtnode_t* sw;
    qtnode_t* se;
};

/** function prototypes */

/* initialize a point, based on x, y coordinates */
point_t* init_point(long double x, long double y);

/* initialize a square, based on 2 points - bottom left and top right */
square_t* init_square(point_t* p1, point_t* p2);

/* initialize a tree node from a square */
qtnode_t* init_tree(square_t* square);

/* insert a data point to the tree */
void insert(qtnode_t* tree, point_t* point);

/* point searching in the tree */
void search_pt(qtnode_t* tree, point_t* point);

/* range search all valid points in tree */
void search_range(qtnode_t* tree, square_t* rectangle);

/* determine which quadrant the point belongs to */
enum quadrant determine_quad(square_t* square, point_t* point);

/* check if two rectangles intersect or not; used for range search */
int rectangle_intersect(square_t* r1, square_t* r2);

/* check whether a point is within a square, or range, or not; used
 * for range search
 */
int in_sq(square_t* square, point_t* point);

/* point comparison: check if 2 points lie in the same exact location;
 * used to avoide infinite recursion
 */
int point_cmp(point_t* p1, point_t* p2);

/* print the entire tree using level-order traversal */
void print_tree(qtnode_t* tree);

/* free the entire tree structure recursively */
void free_tree(qtnode_t* tree);
