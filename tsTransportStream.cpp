#include "tsTransportStream.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>

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
  if(Input[0] == 71){
    //m_SB
    m_SB = Input[0];

    //m_E
    uint8_t mask = 0b10000000;
    if((mask & Input[1]) > 0){
      m_E = 1;
    } else m_E = 0;

    //m_S
    mask = 0b01000000;
    if((mask & Input[1]) > 0){
      m_S = 1;
    } else m_S = 0;

    //m_T
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

    return 4;
  } 
  else{
    return -1;
  } 
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const
{
  //print + rzutowanie na int by poprawnie wyswietlilo (nie w char, tylko int)
  std::cout << "TS: SB=" << (int)m_SB << " E=" << (int)m_E << " S=" << (int)m_S << " T=" << (int)m_T << " PID=" << (int)m_PID << " TSC=" << (int)m_TSC << " AF=" << (int)m_AFC << " CC=" << (int)m_CC; 
}

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xTS_AdaptationField
//=============================================================================================================================================================================

/// @brief Reset - reset all TS packet header fields
void xTS_AdaptationField::Reset()
{
  //Reset
  m_AdaptationFieldControl = 0;
  m_AFL = 0;
  m_DC = 0;
  m_RA = 0;
  m_SP = 0;
  m_PR = 0;
  m_OR = 0;
  m_SF = 0;
  m_TP = 0; 
  m_EX = 0;
  m_PCR = 0;
  m_PCRB = 0;
  m_PCRE = 0;
  m_Time = 0; 
  m_Stuffing = 0;
}

  /**
  @brief Parse adaptation field
  @param PacketBuffer is pointer to buffer containing TS packet
  @param AdaptationFieldControl is value of Adaptation Field Control field of corresponding TS packet header
  @return Number of parsed bytes (length of AF or -1 on failure)
  */
