This is a simple sequential power board control code. 

The valid tokens are below. The tokens are NOT case sensitive.
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
  NO_ACTION

refer to the state diagram for information on these tokens.

TODO in next version app:
1. create an additional thread which is ready to take an user input even while the code is in state transition. This is required for KILL command.
2. Add user signals. This will be required to have signal based timer to user entry triggering a signal.
