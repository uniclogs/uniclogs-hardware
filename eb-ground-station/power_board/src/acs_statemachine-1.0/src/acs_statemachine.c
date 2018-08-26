#include "acs_statemachine.h"

char *state_name[] = {
// GLobal
    "KILL,",
// Main state switcher
    "PWR_UP",
    "PWR_ON",
    "BAND_SWITCH",
    "UHF_TRANSMIT",
    "L_TRANSMIT",
    "S_SYS_ON",
    "S_SYS_OFF",

// VHF Control
    "VHF_TRANSMIT",
    "V_SWITCH",
    "V_SHUTDOWN",
    "V_PA_COOL",
    "V_PA_DOWN",
    "V_UHF_LHCP",i
    "V_UHF_RHCP",
    "VHF_TRANS_ON",
    "VHF_TRANS_OFF",
    "VHF_LHCP",
    "VHF_LHCP_1",
    "VHF_LHCP_2",
    "VHF_RHCP",
    "VHF_RHCP_1",
    "VHF_RHCP_2",

// UHF Control
    "UHF_TRANSMIT",
    "U_SWITCH",
    "U_SHUTDOWN",
    "U_PA_COOL",
    "U_PA_DOWN",
    "U_UHF_LHCP",
    "U_UHF_RHCP",
    "UHF_TRANS_ON",
    "UHF_TRANS_OFF",
    "UHF_LHCP",
    "UHF_LHCP_1",
    "UHF_LHCP_2",
    "UHF_RHCP",
    "UHF_RHCP_1",
    "UHF_RHCP_2",

//L Band Control
    "L_TRANSMIT",
    "L_SWITCH",
    "L_SHUTDOWN",
    "L_PA_COOL",
    "L_PA_DOWN",
    "L_VHF_LHCP",
    "L_VHF_RHCP",
    "L_TRANS_ON",
    "L_TRANS_OFF",
    "L_UHF_LHCP",
    "L_UHF_RHCP"
};

char *function_name[] = {
    "FN_RW_SETDC",
    "FN_MTQR_SETDC"
};

static void print_name(char *name){
    printf("%s",name);  
}

static void print_state(acs_state state){
    print_name(state_name[state]);
}

static void print_function(acs_function function){
    print_name(function_name[function]);
}

static int printTransition(acs_state state,char *s){
    printf("%s ",s);    
    print_state(state);
    printf("\n");
    return state;
}

static int printFunctionCall(acs_function function,char *s){
    printf("%s ",s);    
    print_function(function);
    printf("\n");
    return function;
}

// TODO MAX
// Add functions, the whole bunch of em
// Maybe have a generic "no output" entry transition?

// START Main State Switcher
// TODO CHange print transition for all of these
static int kill(ACS *acs){
    (void)acs;
    return printTransition(ST_MTQR,ENTRY_STRING);
}

// Generic, no output transition entrance function
static int entry_generic(ACS *acs)
{
  (void)acs;
  return 1;
}

static int pwr_up(ACS *acs){
    (void)acs;
    return printTransition(ST_RDY,ENTRY_STRING);
}

static int pwr_on(ACS *acs){
    (void)acs;
    return printTransition(ST_RDY,ENTRY_STRING);
}

static int band_switch(ACS *acs){
    (void)acs;
    return printTransition(ST_RW,EXIT_STRING);
}


static int S_Sys_On(ACS *acs){
    (void)acs;
    return printTransition(ST_MTQR,EXIT_STRING);
}

static int S_Sys_Off(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,ENTRY_STRING);
}

// START VHF Band States

