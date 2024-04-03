#include "tsTransportStream.h"
#include <iostream>

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields (done4)
void xTS_PacketHeader::Reset()
{
  m_SB = 0;
  m_E = 0;
  m_S = 0;
  m_T = 0;
  m_PID = 0;
  m_TSC = 0;
  m_AFC = 0;
  m_CC = 0;
}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t* Input)
{
  //i`m empty (done9)
  m_SB = Input[0];

  uint8_t mask = 0b10000000;
  if((mask & Input[1]) > 0){
    m_E = 1;
  } else m_E = 0;

  mask = 0b01000000;
  if((mask & Input[1]) > 0){
    m_S = 1;
  } else m_S = 0;

  mask = 0b00100000;
  if((mask & Input[1]) > 0){
    m_T = 1;
  } else m_T = 0;

  //m_PID
  uint16_t m_PID_value = 0;

  mask = 0b00010000;
  if((mask & Input[1]) > 0){
    m_PID_value += 4096;
  } else m_PID_value += 0; 

  mask = 0b00001000;
  if((mask & Input[1]) > 0){
    m_PID_value += 2048;
  } else m_PID_value += 0; 

  mask = 0b00000100;
  if((mask & Input[1]) > 0){
    m_PID_value += 1024;
  } else m_PID_value += 0; 

  mask = 0b00000010;
  if((mask & Input[1]) > 0){
    m_PID_value += 512;
  } else m_PID_value += 0;

  mask = 0b00000001;
  if((mask & Input[1]) > 0){
    m_PID_value += 256;
  } else m_PID_value += 0;
 
  m_PID = m_PID_value + ((uint16_t)Input[2]);

  //m_TSC 
  mask = 0b10000000; 
  if((mask & Input[3]) > 0){
    m_TSC += 2;
  } else m_TSC += 0;

  mask = 0b01000000; 
  if((mask & Input[3]) > 0){
    m_TSC += 1;
  } else m_TSC += 0;

  //m_AFC
  mask = 0b00100000; 
  if((mask & Input[3]) > 0){
    m_AFC += 2;
  } else m_AFC += 0;

  mask = 0b00010000; 
  if((mask & Input[3]) > 0){
    m_AFC += 1;
  } else m_AFC += 0;

  //m_CC
  mask = 0b00001000; 
  if((mask & Input[3]) > 0){
    m_CC += 8;
  } else m_CC += 0;

  mask = 0b00000100; 
  if((mask & Input[3]) > 0){
    m_CC += 4;
  } else m_CC += 0;

  mask = 0b00000010; 
  if((mask & Input[3]) > 0){
    m_CC += 2;
  } else m_CC += 0;

  mask = 0b00000001; 
  if((mask & Input[3]) > 0){
    m_CC += 1;
  } else m_CC += 0;

  return NOT_VALID;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const
{
  //print sth (done10)
  std::cout << "TS:  SB=" << (int)m_SB << "  E=" << (int)m_E << " S=" << (int)m_S << " T=" << (int)m_T << " PID=" << (int)m_PID << " TSC=" << (int)m_TSC << " AF=" << (int)m_AFC << "  CC=" << (int)m_CC << std::endl; 
}

//=============================================================================================================================================================================
