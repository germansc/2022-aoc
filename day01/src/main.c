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
        int32_t result;

        line = fgets(buffer, sizeof(buffer), stdin);
        result = sscanf(buffer, "%u", &cal);

        if ( (line != NULL) && (result == 1) )
        {
            /* We got a match... add it to the toll. */
            sum += cal;

        } else {

            /* No match, no EOF... so it's an elf separator. */
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

    printf("The top elf is carrying %u calories\n", best[0]);
    printf("The top 3 elfs are carrying %u calories combined\n\n", best[0] + best[1] + best[2]);
    return 0;
}