int32_t xTS_AdaptationField::Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl)
{
  m_AdaptationFieldControl = AdaptationFieldControl;

  if(m_AdaptationFieldControl==2 || m_AdaptationFieldControl==3){
    //parsing 
    m_AFL = PacketBuffer[4];

    //m_DC
    uint8_t mask = 0b10000000;
    if((mask & PacketBuffer[5]) > 0){
      m_DC = 1;
    } else m_DC = 0;

    //m_RA
    mask = 0b01000000;
    if((mask & PacketBuffer[5]) > 0){
      m_RA = 1;
    } else m_RA = 0;

    //m_SP
    mask = 0b00100000;
    if((mask & PacketBuffer[5]) > 0){
      m_SP = 1;
    } else m_SP = 0;

    //m_PR
    mask = 0b00010000;
    if((mask & PacketBuffer[5]) > 0){
      m_PR = 1;
    } else m_PR = 0;

    //m_OR
    mask = 0b00001000;
    if((mask & PacketBuffer[5]) > 0){
      m_OR = 1;
    } else m_OR = 0;

    //m_SF
    mask = 0b00000100;
    if((mask & PacketBuffer[5]) > 0){
      m_SF = 1;
    } else m_SF = 0;

    //m_TP
    mask = 0b00000010;
    if((mask & PacketBuffer[5]) > 0){
      m_TP = 1;
    } else m_TP = 0;

    //m_EX
    mask = 0b00000001;
    if((mask & PacketBuffer[5]) > 0){
      m_EX = 1;
    } else m_EX = 0;

    //parsing PCR fields + time + stuffing 
    if(m_PR == 1){
      m_PCRB = m_PCRB + PacketBuffer[6];
      m_PCRB = m_PCRB << 8;
      m_PCRB = m_PCRB + PacketBuffer[7];
      m_PCRB = m_PCRB << 8;
      m_PCRB = m_PCRB + PacketBuffer[8];
      m_PCRB = m_PCRB << 8;
      m_PCRB = m_PCRB + PacketBuffer[9];
      m_PCRB = m_PCRB << 1;
      uint8_t tempVal = 0;
      if((0b10000000 & PacketBuffer[10])>0){
        tempVal = 1;
      }
      m_PCRB = m_PCRB + tempVal;

      m_PCRE = m_PCRE + (0b00000001 & PacketBuffer[10]);
      m_PCRE = m_PCRE << 8;
      m_PCRE = m_PCRE + PacketBuffer[11];

      m_PCR = (m_PCRB*300) + m_PCRE;

      m_Time = (float)m_PCR/(float)xTS::ExtendedClockFrequency_Hz;

      m_Stuffing = 0;
    }
    else{
      m_Stuffing = m_AFL - 1;
    }

    return m_AFL;
  }
  else{
    return -1;
  }
}
/// @brief Print all TS packet header fields
void xTS_AdaptationField::Print() const
{
  //print - opcja z PCR
  // if(m_PR == 1){
  //   std::cout << " AF: L=" << (int)m_AFL << " DC=" << (int)m_DC << " RA=" << (int)m_RA << " SP=" << (int)m_SP << " PR=" << (int)m_PR << " OR=" << (int)m_OR << " SF=" << (int)m_SF << " TP=" << (int)m_TP << " EX=" << (int)m_EX << " PCR=" << int(m_PCR) << " (Time=" << std::setprecision(6) << std::fixed << m_Time << "s)" << " Stuffing:" << (int)m_Stuffing;
  // }
  // else{
  //   std::cout << " AF: L=" << (int)m_AFL << " DC=" << (int)m_DC << " RA=" << (int)m_RA << " SP=" << (int)m_SP << " PR=" << (int)m_PR << " OR=" << (int)m_OR << " SF=" << (int)m_SF << " TP=" << (int)m_TP << " EX=" << (int)m_EX << " Stuffing:" << (int)m_Stuffing;
  // }

  std::cout << " AF: L=" << (int)m_AFL << " DC=" << (int)m_DC << " RA=" << (int)m_RA << " SP=" << (int)m_SP << " PR=" << (int)m_PR << " OR=" << (int)m_OR << " SF=" << (int)m_SF << " TP=" << (int)m_TP << " EX=" << (int)m_EX << " Stuffing=" << (int)m_Stuffing;
}

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xPES_PacketHeader
//=============================================================================================================================================================================

void xPES_PacketHeader::Reset() 
{
  m_PacketStartCodePrefix = 0;
  m_StreamId = 0;
  m_PacketLength = 0;
  indexStartHeader = 0; 
  indexStartNoHeader = 0;
  additionalHeaderSize = 0;
}

int32_t xPES_PacketHeader::Parse(const uint8_t* PacketBuffer, uint16_t PacketIdentifier, uint8_t ifAF, uint8_t AdaptationFieldLength) 
{
  indexStartHeader = 0; //zmienna przechowujaca informacje od jakiego bajtu zaczyna sie Payload dla danego pakietu 188 bajtowego
  indexStartNoHeader = 0; //zmienna przechowujaca informacje od jakiego bajtu zaczyna sie Payload dla danego pakietu 188 bajtowego
  uint8_t AFL_Flag = 1;

  if(ifAF == 0 || ifAF == 1){
    AdaptationFieldLength = 0;
    AFL_Flag = 0;
  }

  if(PacketIdentifier == 136) { //na start sprawdzamy, czy PID jest 136 
    indexStartNoHeader = 4 + AFL_Flag + AdaptationFieldLength;
    indexStartHeader = indexStartNoHeader;

    if(PacketBuffer[indexStartHeader] == 0 && PacketBuffer[indexStartHeader+1] == 0 && PacketBuffer[indexStartHeader+2] == 1){
      m_PacketStartCodePrefix = 1;
    }

    m_StreamId = PacketBuffer[indexStartHeader+3];

    m_PacketLength = m_PacketLength + PacketBuffer[indexStartHeader+4];
    m_PacketLength = m_PacketLength << 8;
    m_PacketLength = m_PacketLength + PacketBuffer[indexStartHeader+5];
  }

  if (m_StreamId != eStreamId_program_stream_map 
      && m_StreamId != eStreamId_padding_stream 
      && m_StreamId != eStreamId_private_stream_2
      && m_StreamId != eStreamId_ECM
      && m_StreamId != eStreamId_EMM
      && m_StreamId != eStreamId_program_stream_directory
      && m_StreamId != eStreamId_DSMCC_stream
      && m_StreamId != eStreamId_ITUT_H222_1_type_E) {
        additionalHeaderSize = 3 + PacketBuffer[indexStartHeader+8];
        indexStartHeader = indexStartHeader + 3 + PacketBuffer[indexStartHeader+8] + 6;
  }

  return 1;
}

