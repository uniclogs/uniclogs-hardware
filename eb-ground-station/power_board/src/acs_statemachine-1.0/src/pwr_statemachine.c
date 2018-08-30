#include "pwr_statemachine.h"

char *state_name[] = {
// GLobal
    "KILL,",
// Main state switcher
    "PWR_UP",
    "PWR_ON",
    "BAND_SWITCH",
    "L_TRANSMIT",
    "S_SYS_ON",
    "S_SYS_OFF",

// VHF Control
    "VHF_TRANSMIT",
    "V_SWITCH",
    "V_SHUTDOWN",
    "V_PA_COOL",
    "V_PA_DOWN",
    "V_UHF_LHCP",
    "V_UHF_RHCP",
    "VHF_TRANS_ON",
    "VHF_TRANS_OFF",
    "VHF_LHCP",
    "VHF_RHCP",

// UHF Control
    "UHF_TRANSMIT",
    "U_SWITCH",
    "U_SHUTDOWN",
    "U_PA_COOL",
    "U_PA_DOWN",
    "U_VHF_LHCP",
    "U_VHF_RHCP",
    "UHF_TRANS_ON",
    "UHF_TRANS_OFF",
    "UHF_LHCP",
    "UHF_RHCP",

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

static int get_input()
{ 
    input_tokens in;
    char input[10]="\0";

    printf("\nrequest input token$ ");
    scanf("%s", input);

    printf("INPUT %s\n", input);
    printf("TEST %d\n", strcmp("T_PWR_ON\0", input));

    if (!strcmp("V_TX",input))
          in = V_TX;
    else if (!strcmp("U_TX",input))
          in = U_TX;
    else if (!strcmp("L_TX",input))
          in = L_TX;
    else if (!strcmp("T_PWR_ON",input))
    {
          in = T_PWR_ON;
          printf("HERE\n");
    }
    else if (!strcmp("OPERATE",input))
          in = OPERATE;
    else if (!strcmp("S_ON",input))
          in = S_ON;
    else if (!strcmp("S_OFF",input))
          in = S_OFF;
    else if (!strcmp("T_KILL",input))
          in = T_KILL;
    else if (!strcmp("V_LEFT",input))
          in = V_LEFT;
    else if (!strcmp("V_RIGHT",input))
          in = V_RIGHT;
    else if (!strcmp("V_TX_ON",input))
          in = V_TX_ON;
    else if (!strcmp("V_TX_OFF",input))
          in = V_TX_OFF;
    else if (!strcmp("SHUTDOWN",input))
          in = SHUTDOWN;
    else if (!strcmp("U_TX_ON",input))
          in = U_TX_ON;
    else if (!strcmp("U_TX_OFF",input))
          in = U_TX_OFF;
    else if (!strcmp("L_TX_ON",input))
          in = L_TX_ON;
    else if (!strcmp("L_TX_OFF",input))
          in = L_TX_OFF;
    printf("IN: %d\n", in);
    return in;
}

static void print_name(char *name){
    printf("%s",name);  
}

static void print_state(pwr_state state){
    print_name(state_name[state]);
}

static void print_function(pwr_function function){
    print_name(function_name[function]);
}

static int printTransition(pwr_state state,char *s){
    printf("%s ",s);    
    print_state(state);
    printf("\n");
    return state;
}

static int printFunctionCall(pwr_function function,char *s){
    printf("%s ",s);    
    print_function(function);
    printf("\n");
    return function;
}

static int rmw_output(int new_out, int mask)
{
  printf("GPIO_OUT: %04x\n", gpio_out);
  printf("NEW: %04x\n", new_out);
  printf("MASK: %04x\n", mask);
  gpio_out = gpio_out & ~mask;
  printf("OUT1: %04x\n", gpio_out);
  gpio_out = gpio_out | new_out;
  printf("OUT2: %04x\n", gpio_out);
  return 1;
}

// TODO MAX
// Add functions, the whole bunch of em
// Maybe have a generic "no output" exit transition?

// START Main State Switcher
// TODO CHange print transition for all of these
static int kill(PWR *pwr){
    (void)pwr;
    int result = rmw_output(0x0000, 0xFFFF);
    return printTransition(PWR_UP,ENTRY_STRING);
}

// Generic, no output transition entrance function
static int exit_generic(PWR *pwr)
{
  (void)pwr;
  return 1;
}

static int pwr_up(PWR *pwr){
    (void)pwr;
    int result = rmw_output(0x0000, 0xFFFF);

    return printTransition(PWR_UP,ENTRY_STRING);
}

static int pwr_on(PWR *pwr){
    (void)pwr;
    int result = rmw_output(SDR_ROCK | SDR_LIME | ROT_PWR, SDR_ROCK | SDR_LIME | ROT_PWR);
    return printTransition(PWR_ON,ENTRY_STRING);
}

static int band_switch(PWR *pwr){
    (void)pwr;
    return printTransition(BAND_SWITCH,EXIT_STRING);
}


static int s_sys_on(PWR *pwr){
    (void)pwr;
    int result = rmw_output(S_PWR, S_PWR);
    return printTransition(BAND_SWITCH, EXIT_STRING);
}

static int s_sys_off(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!S_PWR, S_PWR);
    return printTransition(BAND_SWITCH,ENTRY_STRING);
}

