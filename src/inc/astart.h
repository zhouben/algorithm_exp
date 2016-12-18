#ifndef _ASTAR_H_
#define _ASTAR_H_

typedef enum _point_state
{
    BLANK,
    BARRIER,
    OPEN,
    CLOSED
} POINT_STATE;

/*
 * f = g + h
 */
typedef struct _point
{
    int x;
    int y;
    POINT_STATE state;
    struct _point* parent;
    int g;
    int h;
} POINT;

int astar_search(POINT* map, int width, int height, int src_x, int src_y, int dest_x, int dest_y);
#endif