static int vhf_band_transmit(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_token_switcher(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_shutdown(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_pa_cooldwon(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_pa_down(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_uhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_uhf_rhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_vhf_trans_on(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_vhf_trans_off(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_lhcp1(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_lhcp2(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_rhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_rhcp1(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int vhf_rhcp2(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

// START UHF
//=============================
static int uhf_band_transmit(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_token_switcher(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_shutdown(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_pa_cooldwon(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_pa_down(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_vhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_vhf_rhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_uhf_trans_on(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_uhf_trans_off(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_lhcp1(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_lhcp2(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_rhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_rhcp1(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int uhf_rhcp2(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}
//=============================

//START  L Band Control
//=============================
static int L_band_transmit(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int L_token_switcher(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_shutdown(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_pa_cooldown(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_pa_down(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_vhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_vhf_rhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_trans_on(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}


static int L_trans_off(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int L_uhf_lhcp(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}

static int L_uhf_rhcp2(ACS *acs){
    (void)acs;
    return printTransition(ST_MAX_PWR,EXIT_STRING);
}



//=============================

// functions
static int fn_rw_setdc(ACS *acs){
    (void)acs;
    printFunctionCall(FN_RW_SETDC,FUNC_STRING);
    return EXIT_SUCCESS;
}

static int fn_mtqr_setdc(ACS *acs){
    (void)acs;
    printFunctionCall(FN_MTQR_SETDC,FUNC_STRING);
    return EXIT_SUCCESS;
}

acs_function_rule func[] = {
    //{ST_RW,             FN_RW_SETDC,        &fn_rw_setdc},
    //{ST_MTQR,       FN_MTQR_SETDC,  &fn_mtqr_setdc},
    //{ST_MAX_PWR,    FN_RW_SETDC,        &fn_rw_setdc},
    //{ST_MAX_PWR,    FN_MTQR_SETDC,  &fn_mtqr_setdc}
};

#define FUNC_COUNT (int)(sizeof(func)/sizeof(acs_function_rule))

static int callFunction(ACS *acs){
    int i;
    acs_function function;

    for(i = 0;i < FUNC_COUNT;++i){
        if(acs->cur_state == func[i].state){
            if((acs->function == func[i].function)){
                function = (func[i].fn)(acs);
                if(function){
                    printf("function call error!\n");
                }
                break;
            }
        }
    }

    return acs->cur_state;
}


// TODO Max
// This is the big boy to take care of
acs_transition_rule trans[] = {
    // START MAIN
    // ================================================
    
    {PWR_UP, PWR_ON, &entry_generic, &pwr_on},
    {PWR_ON, BAND_SWITCH, &entry_generic, &band_switch},
    {BAND_SWITCH, S_SYS_ON, &entry_generic, &s_sys_on},
    {BAND_SWITCH, S_SYS_OFF, &entry_generic, &s_sys_off},
    {BAND_SWITCH, VHF_TRANSMIT, &entry_generic, &vhf_band_transmit},
    {BAND_SWITCH, UHF_TRANSMIT, &entry_generic, &uhf_band_transmit},
    {BAND_SWITCH, L_TRANSMIT, &entry_generic, &l_band_transmit},
    {BAND_SWITCH, KILL, &entry_generic, &kill},
    {PWR_ON, KILL, &entry_generic, &kill},

    // START VHF
    // ================================================
    {VHF_TRANSMIT, V_SWITCH, &entry_generic, &vhf_band_transmit},
    {V_SWITCH, V_UHF_LHCP, &entry_generic, &vhf_uhf_lhcp},
    {V_SWITCH, V_UHF_RHCP, &entry_generic, &vhf_uhf_rhcp},
    {V_SWITCH, V_LHCP, &entry_generic, &vhf_lhcp},
    {V_SWITCH, V_RHCP, &entry_generic, &vhf_rhcp},
    {V_SWITCH, V_TRANS_ON, &entry_generic, &vhf_trans_on},
    {V_SWITCH, V_TRANS_OFF, &entry_generic, &vhf_trans_off},
    {V_SWITCH, V_SHUTDOWN, &entry_generic, &vhf_shutdown},
    {V_SWITCH, V_PA_COOL, &entry_generic, &vhf_pa_cooldown},
    {V_SWITCH, V_PA_DOWN, &entry_generic, &vhf_pa_down},
    {V_SWITCH, KILL, &entry_generic, &kill},
    {V_PA_COOL, KILL, &entry_generic, &kill},
    {V_PA_DOWN, BAND_SWITCH, &entry_generic, &band_switch},

    // START UHF
    // ================================================
    {UHF_TRANSMIT, U_SWITCH, &entry_generic, &uhf_band_transmit},
    {U_SWITCH, U_UHF_LHCP, &entry_generic, &uhf_lhcp},
    {U_SWITCH, U_UHF_RHCP, &entry_generic, &uhf_rhcp},
    {U_SWITCH, U_LHCP, &entry_generic, &uhf_vhf_lhcp},
    {U_SWITCH, U_RHCP, &entry_generic, &uhf_vhf_rhcp},
    {U_SWITCH, U_TRANS_ON, &entry_generic, &uhf_trans_on},
    {U_SWITCH, U_TRANS_OFF, &entry_generic, &uhf_trans_off},
    {U_SWITCH, U_SHUTDOWN, &entry_generic, &uhf_shutdown},
    {U_SWITCH, U_PA_COOL, &entry_generic, &uhf_pa_cooldown},
    {U_SWITCH, U_PA_DOWN, &entry_generic, &uhf_pa_down},
    {U_SWITCH, KILL, &entry_generic, &kill},
    {U_PA_COOL, KILL, &entry_generic, &kill},
    {U_PA_DOWN, EXIT, &entry_generic, &exit},
    {U_PA_DOWN, BAND_SWITCH, &entry_generic, &band_switch},

    // START L BAND
    // ================================================
    {L_TRANSMIT, L_SWITCH, &entry_generic, &l_band_transmit},
    {L_SWITCH, L_UHF_LHCP, &entry_generic, &l_uhf_lhcp},
    {L_SWITCH, L_UHF_RHCP, &entry_generic, &l_uhf_rhcp},
    {L_SWITCH, L_VHF_LHCP, &entry_generic, &l_vhf_lhcp},
    {L_SWITCH, L_VHF_RHCP, &entry_generic, &l_vhf_rhcp},
    {L_SWITCH, L_TRANS_ON, &entry_generic, &l_trans_on},
    {L_SWITCH, L_TRANS_OFF, &entry_generic, &l_trans_off},
    {L_SWITCH, L_SHUTDOWN, &entry_generic, &l_shutdown},
    {L_SWITCH, L_PA_COOL, &entry_generic, &l_pa_cooldown},
    {L_SWITCH, L_PA_DOWN, &entry_generic, &l_pa_down},
    {L_SWITCH, KILL, &entry_generic, &kill},
    {L_PA_COOL, KILL, &entry_generic, &kill},
    {L_PA_DOWN, BAND_SWITCH, &entry_generic, &band_switch}


};

#define TRANS_COUNT (int)(sizeof(trans)/sizeof(acs_transition_rule))

void print_welcome(){
    printf("state enum: "); 
    printf("\t%d, \t%d, \t %d, \t %d\n",
            ST_RDY,ST_RW,ST_MTQR,ST_MAX_PWR);
    
    printf("state names: \t"); 
    print_state(ST_RDY); printf(", ");
    print_state(ST_RW); printf(", ");
    print_state(ST_MTQR); printf(", ");
    print_state(ST_MAX_PWR); printf("\n");
}

acs_function requestFunction(ACS *acs){
    acs_function function;
    char input[3]="";

    printf("\nrequest function call$ ");
    scanf(" %s", input);
    function = atoi(input);
    if(function < 0 || function >= FUNC_COUNT){
        printf("error, invalid function call: %d\n",function);
        return EXIT_FAILURE
    }
    printf("function call request %s received\n", function_name[function]);
    acs->function = function;
    callFunction(acs);
    return EXIT_SUCCESS;
}

acs_state requestTransition(ACS *acs){
    acs_state state;
    char input[3]="\0";
    int i;

    printf("\nrequest state transition$ ");
    scanf(" %s", input);
    state = atoi(input);
    if(state < 0 || state >= MAX_STATES){
        printf("error, invalid state tranition: %d\n",state);
        return EXIT_FAILURE; 
    }
    printf("transition request %s received\n", state_name[state]);
    printf("TRANS_COUNT: %d\n", TRANS_COUNT);
    for (i = 0;i < TRANS_COUNT;++i){
        printf("(%d,%d),", acs->cur_state,trans[i].cur_state);
        printf("(%d,%d)\n", state,trans[i].req_state);
        if((acs->cur_state==trans[i].cur_state)&&(state==trans[i].req_state)){
            acs->fn_exit(acs);
            acs->fn_exit=trans[i].fn_exit;
            acs->cur_state = (trans[i].fn_entry)(acs);
            break;
        }
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int handleEvents(ACS *acs){
    int event;
    char input[3]="\0";
    printf("current state: %d ",acs->cur_state);
    print_state(acs->cur_state);
    
    printf("\nevent$ ");
    scanf(" %s", input);
    event = atoi(input);
    if(event == 0){
        requestTransition(acs);
    }else if(event == 1){
        requestFunction(acs);
    }else{
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

extern int acs_statemachine(ACS *acs){
    print_welcome();

    printf("Starting state machine!\n\n");
    acs->cur_state = entry_rdy(acs);
    acs->fn_exit = exit_rdy;
    printf("\nIntitializing ACS to state: %s\n\n",state_name[acs->cur_state]);
    
    while(1){
        handleEvents(acs);
    }
    
    return EXIT_SUCCESS;
}

