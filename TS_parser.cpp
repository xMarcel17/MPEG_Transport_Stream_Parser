#include "tsCommon.h"
#include "tsTransportStream.h"
#include <fstream>
#include <iostream>

//=============================================================================================================================================================================

int main(int argc, char *argv[ ], char *envp[ ])
{
  // TODO - open file (done5)
  //stworzenie zmiennej 'file', ktora bedzie wskazywac na nasz plik example_new.ts
  std::fstream file; 
  //przypisanie pliku "example_new.ts" do zmiennej file (uzyskanie dostepu do tego pliku) oraz "poinformowanie kompilatora" o trybach otwarcia pliku
  file.open("/home/lab/Downloads/example_new.ts", std::ios::in | std::ios::binary); //PAMIETAC o ustawieniu odpowiedniej sciezki w zaleznosci od tego, gdzie zapisany jest plik

  // TODO - check if file if opened (done6)
  if(file.is_open() && file.good()){  //is_open() sprawdza, czy plik zostal otwarty, a good() zwraca informacje, czy zadna flaga bledu nie jest ustawiona
    std::cout << "File was accessed!" << std::endl;
  }
  else{
    std::cout << "Couldn't access the file!" << std::endl;
    return EXIT_FAILURE; //konczy dzialanie main (programu), gdy nie mozna uzyskac dostepu do pliku
  }

  xTS_PacketHeader TS_PacketHeader;
  char *TS_PacketBuffer = new char[xTS::TS_PacketLength]; //stworzenie tablicy dynamicznej (bufora) przechowujacej pojedynczy pakiet strumienia transportowego

  int32_t TS_PacketId = 0;
  while(!file.eof() == true && TS_PacketId < 34) //petla wykonuje sie dopoki nie zostanie osiagniety koniec pliku
  {
    // TODO - read from file (done7)
    file.read(TS_PacketBuffer, xTS::TS_PacketLength); //read() zczytuje pierwsze 188B do bufora i w kolejnej iteracji zczytuje kolejne 188B itd.
    uint8_t *TS_PacketBufferFinal = reinterpret_cast<uint8_t*>(TS_PacketBuffer); //rzutowanie typu tablicy na uint8_t (z racji iz nasz Input w Parse() to uint8_t, wiec musimy to wykonac) 

    if(TS_PacketBuffer[0] == 71){ //upewnienie sie, ze poczatek pakietu (pierwsze 8 bitow - Sync Byte) jest rowny 0x47)
      TS_PacketHeader.Reset();
      TS_PacketHeader.Parse(TS_PacketBufferFinal);

      printf("%010d ", TS_PacketId);
      TS_PacketHeader.Print();
      printf("\n");

      TS_PacketId++;
    }
  }
  
  // TODO - close file (done8)
  file.close();

  delete[] TS_PacketBuffer; //zwolnienie zaalokowanej pamieci (tablicy) po jej uzyciu, by uniknac wyciekow pamieci
  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
