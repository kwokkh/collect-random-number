# collect-random-number

This project generates random numbers using rand() function that generates 2 bytes and store it into files until the whole drive is full.
It calculates a simple checksum and store it at the end of the file. It is a simple method for checking data integrity.  

Another program reads back the content of files and checks its checksum to be sure that 
the file is not corrupted. It calculates a simple checksum and checks it against the number stored at the end of that file.

Tested on windows 11

# Compile
Uses gcc to compile.
 gcc -o w4kb.exe .\collect_rand_write.c
 gcc -o r4kb.exe .\collect_rand_read.c

# Run
Make sure that the DIR_TO_STORE_RAND or the drive and directory mentioned by "-d" option is available and directory created.

Run the filename of the output from the compile above with relevant options.

Use option -? for help on options
