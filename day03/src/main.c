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

uint32_t score(char c)
{
    uint32_t val;

    if ((c >= 'a') && (c <= 'z'))
        val = c - 'a' + 1;
    else if ((c >= 'A') && (c <= 'Z'))
        val = c - 'A' + 27;
    else
        val = 0;

    return val;
}

char scoreToItem(uint32_t s)
{
    char c = 0;
    if (s < 27)
        c = s - 1 + 'a';
    else
        c = s - 27 + 'A';

    return c;
}

uint32_t processSingleSack(char* sack, uint32_t size)
{
    const uint32_t compSize = size / 2;
    uint32_t i, j;

    /* Assuming there is only one repeated item in the sack. */
    for (i = 0; i < compSize; i++)
        for (j = compSize; j < size; j++)
                if (sack[i] == sack[j])
                    return score(sack[i]);

    /* If no repeated item was found. */
    return 0;
}

uint32_t processGroupSacks(char* sack1, char* sack2, char* sack3)
{
    uint32_t i, j, k;

    for (i = 0; i < strlen(sack1); i++)
        for (j = 0; j < strlen(sack2); j++)
            if (sack1[i] == sack2[j])
                for (k = 0; k < strlen(sack3); k++)
                    if (sack1[i] == sack3[k])
                        return score(sack1[i]);

    /* If no repeated item was found. */
    return 0;
}


int32_t main()
{
    char rucksack[MAX_LENGHT];
    char sackGroup[3][MAX_LENGHT];
    int32_t size, count = 0;
    uint32_t val, part1 = 0;
    uint32_t badge, part2 = 0;

    /* Read lines from stdin until EOF */
    while((size = getLine(rucksack, MAX_LENGHT)) > -1)
    {
        /* Skip empty lines... if any.*/
        if (size > 0) {
            val = processSingleSack(rucksack, size);
            printf("%-*s | size: %d | value: %u\n", MAX_LENGHT, rucksack, size, val);
            part1 += val;

            /* Part two, add it to the group and process them on the third one */
            strncpy(sackGroup[count++], rucksack, size + 1);
            if (count == 3)
            {
                count = 0;
                badge = processGroupSacks(sackGroup[0], sackGroup[1], sackGroup[2]);
                printf("Badge: %c | Priority: %u\n\n", scoreToItem(badge), badge);
                part2 += badge;
            }
        }
    }

    /* Print the results for both parts of the challenge. */
    printf("\nThe sum of priorities is: %u\n", part1);
    printf("The sum of badge priorities is: %u\n", part2);

    return 0;
}

