#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (128)

typedef struct {
    char tree[MAX_INPUT_LINE_LENGHT][MAX_INPUT_LINE_LENGHT];
    uint8_t rows;
    uint8_t cols;
} forestType;


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

int8_t getHeight(char c)
{
    int8_t ret = -1;

    if ((c >= '0') && (c <= '9'))
        ret = c - '0';

    return ret;
}

uint8_t isVisible(const forestType* f, uint8_t row, uint8_t col, uint32_t* score)
{
    if ((row == 0) || (col == 0) || (row == f->rows - 1) || (col == f->cols - 1))
        return 1;

    uint8_t visible = 0;
    uint8_t u = 1, d = 1, l = 1, r = 1;
    int8_t treeHeight = getHeight(f->tree[row][col]);

    /* Up */
    for (u = 1; u <= row; u++)
        if ( getHeight(f->tree[row - u][col]) >= treeHeight )
            break;

    if (u > row)
    {
        visible = 1;
        /* Bring back the index to use as in-sight trees. */
        u--;
    }

    /* Down */
    for (d = 1; d <= f->rows - 1 - row; d++)
        if ( getHeight(f->tree[row + d][col]) >= treeHeight )
            break;

    if (d > f->rows - row - 1)
    {
        visible = 1;
        /* Bring back the index to use as in-sight trees. */
        d--;
    }

    /* Left */
    for (l = 1; l <= col; l++)
        if ( getHeight(f->tree[row][col - l]) >= treeHeight )
            break;

    if (l > col)
    {
        visible = 1;
        /* Bring back the index to use as in-sight trees. */
        l--;
    }

    /* Right */
    for (r = 1; r <= f->cols - col - 1; r++)
        if ( getHeight(f->tree[row][col + r]) >= treeHeight )
                break;

    if (r > f->cols - col -1)
    {
        visible = 1;
        /* Bring back the index to use as in-sight trees. */
        r--;
    }

    *score = (u * l * r * d);

    return visible;
}

int32_t main()
{
    forestType forest;
    int32_t size;
    uint8_t i, j;
    uint32_t part1 = 0;
    uint32_t part2 = 0;

    memset(&forest, 0, sizeof(forestType));

    /* Read lines from stdin until EOF */
    while((size = getLine(forest.tree[forest.rows++], MAX_INPUT_LINE_LENGHT)) > 0)
        if (size > forest.cols)
            forest.cols = size;

    /* Remove the last increment */
    forest.rows--;

    printf("Forest -> %ux%u\n", forest.rows, forest.cols);

    for (i = 0; i < forest.rows; i++)
        for (j = 0; j < forest.cols; j++)
        {
            uint32_t prettyScore;
            part1 += isVisible(&forest, i, j, &prettyScore);
            if (prettyScore > part2)
                part2 = prettyScore;
        }

    /* Print the results for both parts of the challenge. */
    printf("Part 1: %u\n", part1);
    printf("Part 2: %u\n", part2);

    return 0;
}