// START VHF Band States

static int vhf_band_transmit(PWR *pwr){
    (void)pwr;
    int result = rmw_output(U_LNA || V_PA || V_KEY, U_LNA || V_PA || V_KEY);
    return printTransition(V_SWITCH,ENTRY_STRING);
}

static int vhf_token_switcher(PWR *pwr){
    (void)pwr;
    return printTransition(V_SWITCH,ENTRY_STRING);
}

static int vhf_shutdown(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!V_PTT && !V_POL && !U_POL && !U_LNA, V_PTT || V_POL || U_POL || U_LNA);
    return printTransition(V_PA_COOL,EXIT_STRING);
}

static int vhf_pa_cooldown(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}

static int vhf_pa_down(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}

static int vhf_uhf_lhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(U_POL, U_POL);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int vhf_uhf_rhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!U_POL, U_POL);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int vhf_trans_on(PWR *pwr){
    (void)pwr;
    int result = rmw_output(V_PTT, V_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int vhf_trans_off(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!V_PTT, V_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int vhf_lhcp(PWR *pwr){
    (void)pwr;
    int temp = gpio_out && V_PTT;
    int result = rmw_output(!V_PTT, V_PTT);
    //delay 100ms
    result = rmw_output(V_POL, V_POL);
    //delay 100ms
    result = rmw_output(temp, V_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int vhf_rhcp(PWR *pwr){
    (void)pwr;
    int temp = gpio_out && V_PTT;
    int result = rmw_output(!V_PTT, V_PTT);
    //delay 100ms
    result = rmw_output(V_POL, V_POL);
    //delay 100ms
    result = rmw_output(temp, V_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}


// START UHF
//=============================
static int uhf_band_transmit(PWR *pwr){
    (void)pwr;
    int result = rmw_output(V_LNA || U_PA || U_KEY, V_LNA || U_PA || U_KEY);
    return printTransition(U_SWITCH,EXIT_STRING);
}

static int uhf_token_switcher(PWR *pwr){
    (void)pwr;
    return printTransition(U_SWITCH,EXIT_STRING);
}

static int uhf_shutdown(PWR *pwr){
    (void)pwr;
    int result = rmw_output(U_PTT || V_POL || U_POL || V_LNA, U_PTT || V_POL || U_POL || V_LNA);
    return printTransition(U_PA_COOL,EXIT_STRING);
}

static int uhf_pa_cooldown(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}

static int uhf_pa_down(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}

static int uhf_vhf_lhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(V_POL, V_POL);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int uhf_vhf_rhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!V_POL, V_POL);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int uhf_trans_on(PWR *pwr){
    (void)pwr;
    int result = rmw_output(U_PTT, U_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int uhf_trans_off(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!U_PTT, U_PTT);
    return printTransition(V_SWITCH,EXIT_STRING);
}

static int uhf_lhcp(PWR *pwr){
    (void)pwr;
    int temp = gpio_out && U_PTT;
    int result = rmw_output(!U_PTT, U_PTT);
    //delay 100ms
    result = rmw_output(U_POL, U_POL);
    //delay 100ms
    result = rmw_output(temp, U_PTT);
    return printTransition(U_SWITCH, EXIT_STRING);
}

static int uhf_rhcp(PWR *pwr){
    (void)pwr;
    int temp = gpio_out && U_PTT;
    int result = rmw_output(!U_PTT, U_PTT);
    //delay 100ms
    result = rmw_output(!U_POL, !U_POL);
    //delay 100ms
    result = rmw_output(temp, U_PTT);
    return printTransition(U_SWITCH,EXIT_STRING);
}

//=============================

//START  L Band Control
//=============================
static int l_band_transmit(PWR *pwr){
    (void)pwr;
    int result = rmw_output(V_LNA || U_LNA || L_PA, V_LNA || U_LNA || L_PA);
    return printTransition(L_SWITCH,EXIT_STRING);
}

static int l_token_switcher(PWR *pwr){
    (void)pwr;
    return printTransition(L_SWITCH,EXIT_STRING);
}


static int l_shutdown(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!L_PTT && !V_POL && !U_POL && !V_LNA && !U_LNA, L_PTT || V_POL || U_POL || V_LNA || U_LNA);
    return printTransition(TODO,EXIT_STRING);
}


static int l_pa_cooldown(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}


static int l_pa_down(PWR *pwr){
    (void)pwr;
    return printTransition(TODO,EXIT_STRING);
}


static int l_vhf_lhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(V_POL, V_POL);
    return printTransition(L_SWITCH,EXIT_STRING);
}


static int l_vhf_rhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!V_POL, V_POL);
    return printTransition(L_SWITCH,EXIT_STRING);
}


static int l_trans_on(PWR *pwr){
    (void)pwr;
    int result = rmw_output(L_PTT, L_PTT);
    return printTransition(L_SWITCH,EXIT_STRING);
}


static int l_trans_off(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!L_PTT, L_PTT);
    return printTransition(L_SWITCH,EXIT_STRING);
}

static int l_uhf_lhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(U_POL, U_POL);
    return printTransition(L_SWITCH,EXIT_STRING);
}

