#include <unistd.h>                       //Needed for I2C port
#include <stdint.h>
#include <fcntl.h>                        //Needed for I2C port
#include <sys/ioctl.h>                    //Needed for I2C port
#include <linux/i2c-dev.h>                //Needed for I2C port
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "pwrBoard.h"

//I2C bus
char *filename = (char*)"/dev/i2c-1";
int file_i2c;
  
int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;
    
    printf("started powerboard code \n");
    initialize();
    while(1){
      getInput();
      processToken();
      printf("Before Change %d, %d, next, %d, %d \n",pwrConfig.state, pwrConfig.sec_state,pwrConfig.next_state, pwrConfig.next_sec_state);
      changeState();
      printf("After Change %d, %d, next, %d, %d \n",pwrConfig.state, pwrConfig.sec_state,pwrConfig.next_state, pwrConfig.next_sec_state);
    }
      
    return 0;
}

int initialize(){
  
  uint8_t buffer[3] = {0};
  uint8_t length;

  pwrConfig.next_state = PWR_UP;
  pwrConfig.next_sec_state = NONE;

  if ((file_i2c = open(filename, O_RDWR)) < 0)
  {
    //ERROR HANDLING: you can check errno to see what went wrong
    printf("Failed to open the i2c bus");
    return 1;
  }
  else
  {
    printf("Successfully opened I2C bus \n");
  }  

  //acquire i2c bus  
  int addr = 0x20;          //<<<<<The I2C address of the slave
  if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
  {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    //ERROR HANDLING; you can check errno to see what went wrong
    return 1;
  }
  else
  {
    printf("Successfully acquired bus. \n");
  }  
  
   //make GPIOA as output
  buffer[2] = 0x40;   //written based on datasheet.
  buffer[1] = 0x00;   //change GPIOA direction
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus.\n");
  }  
  
  //make GPIOB as output
  buffer[2] = 0x40;   //written based on datasheet.
  buffer[1] = 0x01;   //change GPIOB direction
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus.\n");
  }  
}

int getInput(){
  char input[10]="\0";
  int i;
  
  printf("\nEnter input token: ");
  scanf("%s", input);
  
  for(i=0;i<NUM_TOKENS;i++){  
    if(!strcmp(input,inputTokens[i])){
      pwrConfig.token = i;
      printf("Token entered %s \n",inputTokens[i]);
      break;
    }
  }
  
}


/*
TODO: make a new thread for this. 
Create user signals, timers in thsi thread as well. 
This would ensure that we can process any tokens even if we are waiting on a state change.
*/
int processToken(){     
  
  switch(pwrConfig.state){
  case SYS_KILL:
    usleep(1);   
  case PWR_UP:
    if(pwrConfig.token == PWR_ON)
      pwrConfig.next_state = SYS_PWR_ON;
    else
      killOrError();
    break;    
  case SYS_PWR_ON:
    if(pwrConfig.token == OPERATE)
      pwrConfig.next_state = BAND_SWITCH;
    else
      killOrError();
    break;    
  case BAND_SWITCH:
    if(pwrConfig.token == S_ON)
      pwrConfig.next_state = S_SYS_ON;
    else if(pwrConfig.token == S_OFF)
      pwrConfig.next_state = S_SYS_OFF;
    else if(pwrConfig.token == V_TX){
      pwrConfig.next_state = V_TX;
      pwrConfig.next_sec_state = VHF_TRANSMIT;
    }
    else if(pwrConfig.token == U_TX){
      pwrConfig.next_state = U_TX;
      pwrConfig.next_sec_state = UHF_TRANSMIT;
    }
    else if(pwrConfig.token == L_TX){
      pwrConfig.next_state = L_TX;
      pwrConfig.next_sec_state = L_TRANSMIT;
    }
    else
      killOrError();
    break;         
  case S_SYS_ON:
    BandSwitchErrorRecovery();
    break;       
    
  case S_SYS_OFF:
    BandSwitchErrorRecovery();
    break;        

  case V_TRAN:
    processVHFTokens();
    break;
    
  case U_TRAN:
    processUHFTokens();
    break;     
    
  case L_TRAN:
    processLBandTokens();
    break;
    
  default:
    killOrError();
    break;    
  }    
}