void xPES_PacketHeader::Print() const 
{
    std::cout << " PES: PSCP=" << (int)m_PacketStartCodePrefix << " SID=" << (int)m_StreamId << " L=" << (int)m_PacketLength;
}

//=============================================================================================================================================================================

//=============================================================================================================================================================================
// xPES_Assembler
//=============================================================================================================================================================================
void  xPES_Assembler::Init (int32_t PID)
{
  m_PID = PID;
  m_DataOffset = 0;
  m_LastContinuityCounter = -47; //randomowa liczba na start
  m_Started = false;
}

void SaveBufferToFile(const uint8_t* Buffer, int BufferSize, const std::string& FileName)
{
    std::ofstream out(FileName, std::ios::binary | std::ios::app); // Otwarcie pliku w trybie binarnym z dopisywaniem danych
    if(out.is_open())
    {
        out.write(reinterpret_cast<const char*>(Buffer), BufferSize); // Zapis danych
        out.close(); // Zamknięcie pliku
    }
    else
    {
        std::cout << "Couldn't open the file!" << std::endl;
    }
}

xPES_Assembler::eResult xPES_Assembler::AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField)
{
  m_PESH.Reset();
  m_PESH.Parse(TransportStreamPacket, PacketHeader->getPID(), PacketHeader->getAFC(), AdaptationField->getAdaptationFieldLength());

  if(PacketHeader->getS() == 1 && m_Started == false){
    Init(PacketHeader->getPID());
    if(m_PID != PacketHeader->getPID()){
      return eResult::UnexpectedPID;
    }

    xBufferReset();
    xBufferAppend(TransportStreamPacket, m_PESH.getIndexStartHeader());
    m_LastContinuityCounter = PacketHeader->getCC();
    m_Started = true; 
    return eResult::AssemblingStarted;
  }
  else if(PacketHeader->getS() == 0 && m_Started == true){
    if((m_LastContinuityCounter+1)%16 == PacketHeader->getCC()){
      xBufferAppend(TransportStreamPacket, m_PESH.getIndexStartNoHeader());
      m_LastContinuityCounter = PacketHeader->getCC();
      if(m_DataOffset == m_BufferSize){
        m_Started = false;
        SaveBufferToFile(m_Buffer, m_BufferSize, "PID136.mp2");
        return eResult::AssemblingFinished;
      }
      return eResult::AssemblingContinue;
    }
  }
  else{
    return eResult::StreamPackedLost;
  }
}

void  xPES_Assembler::xBufferReset()
{
  m_BufferSize = m_PESH.getPacketLength() - m_PESH.getAdditionalHeaderSize();
  m_Buffer = new uint8_t[m_BufferSize];
  std::memset(m_Buffer, 0, m_BufferSize);
}

void  xPES_Assembler::xBufferAppend(const uint8_t* Data, int32_t Size)
{
  for(int i=Size; i<188 && m_DataOffset<m_BufferSize; i++){
    m_Buffer[m_DataOffset] = Data[i];
    m_DataOffset++;
  }
}
//=============================================================================================================================================================================