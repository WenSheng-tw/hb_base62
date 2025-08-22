# hb_base62
harbour support base62 encode and decode
# Harbour Base62 模組

提供高壓縮率、可逆的 Base62 編碼/解碼函數，與 JavaScript 的 base62 實作完全相容。

## 函數

- `HB_BASE62ENCODE( <cBinary> ) → <cBase62>`
- `HB_BASE62DECODE( <cBase62> ) → <cBinary>`

## 編譯方式

```bash
hbmk2 base62.hbp
