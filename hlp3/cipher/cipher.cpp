
#define GRP1 2
#define  GRP2  6
#define  GRP3  14
#define  GRP4  26
#include <bitset>

/* 
   Encoding and decoding for 26 characters of Latin alphabet. 
   Instead of using stadard ASCII which uses 8 bits per character, we will 
   use the following mapping: 
   
   note each character is encoded by 2 bits [for the group] plus 
	 1 or 2 or 3 or 4 bits [for the specific character in the group]:

   0  a <-> 0     Group 0 (00) requires only 1 bit to encode 1 character
   1  b <-> 1     example: b is encoded as 001 [00 specifies group, 1 is index inside the group]
   ----------
   2  c <-> 00    Group 1 (01) requires only 2 bits to encode 1 character
   3  d <-> 01
   4  e <-> 10    example: e is encoded as 0110 [01 specifies group, 10 is index inside the group]
   5  f <-> 11
   ----------
   6  g <-> 000   Group 2 (10) requires only 3 bits to encode 1 character
   7  h <-> 001
   8  i <-> 010
   9  j <-> 011   example: j is encoded as 10011 [10 specifies group, 011 is index inside the group]
   10 k <-> 100
   11 l <-> 101
   12 m <-> 110
   13 n <-> 111
   ----------
   14 o <-> 0000  Group 3 (11) requires only 4 bits to encode 1 character
   15 p <-> 0001
   16 q <-> 0010
   17 r <-> 0011
   18 s <-> 0100
   19 t <-> 0101  t is encoded as 110101 [11 specifies group, 0101 is index inside the group]
   20 u <-> 0110
   21 v <-> 0111
   22 w <-> 1000
   23 x <-> 1001
   24 y <-> 1010
   25 z <-> 1011
*/

/*
   When writing bits into encrypted text start at least significant bit (random choice, but
   that's how decoder will be reading)
   To write 01 into encrypted text
   7 6 5 4 3 2 1 0  <- bit positions
   - - - - - - 0 1
   to write a 'j' into encrypted text
   first write 10
   7 6 5 4 3 2 1 0  <- bit positions
   - - - - - - 1 0
   then 011
   7 6 5 4 3 2 1 0  <- bit positions
   - - - 1 1 0 1 0

   Note that you write bits without padding, so encrypted character
   may span 2 bytes (start in byte X and end in byte X+1)

   There will be unused bits in the end of the encrypted text, those
   should never be read, so may be garbage.

   Example: encode text "cccc"
   c - group 01, position in group 00, so 
   7 6 5 4 3 2 1 0  <- bit positions
           0 0 0 1 
   4 c's produce 16 bits (2 bytes)
   0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1
   if you write them into a file and "od -x" you will see
   1111 - 4 hexadecimal 1's, each 0x1 = 0001.
*/


/*
To decipher string, first read 2 bits that represent the group, i.e. 
how many bits to read during the second step
 b1b2 -> how many bits to read
 if 
 00 -> read 1 bit  next, character with be    a or b
 01 -> read 2 bits next, character with be in range c-f
 10 -> read 3 bits next, character with be in range g-n
 11 -> read 4 bits next, character with be in range o-z

 example 
 10110, 10 - read next 3 bits: 110 - 'm'
 0110 , 01 - read next 2 bits: 10  - 'e'
 000  , 00 - read next 1 bit : 0   - 'a'
 notice illegal sequences 1111..: 11 means read 4 bits, 11.. - there are no characters have representation 11..
 */

#include <iostream>
#include <cstdint>
#include "cipher.h"

void print_bit(char v, int32_t pos) {
  char one = 1;
  if ( v & one<<pos ) { std::cout << "1"; } else { std::cout << "0"; }
}

void print_bits(char* buffer, int32_t start_pos, int32_t how_many) {
  std::cout << "Bits: ";
  for (int32_t i{}; i < how_many ;) { // for each char
    char *ch = buffer + (start_pos+i)/8;
    for (int32_t j{}; j<8 && i<how_many; ++j, ++i ) { // from more significant to less
      print_bit( *ch, j );
    }
  }
  std::cout << '\n';
}

int32_t read_bit(char const* buffer, int32_t i) {
  char const * ch = buffer + i/8;
  int32_t pos  = i%8;
  return (*ch & 1<<pos) ? 1:0;
}

// decode num_chars characters from ciphertext
// function assumes output C-style string is allocated by someone else and big enough
void decode(char const* ciphertext, int32_t num_chars, char* plaintext) {
  int32_t pos{};
  for(int32_t i{}; i < num_chars; ++i ) {
    // read 2 bits for group (00,01,10,11)
    int32_t group_index = read_bit( ciphertext, pos ) + 2*read_bit( ciphertext, pos+1 );
    int32_t index{}; // index inside group
    pos += 2;

    for(int32_t j{}; j < group_index+1; ++j ) {
      index += (read_bit(ciphertext, pos) << j);
      ++pos;
    }
    plaintext[i] = 'a' + ( (1<<(group_index+1)) -2) + index;
  }
  plaintext[num_chars] = 0; // null terminate final result
}
// assume all lower case input a-z (no spaces)
void encode(char const * plaintext, char* encryptedtext, int32_t *num_bits_used ) {
  *num_bits_used = 0;
	int i {0};

  while (*(plaintext + i)){
      int idx { static_cast<int>(*(plaintext + i)) - static_cast<int>('a')};
      int bits{0}, leftshift {0};
      if (idx < GRP1){
        bits |= (0 << leftshift);
        leftshift += 2;
        bits |= (idx << leftshift);
        leftshift += 1;
      }else if (idx < GRP2){
        idx -= static_cast<int>('c' - 'a');
        bits |= (1 << leftshift);
        leftshift += 2;
        bits |= (idx << leftshift);
        leftshift += 2;
      }else if (idx < GRP3){
        idx -= static_cast<int>('g' - 'a');
        bits |= (2 << leftshift);
        leftshift += 2;
        bits |= (idx << leftshift);
        leftshift += 3;
      }else if (idx < GRP4){
        idx -= static_cast<int>('o' - 'a');
        bits |= (3 << leftshift);
        leftshift += 2;
        bits |= (idx << leftshift);
        leftshift += 4;
      }
      //std::cerr << bits << " " << leftshift << " \n";

      // std::cerr << bits << std::endl;
      for (int j{}; j < leftshift; ++j){
        bool bit { static_cast<bool>(bits & (1 << j)) };
        
        // std::cerr << bit << " " << (1 << j) << std::endl;
        // std::cerr << *num_bits_used /8 << " " << *num_bits_used % 8 << std::endl;
        encryptedtext[(*num_bits_used) / 8] |= (static_cast<int>(bit) << ((*num_bits_used) % 8));
        *num_bits_used += 1;
      }
      //*num_bits_used += leftshift;
      i++;
  }
  //std::cout << "bitssssss" <<*num_bits_used << std::endl;
}