static int l_uhf_rhcp(PWR *pwr){
    (void)pwr;
    int result = rmw_output(!U_POL, U_POL);
    return printTransition(L_SWITCH,EXIT_STRING);
}



//=============================

// functions
static int fn_rw_setdc(PWR *pwr){
    (void)pwr;
    printFunctionCall(FN_RW_SETDC,FUNC_STRING);
    return EXIT_SUCCESS;
}

static int fn_mtqr_setdc(PWR *pwr){
    (void)pwr;
    printFunctionCall(FN_MTQR_SETDC,FUNC_STRING);
    return EXIT_SUCCESS;
}

pwr_function_rule func[] = {
    //{ST_RW,             FN_RW_SETDC,        &fn_rw_setdc},
    //{ST_MTQR,       FN_MTQR_SETDC,  &fn_mtqr_setdc},
    //{TODO,    FN_RW_SETDC,        &fn_rw_setdc},
    //{TODO,    FN_MTQR_SETDC,  &fn_mtqr_setdc}
};

#define FUNC_COUNT (int)(sizeof(func)/sizeof(pwr_function_rule))

static int callFunction(PWR *pwr){
    int i;
    pwr_function function;

    for(i = 0;i < FUNC_COUNT;++i){
        if(pwr->cur_state == func[i].state){
            if((pwr->function == func[i].function)){
                function = (func[i].fn)(pwr);
                if(function){
                    printf("function call error!\n");
                }
                break;
            }
        }
    }

    return pwr->cur_state;
}


