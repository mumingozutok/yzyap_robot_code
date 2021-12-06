#ifndef __MCU_JSON_PARSER_H
#define __MCU_JSON_PARSER_H

uint8_t calc_chksum(uint8_t* buf, uint8_t len){
  uint8_t sum = 0;
  for(uint8_t i = 0;i<len;i++){
    sum = sum + buf[i];
  }
  return sum;
}

void mcu_comm(DynamicJsonDocument doc){
  const char* target = doc["target"];
  const char* cmd = doc["cmd"];
  uint8_t i,data_array[64];// = {0xFF,0xFE,0x03,0x01,0x02,0x03,0x06};
  
  Serial.print("Target: ");
  Serial.println(target); //mcu,esp

  data_array[0] = doc["cmd"][0];
  data_array[1] = doc["cmd"][1];
  
  if(data_array[0] == 0xFF & data_array[1] == 0xFE){
    data_array[2] = doc["cmd"][2];
    for(i=0;i<data_array[2];i++){
      data_array[3+i] = doc["cmd"][3+i]; 
    }
    //calculate the checksum of the data and put in the data array
    data_array[3+i] = calc_chksum(&data_array[3], data_array[2]);
    
    com2stm_PrintBuffer(data_array, 3+i+1); 
  } 
}

bool mcu_json_parser(DynamicJsonDocument doc){
  mcu_comm(doc);
  return true;
}

#endif
