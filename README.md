# SHA-256 : Secure Hashing Algorithm
Its purpose is to transform any data (text, file, number) into a unique fixed-size output (called a hash or digest).
1. Preprocessing (Padding)
  - Add a 1 bit to the input.
  - Pad with 0s until the length is 64 bits less than a multiple of 512.
  - Then, append the original input length (in bits) as a 64-bit big-endian number.
Purpose: Make total length a multiple of 512 bits.
2. 
