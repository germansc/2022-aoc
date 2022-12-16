#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <math.h>

#define MAX_INPUT_LINE_LENGHT    (255)
#define MAX_SENSORS               (35)

#define TARGET_ROW          (2000000U)
#define PART2_LIMIT         (4000000U)

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

int8_t canBeBeacon(sensorType* slist, uint32_t len, positionType pos)
{
    if (slist == NULL)
        return -1;

    uint32_t i;

    for (i = 0; i < len; i++)
        if (getRange(pos, slist[i].pos) <= slist[i].range)
            return 0;

    /* By default, any position can containe a beacon. */
    return 1;
}

int8_t isInMainBoundary(positionType p)
{
    return ((p.x >= 0) && (p.x <= (int32_t)PART2_LIMIT) && (p.y >= 0) && (p.y <= (int32_t)PART2_LIMIT));
}

int8_t getNextPerimeterPoint(sensorType* s, positionType* p)
{
    /* QUAD 1 */
    if (p->x >= s->pos.x && p->y <= s->pos.y) {
        p->x++;
        p->y++;
    } else if (p->x >= s->pos.x && p->y >= s->pos.y) {
        p->x--;
        p->y++;
    } else if (p->x <= s->pos.x && p->y >= s->pos.y){
        p->x--;
        p->y--;
    } else {
        p->x++;
        p->y--;
    }

    /* Check if we are back at the top and return 0 in that case. */
    if ((p->x == s->pos.x) && (p->y < s->pos.y))
        return 0;
    else
        return 1;
}


int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t part1 = 0;

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
        printf("\nPART 1 Scan: (%d,%d) to (%d,%d)\n", start.x, start.y, end.x, end.y);

    /* Part 1 */
    part1 = 0;
    
    while (start.x++ <= end.x)
        if (canBeBeacon(s, totalSensors, start) == 0)
            part1++;

    printf("PART 1: %u positions cannot contain a becon at row %u\n", part1, TARGET_ROW);

    /* Part 2 */
    uint8_t sensor;
    uint8_t found = 0;
    positionType test;
    
    for (sensor = 0; sensor < totalSensors; sensor++)
    {
        test.x = s[sensor].pos.x;
        test.y = s[sensor].pos.y - s[sensor].range - 1;

        /* Check the perimeter. */
        do {
            if (isInMainBoundary(test) && canBeBeacon(s, totalSensors, test))
            {
                found = 1;
                break;
            }
        } while (getNextPerimeterPoint(s + sensor, &test) != 0);

        if (found)
            break;
    }

    uint64_t freq = (test.x * 4000000UL + test.y);
    printf("PART 2: Position found at (%u, %u) with frequency %lu\n", test.x, test.y, freq);

    return 0;
}
