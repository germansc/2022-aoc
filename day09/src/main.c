#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (16)
#define MAP_WIDTH                (512)
#define MAP_HEIGHT               (512)
#define TAIL_KNOTS               (9)

#define COORD(X,Y)               (X + MAP_WIDTH * Y)

typedef struct {
    uint32_t x;
    uint32_t y;
} positionType;


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


void moveKnot(positionType* h, char dir)
{
    switch (dir)
    {
        case 'U':
            if (h->y != 0)
                h->y -= 1;
            else
                printf("ERR out of bounds %c\n", dir);
            break;

        case 'D':
            if (h->y != MAP_HEIGHT - 1)
                h->y += 1;
            else
                printf("ERR out of bounds %c\n", dir);
            break;

        case 'L':
            if (h->x != 0)
                h->x -= 1;
            else
                printf("ERR out of bounds %c\n", dir);
            break;

        case 'R':
            if (h->x != MAP_WIDTH - 1)
                h->x += 1;
            else
                printf("ERR out of bounds %c\n", dir);
            break;

        default:
            printf("UNK Direction\n");
            break;
    }
}

void moveLink(positionType* t, positionType* h)
{
    /* See if we have to move at all. */
    int8_t dx = h->x - t->x;
    int8_t dy = h->y - t->y;

    if (((dx < 2) && (dx > -2)) && ((dy < 2) && (dy > -2)))
        return;

    if ( dx > 0 )
        moveKnot(t, 'R');
    else if (dx < 0)
        moveKnot(t, 'L');

    if ( dy > 0 )
        moveKnot(t, 'D');
    else if (dy < 0)
        moveKnot(t, 'U');
}

uint8_t isPositionNew(positionType* knot, char* hist)
{
    if (hist[COORD(knot->x, knot->y)] == '#')
        return 0;

    /* New place, mark as visited. */
    hist[COORD(knot->x, knot->y)] = '#';
    return 1;
}


int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t part1;
    uint32_t part2;
    char direction;
    uint32_t count;
    uint8_t k = 0;

    positionType head, tail[TAIL_KNOTS];
    char beenPart1[MAP_WIDTH * MAP_HEIGHT];
    char beenPart2[MAP_WIDTH * MAP_HEIGHT];

    memset(beenPart1, '.', sizeof(beenPart1));
    memset(beenPart2, '.', sizeof(beenPart2));

    /* Assume we start in the middle of the map. */
    head.x = MAP_WIDTH / 2;
    head.y = MAP_HEIGHT / 2;

    while (k < TAIL_KNOTS)
        tail[k++] = head;

    beenPart1[COORD(head.x,head.y)] = '#';
    beenPart2[COORD(head.x,head.y)] = '#';
    part1 = 1;
    part2 = 1;

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > 0)
        if (sscanf(line, "%c %u", &direction, &count) == 2)
            while (count-- > 0)
            {
                /* PART 1 */
                moveKnot(&head, direction);
                moveLink(&tail[0], &head);

                for (k = 1; k < TAIL_KNOTS; k++)
                    moveLink(&tail[k], &tail[k-1]);

                part1 += isPositionNew(&tail[0], beenPart1);
                part2 += isPositionNew(&tail[TAIL_KNOTS - 1], beenPart2);
            }

    /* Print the results for both parts of the challenge. */
    printf("Part 1: %u\n", part1);
    printf("Part 2: %u\n", part2);

    return 0;
}

