#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (255)

#define MAP_WIDTH                (1024)  // Input x values ranges from 492 to 562
#define MAP_HEIGHT               (180) // Input y values ranges from 13 to 173

#define MAP_XOFFSET              (0)
#define MAP_YOFFSET              (0)

#define COORD(X,Y)               ((X - MAP_XOFFSET) + MAP_WIDTH * (Y - MAP_YOFFSET))

typedef struct {
    uint32_t x;
    uint32_t y;
} positionType;

typedef struct {
    char map[MAP_HEIGHT * MAP_WIDTH];
    uint32_t width;
    uint32_t height;
    uint32_t minx, maxx, miny, maxy;
} mapType;


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

void mapUpdateLimits(mapType* m, positionType v)
{
    if (v.x > m->maxx)
        m->maxx = v.x;

    if (v.x < m->minx)
        m->minx = v.x;

    if (v.y > m->maxy)
        m->maxy = v.y;

    if (v.y < m->miny)
        m->miny = v.y;
}

void buildWalls(mapType* m, char* input)
{
    if (m == NULL || input == NULL)
        return;

    int32_t used;
    positionType start = {0}, end = {0};
    sscanf(input, "%u,%u%n", &start.x, &start.y, &used);
    mapUpdateLimits(m, start);

    input += used;

    while(*input != '\0')
    {
        sscanf(input, " -> %u,%u%n", &end.x, &end.y, &used);
        mapUpdateLimits(m, end);

        int32_t dx = end.x - start.x;
        int32_t dy = end.y - start.y;
        int32_t i;

        if (dx > 0)
            for (i = 0; i <= dx; i++)
                m->map[COORD(start.x + i, start.y)] = '#';
        else
            for (i = 0; i <= -dx; i++)
                m->map[COORD(start.x - i, start.y)] = '#';

        if (dy > 0)
            for (i = 0; i <= dy; i++)
                m->map[COORD(start.x, start.y + i)] = '#';
        else
            for (i = 0; i <= -dy; i++)
                m->map[COORD(start.x, start.y - i)] = '#';

        input += used;
        start = end;
    }

    m->height = m->maxy - m->miny;
    m->width  = m->maxx - m->minx;
}

void printMap(mapType* m, uint32_t startX, uint32_t startY, uint32_t width, uint32_t height)
{
    if (m == NULL)
        return;

    if (width > MAP_WIDTH)
        width = MAP_WIDTH;

    if (height > MAP_HEIGHT)
        height = MAP_HEIGHT;

    printf("\nMAP Ranges: %u - %u | %u - %u\n\n", m->minx, m->maxx, m->miny, m->maxy);
    printf("   %u\n   %u\n   %u\n", startX/99, (startX/10) % 10, startX % 10);

    uint32_t i,j;
    for (j = 0; j < height; j++)
    {
        printf("%2u ", startY + j);
        for (i = 0; i < width; i++)
        {
            printf("%c", m->map[COORD(startX + i, startY + j)]);
        }
        printf("\n");
    }

    printf("\n");
}

int8_t solveSand(mapType* m, positionType startingPos, uint8_t part)
{
    if (m == NULL)
        return -1;

    positionType sand = startingPos;

    while ((sand.y < m->maxy) || (part == 2))
    {
        if (m->map[COORD(sand.x, sand.y + 1)] == '.')
        {
            sand.y++;

            /* Part 2, add infinite floor when below limit */
            if (sand.y == m->maxy + 2)
            {
                m->map[COORD(sand.x, sand.y)] = '#';
                m->map[COORD(sand.x, sand.y - 1)] = 'o';
                return 0;
            }

        } else if ((sand.x - 1 > MAP_XOFFSET) && (m->map[COORD(sand.x - 1, sand.y + 1)] == '.')) {
            sand.x--;
            sand.y++;
        } else if ((sand.x + 1 < MAP_XOFFSET + MAP_WIDTH) && (m->map[COORD(sand.x + 1, sand.y + 1)] == '.')) {
            sand.x++;
            sand.y++;
        } else {
            m->map[COORD(sand.x, sand.y)] = 'o';

            /* if the item blocks the entry point, done part 2 */
            if (sand.x == startingPos.x && sand.y == startingPos.y)
                return 2;
            else
                /* Stable position */
                return 0;
        }
    }

    /* Falling below the max Y, done part 1. */
    return 1;
}

int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t part1 = 0;
    uint32_t part2 = 0;

    mapType map = { 0 };
    map.minx = map.miny = 9999;

    memset(map.map, '.', sizeof(map.map));
    map.map[COORD(500,0)] = '+';

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > -1)
    {
        if (size == 0)
            continue;

        buildWalls(&map, line);
    }

    while (solveSand(&map, (positionType){500, 0}, 1) != 1)
        part1++;

    part2 = part1;

    printMap(&map, map.minx - 2, 0, map.width + 6, map.maxy + 2);

    while (solveSand(&map, (positionType){500, 0}, 2) != 2)
        part2++;

    printMap(&map, 500 - map.width, 0, 2 * map.width, map.maxy + 4);

    /* Print the results for both parts of the challenge. */
    printf("Part 1: %u\n", part1);
    printf("Part 2: %u\n", part2 + 1);


    return 0;
}

