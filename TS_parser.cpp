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
  file.open("C:/Users/Rafal/Downloads/example_new.ts", std::ios::in | std::ios::binary);

  // TODO - check if file if opened (done6)
  if(file.is_open() == true && file.good() == true){  //is_open() sprawdza, czy plik zostal otwarty, a good()
    std::cout << "File was accessed!" << std::endl;
  }
  else{
    std::cout << "Couldn't access the file!" << std::endl;
    return 0; //konczy dzialanie main (programu), gdy nie mozna uzyskac dostepu do pliku
  }

  xTS_PacketHeader TS_PacketHeader;

  int32_t TS_PacketId = 0;
  while(/*not eof*/ 0)
  {
    // TODO - read from file

    TS_PacketHeader.Reset();
    TS_PacketHeader.Parse(/*TS_PacketBuffer*/ nullptr);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf("\n");

    TS_PacketId++;
  }
  
  // TODO - close file

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
