#include "acs_statemachine.h"

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    ACS acs = {0,0,0};

    acs_statemachine(&acs);

    return 0;
}
