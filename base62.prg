REQUEST BASE62

PROCEDURE Main()
   LOCAL c := "1234567890"
   LOCAL encoded := HB_BASE62ENCODE( c )
   LOCAL decoded := HB_BASE62DECODE( encoded )

   ? "原始資料:", c
   ? "編碼結果:", encoded
   ? "解碼結果:", decoded
RETURN
