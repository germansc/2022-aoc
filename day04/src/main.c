#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_LENGHT (64)

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


uint8_t fullyOverlapped(const uint32_t* range1, const uint32_t* range2)
{
    uint8_t result;

    if ((range1[0] >= range2[0]) && (range1[1] <= range2[1]))
        result = 1;
    else if ((range2[0] >= range1[0]) && (range2[1] <= range1[1]))
        result = 1;
    else
        result = 0;

    return result;
}


uint8_t notOverlapped(const uint32_t* range1, const uint32_t* range2)
{
    uint8_t result;

    if ((range1[0] > range2[1]) || (range1[1] < range2[0]))
        result = 1;
    else
        result = 0;

    return result;
}


int32_t main()
{
    char line[MAX_LENGHT];
    int32_t size;
    uint32_t range1[2];
    uint32_t range2[2];
    uint32_t part1 = 0;
    uint32_t part2 = 0;
    uint32_t totalPairs = 0;

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_LENGHT)) > -1)
    {
        /* Skip empty lines... if any.*/
        if (size > 0) {
            if (sscanf(line, "%u-%u,%u-%u", range1, range1+1, range2, range2+1) == 4)
            {
                part1 += fullyOverlapped(range1, range2);
                part2 += notOverlapped(range1, range2);
                totalPairs++;
            }
        }
    }

    /* Print the results for both parts of the challenge. */
    printf("There are %u fully overlapped ranges in %u pairs.\n", part1, totalPairs);
    printf("There are %u not overlapped ranges, so there are %u pairs with some overlap.\n", part2, totalPairs - part2);

    return 0;
}

