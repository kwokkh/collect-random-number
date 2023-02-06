/** --------------------------------------------------------------------------------------------
 * Collect Random Numbers
 * Copyright (c) 2022, Alphonsus KKH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *     * Neither the name of Collect Random Numbers nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------------------------*/
/// \details  This file generates random numbers using rand() function that generates 2 bytes
///           and store it into files until the whole drive is full.
///           It calculates a simple checksum and store it at the end of the file.
///           It is a simple method for checking data integrity.
///
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "collect_rand.h"

struct arguments
{
  enum
  {
    STORE,
    VERIFY,
    STORE_VERIFY,
    NONE
  } collect_mode;
  int num_of_4kb_blocks;  // = COUNT_FOR_FILE_SIZE_LIMIT;
  int num_of_files_limit; // = NUM_OF_FILE_LIMIT;
  char drive_dir[200];
};

//-------------------------------------------------------------------------------------------------------
// prog entry point
//-------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  int opt;
  struct arguments arguments;
  arguments.num_of_4kb_blocks = COUNT_FOR_FILE_SIZE_LIMIT;
  arguments.num_of_files_limit = NUM_OF_FILE_LIMIT;
  arguments.collect_mode = NONE;
  strcpy(arguments.drive_dir, DIR_TO_STORE_RAND);

  // parse arg passed into this prog
  while ((opt = getopt(argc, argv, "svb:n:d:h?")) != -1)
  {
    switch (opt)
    {
    case 's':
    {
      if (arguments.collect_mode == VERIFY)
      {
        arguments.collect_mode = STORE_VERIFY;
        printf("To generate, store and verify\n");
      }
      else
      {
        arguments.collect_mode = STORE;
        printf("To generate and store only\n");
      }
      break;
    }
    case 'v':
    {
      if (arguments.collect_mode == STORE)
      {
        arguments.collect_mode = STORE_VERIFY;
        printf("To generate, store and verify\n");
      }
      else
      {
        arguments.collect_mode = VERIFY;
        printf("To verify only\n");
      }
      break;
    }
    case 'b':
      arguments.num_of_4kb_blocks = atoi(optarg);
      printf("Num of 4kb blocks = %d\n", arguments.num_of_4kb_blocks);
      break;
    case 'n':
      arguments.num_of_files_limit = atoi(optarg);
      printf("Num of files = %d\n", arguments.num_of_files_limit);
      break;
    case 'd':
      strcpy(arguments.drive_dir, optarg);
      printf("Drive and dir = %s\n", arguments.drive_dir);
      break;
    case 'h':
    case '?':
      printf("version %s\n", VERSION_NUMBER);
      printf("Usage progname [-s] [-v] [-b #] [-n #]\n");
      printf("-s generate rand and store them.\n");
      printf("-v verify contents in file.\n");
      printf("-b number of 4kb blocks in file.\n");
      printf("-n number of files.\n");
      printf("-d drive and directory to store verify data\n");
      break;
    default:
      arguments.collect_mode = STORE;
      printf("To generate and store only\n");
    }
  }

  // run the prog according to the passed in args
  switch (arguments.collect_mode)
  {
  case STORE:
    printf("---------------- Generate random numbers and store ----------------\n");
    Write_Rand(arguments.num_of_4kb_blocks, arguments.num_of_files_limit, arguments.drive_dir);
    break;
  case STORE_VERIFY:
    printf("---------------- Generate random numbers, store and verify ----------------\n");
    Write_Rand(arguments.num_of_4kb_blocks, arguments.num_of_files_limit, arguments.drive_dir);
  case VERIFY:
    printf("---------------- verify contents ----------------------------------\n");
    Verify_Rand(arguments.num_of_4kb_blocks, arguments.num_of_files_limit, arguments.drive_dir);
    break;
  default:
    printf("Version %s\n", VERSION_NUMBER);
    printf("Usage progname [-s] [-v] [-b #] [-n #]\n");
    printf("-s generate rand and store them.\n");
    printf("-v verify contents in file.\n");
    printf("-b number of 4kb blocks in file.\n");
    printf("-n number of files.\n");
    printf("-d drive and directory to store verify data\n");
    break;
  }
};

