#ifndef _str_to_int_h_
#define _str_to_int_h_
//-----------------------------------------------------------------------------
#include "../base.h"
#include "types.h"

// >>>>>>>>>>>> str2int v.1.2 (August 2007) >>>>>>>>>>>>
// >>>>
// >>>> usage:
// >>>>   str2int("-1235.890") -> -1235    (DEC, signed)
// >>>>   str2int("-0xFAE37")  -> -1027639 (HEX, signed)
// >>>>   str2int("0b1001110)  ->  78      (BIN, signed)
// >>>>   str2int("053162")    ->  22130   (OCT, signed)
// >>>>   str2int("$FAE37")    ->  1027639 (HEX, signed)
// >>>>
// >>>> new in ver. 1.0.3:
// >>>>   str2int("10M")   -> 10*1024*1024 [M or m == Mega]
// >>>>   str2int("$10k")  -> 16*1024      [k or K == Kilo]
// >>>>   str2int("0x10m") -> 16*1024*1024 [M or m == Mega]


#ifdef __cplusplus
namespace c_base {
extern "C" {
#endif


// set the following value to 1 to interpret "00123" as decimal number
// set the following value to 0 to interpret "00123" as octal number
#define  LEADING_00_IS_DEC_VALUE  0

int          str2int(const char* str);
unsigned int str2uint(const char* str);
unsigned int strbase2uint(const char* str, unsigned int base);
bool_t       is_dec_value(const char* str);

int          str2intw(const wchar_t* strw);
unsigned int str2uintw(const wchar_t* strw);
unsigned int strbase2uintw(const wchar_t* strw, unsigned int base);
bool_t       is_dec_valuew(const wchar_t* strw);

/////////////////////////////////////////////////////////////////////////////
// definitions
/////////////////////////////////////////////////////////////////////////////
#ifdef UNICODE
  #define _tstr2int      str2intw
  #define _tstr2uint     str2uintw
  #define _tstrbase2uint strbase2uintw
  #define _tis_dec_value is_dec_valuew
#else
  #define _tstr2int      str2int
  #define _tstr2uint     str2uint
  #define _tstrbase2uint strbase2uint
  #define _tis_dec_value is_dec_value
#endif // !UNICODE


#ifdef __cplusplus
}
}
#endif


//-----------------------------------------------------------------------------
#endif