int processVHFTokens(){
  switch(pwrConfig.next_sec_state){
    case VHF_TRANSMIT:
      usleep(1);
    case V_SWITCH:
      if(pwrConfig.token == V_LEFT) 
        pwrConfig.next_sec_state =  V_LHCP;
      else if(pwrConfig.token == V_RIGHT)
        pwrConfig.next_sec_state =  V_RHCP;      
      else if(pwrConfig.token == V_TX_ON)
        pwrConfig.next_sec_state =  V_TRANS_ON;         
      else if(pwrConfig.token == V_TX_OFF)
        pwrConfig.next_sec_state =  V_TRANS_OFF; 
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.next_sec_state =  V_UHF_RHCP; 
      else if(pwrConfig.token == U_LEFT)
        pwrConfig.next_sec_state =  V_UHF_LHCP; 
      else if(pwrConfig.token == SHUTDOWN)
        pwrConfig.next_sec_state =  V_SHUTDOWN;
      else
        killOrError();
      break;
    case V_SHUTDOWN:
      VHFErrorRecovery();   //TODO: where to have the kill
      break;      
    case V_PA_COOL:    
      VHFErrorRecovery();   //TODO: where to have the kill 
      break;  
    case V_PA_DOWN:        
      VHFErrorRecovery();   //TODO: where to have the kill 
      break;
    default:
      killOrError();
      break;    
  }               
}


int processUHFTokens(){
  switch(pwrConfig.next_sec_state){
    case VHF_TRANSMIT:
      usleep(1);
    case V_SWITCH:
      if(pwrConfig.token == U_LEFT) 
        pwrConfig.next_sec_state =  U_LHCP;
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.next_sec_state =  U_RHCP;      
      else if(pwrConfig.token == U_TX_ON)
        pwrConfig.next_sec_state =  U_TRANS_ON;         
      else if(pwrConfig.token == U_TX_OFF)
        pwrConfig.next_sec_state =  U_TRANS_OFF; 
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.next_sec_state =  U_VHF_RHCP; 
      else if(pwrConfig.token == U_LEFT)
        pwrConfig.next_sec_state =  U_VHF_LHCP; 
      else if(pwrConfig.token == SHUTDOWN)
        pwrConfig.next_sec_state =  U_SHUTDOWN;
      else
        killOrError();
      break;
    case U_SHUTDOWN:
      UHFErrorRecovery();   //TODO: where to have the kill
      break;      
    case U_PA_COOL:    
      UHFErrorRecovery();   //TODO: where to have the kill 
      break;  
    case U_PA_DOWN:        
      UHFErrorRecovery();   //TODO: where to have the kill 
      break;
    default:
      killOrError();
      break;    
  }               
}


int processLBandTokens(){
  switch(pwrConfig.next_sec_state){
    case L_TRANSMIT:
      usleep(1);
    case L_SWITCH:
      if(pwrConfig.token == U_LEFT) 
        pwrConfig.next_sec_state =  L_UHF_LHCP;
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.next_sec_state =  L_UHF_RHCP;      
      else if(pwrConfig.token == L_TX_ON)
        pwrConfig.next_sec_state =  L_TRANS_ON;         
      else if(pwrConfig.token == L_TX_OFF)
        pwrConfig.next_sec_state =  L_TRANS_OFF; 
      else if(pwrConfig.token == V_RIGHT)
        pwrConfig.next_sec_state =  L_VHF_RHCP; 
      else if(pwrConfig.token == V_LEFT)
        pwrConfig.next_sec_state =  U_VHF_LHCP; 
      else if(pwrConfig.token == SHUTDOWN)
        pwrConfig.next_sec_state =  U_SHUTDOWN;
      else
        killOrError();
      break;
    case L_SHUTDOWN:
      LErrorRecovery();   //TODO: where to have the kill
      break;      
    case L_PA_COOL:    
      LErrorRecovery();   //TODO: where to have the kill 
      break;  
    case L_PA_DOWN:        
      LErrorRecovery();   //TODO: where to have the kill 
      break;
    default:
      killOrError();
      break;    
  }               
}



