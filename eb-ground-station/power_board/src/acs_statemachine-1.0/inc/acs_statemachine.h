#ifndef _ACS_STATEMACHINE_H_
#define _ACS_STATEMACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ENTRY_STRING 	"*Entering state: " 
#define EXIT_STRING  	"*Exiting state: " 
#define FUNC_STRING 	"*Calling function: " 

#define CAN_BUF_SIZE	8

typedef uint8_t CAN_STATUS[CAN_BUF_SIZE];
typedef uint8_t CAN_MESSAGE[CAN_BUF_SIZE];

extern char *state_name[];
extern char *event_name[];

typedef enum{
	ERROR_CODE=0,		//BYTE0
	STATE,					//BYTE1
	LAST_STATE,			//BYTE2
	STATUS3,				//BYTE3
	STATUS4,				//BYTE4
	STATUS5,				//BYTE5
	STATUS6,				//BYTE6
	STATUS7					//BYTE7
}status_buf;

typedef enum{
	CMD=0,					//BYTE0
	MSG1,						//BYTE1
	MSG2,						//BYTE2
	MSG3,						//BYTE3
	MSG4,						//BYTE4
	MSG5,						//BYTE5
	MSG6,						//BYTE6
	MSG7						//BYTE7
}message_buf;

typedef enum{
//	EXIT_SUCCESS,
//	EXIT_FAILURE,
	ERROR_THING
}return_code;

typedef enum{
	ST_RDY,		// low power
	ST_RW,
	ST_MTQR,
	ST_MAX_PWR
}acs_state;

#define MAX_STATES (int)(sizeof(acs_state))

typedef enum{
	FN_RW_SETDC,
	FN_MTQR_SETDC//,
}acs_function;

typedef struct ACS ACS;

struct ACS{
	acs_state cur_state;
	acs_function function;
	int (*fn_exit)(ACS *acs);
};

typedef struct{
	acs_state cur_state;
	acs_state req_state;
	int (*fn_entry)(ACS *acs);
	int (*fn_exit)(ACS *acs);
}acs_transition_rule;

typedef struct{
	acs_state state;
	acs_function function;
	int (*fn)(ACS *acs);
}acs_function_rule;

extern acs_transition_rule trans[];
extern acs_function_rule func[];

extern int acs_statemachine(ACS *acs);

#endif
