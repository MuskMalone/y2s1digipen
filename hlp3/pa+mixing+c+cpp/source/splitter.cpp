/**
 * @file splitter.cpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief cpp file
 * @version 0.1
 * @date 2023-09-06
 * @param assignment 1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "splitter.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
// this is the maximum size of buffer for reading data from input file
// and to write data to output file ...
// if a file contains 10000 bytes, this means that you have to repeatedly
// [thrice] read the file for 4096 bytes, followed by the next 4096 bytes,
// followed by the final 1808 bytes.
// Not following this recipe by increasing the buffer to a higher value
// will mean that your submission is not following the specified requirements
// and will receive a zero grade!!!
constexpr int FOUR_K {4096}; // Don't modify this!!!
int count{1};
namespace{
  /**
   * @brief copy by chunks
   * 
   * @param inputstream 
   * @param outputstream 
   * @param chunksize 
   */
  void copy_by_chunks(std::ifstream& inputstream, std::ofstream &outputstream, long long chunksize){
      long long chunkcount {chunksize / FOUR_K};
      //printf("==============%lld, %lld, %lld\n", chunksize, chunkcount, chunksize % FOUR_K);

      for (long long j{}; j < chunkcount; ++j){
        char* bytes {new char[FOUR_K]};
        inputstream.read(bytes, FOUR_K);
        outputstream.write(bytes, FOUR_K);
        delete [] bytes;
      }
      if (chunksize % FOUR_K){
        char* bytes {new char[chunksize % FOUR_K]};
        inputstream.read(bytes, chunksize % FOUR_K);
        outputstream.write(bytes, chunksize % FOUR_K);
        delete [] bytes;
      }

  }
}
/**
 * @brief split and join
 * 
 * @param argc 
 * @param char_argv 
 * @return SplitResult 
 */
SplitResult split_join(int argc, char *char_argv[]) {
  SplitResult rs = E_NO_ACTION;
  std::vector<std::string> argv{};
  for (int i {}; i < argc; ++i)argv.push_back(char_argv[i]);
  if (argc < 2) return rs;
  if (argv[1] == "-s"){
    if (argc < 7) return rs;
    long long chunksize { std::stoll(argv[2]) };
    std::string output { argv[4] }, input{ argv[6] };
    std::ifstream inputstream {input, std::ios::binary | std::ios::ate};
    if (!inputstream.is_open()) return rs = E_BAD_SOURCE; 
    long long filesize = static_cast<long long>(inputstream.tellg());
    if (filesize <= 0) return rs = E_SMALL_SIZE;
    inputstream.seekg(0);
    // char* bytes {new char[filesize]};
    // inputstream.read(bytes, filesize);
    long long filecount { filesize/chunksize };
    //printf("%lld, %lld, %lld, %lld\n", filesize, chunksize, filecount, filesize%chunksize);

    for (long long i{}; i < filecount; ++i){
      std::string str = std::to_string(i + 1);
      size_t n = 4;
      int precision = n - std::min(n, str.size());
      std::string s = std::string(precision, '0').append(str);
      //printf("%s\n", (output+s).c_str());

      std::ofstream outputstream {output + s, std::ios::binary};
      copy_by_chunks(inputstream, outputstream, chunksize);
    }
    if (filesize%chunksize){
      //printf("printed one more\n");
      std::string str = std::to_string(filecount + 1);
      size_t n = 4;
      int precision = n - std::min(n, str.size());
      std::string s = std::string(precision, '0').append(str);
      //printf("%s\n", (output+s).c_str());
      std::ofstream outputstream {output + s, std::ios::binary};
      copy_by_chunks(inputstream, outputstream, filesize%chunksize);
    }
    
    rs = E_SPLIT_SUCCESS;
  }
  if (argv[1] == "-j"){
    if (argc < 6) return rs;
    std::string output { argv[3] };
    std::ofstream outputstream{ output , std::ios::binary };
    if (!outputstream.is_open()) return rs = E_BAD_DESTINATION;
    for (int i{5}; i < argc; ++i){
      std::ifstream inputstream{ argv[i], std::ios::binary | std::ios::ate };
      if (!inputstream.is_open()) return rs = E_BAD_SOURCE; 
      long long filesize = static_cast<long long>(inputstream.tellg());
      inputstream.seekg(0);

      copy_by_chunks(inputstream, outputstream, filesize);
    }
    rs = E_JOIN_SUCCESS;
  }
  else return rs;
  // parse command-line arguments to determine whether to split a file
  // into smaller files	or to join smaller files into original file ...

  // ...

  return rs;
}

