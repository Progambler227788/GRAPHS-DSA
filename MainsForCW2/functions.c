#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUSY '+'
#define FREE ' '
#define BUSYSIZE -1
#define FREESIZE 0

struct space {
    char *memory;
    int *sizes;
    int len;
};

void printMemory(struct space *mem) {
    int i = 0;
    while (i < mem->len) {
        printf("%c", *(mem->memory + i));
        i++;
    }
    printf("\n");
}

void printSizes(struct space *mem) {
    int i = 0;
    while (i < mem->len) {
        int n = *(mem->sizes + i);
        printf("%d", n);
        i++;
    }
    printf("\n");
}

void copyString(char *sIn, char *sOut, int len) {
    int t = 0;
    while (t < len) {
        *(sOut + t) = *(sIn + t);
        t++;
    }
}

int stringLen(char *s) {
    int t = 0;
    while (*(s + t) != '\0') {
        t++;
    }
    return t;
}

void initializeMemory(int memSize, struct space *mem) {
    mem->memory = (char *)malloc(memSize * sizeof(char));
    mem->sizes = (int *)malloc(memSize * sizeof(int));
    mem->len = memSize;

    for (int i = 0; i < mem->len; ++i) {
        *(mem->memory + i) = FREE;
        *(mem->sizes + i) = FREESIZE;
    }

    printf("Memory initialized:\n");
    printMemory(mem);
    printf("Sizes initialized:\n");
    printSizes(mem);
}

void cleanMemory(struct space *mem) {
    for (int i = 0; i < mem->len; ++i) {
        *(mem->memory + i) = FREE;
        *(mem->sizes + i) = FREESIZE;
    }

    printf("Memory cleaned:\n");
    printMemory(mem);
    printf("Sizes cleaned:\n");
    printSizes(mem);

    free(mem->memory);
    free(mem->sizes);
}

int stackAllocator(int nbytes, struct space *mem) {
    int i = 0;
    while (i < mem->len) {
        if (*(mem->sizes + i) == FREESIZE) {
            int j = i + 1;
            int freeCount = 1;

            // Check for nbytes available entries after the current FREESIZE entry
            while (j < mem->len && *(mem->sizes + j) == FREESIZE && freeCount < nbytes) {
                j++;
                freeCount++;
            }

            if (freeCount == nbytes) {
                *(mem->sizes + i) = nbytes;
                for (int k = 0; k < nbytes; k++) {
                    *(mem->sizes + i + k + 1) = BUSYSIZE;
                    *(mem->memory + i + k) = BUSY;
                }
                return i;
            }
        }
        i++;
    }

    return mem->len;
}

void deallocator(int p, struct space *mem) {
    if (p < 0 || p >= mem->len || *(mem->sizes + p) == FREESIZE) {
        // Invalid or already deallocated index
        return;
    }

    int nbytes = *(mem->sizes + p);
    for (int i = p; i < p + nbytes; ++i) {
        *(mem->sizes + i) = FREESIZE;
        *(mem->memory + i) = FREE;
    }
}

int spaceScanner(int nbytes, struct space *mem) {
    int i = 0;
    while (i < mem->len) {
        if (*(mem->sizes + i) == FREESIZE) {
            int j = i + 1;
            int freeCount = 1;

            // Check for nbytes available entries after the current FREESIZE entry
            while (j < mem->len && *(mem->sizes + j) == FREESIZE && freeCount < nbytes) {
                j++;
                freeCount++;
            }

            if (freeCount == nbytes) {
                return i;
            }
        }
        i++;
    }

    return mem->len;
}

int heapAllocatorQ3(int nbytes, struct space *mem) {
    int p = spaceScanner(nbytes, mem);
    if (p == mem->len) {
        return mem->len; // No suitable block found
    }

    for (int i = p; i < p + nbytes; ++i) {
        *(mem->sizes + i) = FREESIZE;
        *(mem->memory + i) = FREE;
    }

    return p;
}

void increaseMemory(int nbytes, struct space *mem) {
    int newLen = mem->len;
    while (newLen < nbytes) {
        newLen = 2 * (newLen + 1);
    }

    char *tempMemory = mem->memory;
    int *tempSizes = mem->sizes;
    int tempLen = mem->len;

    initializeMemory(newLen, mem);

    copyString(tempMemory, mem->memory, tempLen);
    copyArray(tempSizes, mem->sizes, tempLen);

    free(tempMemory);
    free(tempSizes);
}

int readString(char **s) {
    int len = 0;
    *s = (char *)malloc(1);
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        len++;
        *s = (char *)realloc(*s, len + 1);
        char *temp = *s;
        copyString(temp, s, len);
        free(temp);
        *(*s + len - 1) = ch;
    }
    *(*s + len) = '\0';

    if (ch == '\n') {
        return 1;
    } else {
        return 0;
    }
}

void copyArray(int *old, int *new, int len) {
    for (int i = 0; i < len; ++i) {
        *(new + i) = *(old + i);
    }
}

int heapAllocatorQ3(int nbytes, struct space *mem) {
    int t0;
    while ((t0 = spaceScanner(nbytes, mem)) == mem->len) {
        increaseMemory(nbytes, mem);
    }

    return t0;
}