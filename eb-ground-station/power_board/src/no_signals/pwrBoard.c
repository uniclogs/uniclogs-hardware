#include <unistd.h>                       //Needed for I2C port
#include <stdint.h>
#include <fcntl.h>                        //Needed for I2C port
#include <sys/ioctl.h>                    //Needed for I2C port
#include <linux/i2c-dev.h>                //Needed for I2C port
#include <stdio.h>

#include "pwrboard.h"

//I2C bus
char *filename = (char*)"/dev/i2c-1";
int file_i2c;
  
int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;
    
    printf("started powerboard code \n");
    initialize();
    while(true){
      getInput();
      processToken();
      changeState();
    }
      
    return 0;
}

int initialize(){
  
  uint8_t buffer[3] = {0};
  
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
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
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
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
  }  
}

int getInput(){
  char input[10]="\0";
  int i;
  
  printf("\nEnter input token: ");
  scanf("%s", input);
  
  for(i=0;i<size(state_name[]);i++){
    if(input == state_name[i]){
      pwrConfig.token = i;
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
  case KILL:
    pause(1);
    
  case PWR_UP:
    if(pwrConfig.token == T_PWR_ON)
      pwrConfig.next_state = PWR_ON;
    else
      killOrError();
    break;
    
  case PWR_ON:
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
      pwrConfig.sec_state = VHF_TRANSMIT;
    }
    else if(pwrConfig.token == U_TX){
      pwrConfig.next_state = U_TX;
      pwrConfig.sec_state = UHF_TRANSMIT;
    }
    else if(pwrConfig.token == L_TX){
      pwrConfig.next_state = L_TX;
      pwrConfig.sec_state = L_TRANSMIT;
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

  case V_TX:
    processVHFTokens();
    break;
    
  case U_TX:
    processUHFTokens();
    break;     
    
  case L_TX:
    processLBandTokens();
    break;
    
  case default:
    killOrError();
    break;    
  }    
}


int processVHFTokens(){
  switch(pwrConfig.sec_state){
    case VHF_TRANSMIT:
      sleep(1);
    case V_SWITCH:
      if(pwrConfig.token == V_LEFT) 
        pwrConfig.sec_state =  VHF_LHCP;
      else if(pwrConfig.token == V_RIGHT)
        pwrConfig.sec_state =  V_RHCP;      
      else if(pwrConfig.token == V_TX_ON)
        pwrConfig.sec_state =  V_TRANS_ON;         
      else if(pwrConfig.token == V_TX_OFF)
        pwrConfig.sec_state =  V_TRANS_OFF; 
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.sec_state =  V_UHF_RHCP; 
      else if(pwrConfig.token == U_LEFT)
        pwrConfig.sec_state =  V_UHF_LHCP; 
      else if(pwrConfig.token == SHUTDOWN)
        pwrConfig.sec_state =  V_SHUTDOWN;
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
    case default:
      killOrError();
      break;    
  }               
}


int processUHFTokens(){
  switch(pwrConfig.sec_state){
    case VHF_TRANSMIT:
      sleep(1);
    case V_SWITCH:
      if(pwrConfig.token == U_LEFT) 
        pwrConfig.sec_state =  UHF_LHCP;
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.sec_state =  U_RHCP;      
      else if(pwrConfig.token == U_TX_ON)
        pwrConfig.sec_state =  U_TRANS_ON;         
      else if(pwrConfig.token == U_TX_OFF)
        pwrConfig.sec_state =  U_TRANS_OFF; 
      else if(pwrConfig.token == U_RIGHT)
        pwrConfig.sec_state =  U_VHF_RHCP; 
      else if(pwrConfig.token == U_LEFT)
        pwrConfig.sec_state =  U_VHF_LHCP; 
      else if(pwrConfig.token == SHUTDOWN)
        pwrConfig.sec_state =  U_SHUTDOWN;
      else
        killOrError();
      break;
    case U_SHUTDOWN:
      VHFErrorRecovery();   //TODO: where to have the kill
      break;      
    case U_PA_COOL:    
      VHFErrorRecovery();   //TODO: where to have the kill 
      break;  
    case U_PA_DOWN:        
      VHFErrorRecovery();   //TODO: where to have the kill 
      break;
    case default:
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
  if(pwrConfig.token == T_KILL)
    pwrConfig.next_state = PWR_ON;
  else
    printf("Incorrect token entered. Please validate. No action taken by code. \n ");
}

int VHFErrorRecovery(){
  printf("The system should not have been in this state. Corrective action taken \n");
  printf("Please reenter your token and manually validate the action. \n");
  pwrConfig.next_state = BAND_SWITCH;
}

stateError(){
  printf("ERROR: There is a program error. Contact coder. \n");
  printf("Results unpredictable. Please Kill and start over. \n");
}

stateWarning(){
  printf("WARNING: The system should not have been in this state. Contact coder. \n");
  printf("Recovery action taken. Verify output manually \n");
}

int changeState(){
  switch(pwrConfig.state){
    case KILL:
      MPC23017BitReset();
      break;
    case PWR_UP:
      MPC23017BitReset();
      break;
    case PWR_ON:
      MPC23017BitSet(SDR-ROCK);
      MPC23017BitSet(SDR-LIME);
      MPC23017BitSet(ROT-PWR);
      break;
    case BAND_SWITCH:
    case S_SYS_ON:
      MPC23017BitSet(S-PWR);
      break;
    case S_SYS_OFF:
      MPC23017BitClear(S-PWR);
      break;
      
    case V_TX:
      switch(pwrConfig.sec_state){
        case VHF_TRANSMIT:
          MPC23017BitSet(U-LNA);
          MPC23017BitSet(V-PA);
          MPC23017BitSet(V-KEY);
          break;
        case V_SWITCH:
        case V_SHUTDOWN:
          MPC23017BitClear(U-LNA);
          MPC23017BitClear(U-POL);
          MPC23017BitClear(V-POL);
          MPC23017BitClear(V-PTT);
          
          pwrConfig.sec_state = V_PA_COOL;
          pause(120);    // TODO: create a timeout and a user signal.
          
          pwrConfig.sec_state = V_PA_DOWN;
          MPC23017BitClear(V-PA);
          MPC23017BitClear(V-KEY);          
          break;        
        case V_PA_COOL:
        case V_PA_DOWN:
        case V_UHF_LHCP:
          MPC23017BitSet(U-POL);
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U-POL);
          break;
        case V_TRANS_ON:
          MPC23017BitSet(V-PTT);
          break;        
        case V_TRANS_OFF:
          MPC23017BitClear(V-PTT);
          break;        
        case V_LHCP:
          temporary = MPC23017BitRead(V-PTT);
          MPC23017BitClear(V-PTT);
          pause(100);
          MPC23017BitSet(V-POL);
          pause(100);
          if(temporary)
            MPC23017BitSet(V-PTT);
          else
            MPC23017BitClear(V-PTT);
          break;
          
        case V_RHCP:
          temporary = MPC23017BitRead(V-PTT);
          MPC23017BitClear(V-PTT);
          pause(100);
          MPC23017BitClear(V-POL);
          pause(100);
          if(temporary)
            MPC23017BitSet(V-PTT);
          else
            MPC23017BitClear(V-PTT);
          break;        
        case default:
          stateError();
          break;        
      }
      
    case U_TX:
      switch(pwrConfig.sec_state){
      switch(pwrConfig.sec_state){
        case VHF_TRANSMIT:
          MPC23017BitSet(U-LNA);
          MPC23017BitSet(V-PA);
          MPC23017BitSet(V-KEY);
          break;
        case V_SWITCH:
        case V_SHUTDOWN:
          MPC23017BitClear(U-LNA);
          MPC23017BitClear(U-POL);
          MPC23017BitClear(V-POL);
          MPC23017BitClear(V-PTT);
          
          pwrConfig.sec_state = V_PA_COOL;
          pause(120);    // TODO: create a timeout and a user signal.
          
          pwrConfig.sec_state = V_PA_DOWN;
          MPC23017BitClear(V-PA);
          MPC23017BitClear(V-KEY);          
          break;        
        case V_PA_COOL:
        case V_PA_DOWN:
        case V_UHF_LHCP:
          MPC23017BitSet(U-POL);
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U-POL);
          break;
        case V_TRANS_ON:
          MPC23017BitSet(V-PTT);
          break;        
        case V_TRANS_OFF:
          MPC23017BitClear(V-PTT);
          break;        
        case V_LHCP:
          temporary = MPC23017BitRead(V-PTT);
          MPC23017BitClear(V-PTT);
          pause(100);
          MPC23017BitSet(V-POL);
          pause(100);
          if(temporary)
            MPC23017BitSet(V-PTT);
          else
            MPC23017BitClear(V-PTT);
          break;
          
        case V_RHCP:
          temporary = MPC23017BitRead(V-PTT);
          MPC23017BitClear(V-PTT);
          pause(100);
          MPC23017BitClear(V-POL);
          pause(100);
          if(temporary)
            MPC23017BitSet(V-PTT);
          else
            MPC23017BitClear(V-PTT);
          break;        
        case default:
          stateError();
          break;                
      }
      
    case L_TX:
      switch(pwrConfig.sec_state){
        case L_TRANSMIT:
          MPC23017BitSet(U-LNA);
          MPC23017BitSet(V-LNA);
          MPC23017BitSet(L-PA);
          break;
        case L_SWITCH:
        case L_SHUTDOWN:
          MPC23017BitClear(L-PTT);
          MPC23017BitClear(U-POL);
          MPC23017BitClear(V-POL);
          MPC23017BitClear(V-LNA);
          MPC23017BitClear(U-LNA);
          
          pwrConfig.sec_state = L_PA_COOL;
          pause(120);          // TODO: create a timeout and a user signal.
          
          pwrConfig.sec_state = L_PA_DOWN;
          MPC23017BitClear(L-PA);         
          break;        
        case L_PA_COOL:
        case L_PA_DOWN:
        case L_UHF_LHCP:
          MPC23017BitSet(U-POL);
          break;
        case V_UHF_RHCP:
          MPC23017BitClear(U-POL);
          break;
        case L_TRANS_ON:
          MPC23017BitSet(L-PTT);
          break;        
        case L_TRANS_OFF:
          MPC23017BitClear(L-PTT);
          break;        
        case L_VHF_LHCP:
          MPC23017BitSet(V-POL);
          break;          
        case L_VHF_RHCP:
          MPC23017BitClear(V-POL);
          break;        
        case default:
          stateError();
          break;                
      }
      
    case default:
      stateError();
      break;
  }
}


int MPC23017BitSet(int bit){
  
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  unit8_t reg_address = 0;
  uint8_t reg_value = 0;
  
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
  if (read(file_i2c, buffer, length) != length)         //read() returns 
  //the number of bytes actually read, if it doesn't match then an error 
  //occurred (e.g. no response from the device)
  {
    //ERROR HANDLING: i2c transaction failed
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
  
  //write the new value
  buffer[2] = 0x40;   //write command.
  buffer[1] = reg_address;   //address of register
  buffer[0] = reg_value;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
  }  
}


int MPC23017BitClear(int bit){
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  unit8_t reg_address = 0;
  uint8_t reg_value = 0;
  unit8_t mask = 0xFF;
  
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
  if (read(file_i2c, buffer, length) != length)         //read() returns 
  //the number of bytes actually read, if it doesn't match then an error 
  //occurred (e.g. no response from the device)
  {
    //ERROR HANDLING: i2c transaction failed
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
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
  }   
}


int MPC23017BitReset(){
  uint8_t buffer[3] = {0};
  
  //reset GPIOA
  buffer[2] = 0x40;   //write command.
  buffer[1] = 0x12;   //address of register
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
  } 
  
  //reset GPIOB
  buffer[2] = 0x40;   //write command.
  buffer[1] = 0x12;   //address of register
  buffer[0] = 0x00;   //for now directions of all pins are changed
  length = 3;        //Number of bytes to write
   if (write(file_i2c, buffer, length) != length)    //write() returns 
  //the number of bytes actually written, if it doesn't match then an 
  //error occurred (e.g. no response from the device)
  {
    // ERROR HANDLING: i2c transaction failed 
    printf("Failed to write to the i2c bus %d.\n",write(file_i2c,buffer, length));
  } 
}

int MPC23017BitRead(int bit){
  uint8_t buffer[3] = {0};
  uint8_t shift_value = 0;
  unit8_t reg_address = 0;
  uint8_t reg_value = 0;
  unit8_t mask = 0xFF;
  
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
  if (read(file_i2c, buffer, length) != length)         //read() returns 
  //the number of bytes actually read, if it doesn't match then an error 
  //occurred (e.g. no response from the device)
  {
    //ERROR HANDLING: i2c transaction failed
    printf("Failed to read from the i2c bus.\n");
    printf("Data read: %x\n", buffer[0]);
  }
  else
  {
    printf("Data read: %x\n", buffer[0]);
    return buffer[0];
  }
  
  return -1;
}