//-------------------------------------------------------------------------------------------------------
//
// Function name : Write_Rand
// Description   : 
///     \brief     This function generates random numbers and store them in files
//
// Parameters:  
///    \param  number_of_4kb_blocks   int   - the number of 4kb blocks to write to the file
///    \param  number_of_files_limit  int   - the number of files to create
///    \param  drive_to_store_verify  char* - the pointer to the string with the drive and directory info
//
/// \return:   0 - no issue
/// \return:  -1 - there is error
//
//-------------------------------------------------------------------------------------------------------
int Write_Rand(int number_of_4kb_blocks, int number_of_files_limit, char *drive_to_store_verify)
{
  // creating a FILE variable
  FILE *file_rand_store_pointer;

  // check sum
  int checksum;

  // counter
  int file_size_counter;
  int number_of_files;
  char strbuf[200] = "\0";
  char sfilename[100] = "\0";
  char sfilenumber[100] = "\0";
  int array_of_random_numbers[BLOCK_SIZE_LIMIT];
  const size_t ARRAY_SIZE = sizeof(array_of_random_numbers); // size of the array
  //------------------------------------------------------
  // initialize variables
  srand(time(NULL)); // Initialization, should only be called once.
  checksum = 0;
  file_size_counter = 0;
  number_of_files = 0;

  printf("size of array = %d \n", ARRAY_SIZE);
  printf("expected file size = %lld \n", (ARRAY_SIZE * number_of_4kb_blocks));
  memset(sfilenumber, '\0', sizeof(sfilenumber));
  //--------------------------------------------------------

  while (number_of_files < number_of_files_limit)
  {
    // generate filename
    strbuf[0] = '\0'; // null the string
    strcat(strbuf, drive_to_store_verify);
    strcat(strbuf, DEFAULT_FILENAME_PREFIX);
    sprintf(sfilenumber, "%d", number_of_files);
    strcat(strbuf, sfilenumber);
    strcat(strbuf, ".bin");

    // open the file in write mode
    file_rand_store_pointer = fopen(strbuf, "wb");

    if (file_rand_store_pointer != NULL)
    {
      printf("filename %s created \033[33msuccessfully\033[0m!\n", strbuf);
    }
    else
    {
      printf("\033[31mFailed\033[0m to create the file %s number %d.\n", strbuf, number_of_files);
      printf("checksum = %d \n", checksum);
      // exit status for OS that an error occurred
      return -1;
    }

    file_size_counter = 0; // reset counter
    while (file_size_counter < number_of_4kb_blocks)
    {
      for (int j = 0; j < BLOCK_SIZE_LIMIT; j++)
      {
        array_of_random_numbers[j] = (rand() << 16) + rand(); // generate 4 bytes of random numbers (assuming int is 4 bytes)
        checksum = checksum + array_of_random_numbers[j];     // calculate checksum for this block
      }

      file_size_counter++;
      if (fwrite(array_of_random_numbers, ARRAY_SIZE, 1, file_rand_store_pointer) != 1)
      {
        printf("\033[31mFailed\033[0m to write to file %s file_size_counter %d\n", strbuf, file_size_counter);
        printf("checksum = 0x%x \n", checksum);
        number_of_files = number_of_files_limit; // break outter loop
        break;
      }
    }
    // write checksum to the end of the file
    if (fwrite(&checksum, sizeof(checksum), 1, file_rand_store_pointer) != 1)
    {
      printf("\033[31mFailed\033[0m to write CS to file %s \n", strbuf);
      printf("checksum = %d \n", checksum);
    }
    else
    {
      printf("checksum = 0x%x \n", checksum);
    }

    // close thie file connection
    fclose(file_rand_store_pointer);
    printf("filename %s closed. \n", strbuf);
    checksum = 0; // init checksum for next file
    number_of_files++;
  }

  return 0;
}

