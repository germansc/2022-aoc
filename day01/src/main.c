#include <stdio.h>
#include <stdint.h>

int32_t main()
{
    uint32_t best[3] = {0};
    uint32_t sum = 0;
    uint32_t cal = 0;
    char buffer[32];
    char* line;

    do
    {
        /* Read lines from stdin until EOF. From fgets man, NULL is returned
         * when EOF is reached. */
        line = fgets(buffer, sizeof(buffer), stdin);

        if ((line != NULL) && (sscanf(buffer, "%u", &cal) == 1))
        {
            /* We got a pattern match... add cals to the sum. */
            sum += cal;

        } else {

            /* No match or EOF... in any case is an elf separator. */
            if (sum > best[0])
            {
                best[2] = best[1];
                best[1] = best[0];
                best[0] = sum;
            } else if (sum > best[1]) {
                best[2] = best[1];
                best[1] = sum;
            } else if (sum > best[2]) {
                best[2] = sum;
            }

            sum = 0;
        }
    } while (line != NULL);

    /* Print the results for both parts of the challenge. */
    printf("The top elf is carrying %u calories\n", best[0]);
    printf("The top 3 elfs are carrying %u calories combined\n\n",
           best[0] + best[1] + best[2]);

    return 0;
}

