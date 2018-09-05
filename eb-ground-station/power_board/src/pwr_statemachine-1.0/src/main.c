#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "pwr_statemachine.h"

static int daemon_flag = 0;
static int verbose_flag = 0;

static struct option long_options[] = {
    {"daemon", no_argument, NULL, 'd'},
    {"verbose", no_argument, NULL, 'v'},
    {0, 0, 0, 0}
    };

int main(int argc, char **argv){
    int c;
    int option_index=0;

    while ((c = getopt_long(argc, argv, "dv", long_options, &option_index)) != -1){
        switch (c){
            case 'd':
                daemon_flag = 1;
                break;
            case 'v':
                verbose_flag = 1;
                break;

            case 0:
                break;
            default:
                abort();
        }
    }

    PWR pwr = {0,0,0};

    /*pwr_statemachine(&pwr);*/

    return 0;
}
