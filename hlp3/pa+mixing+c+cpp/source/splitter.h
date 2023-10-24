/**
 * @file splitter.h
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief polyglot header
 * @version 0.1
 * @date 2023-09-06
 * @param assignment 1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SPLITTER_H
#define SPLITTER_H

// this header file must be amended to make both splitter.cpp AND splitter.c be compatible with
// both driver.c AND driver.cpp
// in other words, there will be a single splitter.h that will be used with driver.c, driver.cpp, splitter.c, and splitter.cpp

#include <stddef.h> // size_t definition

typedef enum {
  E_BAD_SOURCE = 1,
  E_BAD_DESTINATION,
  E_NO_MEMORY,
  E_SMALL_SIZE,
  E_NO_ACTION,
  E_SPLIT_SUCCESS,
  E_JOIN_SUCCESS
} SplitResult;

// provide function header documentation using Doxygen tags to explain to
// your client how they must use this function to either split big file into
// smaller files or join [previously split] smaller files into big file ...
#ifdef __cplusplus
extern "C" {
#endif

SplitResult split_join(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif


#endif // end of #ifndef SPLITTER_H