//-------------------------------------------------------------------------------------------------------
//
// Function name : Verify_Rand
// Description   : 
///     @brief     This function verify the contents in the files against a checksum
//
// Parameters:  
///    @param   number_of_4kb_blocks   int   - the number of 4kb blocks written to the file
///    @param   number_of_files_limit  int   - the number of files to verify
///    @param   drive_to_store_verify  char* - the pointer to the string with the drive and directory info
//
/// @return:   0 - no issue
/// @return:  -1 - there is error
//
//-------------------------------------------------------------------------------------------------------
int Verify_Rand(int number_of_4kb_blocks, int number_of_files_limit, char *drive_to_store_verify)
{
  // creating a FILE variable
  FILE *file_rand_store_pointer;

  // check sum
  int checksum;
  int checksum_from_file;

  // counter
  int file_size_counter;
  int number_of_files;
  char strbuf[200] = "\0";
  char sfilename[100] = "\0";
  char sfilenumber[100] = "\0";
  int array_of_random_numbers[BLOCK_SIZE_LIMIT];
  const size_t ARRAY_SIZE = sizeof(array_of_random_numbers); // size of the array
  //------------------------------------------------------
  // init
  srand(time(NULL)); // Initialization, should only be called once.
  checksum = 0;
  checksum_from_file = 0;
  file_size_counter = 0;
  number_of_files = 0;

  printf("size of array = %d \n", ARRAY_SIZE);
  printf("expected file size = %lld \n", (ARRAY_SIZE * number_of_4kb_blocks));
  memset(sfilenumber, '\0', sizeof(sfilenumber));
  //--------------------------------------------------------

  while (number_of_files < number_of_files_limit)
  {
    // gen filename
    strbuf[0] = '\0'; // null the string
    strcat(strbuf, drive_to_store_verify);
    strcat(strbuf, DEFAULT_FILENAME_PREFIX);
    sprintf(sfilenumber, "%d", number_of_files);
    strcat(strbuf, sfilenumber);
    strcat(strbuf, ".bin");

    // open the file in read binary mode
    file_rand_store_pointer = fopen(strbuf, "rb");

    if (file_rand_store_pointer != NULL)
    {
      printf("filename %s opened \033[33msuccessfully\033[0m!\n", strbuf);
    }
    else
    {
      printf("\033[31mFailed\033[0m to open the file %s.\n", strbuf);
      // exit status for OS that an error occurred
      return -1;
    }

    file_size_counter = 0; // reset counter
    while (file_size_counter < number_of_4kb_blocks)
    {

      file_size_counter++;
      if (fread(array_of_random_numbers, ARRAY_SIZE, 1, file_rand_store_pointer) != 1)
      {
        printf("\033[31mFailed\033[0m to read from file %s \n", strbuf);
        number_of_files = number_of_files_limit; // break outter loop
        break;
      }

      for (int j = 0; j < BLOCK_SIZE_LIMIT; j++)
      {
        checksum = checksum + array_of_random_numbers[j]; // calculate checksum
      }
    }

    if (fread(&checksum_from_file, sizeof(checksum_from_file), 1, file_rand_store_pointer) != 1)
    {
      printf("\033[31mFailed\033[0m to read CS from file %s \n", strbuf);
      number_of_files = number_of_files_limit; // break outter loop
    }
    else
    {
      if (checksum_from_file == checksum)
      {
        printf("Filename %s, checksum 0x%x is \033[32mCORRECT\033[0m.\n", strbuf, checksum);
      }
      else
      {
        printf("File %s, checksum \033[31mERROR!!!\033[0m\n", strbuf);
      }
    }

    // close connection
    fclose(file_rand_store_pointer);
    printf("filename %s closed. \n", strbuf);
    checksum = 0; // init checksum for next file
    number_of_files++;
  }

  return 0;
}