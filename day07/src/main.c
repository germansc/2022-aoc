#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGHT    (64)
#define MAX_FILENAME_LENGHT      (32)
#define MAX_ITEMS_IN_DIR         (10)
#define MAX_CREATED_DIRS         (255)

typedef struct {
    char name[MAX_FILENAME_LENGHT];
    uint32_t size;
} fileType;

/* Directories could be represented with a linked list with one parent and
 * multiple child nodes.
 * */
typedef struct dirType_s {
    char name[MAX_FILENAME_LENGHT];
    fileType files[MAX_ITEMS_IN_DIR];
    struct dirType_s* dirs[MAX_ITEMS_IN_DIR];
    struct dirType_s* parent;
    uint64_t dirsize;
} dirType;


/* I'll keep a global array with all created directories for easier
parsing and cleanup. */
struct disk_s {
    dirType* dirs[MAX_CREATED_DIRS];
    uint8_t indx;
} disk;


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

dirType* createDir(const char* name, dirType* parent)
{
    if (disk.indx == MAX_CREATED_DIRS)
    {
        printf("> Cannot create more dirs.\n");
        return NULL;
    }

    dirType* ptr = malloc(sizeof(dirType));
    if (ptr == NULL)
        return NULL;

    /* Initialize all bytes of the struct to 0. */
    memset(ptr, 0, sizeof(dirType));
    strncpy(ptr->name, name, MAX_FILENAME_LENGHT-1);

    ptr->parent = parent;
    disk.dirs[disk.indx++] = ptr;

    return ptr;
}

dirType* processCommand(const char *command, dirType *curr)
{
    dirType* next = NULL;
    char dirname[MAX_FILENAME_LENGHT];

    /* Assuming only commands will be cd and ls. */
    if (command[2] != 'c')
        return curr;

    if (sscanf(command, "$ cd %31s", dirname) != 1)
        return curr;

    /* CD command:
     * If curr is not null, either move to the parent dir, or create a new dir if not already. */
    uint8_t i;

    if (curr == NULL)
        next = createDir(dirname, curr);
    else if (strncmp(dirname, "..", MAX_FILENAME_LENGHT) == 0)
        next = curr->parent;
    else
        for (i = 0; i < MAX_ITEMS_IN_DIR; i++)
        {
            if (curr->dirs[i] == NULL)
            {
                /* End of list. */
                next = createDir(dirname, curr);
                break;
            }

            if (strncmp(dirname, curr->dirs[i]->name, MAX_FILENAME_LENGHT) == 0)
            {
                next = curr->dirs[i];
                break;
            }
        }

    return next;
}


void processFile(char* item, dirType* curr)
{
    uint32_t filesize;
    char filename[MAX_FILENAME_LENGHT];

    /* Try to parse. */
    if (sscanf(item, "%u %31s", &filesize, filename) != 2)
        return;

    /* Find an empty item in the file list. */
    uint8_t i;
    for (i = 0; i < MAX_ITEMS_IN_DIR; i++)
        if (curr->files[i].name[0] == '\0')
            break;

    /* If the list was full, skip. */
    if (i == MAX_ITEMS_IN_DIR)
    {
        printf(">> ERROR: Directory full!\n");
        return;
    }

    /* Else, add the file to the dir's list. */
    strncpy(curr->files[i].name, filename, MAX_FILENAME_LENGHT);
    curr->files[i].size = filesize;
    curr->dirsize += filesize;

    /* Propagate the file size to all previous dirs. */
    while (curr->parent != NULL)
    {
        curr->parent->dirsize += filesize;
        curr = curr->parent;
    }

    return;
}

int32_t main()
{
    char line[MAX_INPUT_LINE_LENGHT];
    int32_t size;
    uint64_t part1 = 0;

    dirType* currentDir = NULL;

    /* Initialize the disk. */
    memset(&disk, 0, sizeof(struct disk_s));

    /* Read lines from stdin until EOF */
    while((size = getLine(line, MAX_INPUT_LINE_LENGHT)) > 0)
    {
        /* Process commands */
        if (line[0] == '$')
            currentDir = processCommand(line, currentDir);
        else if (line[0] == 'd')
            /* "dir xxx" listing, can be skipped. */
            continue;
        else
            /* xxx file" listing */
            processFile(line, currentDir);
    }

    /* Part 1 */
    uint8_t i;
    for (i = 0; i < disk.indx; i++)
        if (disk.dirs[i]->dirsize <= 100000)
            part1 += disk.dirs[i]->dirsize;


    /* Part 2 */
    uint64_t freespace = 70000000U - disk.dirs[0]->dirsize;
    uint8_t min = 0;

    for (i = 0; i < disk.indx; i++)
        if (freespace + disk.dirs[i]->dirsize >= 30000000)
            if (disk.dirs[i]->dirsize <= disk.dirs[min]->dirsize)
                min = i;

    /* Print the results for both parts of the challenge. */
    printf("Part 1: %lu\n", part1);
    printf("Part 2: %lu\n", disk.dirs[min]->dirsize);

    printf("\nDeleting directory %s would free up %lu bytes, resulting in: %lu\n",
        disk.dirs[min]->name,
        disk.dirs[min]->dirsize,
        freespace + disk.dirs[min]->dirsize);


    /* Cleanup */
    for (i = 0; i < disk.indx; i++)
        free(disk.dirs[i]);

    return 0;
}