// TODO Max
// This is the big boy to take care of
pwr_transition_rule trans[] = {
    // START MAIN
    // ================================================
    
    {PWR_UP, PWR_ON, &exit_generic, &pwr_on, T_PWR_ON},
    {PWR_ON, BAND_SWITCH, &exit_generic, &band_switch, OPERATE},
    {BAND_SWITCH, S_SYS_ON, &exit_generic, &s_sys_on, S_ON},
    {BAND_SWITCH, S_SYS_OFF, &exit_generic, &s_sys_off, S_OFF},
    {BAND_SWITCH, VHF_TRANSMIT, &exit_generic, &vhf_band_transmit, V_TX},
    {BAND_SWITCH, UHF_TRANSMIT, &exit_generic, &uhf_band_transmit, U_TX},
    {BAND_SWITCH, L_TRANSMIT, &exit_generic, &l_band_transmit, L_TX},
    {BAND_SWITCH, KILL, &exit_generic, &kill, KILL},
    {PWR_ON, KILL, &exit_generic, &kill, KILL},

    // START VHF
    // ================================================
    //{VHF_TRANSMIT, V_SWITCH, &exit_generic, &vhf_band_transmit},
    {V_SWITCH, V_UHF_LHCP, &exit_generic, &vhf_uhf_lhcp, U_LEFT},
    {V_SWITCH, V_UHF_RHCP, &exit_generic, &vhf_uhf_rhcp, U_RIGHT},
    {V_SWITCH, V_LHCP, &exit_generic, &vhf_lhcp, V_LEFT},
    {V_SWITCH, V_RHCP, &exit_generic, &vhf_rhcp, V_RIGHT},
    {V_SWITCH, V_TRANS_ON, &exit_generic, &vhf_trans_on, V_TX_ON},
    {V_SWITCH, V_TRANS_OFF, &exit_generic, &vhf_trans_off, V_TX_OFF},
    {V_SWITCH, V_SHUTDOWN, &exit_generic, &vhf_shutdown, SHUTDOWN},
    //{V_SWITCH, V_PA_COOL, &exit_generic, &vhf_pa_cooldown},
    //{V_SWITCH, V_PA_DOWN, &exit_generic, &vhf_pa_down},
    {V_SWITCH, KILL, &exit_generic, &kill, KILL},
    {V_PA_COOL, KILL, &exit_generic, &kill, KILL},
    //{V_PA_DOWN, BAND_SWITCH, &exit_generic, &band_switch},

    // START UHF
    // ================================================
    //{UHF_TRANSMIT, U_SWITCH, &exit_generic, &uhf_band_transmit},
    {U_SWITCH, U_LHCP, &exit_generic, &uhf_lhcp, U_LEFT},
    {U_SWITCH, U_RHCP, &exit_generic, &uhf_rhcp, U_RIGHT},
    {U_SWITCH, U_VHF_LHCP, &exit_generic, &uhf_vhf_lhcp, V_LEFT},
    {U_SWITCH, U_VHF_RHCP, &exit_generic, &uhf_vhf_rhcp, V_RIGHT},
    {U_SWITCH, U_TRANS_ON, &exit_generic, &uhf_trans_on, U_TX_ON},
    {U_SWITCH, U_TRANS_OFF, &exit_generic, &uhf_trans_off, U_TX_OFF},
    {U_SWITCH, U_SHUTDOWN, &exit_generic, &uhf_shutdown, SHUTDOWN},
    //{U_SWITCH, U_PA_COOL, &exit_generic, &uhf_pa_cooldown},
    //{U_SWITCH, U_PA_DOWN, &exit_generic, &uhf_pa_down},
    {U_SWITCH, KILL, &exit_generic, &kill, KILL},
    {U_PA_COOL, KILL, &exit_generic, &kill, KILL},
    //{U_PA_DOWN, BAND_SWITCH, &exit_generic, &band_switch},

    // START L BAND
    // ================================================
    //{L_TRANSMIT, L_SWITCH, &exit_generic, &l_band_transmit},
    {L_SWITCH, L_UHF_LHCP, &exit_generic, &l_uhf_lhcp, U_LEFT},
    {L_SWITCH, L_UHF_RHCP, &exit_generic, &l_uhf_rhcp, U_RIGHT},
    {L_SWITCH, L_VHF_LHCP, &exit_generic, &l_vhf_lhcp, V_LEFT},
    {L_SWITCH, L_VHF_RHCP, &exit_generic, &l_vhf_rhcp, V_RIGHT},
    {L_SWITCH, L_TRANS_ON, &exit_generic, &l_trans_on, L_TX_ON},
    {L_SWITCH, L_TRANS_OFF, &exit_generic, &l_trans_off, L_TX_OFF},
    {L_SWITCH, L_SHUTDOWN, &exit_generic, &l_shutdown, SHUTDOWN},
    //{L_SWITCH, L_PA_COOL, &exit_generic, &l_pa_cooldown},
    //{L_SWITCH, L_PA_DOWN, &exit_generic, &l_pa_down},
    {L_SWITCH, KILL, &exit_generic, &kill, KILL},
    {L_PA_COOL, KILL, &exit_generic, &kill, KILL}
    //{L_PA_DOWN, BAND_SWITCH, &exit_generic, &band_switch}


};

