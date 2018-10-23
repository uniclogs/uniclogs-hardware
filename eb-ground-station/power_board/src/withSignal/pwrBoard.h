#define V_KEY    0
#define V_PTT    1
#define V_PA     2
#define V_LNA    3
#define V_POL    4
#define U_KEY    5
#define U_PTT    6
#define U_PA     7

#define U_LNA    8
#define U_POL    9
#define L_PTT    10
#define L_PA     11
#define S_PWR    12
#define SDR_ROCK 13
#define SDR_LIME 14
#define ROT_PWR  15


#define NUM_TOKENS 22


char *state_name[] = {
// GLobal
    "KILL,",
// Main state switcher
    "PWR_UP",
    "PWR_ON",
    "BAND_SWITCH",
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


char *inputTokens[] = {
  "NO_ACTION",
  "V_TX",
  "U_TX",
  "L_TX",
  "PWR_ON",
  "OPERATE",
  "S_ON",
  "S_OFF",
  "KILL",

  "V_LEFT",
  "V_RIGHT",
  "V_TX_ON",
  "V_TX_OFF",
  "SHUTDOWN",

  "U_LEFT",
  "U_RIGHT",
  "U_TX_ON",
  "U_TX_OFF",

  "L_TX_ON",
  "L_TX_OFF",

  "EXIT",
  "STATUS",
  "MAX_TOKENS"
};

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
    SYS_KILL,
    
    PWR_UP,     // low power
    SYS_PWR_ON,
    BAND_SWITCH,
    S_SYS_ON,
    S_SYS_OFF,
    
    V_TRAN,
    U_TRAN,
    L_TRAN
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
int killOrError();
int VHFErrorRecovery();
void stateError();
void stateWarning();
int changeState();
int MPC23017BitSet(int bit);
int MPC23017BitClear(int bit);
int MPC23017BitReset();
int MPC23017BitRead(int bit);


