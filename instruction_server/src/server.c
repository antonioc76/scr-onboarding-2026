#include <stdio.h>
#include <lcm/lcm.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


struct node {
    int key;
    struct node * next;
};

char * readFromFile(char * filename, char * buffer, int bufferSize) {
    FILE * f = fopen(filename, "r");

    if (f == NULL) {
        fprintf(stderr, "file not found\n");
        return "error";
    }

    char * token;

    fgets(buffer, bufferSize, f);

    fclose(f);

    printf("%s\n", buffer);

    return buffer;
}

struct node * createRing(char * buffer, int bufferSize) {
    char * token = strtok(buffer, ", ");
    
    struct node * head = malloc(sizeof(struct node));
    head->key = atoi(token);
    
    struct node * t = head;
    while((token = strtok(NULL, ", ")) != NULL) {
        printf("%d\n", atoi(token));

        struct node * new = malloc(sizeof(struct node));
        new->key = atoi(token);
        t->next = new;

        t = t->next;
    }

    t->next = head;

    return head;
}

int main (int argc, char * argv) {
    struct node * head = malloc(sizeof(struct node));

    char buffer[100];
    char * token;

    if (readFromFile("teammap.txt", buffer, sizeof(buffer)/sizeof(char)) == "error") {
        return 1;
    }

    head = createRing(buffer, sizeof(buffer)/sizeof(char));

    lcm_t * lcm = lcm_create(NULL);

    if (!lcm) {
        fprintf(stderr, "lcm not initialized properly\n");
        return 1;
    }

    return 0;
}