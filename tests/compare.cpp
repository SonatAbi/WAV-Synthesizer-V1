// byte by byte comparison of two WAV files
// each sample must differ by at most 1
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <string>

int main(int argc, char *argv[])
{
  if(argc != 3){
    std::cerr << "Error: two file arguments expected." << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream in1(argv[1], std::ios::binary);
  if(!in1){
    std::cerr << "Error: cannot open file " << argv[1] << " for reading." << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream in2(argv[2], std::ios::binary);
  if(!in2){
    std::cerr << "Error: cannot open file " << argv[2] << " for reading." << std::endl;
    return EXIT_FAILURE;
  }

  // read header and compare, must be exactly the same
  std::size_t bytenum = 0;
  const std::size_t HEADER_SIZE = 44;
  bool status = false;
  while( !(in1.eof() ||  in2.eof()) && bytenum < HEADER_SIZE){

     std::fstream::char_type b1, b2;
     in1.read(reinterpret_cast<std::fstream::char_type*>(&b1), sizeof b1);
     in2.read(reinterpret_cast<std::fstream::char_type*>(&b2), sizeof b2);

    if(b1 != b2){
      std::cerr << "Files differ in header at byte " << bytenum << std::endl;
      status = true;
      break;
    }
    bytenum += 1;
  }

  // if headers the same, read samples, compare absolute difference to 1
  if(!status){
    while( !(in1.eof() ||  in2.eof()) ){
      std::int16_t s1, s2;
      in1.read(reinterpret_cast<std::fstream::char_type*>(&s1), sizeof s1);
      in2.read(reinterpret_cast<std::fstream::char_type*>(&s2), sizeof s2);

      std::int16_t diff = (s1 > s2) ? (s1-s2) : (s2-s1);
      
      if(diff > 1){
	std::cerr << "Samples differ at byte " << bytenum << " by " << diff << std::endl;
	status = true;
	break;
      }
      bytenum += 2;
      
    }
  }
  
  in1.close();
  in2.close();


      
  if(status){
    return EXIT_FAILURE;
  }
 
  return EXIT_SUCCESS;
}
