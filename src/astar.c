#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "astar.h"

typedef struct _list
{
    int head;
    int tail;
    int num;
    int max;
    POINT* entries[1];
} LIST;

static LIST* init_list(int capacity)
{
    LIST* list;
    int siz;
    siz = sizeof(LIST) + (capacity - 1) * sizeof(POINT *);
    list = (LIST*)malloc(siz);
    bzero((void *)list, siz);
    list->max = capacity;
    return list;
}

static void append_point(LIST* list, POINT* point)
{
    list->num++;
    list->entries[list->tail++] = point;
    if (list->tail == list->max)
    {
        list->tail = 0;
    }
}

static POINT* fetch_point(LIST* list)
{
    POINT* ret_ptr = list->entries[list->head];
    if (++list->head == list->max)
    {
        list->head = 0;
    }
    list->num--;
    return ret_ptr;
}

int compre_func(const void* a, const void* b)
{
    POINT* pa = *(POINT**)a;
    POINT* pb = *(POINT**)b;
    return ((pa->g + pa->h) - (pb->g + pb->h));
}

static void sort_list(LIST* list)
{
    int i;
    POINT* p;
    if (list->tail < list->head)
    {
        memmove(&list->entries[list->tail], &list->entries[list->head], (list->max - list->head));
        list->head = 0;
        list->tail = list->num;
    }
    else if (list->tail == list->head)
    {
        return;
    }
    qsort(list->entries + list->head, list->num, sizeof(POINT*), compre_func);
    printf("points in list %d:\n", list->num);
    for(i = 0; i < list->num; i++)
    {
        p = list->entries[list->head + i];
        printf("%d.%d(%4d) ", p->x, p->y, p->g + p->h);
    }
    printf("\n");
}

static int calculate_range(POINT* point, int dest_x, int dest_y)
{
    int diff_x;
    int diff_y;
    diff_x = (point->x > dest_x) ? point->x - dest_x : dest_x - point->x;
    diff_y = (point->y > dest_y) ? point->y - dest_y : dest_y - point->y;
    if (diff_x > diff_y)
    {
        return (diff_y * 14 + (diff_x - diff_y) * 10);
    }
    else
    {
        return (diff_x * 14 + (diff_y - diff_x) * 10);
    }
}

static int list_is_empty(LIST* list)
{
    return (list->num == 0) ? 1 : 0;
}
static POINT* get_neighbour(POINT* map, int width, int height, POINT* curr_point, int dir, int* range)
{
    int curr_x;
    int curr_y;
    int next_x;
    int next_y;

    curr_x = curr_point->x;
    curr_y = curr_point->y;

    switch(dir)
    {
    case 0: next_x = curr_x + 1; next_y = curr_y + 0; *range = 10; break; // right
    case 1: next_x = curr_x + 1; next_y = curr_y + 1; *range = 14; break; // right-down
    case 2: next_x = curr_x + 0; next_y = curr_y + 1; *range = 10; break; // down
    case 3: next_x = curr_x - 1; next_y = curr_y + 1; *range = 14; break; // left-down
    case 4: next_x = curr_x - 1; next_y = curr_y + 0; *range = 10; break; // left
    case 5: next_x = curr_x - 1; next_y = curr_y - 1; *range = 14; break; // left-up
    case 6: next_x = curr_x + 0; next_y = curr_y - 1; *range = 10; break; // up
    case 7: next_x = curr_x + 1; next_y = curr_y - 1; *range = 14; break; // right-up
    default: break;
    }
    if ((next_x < 0) || (next_x >= width) || (next_y < 0) ||(next_y >= height))
    {
        return NULL;
    }
    else
    {
        return &map[next_y * width + next_x];
    }
}
/*
 * return value:
 * 0: successful!
 * 1: cannot find out a path
 */
int astar_search(POINT* map, int width, int height, int src_x, int src_y, int dest_x, int dest_y)
{
    LIST* open_list;
    LIST* closed_list;
    POINT* curr_point;
    POINT* next_point;
    int curr_x;
    int curr_y;
    int next_x;
    int next_y;
    int ret = 1;
    int range;
    int i;
    int capacity;
    
    capacity = (width * height);
    open_list = init_list(capacity); 
    closed_list = init_list(capacity); 

    append_point(open_list, &map[src_y * width + src_x]);
    while(!list_is_empty(open_list))
    {
        curr_point = fetch_point(open_list);
        printf("curr point %d.%d\n", curr_point->x, curr_point->y);
        if ((curr_point->x == dest_x) && (curr_point->y == dest_y))
        {
            ret = 0;
            break;
        }
        for(i = 0; i < 8; i++)
        {
            next_point = get_neighbour(map, width, height, curr_point, i, &range);
            if (NULL != next_point)
            {
                switch(next_point->state)
                {
                case OPEN:
                    if (next_point->g > curr_point->g + range)
                    {
                        next_point->parent = curr_point;
                        next_point->g = curr_point->g + range;
                    }
                    else
                    {
                        printf("skip open point: %d.%d\n", next_point->x, next_point->y);
                    }
                    break;
                case CLOSED:
                    break;
                case BLANK:
                    next_point->g = curr_point->g + range;
                    next_point->h = calculate_range(next_point, dest_x, dest_y);
                    next_point->state = OPEN;
                    next_point->parent = curr_point;
                    printf("append %d.%d (%d.%d) to open\n", next_point->x,next_point->y, next_point->g, next_point->h);
                    append_point(open_list, next_point);
                    break;
                case BARRIER:
                    break;
                default:
                    break;
                }
            }
        }
        curr_point->state = CLOSED;
        append_point(closed_list, curr_point);
        sort_list(open_list);
    }
    if (0 == ret)
    {
        next_point = &map[dest_y * width + dest_x];
        printf("total path length %d.%d\n", next_point->g, next_point->h);
        while(next_point != NULL)
        {
            next_point->state = PATH;
            next_point = next_point->parent;
        }
    }
    free(open_list);
    free(closed_list);
    return ret;
}