int BandSwitchErrorRecovery(){
  printf("The system should not have been in this state. Corrective action taken \n");
  printf("Please reenter your token and manually validate the action. \n");
  pwrConfig.next_state = BAND_SWITCH;
}


int killOrError(){
  if(pwrConfig.token == KILL)
    pwrConfig.next_state = SYS_PWR_ON;
  else
    printf("Incorrect token entered. Please validate. No action taken by code. \n ");
}

int VHFErrorRecovery(){
  printf("The system should not have been in this state. Corrective action taken \n");
  printf("Please reenter your token and manually validate the action. \n");
  pwrConfig.next_state = V_SWITCH;
}

int UHFErrorRecovery(){
  printf("The system should not have been in this state. Corrective action taken \n");
  printf("Please reenter your token and manually validate the action. \n");
  pwrConfig.next_state = U_SWITCH;
}

int LErrorRecovery(){
  printf("The system should not have been in this state. Corrective action taken \n");
  printf("Please reenter your token and manually validate the action. \n");
  pwrConfig.next_state = L_SWITCH;
}

void stateError(){
  printf("ERROR: There is a program error. Contact coder. \n");
  printf("Results unpredictable. Please Kill and start over. \n");
}

void stateWarning(){
  printf("WARNING: The system should not have been in this state. Contact coder. \n");
  printf("Recovery action taken. Verify output manually \n");
}

