/*----------------------------------------------------------------------------------------------
 * Collect Random Numbers
 * Copyright (c) 2022, Alphonsus
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
//
// This a header file

const int NUM_OF_FILE_LIMIT = 1; // number of files to be checked, default is only 1

const int BLOCK_SIZE_LIMIT = 1024; //4KB block 4 bytes x 1024
const int COUNT_FOR_FILE_SIZE_LIMIT = 1024*1024;  //limit to 4GB per file
const char DIR_TO_STORE_RAND[200] = "e:\\test\\";
const char DEFAULT_FILENAME_PREFIX[200] = "ssd_size_test";
const char VERSION_NUMBER[10] = "1.0";

int Write_Rand( int number_of_4kb_blocks, int number_of_files_limit, char *drive_to_store_verify );
int Verify_Rand( int number_of_4kb_blocks, int number_of_files_limit, char *drive_to_store_verify );

