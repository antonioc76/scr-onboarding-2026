#include <stdio.h>
#include <lcm/lcm.h>
#include <team_request_t.h>
#include <team_response_t.h>

void teamResponseHandler(const lcm_recv_buf_t *rbuf, const char * channel, const team_response_t * msg, void * user) {
    printf("Heard delivery instructions, deliver to team: %d\n", msg->deliverTo);
}

int main (int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: client {requestingTeamNumber}");
    }

    lcm_t * lcm = lcm_create("udpm://239.255.76.67:7667?ttl=1");

    team_request_t my_data = {
        .requestingTeam = atoi(argv[1])
    };

    team_response_t_subscribe(lcm, "responses", teamResponseHandler, NULL);

    team_request_t_publish(lcm, "requests", &my_data);

    while(1) {
        lcm_handle(lcm);
    }

    lcm_destroy(lcm);
    
    return 0;
}