int changeState(){

  uint8_t temporary;

  switch(pwrConfig.next_state){
    case SYS_KILL:
    case PWR_UP:
      MPC23017BitReset();
      pwrConfig.state = PWR_UP;
      break;
    case SYS_PWR_ON:
      MPC23017BitSet(SDR_ROCK);
      MPC23017BitSet(SDR_LIME);
      MPC23017BitSet(ROT_PWR);
      pwrConfig.state = SYS_PWR_ON;
      break;
    case BAND_SWITCH:
      pwrConfig.state = BAND_SWITCH;
      break;
    case S_SYS_ON:
      MPC23017BitSet(S_PWR);
      pwrConfig.state = BAND_SWITCH;
      break;
    case S_SYS_OFF:
      MPC23017BitClear(S_PWR);
      pwrConfig.state = BAND_SWITCH;
      break;
      
    case V_TRAN:
      pwrConfig.state = V_TRAN;
      switch(pwrConfig.next_sec_state){
        case VHF_TRANSMIT:
          MPC23017BitSet(U_LNA);
          MPC23017BitSet(V_PA);
          MPC23017BitSet(V_KEY);
          pwrConfig.sec_state = VHF_TRANSMIT;
          break;
        case V_SWITCH:
          break;
        case V_SHUTDOWN:
          MPC23017BitClear(U_LNA);
          MPC23017BitClear(U_POL);
          MPC23017BitClear(V_POL);
          MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = V_SHUTDOWN;

          pwrConfig.sec_state = V_PA_COOL;          
          usleep(120);    // TODO: create a timeout and a user signal.

          pwrConfig.sec_state = V_PA_DOWN;             
          MPC23017BitClear(V_PA);
          MPC23017BitClear(V_KEY); 
          pwrConfig.state = BAND_SWITCH;
          pwrConfig.sec_state = NONE;  
      
          break;        
        case V_PA_COOL:
        case V_PA_DOWN:
        case V_UHF_LHCP:
          MPC23017BitSet(U_POL);
          pwrConfig.sec_state = V_SWITCH;
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U_POL);
          pwrConfig.sec_state = V_SWITCH;
          break;
        case V_TRANS_ON:
          MPC23017BitSet(V_PTT);
          pwrConfig.sec_state = V_SWITCH;
          break;        
        case V_TRANS_OFF:
          MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = V_SWITCH;
          break;        
        case V_LHCP:
          temporary = MPC23017BitRead(V_PTT);
          MPC23017BitClear(V_PTT);
          usleep(100);
          MPC23017BitSet(V_POL);
          usleep(100);
          if(temporary)
            MPC23017BitSet(V_PTT);
          else
            MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = V_SWITCH;
          break;
          
        case V_RHCP:
          temporary = MPC23017BitRead(V_PTT);
          MPC23017BitClear(V_PTT);
          usleep(100);
          MPC23017BitClear(V_POL);
          usleep(100);
          if(temporary)
            MPC23017BitSet(V_PTT);
          else
            MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = V_SWITCH;
          break;        
        default:
          stateError();
          break;        
      }
      
    case U_TRAN:
      pwrConfig.state = U_TRAN;
      switch(pwrConfig.next_sec_state){
        case UHF_TRANSMIT:
          MPC23017BitSet(U_LNA);
          MPC23017BitSet(V_PA);
          MPC23017BitSet(V_KEY);
          pwrConfig.sec_state = UHF_TRANSMIT;
          break;
        case U_SWITCH:
          break;
        case U_SHUTDOWN:
          MPC23017BitClear(U_LNA);
          MPC23017BitClear(U_POL);
          MPC23017BitClear(V_POL);
          MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = U_SHUTDOWN;
          
          pwrConfig.sec_state = U_PA_COOL;
          usleep(120);    // TODO: create a timeout and a user signal.
          
          pwrConfig.sec_state = U_PA_DOWN;
          MPC23017BitClear(V_PA);
          MPC23017BitClear(V_KEY);
          pwrConfig.state = BAND_SWITCH; 
          pwrConfig.sec_state = NONE;         
          break;        
        case V_PA_COOL:
        case V_PA_DOWN:
        case V_UHF_LHCP:
          MPC23017BitSet(U_POL);
          pwrConfig.sec_state = U_SWITCH;
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U_POL);
          pwrConfig.sec_state = U_SWITCH;
          break;
        case V_TRANS_ON:
          MPC23017BitSet(V_PTT);
          pwrConfig.sec_state = U_SWITCH;
          break;        
        case V_TRANS_OFF:
          MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = U_SWITCH;
          break;        
        case V_LHCP:
          temporary = MPC23017BitRead(V_PTT);
          MPC23017BitClear(V_PTT);
          usleep(100);
          MPC23017BitSet(V_POL);
          usleep(100);
          if(temporary)
            MPC23017BitSet(V_PTT);
          else
            MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = U_SWITCH;
          break;
          
        case V_RHCP:
          temporary = MPC23017BitRead(V_PTT);
          MPC23017BitClear(V_PTT);
          usleep(100);
          MPC23017BitClear(V_POL);
          usleep(100);
          if(temporary)
            MPC23017BitSet(V_PTT);
          else
            MPC23017BitClear(V_PTT);
          pwrConfig.sec_state = U_SWITCH;
          break;        
        default:
          stateError();
          break;                
      }
      
    case L_TRAN:
      pwrConfig.state = L_TRAN;
      switch(pwrConfig.next_sec_state){
        case L_TRANSMIT:
          MPC23017BitSet(U_LNA);
          MPC23017BitSet(V_LNA);
          MPC23017BitSet(L_PA);
          pwrConfig.sec_state = L_TRANSMIT;
          break;
        case L_SWITCH:
          break;
        case L_SHUTDOWN:
          MPC23017BitClear(L_PTT);
          MPC23017BitClear(U_POL);
          MPC23017BitClear(V_POL);
          MPC23017BitClear(V_LNA);
          MPC23017BitClear(U_LNA);
          pwrConfig.sec_state = L_SHUTDOWN;
          
          pwrConfig.sec_state = L_PA_COOL;
          usleep(120);          // TODO: create a timeout and a user signal.
          
          pwrConfig.sec_state = L_PA_DOWN;
          MPC23017BitClear(L_PA); 
          pwrConfig.state = BAND_SWITCH;
          pwrConfig.sec_state = NONE;        
          break;        
        case L_PA_COOL:
        case L_PA_DOWN:
        case L_UHF_LHCP:
          MPC23017BitSet(U_POL);
          pwrConfig.sec_state = L_SWITCH;
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U_POL);
          pwrConfig.sec_state = L_SWITCH;
          break;
        case L_TRANS_ON:
          MPC23017BitSet(L_PTT);
          pwrConfig.sec_state = L_SWITCH;
          break;        
        case L_TRANS_OFF:
          MPC23017BitClear(L_PTT);
          pwrConfig.sec_state = L_SWITCH;
          break;        
        case L_VHF_LHCP:
          MPC23017BitSet(V_POL);
          pwrConfig.sec_state = L_SWITCH;
          break;          
        case L_VHF_RHCP:
          MPC23017BitClear(V_POL);
          pwrConfig.sec_state = L_SWITCH;
          break;        
        default:
          stateError();
          break;                
      }
      
    default:
      stateError();
      break;
  }
}


