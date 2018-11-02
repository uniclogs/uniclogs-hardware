This is a simple sequential power board control code. 

The valid tokens are below. The tokens are NOT case sensitive.
  *NO_ACTION

  *V_TX,
  *U_TX,
  *L_TX,
  *PWR_ON,
  *OPERATE,
  *S_ON,
  *S_OFF,
  *KILL,

  *V_LEFT,
  *V_RIGHT,
  *V_TX_ON,
  *V_TX_OFF,
  *SHUTDOWN,

  *U_LEFT,
  *U_RIGHT,
  *U_TX_ON,
  *U_TX_OFF,

  *L_TX_ON,
  *L_TX_OFF,
  
  *EXIT,
  *STATUS,


Refer to the [state diagram](https://github.com/oresat/oresat-ground-station/blob/master/eb-ground-station/power_board/Stationd.pdf) for information on these tokens.

TODO in next version app:
1. Create a deamon version of this program.
