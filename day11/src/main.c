#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (64)
#define MAX_MONKEYS              (8)
#define MAX_STARTING_ITEMS       (8)

#ifdef PART1
#define MAX_ROUNDS    (20)
#else
#define MAX_ROUNDS    (10000U)
#endif

typedef struct item_s {
    uint64_t worry;
    struct item_s* next;
} itemType;

typedef struct list_s {
    uint32_t size;
    itemType* first;
    itemType* last;
} itemList;

typedef struct {
    itemList items;
    char op;
    uint32_t opArg;
    uint32_t test;
    uint32_t trueIndex;
    uint32_t falseIndex;
    uint32_t activity;
} monkeyType;


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

itemType* createItem(uint32_t value)
{
    itemType* new = malloc(sizeof(itemType));
    if (new != NULL)
    {
        new->next = NULL;
        new->worry = value;
    }

    return new;
}

void listAdd(itemList* list, itemType* item)
{
    if ((list == NULL) || (item == NULL) )
        return;

    if (list->size == 0)
    {
        list->first = item;
        list->last = item;
    } else {
        list->last->next = item;
        list->last = item;
    }

    list->size++;
}

itemType* listRemove(itemList* list, itemType* item)
{
    if ((list == NULL) || (item == NULL))
        return NULL;

    itemType* cur = list->first;
    if (cur == item)
    {
        list->first = item->next;
    } else {
        uint8_t i;
        for (i = 0; i < list->size; i++)
        {
            if (cur->next == item)
            {
                cur->next = item->next;
                break;
            }
        }
    }

    list->size--;
    return item;
}

void listClean(itemList* list)
{
    if (list == NULL)
        return;

    uint32_t i;
    itemType* cur = list->first;
    itemType* next = NULL;

    for (i = 0; i < list->size; i++)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

void monkeyInspect(monkeyType* monkey)
{
    if ( monkey == NULL )
        return;

    uint8_t i;
    itemType* item = monkey->items.first;

    for (i = 0; i < monkey->items.size; i++)
    {
        switch (monkey->op)
        {
            case '+':
                item->worry += monkey->opArg;
                break;
            case '*':
                item->worry *= monkey->opArg;
                break;
            case 's':
                item->worry *= item->worry;
                break;
            default:
                printf("UNK OP\n");
                break;
        }

        monkey->activity++;

        item = item->next;
    }

    return;
}


int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;

    int8_t i, count, indx = -1;
    uint32_t value[MAX_STARTING_ITEMS], round;
    monkeyType monkeys[MAX_MONKEYS];

    for (i = 0; i < MAX_MONKEYS; i++)
        memset(&monkeys[i], 0, sizeof(monkeyType));

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > -1)
    {
        /* Skip empty line */
        if (size == 0)
            continue;

        if (strncmp(line, "Monkey", 6) == 0)
        {
            indx++;
        } else if (strncmp(line, "  Operation", 11) == 0) {
            if (sscanf(line, "%*s %*s %*s %*s %c %u", &monkeys[indx].op, &monkeys[indx].opArg) != 2)
                monkeys[indx].op = 's';
        } else if (strncmp(line, "  Test:", 7) == 0) {
            sscanf(line, "  Test: divisible by %u", &monkeys[indx].test);
        } else if (strncmp(line, "    If true:", 12) == 0) {
            sscanf(line, "%*s %*s %*s %*s %*s %u", &monkeys[indx].trueIndex);
        } else if (strncmp(line, "    If false:", 13) == 0) {
            sscanf(line, "%*s %*s %*s %*s %*s %u", &monkeys[indx].falseIndex);
        } else if (strncmp(line, "  Starting items", 16) == 0) {
            count = sscanf(line + 17, "%u, %u, %u, %u, %u, %u, %u, %u", value, value+1, value+2, value+3, value+4, value+5, value+6, value+7);
            for (i = 0; i < count; i++)
                listAdd(&monkeys[indx].items, createItem(value[i]));
        } else {
            printf("UNK LINE\n");
        }
    }

#ifndef PART1
    /* Get a whole number divisible by all the tests values.
     * See note in the CALM DOWN section below... */
    uint32_t whole = 1;
    for (i = 0; i <= indx; i++)
        whole *= monkeys[i].test;
#endif

    /* Solve the 20 turns. */
    for (round = 0; round < MAX_ROUNDS; round++)
    {
        for (i = 0; i <= indx; i++)
        {
            /* Inspect all items  */
            monkeyType* m = &monkeys[i];
            monkeyInspect(m);

            /* Solve all item swapping */
            uint8_t j;
            uint8_t total = m->items.size;
            itemType* item = m->items.first;
            for (j = 0; j < total; j++)
            {
                /* CALM DOWN! */
            #ifdef PART1
                item->worry /= 3;
            #else
                /* Given that all tests are a modulo operation, I could keep
                 * the worry level manageable by removing the "whole" part of
                 * the operation, which means applying the modulus now, and then
                 * the modulus to the remainder in the tests, and the result
                 * would be the same.
                 *
                 * To avoid breaking tests by other monkeys, the whole part
                 * removed should be a multiple of all the tests, and given
                 * from the input that all tests are prime numbers, I'll need
                 * to multiply them to get a number divisible by all of them.
                 * */
                item->worry = item->worry % whole;
            #endif

                /* Throw the item... */
                if (item->worry % m->test == 0)
                    listAdd(&monkeys[m->trueIndex].items, listRemove(&m->items, item));
                else
                    listAdd(&monkeys[m->falseIndex].items,listRemove(&m->items, item));

                item = item->next;
            }
        }
    }

    /* Find top monkeys. */
    uint64_t top = 0, sec = 0;

    for (i = 0; i <= indx; i++)
        if (monkeys[i].activity > top)
        {
            sec = top;
            top = monkeys[i].activity;
        } else if (monkeys[i].activity > sec) {
            sec = monkeys[i].activity;
        }

    /* Print the results for both parts of the challenge. */
#ifdef PART1
    printf("PART 1:\n\n");
#else
    printf("PART 2:\n\n");
#endif

    printf("Top %lu | Sec %lu\n", top, sec);
    printf("Monkey Business: %lu\n", top * sec);

    for (i = 0; i < MAX_MONKEYS; i++)
        listClean(&monkeys[i].items);

    return 0;
}