#define TRANS_COUNT (int)(sizeof(trans)/sizeof(pwr_transition_rule))

void print_welcome(){
    //printf("state enum: "); 
    //printf("\t%d, \t%d, \t %d, \t %d\n",
    //        ST_RDY,ST_RW,ST_MTQR,TODO);
    
    //printf("state names: \t"); 
    //print_state(ST_RDY); printf(", ");
    //print_state(ST_RW); printf(", ");
    //print_state(ST_MTQR); printf(", ");
    //print_state(TODO); printf("\n");
}

pwr_function requestFunction(PWR *pwr){
    pwr_function function;
    char input[3]="";

    printf("\nrequest function call$ ");
    scanf(" %s", input);
    function = atoi(input);
    if(function < 0 || function >= FUNC_COUNT){
        printf("error, invalid function call: %d\n",function);
        return EXIT_FAILURE;
    }
    printf("function call request %s received\n", function_name[function]);
    pwr->function = function;
    callFunction(pwr);
    return EXIT_SUCCESS;
}


pwr_state requestTransition(PWR *pwr, input_tokens token){
    int i;
    for (i = 0;i < TRANS_COUNT;++i){
        printf("(%d,%d),", pwr->cur_state,trans[i].cur_state);
        printf("(%d,%d)\n", token, trans[i].trans_token);
        if((pwr->cur_state==trans[i].cur_state)&&(token==trans[i].trans_token)){
            pwr->fn_exit(pwr);
            pwr->fn_exit=trans[i].fn_exit;
            pwr->cur_state = (trans[i].fn_entry)(pwr);
            break;
        }
    }
    printf("\n");

    return EXIT_SUCCESS;
}

int handleEvents(PWR *pwr){
    int token;
    char input[3]="\0";
    printf("current state: %d ",pwr->cur_state);
    print_state(pwr->cur_state);
    token = get_input();
    return requestTransition(pwr, token);
}

extern int pwr_statemachine(PWR *pwr){
    print_welcome();

    printf("Starting state machine!\n\n");
    pwr->cur_state = pwr_up(pwr);
    pwr->fn_exit = exit_generic;
    printf("\nIntitializing PWR to state: %s\n\n",state_name[pwr->cur_state]);
    
    while(1){
        handleEvents(pwr);
    }
    
    return EXIT_SUCCESS;
}