int MPC23017BitSet(int bit){
  
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  uint8_t reg_address = 0;
  uint8_t reg_value = 0;
  uint8_t length;
  
  if (bit<8){
    reg_address = 0x12;
    shift_value = bit;
  }
  else{
    reg_address = 0x13;
    shift_value = bit - 8;
  }
  
  
  //read the register first  
  buffer[0] = 0x41; //Indicate reading address. 
  buffer[1] = reg_address;
  length = 2;
  write(file_i2c, buffer, length);

  length = 1;                   //Number of bytes to read
  if (read(file_i2c, buffer, length) != length){
    printf("Failed to read from the i2c bus.\n");
    printf("Data read: %x\n", buffer[0]);
  }
  else
  {
    printf("Data read: %x\n", buffer[0]);
    reg_value = buffer[0];
  }
  
  //new register value
  reg_value = reg_value | (1 << shift_value);
  printf("Register Value 0x%x \n", reg_value);  
  //write the new value
  buffer[2] = 0x40;   //write command.
  buffer[1] = reg_address;   //address of register
  buffer[0] = reg_value;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length){
    printf("Failed to write to the i2c bus.\n");
  }  
}


int MPC23017BitClear(int bit){
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  uint8_t reg_address = 0;
  uint8_t reg_value = 0;
  uint8_t mask = 0xFF;
  uint8_t length;

  if (bit<8){
    reg_address = 0x12;
    shift_value = bit;
  }
  else{
    reg_address = 0x13;
    shift_value = bit - 8;
  }
  
  
  //read the register first  
  buffer[0] = 0x41; //Indicate reading address. 
  buffer[1] = reg_address;
  length = 2;
  write(file_i2c, buffer, length);

  length = 1;                   //Number of bytes to read
  if (read(file_i2c, buffer, length) != length){
    printf("Failed to read from the i2c bus.\n");
    printf("Data read: %x\n", buffer[0]);
  }
  else
  {
    printf("Data read: %x\n", buffer[0]);
    reg_value = buffer[0];
  }
  
  //new register value
  mask = 0xFF ^ (1 << shift_value);
  reg_value = reg_value & mask;
  
  //write the new value
  buffer[2] = 0x40;   //write command.
  buffer[1] = reg_address;   //address of register
  buffer[0] = reg_value;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
  if (write(file_i2c, buffer, length) != length){
    printf("Failed to write to the i2c bus.\n");
  }   
}


int MPC23017BitReset(){
  uint8_t buffer[3] = {0};
  uint8_t length;
  //reset GPIOA
  buffer[2] = 0x40;   //write command.
  buffer[1] = 0x12;   //address of register
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length){
    printf("Failed to write to the i2c bus.\n");
  } 
  
  //reset GPIOB
  buffer[2] = 0x40;   //write command.
  buffer[1] = 0x12;   //address of register
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length){
    printf("Failed to write to the i2c bus.\n");
  } 
}

int MPC23017BitRead(int bit){
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  uint8_t reg_address = 0;
  uint8_t reg_value = 0;
  uint8_t mask = 0xFF;
  uint8_t length;

  if (bit<8){
    reg_address = 0x12;
    shift_value = bit;
  }
  else{
    reg_address = 0x13;
    shift_value = bit - 8;
  }
  
  
  //read the register first  
  buffer[0] = 0x41; //Indicate reading address. 
  buffer[1] = reg_address;
  length = 2;
  write(file_i2c, buffer, length);

  length = 1;                   //Number of bytes to read
  if (read(file_i2c, buffer, length) != length){
    printf("Failed to read from the i2c bus.\n");
    printf("Data read: %x\n", buffer[0]);
  }
  else{
    printf("Data read: %x\n", buffer[0]);
    return buffer[0];
  };
  return (int)(-1);
}

