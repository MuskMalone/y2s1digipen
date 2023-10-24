/**
 * @file splitter.c
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief c file
 * @version 0.1
 * @date 2023-09-06
 * @param assignment 1
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifdef __cplusplus
extern "C" {
#endif
// add other headers from C standard library ...
#include "splitter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// this is the maximum size of buffer for reading data from input file
// and to write data to output file ...
// if a file contains 10000 bytes, this means that you have to repeatedly
// [thrice] read the file for 4096 bytes, followed by the next 4096 bytes,
// followed by the final 1808 bytes.
// Not following this recipe by increasing the buffer to a higher value
// will mean that your submission is not following the specified requirements
// and will receive a zero grade!!!
#define FOUR_K 4096 // Don't modify this!!!

/**
 * @brief copy by chunks
 * 
 * @param inputstream 
 * @param outputstream 
 * @param chunksize 
 * @return int 
 */
int copy_by_chunks(FILE* inputstream, FILE* outputstream, long long chunksize){
    long long chunkcount = chunksize / FOUR_K;
    //printf("==============%lld, %lld, %lld\n", chunksize, chunkcount, chunksize % FOUR_K);
    for (long long j=0; j < chunkcount; ++j){
      // char* bytes = (char*) malloc(FOUR_K);
      // if (!bytes) return 0;
      char bytes[FOUR_K] = {0};
      fread(bytes, FOUR_K, 1, inputstream);
      //inputstream.read(bytes, FOUR_K);
      fwrite(bytes, FOUR_K, 1, outputstream);
      //outputstream.write(bytes, FOUR_K);
      //delete [] bytes;
      //free(bytes);
    }
    if (chunksize % FOUR_K){
      char* bytes = (char*)malloc(chunksize % FOUR_K);
      if (!bytes) return 0;
      fread(bytes, chunksize%FOUR_K, 1, inputstream);
      //inputstream.read(bytes, chunksize % FOUR_K);
      fwrite(bytes, chunksize%FOUR_K, 1, outputstream);
      //outputstream.write(bytes, chunksize % FOUR_K);
      free(bytes);
    }
    return 1;
}
/**
 * @brief split or join
 * 
 * @param argc 
 * @param argv 
 * @return SplitResult 
 */
SplitResult split_join(int argc, char *argv[]) {
	
  SplitResult rs = E_NO_ACTION;
  if (argc < 2) return rs;
  if (!strcmp(argv[1], "-s")){
    if (argc < 7) return rs;
    long long chunksize = atoi(argv[2]);
    char* output = argv[4], *input = argv[6];
    FILE*inputstream = fopen(input, "rb");
    if (!inputstream) return rs = E_BAD_SOURCE; 

    fseek(inputstream, 0, SEEK_END); // seek to end of file
    long long filesize = ftell(inputstream); // get current file pointer
    fseek(inputstream, 0, SEEK_SET); // seek back to beginning of file

    if (filesize <= 0) return rs = E_SMALL_SIZE;
    // char* bytes {new char[filesize]};
    // inputstream.read(bytes, filesize);   
    long long filecount = filesize/chunksize ;
    //printf("%lld, %lld, %lld, %lld\n", filesize, chunksize, filecount, filesize%chunksize);
    for (long long i=0; i < filecount; ++i){
      char strbuf[100]={0}, numbuf[100] = {0};
      strcat(strbuf, output);
      sprintf(numbuf, "%04lld", i + 1);
      strcat(strbuf, numbuf);
      //printf("%s\n", strbuf);
      FILE* outputstream = fopen(strbuf, "wb");
      if(!copy_by_chunks(inputstream, outputstream, chunksize)) {fclose(outputstream); fclose(inputstream); return rs = E_NO_MEMORY;};
      fclose(outputstream);
    }
    if (filesize%chunksize){
            //printf("printed one more\n");

      char strbuf[100]={0}, numbuf[100] = {0};
      strcat(strbuf, output);
      sprintf(numbuf, "%04lld", filecount + 1);
      strcat(strbuf, numbuf);
      //printf("%s\n", strbuf);
      FILE* outputstream = fopen(strbuf, "wb");
      if(!copy_by_chunks(inputstream, outputstream, filesize%chunksize)) {fclose(outputstream); fclose(inputstream); return rs = E_NO_MEMORY;};
      fclose(outputstream);

    }
    
    rs = E_SPLIT_SUCCESS;
    fclose(inputstream);
  }
  if (!strcmp(argv[1], "-j")){
    if (argc < 6) return rs;
    char* output = argv[3];
    FILE* outputstream = fopen(output , "wb");
    if (!outputstream) return rs = E_BAD_DESTINATION;
    for (int i = 5; i < argc; ++i){
      FILE *inputstream = fopen( argv[i], "rb" );
      if (!inputstream) {
        return rs = E_BAD_SOURCE;
        fclose(outputstream);
      } 

      fseek(inputstream, 0, SEEK_END); // seek to end of file
      long long filesize = ftell(inputstream); // get current file pointer
      fseek(inputstream, 0, SEEK_SET); // seek back to beginning of file

      if(!copy_by_chunks(inputstream, outputstream, filesize)) {fclose(outputstream); fclose(inputstream); return rs = E_NO_MEMORY;};
      fclose(inputstream);
    }
    fclose(outputstream);
    rs = E_JOIN_SUCCESS;
  }
  else return rs;
  // parse command-line arguments to determine whether to split a file
  // into smaller files	or to join smaller files into original file ...

  return rs;
}


#ifdef __cplusplus
}
#endif