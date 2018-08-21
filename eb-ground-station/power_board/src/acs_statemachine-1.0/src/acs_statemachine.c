#include "acs_statemachine.h"

char *state_name[] = {
	"ST_RDY",
	"ST_RW",
	"ST_MTQR",
	"ST_MAX_POWER"
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

// ST_RDY state transistion functions
static int entry_rdy(ACS *acs){
	(void)acs;
	return printTransition(ST_RDY,ENTRY_STRING);
}

static int exit_rdy(ACS *acs){
	(void)acs;
	return printTransition(ST_RDY,EXIT_STRING);
}

// ST_RW state transistion functions
static int entry_rw(ACS *acs){
	(void)acs;
	return printTransition(ST_RW,ENTRY_STRING);
}

static int exit_rw(ACS *acs){
	(void)acs;
	return printTransition(ST_RW,EXIT_STRING);
}

// ST_MTQR state transistion functions
static int entry_mtqr(ACS *acs){
	(void)acs;
	return printTransition(ST_MTQR,ENTRY_STRING);
}

static int exit_mtqr(ACS *acs){
	(void)acs;
	return printTransition(ST_MTQR,EXIT_STRING);
}

// ST_MAX_POWER state transistion functions
static int entry_max_pwr(ACS *acs){
	(void)acs;
	return printTransition(ST_MAX_PWR,ENTRY_STRING);
}

static int exit_max_pwr(ACS *acs){
	(void)acs;
	return printTransition(ST_MAX_PWR,EXIT_STRING);
}

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
	{ST_RW, 			FN_RW_SETDC,		&fn_rw_setdc},
	{ST_MTQR, 		FN_MTQR_SETDC,	&fn_mtqr_setdc},
	{ST_MAX_PWR, 	FN_RW_SETDC,		&fn_rw_setdc},
	{ST_MAX_PWR, 	FN_MTQR_SETDC,	&fn_mtqr_setdc}
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

acs_transition_rule trans[] = {
	{ST_RDY,			ST_RW,				&entry_rw,				&exit_rdy},
	{ST_RDY,			ST_MTQR,			&entry_mtqr,			&exit_rdy},
	{ST_RDY,			ST_MAX_PWR,		&entry_max_pwr,		&exit_rdy},
	{ST_RW,				ST_MAX_PWR,		&entry_max_pwr,		&exit_rw},
	{ST_MTQR,			ST_MAX_PWR,		&entry_max_pwr,		&exit_mtqr},
	{ST_RW,				ST_RDY,				&entry_rdy,				&exit_rw},
	{ST_MTQR,			ST_RDY,				&entry_rdy,				&exit_mtqr},
	{ST_MAX_PWR,	ST_RDY,				&entry_rdy,				&exit_max_pwr},
	{ST_MAX_PWR,	ST_RW,				&entry_rw,				&exit_max_pwr},
	{ST_MAX_PWR,	ST_MTQR,			&entry_mtqr,			&exit_max_pwr},
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
		return EXIT_FAILURE; 
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

