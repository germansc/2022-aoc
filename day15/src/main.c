#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#define MAX_INPUT_LINE_LENGHT    (255)
#define MAX_SENSORS               (35)

#define TARGET_ROW          (2000000U)

typedef struct {
    int32_t x;
    int32_t y;
} positionType;


typedef struct {
    positionType pos;
    positionType beacon;
    uint32_t range;
} sensorType;


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

uint32_t getRange(positionType a, positionType b)
{
    int32_t dx = a.x - b.x;
    int32_t dy = a.y - b.y;

    return (abs(dx) + abs(dy));
}


int8_t loadSensor(char* line, sensorType* s)
{
    if (line == NULL || s == NULL)
        return -1;

    int32_t parsed =
        sscanf(line, "%*s %*s x=%d, y=%d: %*s %*s %*s %*s x=%d, y=%d",
               &s->pos.x, &s->pos.y, &s->beacon.x, &s->beacon.y);

    if (parsed != 4)
        return -1;

    /* Get the "steps" to the sensor */
    s->range = getRange(s->beacon, s->pos);

    return 0;
}



int8_t getScanRange(sensorType* slist, uint32_t len, uint32_t targetrow, positionType* start, positionType* end)
{
    if (slist == NULL || start == NULL || end == NULL)
        return -1;

    int8_t foundIntersection = 0;
    int32_t min = 2147483647;
    int32_t max = -min - 1;

    uint32_t i;
    for (i = 0; i < len; i++)
    {
        uint32_t distToTarget = abs((int32_t)(targetrow - slist[i].pos.y));
        if (slist[i].range > distToTarget)
        {
            int32_t rem = slist[i].range - distToTarget;
            if (slist[i].pos.x - rem < min)
                min = slist[i].pos.x - rem;

            if (slist[i].pos.x + rem > max)
                max = slist[i].pos.x + rem;

            foundIntersection = 1;
        }
    }

    start->x = min;
    end->x = max;
    start->y = end->y = targetrow;

    return foundIntersection;
}

int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t part1 = 0;
    uint32_t part2 = 0;

    sensorType s[MAX_SENSORS] = { 0 };
    uint8_t totalSensors = 0;

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > -1)
        if (size != 0)
            if (loadSensor(line, &s[totalSensors++]) != 0)
                break;

    printf("Loaded %u sensor reports.\n", totalSensors);
    for (part1 = 0; part1 < totalSensors; part1++)
        printf("* Sensor (%d,%d) - Beacon (%d, %d) => Range %u\n",
               s[part1].pos.x, s[part1].pos.y, s[part1].beacon.x,
               s[part1].beacon.y, s[part1].range);

    positionType start, end;
    if (getScanRange(s, totalSensors, TARGET_ROW, &start, &end) == 1)
        printf("\nScanrange: (%d,%d) to (%d,%d)\n", start.x, start.y, end.x, end.y);

    /* Part 1 */
    part1 = 0;

    uint32_t i;
    while (start.x++ < end.x)
    {
        for (i = 0; i < totalSensors; i++)
            if (getRange(start, s[i].pos) < s[i].range)
                continue;

        if (i == totalSensors)
            part1++;
    }

    /* Print the results for both parts of the challenge. */
    printf("Part 1: %u\n", part1);
    printf("Part 2: %u\n", part2 + 1);

    return 0;
}
