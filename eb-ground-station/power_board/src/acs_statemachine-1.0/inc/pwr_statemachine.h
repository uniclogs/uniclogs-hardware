#ifndef _PWR_STATEMACHINE_H_
#define _PWR_STATEMACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ENTRY_STRING    "*Entering state: " 
#define EXIT_STRING     "*Exiting state: " 
#define FUNC_STRING     "*Calling function: " 

#define CAN_BUF_SIZE    8

#define OUT_SIZE 16

uint16_t gpio_out;

//TODO MAX
//Strip out PWR systems, as well as CAN references.

typedef uint8_t CAN_STATUS[CAN_BUF_SIZE];
typedef uint8_t CAN_MESSAGE[CAN_BUF_SIZE];

extern char *state_name[];
extern char *event_name[];

// TODO MAX
// Confirm these

#define V_KEY    0x0001
#define V_PTT    0x0002
#define V_PA     0x0004
#define V_LNA    0x0008
#define V_POL    0x0010
#define U_KEY    0x0020
#define U_PTT    0x0040
#define U_PA     0x0080
#define U_LNA    0x0100
#define U_POL    0x0200
#define L_PTT    0x0400
#define L_PA     0x0800
#define S_PWR    0x1000
#define SDR_ROCK 0x2000
#define SDR_LIME 0x4000
#define ROT_PWR  0x8000

typedef enum{
  V_TX=0,
  U_TX,
  L_TX,
  T_PWR_ON,
  OPERATE,
  S_ON,
  S_OFF,
  T_KILL,

  V_LEFT,
  V_RIGHT,
  U_LEFT,
  U_RIGHT,
  V_TX_ON,
  V_TX_OFF,
  SHUTDOWN,

  U_TX_ON,
  U_TX_OFF,

  L_TX_ON,
  L_TX_OFF
}input_tokens;

typedef enum{
    ERROR_CODE=0,       //BYTE0
    STATE,                  //BYTE1
    LAST_STATE,         //BYTE2
    STATUS3,                //BYTE3
    STATUS4,                //BYTE4
    STATUS5,                //BYTE5
    STATUS6,                //BYTE6
    STATUS7                 //BYTE7
}status_buf;

typedef enum{
    CMD=0,                  //BYTE0
    MSG1,                       //BYTE1
    MSG2,                       //BYTE2
    MSG3,                       //BYTE3
    MSG4,                       //BYTE4
    MSG5,                       //BYTE5
    MSG6,                       //BYTE6
    MSG7                        //BYTE7
}message_buf;

typedef enum{
//  EXIT_SUCCESS,
//  EXIT_FAILURE,
    ERROR_THING
}return_code;

typedef enum{
    KILL,
    PWR_UP,     // low power
    PWR_ON,
    BAND_SWITCH,
    S_SYS_ON,
    S_SYS_OFF,
    VHF_TRANSMIT,
    V_SWITCH,
    V_SHUTDOWN,
    V_PA_COOL,
    V_PA_DOWN,
    V_UHF_LHCP,
    V_UHF_RHCP,
    V_TRANS_ON,
    V_TRANS_OFF,
    V_LHCP,
    V_RHCP,
    UHF_TRANSMIT,
    U_SWITCH,
    U_SHUTDOWN,
    U_PA_COOL,
    U_PA_DOWN,
    U_VHF_LHCP,
    U_VHF_RHCP,
    U_TRANS_ON,
    U_TRANS_OFF,
    U_LHCP,
    U_RHCP,
    L_TRANSMIT,
    L_SWITCH,
    L_SHUTDOWN,
    L_PA_COOL,
    L_PA_DOWN,
    L_VHF_LHCP,
    L_VHF_RHCP,
    L_TRANS_ON,
    L_TRANS_OFF,
    L_UHF_LHCP,
    L_UHF_RHCP,
    TODO
}pwr_state;

#define MAX_STATES (int)(sizeof(pwr_state))

typedef enum{
    FN_RW_SETDC,
    FN_MTQR_SETDC//,
}pwr_function;

typedef struct PWR PWR;

struct PWR{
    pwr_state cur_state;
    pwr_function function;
    int (*fn_exit)(PWR *pwr);
};

typedef struct{
    pwr_state cur_state;
    pwr_state req_state;
    int (*fn_exit)(PWR *pwr);
    int (*fn_entry)(PWR *pwr);
    input_tokens trans_token;
}pwr_transition_rule;

typedef struct{
    pwr_state state;
    pwr_function function;
    int (*fn)(PWR *pwr);
}pwr_function_rule;

extern pwr_transition_rule trans[];
extern pwr_function_rule func[];

extern int pwr_statemachine(PWR *pwr);

#endif
