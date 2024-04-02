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
  file.open("C:/Users/Rafal/Downloads/example_new.ts", std::ios::in | std::ios::binary); //PAMIETAC o ustawieniu odpowiedniej sciezki w zaleznosci od tego, gdzie zapisany jest plik

  // TODO - check if file if opened (done6)
  if(file.is_open() && file.good()){  //is_open() sprawdza, czy plik zostal otwarty, a good() zwraca informacje, czy zadna flaga bledu nie jest ustawiona
    std::cout << "File was accessed!" << std::endl;
  }
  else{
    std::cout << "Couldn't access the file!" << std::endl;
    return EXIT_FAILURE; //konczy dzialanie main (programu), gdy nie mozna uzyskac dostepu do pliku
  }

  xTS_PacketHeader TS_PacketHeader;
  char *TS_PacketBuffer[xTS::TS_PacketLength]; //stworzenie tablicy (bufora) przechowujacej pojedynczy pakiet strumienia transportowego; gdyby nie fakt, ze TS_PacketLength jest const, to nie moglbym tutaj go ustawic jako rozmiar tablicy

  int32_t TS_PacketId = 0;
  while(!file.eof() == true) //petla wykonuje sie dopoki nie zostanie osiagniety koniec pliku
  {
    // TODO - read from file (done7)
    file.read(*TS_PacketBuffer, xTS::TS_PacketLength); //read() zczytuje pierwsze 188B do bufora i w kolejnej iteracji zczytuje kolejne 188B itd.
    
    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(/*TS_PacketBuffer*/ nullptr);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf("\n");

    TS_PacketId++;
  }
  
  // TODO - close file (done8)
  file.close();

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
