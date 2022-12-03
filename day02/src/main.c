#include <stdio.h>
#include <stdint.h>

uint32_t solveRoundPart1(char me, char them)
{
    uint32_t points = 0;

    switch (me)
    {
        case 'X':
            points += 1;
            if (them == 'A')
                points += 3;
            else if (them == 'B')
                points += 0;
            else
                points += 6;
            break;

        case 'Y':
            points += 2;
            if (them == 'B')
                points += 3;
            else if (them == 'C')
                points += 0;
            else
                points += 6;
            break;

        case 'Z':
            points += 3;
            if (them == 'C')
                points += 3;
            else if (them == 'A')
                points += 0;
            else
                points += 6;
            break;

        default:
            printf("%23s","Lizard?\n");
            return -1;
            break;
    }

    return points;
}


uint32_t solveRoundPart2(char me, char them)
{
    uint32_t points = 0;

    switch (me)
    {
        case 'X':
            points += 0;
            if (them == 'A')
                points += 3;
            else if (them == 'B')
                points += 1;
            else
                points += 2;
            break;

        case 'Y':
            points += 3;
            if (them == 'B')
                points += 2;
            else if (them == 'C')
                points += 3;
            else
                points += 1;
            break;

        case 'Z':
            points += 6;
            if (them == 'C')
                points += 1;
            else if (them == 'A')
                points += 2;
            else
                points += 3;
            break;

        default:
            printf("%23s","Lizard?\n");
            return -1;
            break;
    }

    return points;
}
int32_t main()
{
    uint32_t total1 = 0;
    uint32_t total2 = 0;
    char buffer[5];
    char* line;

    /* Read lines from stdin until EOF. From fgets man, NULL is returned
     * when EOF is reached. */
    do
    {
        line = fgets(buffer, sizeof(buffer), stdin);

        /* Assuming all lines will be "A B", checking the middle character
         * can detect empty lines. */
        if ((line != NULL) && (buffer[1] == ' '))
        {
             total1 += solveRoundPart1(buffer[2], buffer[0]);
             total2 += solveRoundPart2(buffer[2], buffer[0]);
        }

    } while (line != NULL);

    /* Print the results for both parts of the challenge. */
    printf("The total score for part 1 is: %u\n",total1);
    printf("The total score for part 2 is: %u\n",total2);

    return 0;
}

