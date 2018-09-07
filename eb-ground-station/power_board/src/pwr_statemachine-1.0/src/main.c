#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pwr_statemachine.h"

#define PID_FILE "/run/stationd/stationd.pid"

static int daemon_flag = 0;

int main(int argc, char *argv[]){
    FILE *run_fp = NULL;
    pid_t process_id = 0;
    pid_t sid = 0;

    int c;

    //Command line argument processing
    while ((c = getopt(argc, argv, "dp:")) != -1){
        switch (c){
            case 'd':
                daemon_flag = 1;
                break;
            case 'p':
                //TODO: Set port override
                break;

            case '?':

            default:
                fprintf(stderr, "Usage: %s [-d] [-p portnum]\n", argv[0]);
                exit(1);
        }
    }

    //Run as daemon if needed
    if (daemon_flag){
        //Fork
        process_id = fork();
        if (process_id < 0){
            fprintf(stderr, "Error: Failed to fork! Terminating...\n");
            exit(EXIT_FAILURE);
        }

        //Parent process, log pid of child and exit
        if (process_id){
            run_fp = fopen(PID_FILE, "w+");
            if (!run_fp){
                fprintf(stderr, "Error: Unable to open file %s\nTerminating...\n", PID_FILE);
                exit(EXIT_FAILURE);
            }
            fprintf(run_fp, "%d\n", process_id);
            fflush(run_fp);
            fclose(run_fp);
            exit(EXIT_SUCCESS);
        }

        //Child process, create new session for process group leader
        sid = setsid();
        if (sid < 0){
            exit(EXIT_FAILURE);
        }

        //Close std streams
        fclose(stdin);
        fclose(stdout);
        fclose(stderr);
    }

    PWR pwr = {0,0,0};
    pwr_statemachine(&pwr);

    return EXIT_SUCCESS;
}
