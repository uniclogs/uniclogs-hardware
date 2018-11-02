#ifndef _PWRBOARD_H
#define _PWRBOARD_H


#include <stdint.h>


#define ROT_PWR  0
#define SDR_ROCK 1
#define V_PA     2
#define SDR_LIME 3
#define L_PA     4
#define U_PA     5
#define U_PTT    6
#define S_PWR    7
#define L_PTT    8
#define V_PTT    9
#define V_POL    10
#define U_POL    11
#define U_KEY    12
#define V_KEY    13
#define V_LNA    14
#define U_LNA    15




typedef enum{
  NO_ACTION,
    
  V_TX,
  U_TX,
  L_TX,
  PWR_ON,
  OPERATE,
  S_ON,
  S_OFF,
  KILL,

  V_LEFT,
  V_RIGHT,
  V_TX_ON,
  V_TX_OFF,
  SHUTDOWN,

  U_LEFT,
  U_RIGHT,
  U_TX_ON,
  U_TX_OFF,

  L_TX_ON,
  L_TX_OFF,
  
  EXIT,
  STATUS,
  MAX_TOKENS
}input_tokens;

typedef enum{ 
    PWR_UP,     // low power
    SYS_PWR_ON,
    BAND_SWITCH,
    S_SYS_ON,
    S_SYS_OFF,

    V_TRAN,
    U_TRAN,
    L_TRAN,

    SYS_KILL
}pwr_state;

typedef enum{  
    NONE,  
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
}secondary_states;


struct pwr_Config {
  pwr_state state;
  secondary_states sec_state;
  pwr_state next_state;
  secondary_states next_sec_state;
  input_tokens token;
  int errorCode;
} pwrConfig;


int pwrBoard();
void handle_kill_signal(int sig);
void handle_token_signal(int sig);
void handle_alarm_signal(int sig);
int initialize();
int i2c_exit();
int getInput();
void upper_string(char s[]);
int processToken();
int processVHFTokens();
int processUHFTokens();
int processLBandTokens();
int BandSwitchErrorRecovery();
int VHFErrorRecovery();
int UHFErrorRecovery();
int LErrorRecovery();
int CoolDown_Wait(); 
int tokenError();
void stateError();
void stateWarning();
int changeState();
int MPC23017BitSet(int bit);
int MPC23017BitClear(int bit);
int MPC23017BitReset();
int MPC23017BitRead(int bit);

#endif

