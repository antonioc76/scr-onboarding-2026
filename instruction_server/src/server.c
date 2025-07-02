#include <stdio.h>
#include <lcm/lcm.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <team_request_t.h>
#include <team_response_t.h>

struct node {
    int key;
    struct node * next;
};

struct userdata {
    lcm_t * lcm;
    struct node * head;
};

struct node * createRing(char * buffer, int bufferSize) {
    char * token = strtok(buffer, ", ");
    
    struct node * head = malloc(sizeof(struct node));
    printf("%d\n", atoi(token));
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

struct node * getNodeByKey(struct node * head, int key) {
    struct node * t = head;
    
    int firstKey = t->key;
    do {
        t = t->next;
        if (t->key == key) {
            return t;
        }
    } while (t->key != firstKey);

    fprintf(stderr, "key not in list, returning NULL\n");
    return NULL;
}

char * readFromFile(char * filename, char * buffer, int bufferSize) {
    FILE * f = fopen(filename, "r");

    if (f == NULL) {
        fprintf(stderr, "file not found\n");
        return "error";
    }

    char * token;

    fgets(buffer, bufferSize, f);

    fclose(f);

    return buffer;
}

static void teamRequestHandler(const lcm_recv_buf_t *rbuf, const char * channel, const team_request_t * msg, void * user) {
    printf("Getting delivery instructions for team: %d\n", msg->requestingTeam);

    struct userdata * data = (struct userdata *)user;
    struct node * head = data->head;
    lcm_t * lcm = data->lcm;

    struct node * cur = getNodeByKey(head, msg->requestingTeam);

    if (cur == NULL) {
        fprintf(stderr, "requesting team not found\n");
        return;
    }
    
    int deliveryTarget = cur->next->key;

    printf("team %d should deliver to team %d\n", msg->requestingTeam, deliveryTarget);

    team_response_t res = {
        .requestingTeam = msg->requestingTeam,
        .deliverTo = deliveryTarget
    };

    printf("sending delivery instructions\n");
    team_response_t_publish(lcm, "responses", &res);

    return;
}

int main (int argc, char * argv) {
    char buffer[100];

    if (readFromFile("teammap.txt", buffer, sizeof(buffer)/sizeof(char)) == "error") {
        return 1;
    }

    struct node * head = createRing(buffer, sizeof(buffer)/sizeof(char));

    lcm_t * lcm = lcm_create(NULL);

    if (!lcm) {
        fprintf(stderr, "lcm not initialized properly\n");
        return 1;
    }

    // subscriptions
    struct userdata * data = malloc(sizeof(struct userdata));
    data->lcm = lcm;
    data->head = head;

    team_request_t_subscribe(lcm, "requests", teamRequestHandler, (void *)data);

    printf("Listening for requests on channel requests\n");

    while(1) {
        lcm_handle(lcm);
    }

    lcm_destroy(lcm);
    return 0;
}