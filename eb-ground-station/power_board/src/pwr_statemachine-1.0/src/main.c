#include "pwr_statemachine.h"

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    PWR pwr = {0,0,0};

    pwr_statemachine(&pwr);

    return 0;
}
