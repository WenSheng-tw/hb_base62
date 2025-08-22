#include "hbapi.h"

static const char s_base62chars[] =
   "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static int base62_decode_char( char c )
{
   if( c >= '0' && c <= '9' ) return c - '0';
   if( c >= 'A' && c <= 'Z' ) return c - 'A' + 10;
   if( c >= 'a' && c <= 'z' ) return c - 'a' + 36;
   return -1;
}

HB_FUNC( HB_BASE62ENCODE )
{
   HB_SIZE nLen = hb_parclen( 1 );
   const unsigned char * pSrc = ( const unsigned char * ) hb_parc( 1 );
   HB_SIZE i;
   char temp[256];
   int pos;
   char * pOut;
   unsigned char lenHi, lenLo;
   unsigned long value[64]; // 模擬大整數（base256）

   if( nLen == 0 || pSrc == NULL )
   {
      hb_retc_null();
      return;
   }

   // 初始化 value[] 為 0
   for( i = 0; i < 64; ++i )
      value[i] = 0;

   // 將 byte array 轉成 base256 整數陣列
   for( i = 0; i < nLen; ++i )
   {
      int carry = pSrc[i];
      int j;
      for( j = 0; j < 64; ++j )
      {
         carry += value[j] << 8;
         value[j] = carry % 62;
         carry /= 62;
      }
   }

   // 轉成 base62 字串
   pos = 0;
   for( i = 63; i != ( HB_SIZE ) -1; --i )
   {
      if( pos == 0 && value[i] == 0 )
         continue;
      temp[pos++] = s_base62chars[value[i]];
   }

   // 加入長度 prefix（2 chars）
   lenHi = s_base62chars[nLen / 62];
   lenLo = s_base62chars[nLen % 62];

   pOut = ( char * ) hb_xgrab( pos + 2 );
   pOut[0] = lenHi;
   pOut[1] = lenLo;
   for( i = 0; i < ( HB_SIZE ) pos; ++i )
      pOut[i + 2] = temp[i];

   hb_retclen_buffer( pOut, pos + 2 );
}

HB_FUNC( HB_BASE62DECODE )
{
   HB_SIZE nLen = hb_parclen( 1 );
   const char * pSrc = hb_parc( 1 );
   HB_SIZE i;
   int lenHi, lenLo, dataLen;
   unsigned char * pOut;
   unsigned long value[64]; // base62 整數陣列

   if( nLen < 3 || pSrc == NULL )
   {
      hb_retc_null();
      return;
   }

   lenHi = base62_decode_char( pSrc[0] );
   lenLo = base62_decode_char( pSrc[1] );
   if( lenHi < 0 || lenLo < 0 )
   {
      hb_retc_null();
      return;
   }

   dataLen = lenHi * 62 + lenLo;
   if( dataLen <= 0 || dataLen > 128 )
   {
      hb_retc_null();
      return;
   }

   // 初始化 value[] 為 0
   for( i = 0; i < 64; ++i )
      value[i] = 0;

   // 將 base62 字串轉成整數陣列
   for( i = 2; i < nLen; ++i )
   {
      int digit = base62_decode_char( pSrc[i] );
      int j;
      if( digit < 0 )
      {
         hb_retc_null();
         return;
      }

      for( j = 0; j < 64; ++j )
      {
         digit += value[j] * 62;
         value[j] = digit & 0xFF;
         digit >>= 8;
      }
   }

   // 轉回 byte array（反向）
   pOut = ( unsigned char * ) hb_xgrab( dataLen );
   for( i = 0; i < ( HB_SIZE ) dataLen; ++i )
      pOut[dataLen - i - 1] = ( unsigned char ) value[i];

   hb_retclen_buffer( ( char * ) pOut, dataLen );
}
