#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_LENGHT (4097)

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

/* Assuming the string will be just letters, I can use the score function
 * from day03 to give each one a unique id. */
uint32_t score(char c)
{
    uint32_t val;

    if ((c >= 'a') && (c <= 'z'))
        val = c - 'a';
    else if ((c >= 'A') && (c <= 'Z'))
        val = c - 'A' + 26;
    else
        val = 0;

    return val;
}

int32_t findMarkerEnd(const char* buffer, const uint32_t makerLenght)
{
    uint32_t k, j;
    uint32_t len = strlen(buffer);
    uint64_t charMask = 0x00;

    for (k = 0; k < len - makerLenght; k++)
    {
        for (j = 0; j < makerLenght; j++)
                if ( charMask & (1 << score(buffer[k+j])))
                    break;
                else
                    charMask |= (1 << score(buffer[k+j]));

        if ( j == makerLenght )
            return k + makerLenght;
        else
            charMask = 0;
    }

    /* No unique makerLenght sub-string found on buffer? */
    return -1;
}


int32_t main()
{
    char line[MAX_LENGHT];
    int32_t size;
    int32_t part1 = -1;
    int32_t part2 = -1;

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_LENGHT)) > -1)
        if (size > 0)
        {
            part1 = findMarkerEnd(line, 4);
            part2 = findMarkerEnd(line, 14);
        }

    /* Print the results for both parts of the challenge. */
    printf("First marker after character %d\n", part1);
    printf("First message after character %d\n", part2);

    return ((part1 == -1) || (part2 == -1))?-1:0;
}

