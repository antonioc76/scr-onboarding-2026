#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node {
    int key;
    struct node * next;
};

struct node * initList() {
    struct node * head = malloc(sizeof(struct node));
    struct node * z = malloc(sizeof(struct node));

    head->next = z;
    z->next = z;

    return head;
}

void insertAfter(struct node * head, int pos, int value) {
    struct node * new = malloc(sizeof(struct node));
    struct node * t = head;
    for (int i = 0; i < pos; i++) {
        t = t->next;
    }

    if (t == t->next) {
        fprintf(stderr, "index out of range\n");
        return;
    }

    struct node * temp = t->next;
    t->next = new;

    new->key = value;
    new->next = temp;
}

int deleteAfter(struct node * head, int pos) {
    struct node * t = head;

    struct node * to_delete = malloc(sizeof(struct node));

    for (int i = 0; i < pos; i++) {
        t = t->next;
    }

    if (t->next == t->next->next) {
        fprintf(stderr, "index out of bounds\n");
        return -1;
    }

    to_delete = t->next;
    t->next = t->next->next;
    
    int deletedKey = to_delete->key;
    free(to_delete);

    return deletedKey;
}

int length(struct node * head) {
    int len = 0;
    
    struct node * t = head;

    while (t != t->next) {
        t = t->next;
        len += 1;
    }

    return len;
}

int killRandom(struct node * head) {
    srand(time(NULL));
    int min = 0;
    int max = length(head) - 2;

    int randomNum = rand() % (max - min + 1) + min;

    int deleted = deleteAfter(head, randomNum);

    return deleted;
}

int main (int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "argv[1] should be the number of teams\n");
        return 1;
    }

    int teamsCount = atoi(argv[1]);

    FILE * f;
    f = fopen("teammap.txt", "w");

    struct node * head = initList();

    for (int i=0; i < teamsCount; i++) {
        insertAfter(head, i, i+1);
    }

    for (int i = 0; i < teamsCount; i++) {
        int killed = killRandom(head);
        if (i != teamsCount-1) {
            printf("%d, ", killed);
            fprintf(f, "%d, ", killed);
        }
        else {
            printf("%d\n", killed);
            fprintf(f, "%d", killed);
        }
    }

    fclose(f);
    
    printf("Generated team mappings for %d teams\n", teamsCount);

    return 0;
}