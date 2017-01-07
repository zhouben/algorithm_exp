#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "astar.h"

POINT* init_map(int width, int height)
{
    POINT* map;
    POINT* p;
    int x, y;
    int siz;
    siz = width * height * sizeof(POINT);
    map = (POINT *)malloc(siz);
    bzero((void*)map, siz);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            p = &map[y * width + x];
            p->x = x;
            p->y = y;
        }
    }
    return map;
}

void dump(POINT* map, int width, int height)
{
    int x;
    int y;
    POINT* p;
    p = map;
    char* str;
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            switch (p->state)
            {
            case BLANK  : str = " "; break;
            case BARRIER: str = "x"; break;
            case OPEN   : str = "o"; break;
            case CLOSED : str = "-"; break;
            case PATH   : str = "+"; break;
            default: break;
            }
            printf("%s", str);
            p++;
        }
        printf("\n");
    }
}

static void insert_barrie(POINT* map, int width, int height)
{
    int x, y;
    y = 3;
    for(x = 0; x < 4; x++)
    {
        map[y * width + x].state = BARRIER;
    }
}

int astar_test()
{
    POINT* map;
    int width = 10;
    int height = 10;
    int src_x = 0;
    int src_y = 0;
    int dest_x = 0;
    int dest_y = 5;
    int ret;
    map = init_map(width, height);
    insert_barrie(map, width, height);
    ret = astar_search(map, width, height, src_x, src_y, dest_x, dest_y);
    if (0 == ret)
    {
        printf("path found!\n");
    }
    else
    {
        printf("Failed to search path\n");
    }
    dump(map, width, height);
    free(map);
}
