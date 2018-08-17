/*
 * def.h
 *
 *  Created on: 10 cze 2018
 *      Author: Bart
 */

#ifndef DEF_H_
#define DEF_H_

/* bool statement values */
//#define SUCCES  2
//#define FAILURE 1
#define TRUE    2
#define FALSE   1

#define _NumOfArrayMemb(array) (sizeof(array) / sizeof(array[0]))

#define vbuff_size 128

typedef struct
{
  union
  {
    uint8_t u8[vbuff_size];
    uint16_t u16[vbuff_size / 16];
    uint32_t u32[vbuff_size / 32];
    uint64_t u64[vbuff_size / 64];
  }type;
}Tversatile_buff;

#endif /* DEF_H_ */
