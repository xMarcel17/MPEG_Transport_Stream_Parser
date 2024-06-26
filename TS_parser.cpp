#include "tsCommon.h"
#include "tsTransportStream.h"
#include <fstream>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  std::fstream file; //stworzenie zmiennej 'file', ktora bedzie wskazywac na nasz plik example_new.ts

  //przypisanie pliku "example_new.ts" do zmiennej file (uzyskanie dostepu do tego pliku) oraz "poinformowanie kompilatora" o trybach otwarcia pliku
  file.open("C:/Users/Rafal/Downloads/example_new.ts", std::ios::in | std::ios::binary); //PAMIETAC o ustawieniu odpowiedniej sciezki w zaleznosci od tego, gdzie zapisany jest plik

  if(file.is_open() && file.good()){  //is_open() sprawdza, czy plik zostal otwarty, a good() zwraca informacje, czy zadna flaga bledu nie jest ustawiona
    std::cout << "File was accessed!" << std::endl;
  }
  else{
    std::cout << "Couldn't access the file!" << std::endl;
    return EXIT_FAILURE; //konczy dzialanie main (programu), gdy nie mozna uzyskac dostepu do pliku
  }

  xTS_PacketHeader TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField; 
  xPES_PacketHeader PES_PacketHeader; 
  xPES_Assembler PES_Assembler;

  char *TS_PacketBuffer = new char[xTS::TS_PacketLength]; //stworzenie tablicy dynamicznej (bufora) przechowujacej pojedynczy pakiet strumienia transportowego

  int32_t TS_PacketId = 0;
  while(!file.eof() == true) //petla wykonuje sie dopoki nie zostanie osiagniety koniec pliku
  {
    file.read(TS_PacketBuffer, xTS::TS_PacketLength); //read() zczytuje pierwsze 188B do bufora i w kolejnej iteracji zczytuje kolejne 188B itd.
    uint8_t *TS_PacketBufferFinal = reinterpret_cast<uint8_t*>(TS_PacketBuffer); //rzutowanie typu tablicy char na uint8_t (z racji iz nasz Input w Parse() to uint8_t, wiec musimy to wykonac) 

    if(TS_PacketBuffer[0] == 71){ //upewnienie sie, ze poczatek pakietu (pierwsze 8 bitow - Sync Byte) jest rowny 0x47
      //PacketHeader
      TS_PacketHeader.Reset();
      TS_PacketHeader.Parse(TS_PacketBufferFinal);

      //AdaptationField
       if(TS_PacketHeader.hasAdaptationField() == true){ //(done15)
         TS_AdaptationField.Reset();
         TS_AdaptationField.Parse(TS_PacketBufferFinal, TS_PacketHeader.getAFC());
      }

      //PES_PacketHeader
      if(TS_PacketHeader.hasPayload() == true && TS_PacketHeader.getPID() == 136){
        if(TS_PacketHeader.getS() == 1){
          PES_PacketHeader.Reset();
          PES_PacketHeader.Parse(TS_PacketBufferFinal, TS_PacketHeader.getPID(), TS_PacketHeader.getAFC(), TS_AdaptationField.getAdaptationFieldLength());
          //PES_PacketHeader.Print();
        }
      }

      if(TS_PacketHeader.getPID() == 136){
        //printf("%010d ", TS_PacketId);
        //TS_PacketHeader.Print();

        xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBufferFinal, &TS_PacketHeader, &TS_AdaptationField);
        switch(Result)
          {
            //case xPES_Assembler::eResult::StreamPackedLost : printf(" PcktLost "); break;
            //case xPES_Assembler::eResult::AssemblingStarted : TS_AdaptationField.Print(); printf(" Assembling Started"); PES_Assembler.PrintPESH(); break;
            //case xPES_Assembler::eResult::AssemblingContinue: printf(" Assembling Continue "); break;
            //case xPES_Assembler::eResult::AssemblingFinished: printf(" Assembling Continue"); TS_AdaptationField.Print(); printf(" Assembling Finished "); printf("PES: PcktLen=%d", PES_Assembler.getNumPacketBytes()+PES_PacketHeader.getAdditionalHeaderSize()+6); printf(" HeadLen=%d", PES_PacketHeader.getAdditionalHeaderSize()+6) ; printf(" DataLen=%d", PES_Assembler.getNumPacketBytes()); break;
            default: break;
          }
        //printf("\n");
      }   

      TS_PacketId++;
    }
  }
  
  //close file
  file.close();

  delete[] TS_PacketBuffer; //zwolnienie zaalokowanej pamieci (tablicy) po jej uzyciu, by uniknac wyciekow pamieci
  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
