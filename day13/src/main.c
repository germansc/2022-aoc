#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (512)
#define MAX_INPUT_LISTS          (302)

typedef enum {
    none,
    int_t,
    list_t
} dataType;

typedef struct item_s {
    void* dataPtr;
    dataType type;
    struct item_s* next;
} itemType;

typedef struct list_s {
    uint8_t size;
    itemType* first;
    itemType* last;
} listType;


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


listType* listCreate(void)
{
    listType* list = malloc(sizeof(listType));
    if ( list != NULL )
    {
        list->first = NULL;
        list->last = NULL;
        list->size = 0;
    }

    return list;
}


int8_t listAddItem(listType* list, void* value, dataType type)
{
    itemType* item = malloc(sizeof(itemType));
    if (item == NULL)
        return -1;

    item->dataPtr = value;
    item->type = type;
    item->next = NULL;

    if (list->first == NULL)
    {
        list->first = item;
        list->last = item;
    } else {
        list->last->next = item;
        list->last = item;
    }

    list->size++;
    return 0;
}

itemType* listGetItem(listType* list)
{
    if ( list == NULL )
        return NULL;

    /* FIFO Extraction */
    itemType* item = list->first;
    if (item != NULL)
    {
        list->first = item->next;
        list->size--;
    }

    return item;
}


void listEmpty(listType* list)
{
    if (list == NULL)
        return;

    itemType* item = NULL;
    itemType* next = NULL;

    item = list->first;

    while (item != NULL)
    {
        next = item->next;

        /* If the item is a list, recursive clean */
        if (item->type == list_t)
            listEmpty(item->dataPtr);

        free(item->dataPtr);
        free(item);
        item = next;
    }

    list->first = NULL;
    list->last = NULL;
    list->size = 0;
}

void listDestroy(listType* list)
{
    if (list == NULL)
        return;

    listEmpty(list);
    free(list);
}

char* parseNestedLists(char* ptr, listType* list)
{
    int8_t value;
    listType* new;
    int8_t* inter;

    while(*ptr != ']')
    {
        if (*ptr == '[')
        {
            /* Create a new sublist, and parse its contents. */
            new = listCreate();
            listAddItem(list, (void *) new, list_t);

            ptr = parseNestedLists(ptr + 1, new);

            /* Advance to the next char... */
            ptr++;

        } else if (sscanf(ptr, "%hhd%s", &value, ptr) == 2) {
            /* Try to parse a number, and store the remaining string. */
            inter = malloc(sizeof(int8_t));
            *inter = value;
            listAddItem(list, (void *) inter, int_t);
        
        } else {
            /* Probably a comma, advance to next char */
            ptr++;
        }
    }

    return ptr;
}


int32_t compareLists(listType* list1, listType* list2)
{
    uint32_t i, minsize;
    listType aux = { 0 };
    itemType auxItem = { 0 };
    int32_t ret;

    if (list1->size <= list2->size)
        minsize = list1->size;
    else
        minsize = list2->size;

    itemType* left = list1->first;
    itemType* right = list2->first;

    for (i = 0; i < minsize; i++)
    {
        if (i != 0)
        {
            left = left->next;
            right = right->next;
        }

        /* Case 1: Both Integers */
        if (left->type == int_t && right->type == int_t)
        {
            uint8_t i1 = *(uint8_t*)left->dataPtr;
            uint8_t i2 = *(uint8_t*)right->dataPtr;

            if (i1 == i2)
                continue;
            else if (i1 < i2)
                return 1;
            else
                return -1;
        }

        /* Case 2: Both lists */
        else if (left->type == list_t && right->type == list_t)
        {
            ret = compareLists(left->dataPtr, right->dataPtr);
            if (ret != 0)
                return ret;
        }

        /* Case 3: Only Left is integer, add it to aux list and compare lists */
        else if (left->type == int_t)
        {
            auxItem = *left;
            auxItem.next = NULL;
            aux.first = &auxItem;
            aux.last = &auxItem;
            aux.size = 1;

            ret = compareLists(&aux, right->dataPtr);
            if (ret != 0)
                return ret;

        /* Case 3.5: Only Right is integer, add it to aux list and compare lists */
        } else {
            auxItem = *right;
            auxItem.next = NULL;
            aux.first = &auxItem;
            aux.last = &auxItem;
            aux.size = 1;

            ret = compareLists(left->dataPtr, &aux);
            if (ret != 0)
                return ret;
        }
    }

    /* Check who run out of items... */
    if (list1->size == list2->size)
        return 0;
    else if (list1->size > list2->size)
        return -1;
    else
        return 1;
}

int32_t compare(const void* ptr1, const void* ptr2)
{
    listType* l1 = *(listType**) ptr1;
    listType* l2 = *(listType**) ptr2;

    /* Turn around, so it returns 1 if b > a */
    return compareLists(l2, l1);
}


int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint32_t index = 1;
    uint32_t part1 = 0;
    uint32_t part2 = 1;

    uint32_t totalLists = 0;
    listType* lists[MAX_INPUT_LISTS] = { 0 };

    /* Read lines from stdin until EOF */
    while ((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > -1)
    {
        /* Skip empty lines */
        if (size == 0)
            continue;

        /* Sanity check, and parse the first list... */
        if (*line == '[')
        {
            lists[totalLists] = listCreate();
            parseNestedLists(line + 1, lists[totalLists]);

            totalLists++;
        }
    }

    printf("Parsed %u lists \n", totalLists);

    /* ---------------------------------------------------------------------- */
    /* PART 1 */
    uint32_t i;
    for (i = 0; i < totalLists; i+=2)
    {
        if (compareLists(lists[i], lists[i + 1]) == 1)
            part1 += index;
        
        index++;
    }

    printf("Part 1: The sum of indeces is %u\n", part1);

    /* ---------------------------------------------------------------------- */
    /* PART 2 */

    /* Add the two dividers to the list, and start a sorting process. */
    char divider[6] = "[[2]]";
    char divider2[6] = "[[6]]";
    listType* dPtr1, *dPtr2;

    dPtr1 = listCreate();
    lists[totalLists] = dPtr1;
    parseNestedLists(divider + 1, lists[totalLists++]);

    dPtr2 = listCreate();
    lists[totalLists] = dPtr2;
    parseNestedLists(divider2 + 1, lists[totalLists++]);

    qsort(lists, totalLists, sizeof(listType*), compare);

    /* Search the indices */
    for (i = 0; i < totalLists; i++)
        if ((lists[i] == dPtr1) || (lists[i] == dPtr2))
            part2 *= (i + 1);

    printf("Part 2: The product of the divider indeces is %u\n", part2);

    /* Clean up. */
    for (i = 0; i < totalLists; i++)
        listDestroy(lists[i]);

    return 0;
}
