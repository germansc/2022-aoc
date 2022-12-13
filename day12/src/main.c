#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAP_MAX_WIDTH            (128)
#define MAP_MAX_HEIGHT           (128)
#define MAX_INPUT_LINE_LENGHT    (MAP_MAX_WIDTH + 1)

#define COORD(X,Y)               (X + MAP_MAX_WIDTH * Y)

typedef enum {
    none = 0,
    up,
    down,
    left,
    right
} directionType;

typedef struct {
    uint8_t x;
    uint8_t y;
} positionType;

typedef struct {
    uint32_t value;
    uint8_t visited;
} cellType;

typedef struct {
    cellType cell[MAP_MAX_WIDTH * MAP_MAX_HEIGHT];
    uint8_t width;
    uint8_t height;
} mapType;

typedef struct item_s {
    uint32_t coord;
    struct item_s* next;
    uint32_t size;
} itemListType;


int32_t getLine(char* buff, uint32_t size)
{
    int32_t len = -1;

    /* fgets() returns NULL on error or when end of file occurs while no
     * characters have been read. */
    if (fgets(buff, size, stdin) != NULL)
    {
        len = strnlen(buff, size);
        if (buff[len-1] == '\n')
        {
            buff[len-1] = '\0';
            len--;
        }
    }

    return len;
}

int8_t listGetItem(itemListType* list, uint32_t* val)
{
    struct item_s* item = list->next;
    if (item == NULL)
        return -1;

    *val = item->coord;
    list->next = item->next;

    list->size--;
    free(item);

    return 0;
}

void listInsertItem(itemListType* list, positionType pos)
{
    if (list == NULL)
        return;

    struct item_s* item = malloc(sizeof(struct item_s));
    if (item == NULL)
        return;

    item->coord = COORD(pos.x, pos.y);
    item->next = NULL;

    struct item_s* last = list->next;
    if (last != NULL)
    {
        while(last->next != NULL)
            last = last->next;

        last->next = item;
    } else {
        list->next = item;
    }

    list->size++;
}

int8_t listValueInList(itemListType* list, positionType p)
{
    if (list == NULL)
        return -1;

    uint32_t value = COORD(p.x, p.y);
    struct item_s* i = list->next;

    while(i != NULL)
        if (i->coord == value)
            return 1;
        else
         i = i->next;

    return 0;
}

void listClear(itemListType* list)
{
    if (list == NULL)
        return;

    struct item_s* n;
    struct item_s* i = list->next;

    while(i != NULL)
    {
        n = i->next;
        free(i);
        i = n;
    }

    list->size = 0;
}

int8_t charToHeight(char c)
{
    int8_t h = -1;

    if ((c >= 'a') && (c <= 'z'))
        h = c - 'a';
    else if (c == 'S')
        h = 0;
    else if (c == 'E')
        h = 'z' - 'a';

    return h;
}

uint8_t getNeighPos(mapType* map, uint32_t offset, directionType dir, positionType* neigh)
{
    positionType p;
    p.x = offset % MAP_MAX_WIDTH;
    p.y = offset / MAP_MAX_WIDTH;

    switch (dir) {
        case up:
            if (p.y == 0)
                return 0;

            neigh->y = p.y - 1;
            neigh->x = p.x;
            break;

        case down:
            if (p.y == map->height-1)
                return 0;

            neigh->y = p.y + 1;
            neigh->x = p.x;
            break;

        case left:
            if (p.x == 0)
                return 0;

            neigh->x = p.x - 1;
            neigh->y = p.y;
            break;

        case right:
            if (p.x == map->width - 1)
                return 0;

            neigh->x = p.x + 1;
            neigh->y = p.y;
            break;

        default:
            printf("UKN DIR\n");
            return 0;
            break;
    }

    return 1;
}

uint8_t canMove(mapType* map, uint32_t offset, directionType dir)
{
    positionType dest;

    if (getNeighPos(map, offset, dir, &dest) == 0)
        return 0;

    uint32_t currentHeight = map->cell[offset].value;

    if (map->cell[COORD(dest.x, dest.y)].value > currentHeight + 1)
        return 0;

    return 1;
}


int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    int32_t i;
    positionType start = {0};
    positionType end = {0};

    mapType height;
    mapType steps;
    // uint32_t shortests = MAP_MAX_HEIGHT * MAP_MAX_WIDTH;

    memset(&height, 0, sizeof(mapType));
    memset(&steps, 0, sizeof(mapType));

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > 0)
    {
        for (i = 0; i < size; i++)
        {
            if (line[i] == 'S')
            {
                start.x =  i; start.y = height.height;
            } else if (line[i] == 'E') {
                end.x =  i; end.y = height.height;
            }

            height.cell[COORD(i,height.height)].value = charToHeight(line[i]);
        }

        height.height++;
        if (size > height.width)
            height.width = size;
    }

    steps.height = height.height;
    steps.width = height.width;

    printf("A %ux%u size map was loaded.\n", steps.width, steps.height);
    printf("Starting at (%u,%u)\n", start.x, start.y);
    printf("Ending at (%u,%u)\n", end.x, end.y);

    positionType neigh;

    uint32_t current;
    uint32_t count = 0;

    itemListType edges = { 0 };
    itemListType nextEdges = { 0 };

    /* Initialize to any other value than none */
    listInsertItem(&edges, start);

#ifndef PART1
    /* Part 2 - Fill the starting edge list will all cells of height 0. */
    for (neigh.x = 0; neigh.x < height.width; neigh.x++)
        for (neigh.y = 0; neigh.y < height.height; neigh.y++)
            if ((height.cell[COORD(neigh.x, neigh.y)].value == 0) && !listValueInList(&edges, neigh))
                listInsertItem(&edges, neigh);

    printf("There are %u possible starting points.\n", edges.size);
#endif

    do
    {
        count++;
        while (listGetItem(&edges, &current) >= 0)
        {
            /* Mark as visited and break if reached the end point */
            steps.cell[current].visited = 1;
            if (current == (uint32_t) COORD(end.x,end.y))
                break;

            directionType dir;
            for (dir = up; dir <= right; dir++)
            {
                if (canMove(&height, current, dir))
                {
                    getNeighPos(&height, current, dir, &neigh);

                    cellType* cell = &steps.cell[COORD(neigh.x, neigh.y)];
                    if ((cell->value == 0) || (cell->value > count))
                        cell->value = count;

                    if ( (cell->visited == 0) && (listValueInList(&nextEdges, neigh) == 0) && (listValueInList(&edges, neigh) == 0))
                        listInsertItem(&nextEdges, neigh);
                }
            }
        }

        edges = nextEdges;
        nextEdges.size = 0;
        nextEdges.next = NULL;

    } while (steps.cell[COORD(end.x, end.y)].visited == 0);

    /* Print the results for both parts of the challenge. */
    uint32_t part1 = steps.cell[COORD(end.x, end.y)].value;

#ifdef PART1
    printf("\nPART 1:\n");
#else
    printf("\nPART 2:\n");
#endif


    printf("The shortest path takes %u steps.\n", part1);

    listClear(&nextEdges);
    listClear(&edges);

    return 0;
}

