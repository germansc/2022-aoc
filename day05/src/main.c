#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGHT (64)
#define MAX_STACK_SIZE  (64)
#define TOTAL_STACKS    (9)
#define AUX_INDEX       (2*TOTAL_STACKS)

typedef struct {
    char* stack;
    uint32_t top;
    uint32_t size;
} crateStackType;


int32_t createStack(crateStackType* s, uint32_t maxSize)
{
    if ((s->stack == NULL) && (s->stack = malloc(maxSize)) != NULL)
    {
        s->top = 0;
        s->size = maxSize;
        return 0;
    } else {
        return -1;
    }
}

void destroyStack(crateStackType* s)
{
    if (s->stack != NULL)
    {
        free(s->stack);
        s->top = 0;
        s->size = 0;
    }
}

char stackPop(crateStackType* s)
{
    char c = 0;

    if ((s != NULL) && s->top > 0)
    {
        c = s->stack[s->top-1];
        s->top--;
    }

    return c;
}

/* Insert an element at the top of the stack. */
void stackPush(crateStackType* s, char c)
{
    if ( (s != NULL) && (s->top < s->size) )
    {
        s->stack[s->top++] = c;
    }
}

/* Insert an element at the bottom of the stack... sleight of hand */
void stackSlide(crateStackType* s, char c)
{
    if ( (s != NULL) && (s->top < s->size) )
    {
        uint32_t k;

        /* Shift all elements one level. */
        for (k = 0; k < s->top; k++)
            s->stack[s->top - k] = s->stack[s->top-k-1];

        /* Add the new element at the bottom of the stack. */
        s->stack[0] = c;
        s->top++;
    }
}

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




int32_t main()
{
    char line[MAX_LINE_LENGHT];
    int32_t size;

    /* The first TOTAL_STACKS will correspond to part 1 of the puzzle.
     * The second TOTAL_STACKS will correspondo to part 2 of the puzzle.
     * The last STACK will be an auxiliary stack to  simplify part 2. */
    crateStackType stacks[2*TOTAL_STACKS + 1] = {{0}};
    uint32_t k, n, src, dst;

    for (k = 0; k < 2 * TOTAL_STACKS + 1; k++)
        createStack(&stacks[k], MAX_STACK_SIZE);

    /* Get the initial state of the stacks. Reads until the empty line that
     * separates the stacks state from the crane instructions. */
    while((size = getLine(line, MAX_LINE_LENGHT)) > 0)
    {
        /* if the second character is a '1', we reached the stack enumeration
         * line of the input. */
        if (line[1] != '1')
            for (k = 0; (k < TOTAL_STACKS) && (4*k+3 <= (uint32_t)size); k++)
                if (line[4*k] == '[')
                {
                    /* Load both PART1 and PART2 stacks on this step. */
                    stackSlide(&stacks[k], line[4*k+1]);
                    stackSlide(&stacks[TOTAL_STACKS + k], line[4*k+1]);
                }
    }

    /* Read instructions and apply them to the stacks until EOF. */

    while((size = getLine(line, MAX_LINE_LENGHT)) > -1)
        if (size > 0)
        {
            if (sscanf(line, "move %u from %u to %u", &n, &src, &dst) == 3)
            {
                    /* Turn index into offset. */
                src--;
                dst--;

                /* PART 1 */
                for (k = 0; k < n; k++)
                    stackPush(&stacks[dst], stackPop(&stacks[src]));

                /* PART 2 - Pop the crates to the AUX stack, and the pop them
                 * back to the dst stack so they are flipped again. */
                for (k = 0; k < n; k++)
                    stackPush(&stacks[AUX_INDEX], stackPop(&stacks[TOTAL_STACKS+src]));

                for (k = 0; k < n; k++)
                    stackPush(&stacks[TOTAL_STACKS+dst], stackPop(&stacks[AUX_INDEX]));
            }
        }

    /* Print Results */
    printf("PART 1:\n");
    for (n = 0; n < TOTAL_STACKS; n++)
    {
        printf("STACK %u: ", n);
        for (k=0; k < stacks[n].top; k++)
            printf("%c ", stacks[n].stack[k]);
        printf("\n");
    }

    printf("Answer: ");
    for (n = 0; n < TOTAL_STACKS; n++)
        printf("%c", stackPop(&stacks[n]));
    printf("\n\n");


    printf("PART 2:\n");
    for (n = 0; n < TOTAL_STACKS; n++)
    {
        printf("STACK %u: ", n);
        for (k=0; k < stacks[TOTAL_STACKS+n].top; k++)
            printf("%c ", stacks[TOTAL_STACKS+n].stack[k]);
        printf("\n");
    }

    printf("Answer: ");
    for (n = 0; n < TOTAL_STACKS; n++)
        printf("%c", stackPop(&stacks[TOTAL_STACKS+n]));
    printf("\n\n");


    /* Cleanup */
    for (k = 0; k < 2 * TOTAL_STACKS + 1; k++)
        destroyStack(&stacks[k]);

    return 0;
}

