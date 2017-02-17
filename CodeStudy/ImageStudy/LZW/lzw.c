/***********************************************************************************************************
	LZW.c

本演示程序提供了LZW编码法的压缩和解压缩函数，并实现了对图象
文件的压缩和解压缩
**********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BITS 12                   /* Setting the number of bits to 12, 13*/
#define HASHING_SHIFT BITS-8      /* or 14 affects several constants.    */
#define MAX_VALUE (1 << BITS) - 1 /* Note that MS-DOS machines need to   */
#define MAX_CODE MAX_VALUE - 1    /* compile their code in large model if*/
                                  /* 14 bits are selected.               */
#if BITS == 14
  #define TABLE_SIZE 18041        /* The string table size needs to be a */
#endif                            /* prime number that is somewhat larger*/
#if BITS == 13                    /* than 2**BITS.                       */
  #define TABLE_SIZE 9029
#endif
#if BITS <= 12
  #define TABLE_SIZE 5021
#endif

/* 函数原型 */
int LZW_Compression(char *in_filename, char *out_filename);
int LZW_Decompression(char *in_filename, char *out_filename);

/* 内部函数 */
int find_match(int hash_prefix,unsigned int hash_character);
char *decode_string(unsigned char *buffer,unsigned int code);
unsigned int input_code(FILE *input);
void output_code(FILE *output,unsigned int code);

/* 全局变量，编码/解码使用的内存缓冲区 */
int *code_value;                  /* This is the code value array        */
unsigned int *prefix_code;        /* This array holds the prefix codes   */
unsigned char *append_character;  /* This array holds the appended chars */
unsigned char decode_stack[4000]; /* This array holds the decoded string */


/* 主程序 */
void main(int argc, char *argv[])
{
	printf("LZW compression and decompression utility\n");

	if (4 != argc) 
	{
		printf("\nUsage : lzw -c|d sourcefilename targetfilename\n");
		exit(0);
	}

	if (! strcmp(argv[1], "-c"))
		LZW_Compression(argv[2], argv[3]);
	else if (! strcmp(argv[1], "-d"))
		LZW_Decompression(argv[2], argv[3]);
	else
		printf("\nUnknow command.\n");
}

/**************************************************************************************************
	LZW_Compression()

  本函数用LZW算法压缩指定的文件，并将结构存储到新的文件中                                     
***************************************************************************************************/
int LZW_Compression(char *in_filename, char *out_filename)
{
  unsigned int next_code;
  unsigned int character;
  unsigned int string_code;
  unsigned int index;
  int i;
  FILE *input;
  FILE *output;

  /* allocate memory for compression */
  code_value=malloc(TABLE_SIZE*sizeof(unsigned int));
  prefix_code=malloc(TABLE_SIZE*sizeof(unsigned int));
  append_character=malloc(TABLE_SIZE*sizeof(unsigned char));
  if (code_value==NULL || prefix_code==NULL || append_character==NULL)
  {
    printf("Fatal error allocating table space!\n");
    return 0;
  }

  /* open files */
  input=fopen(in_filename,"rb");
  output=fopen(out_filename,"wb");
  if (input==NULL || output==NULL)
  {
    printf("Fatal error opening files.\n");
    return 0;
  };

  /* compressing... */

  next_code=256;              /* Next code is the next available string code*/
  for (i=0;i<TABLE_SIZE;i++)  /* Clear out the string table before starting */
    code_value[i]=-1;

  i=0;
  printf("Compressing...\n");
  string_code=getc(input);    /* Get the first code                         */
/*
** This is the main loop where it all happens.  This loop runs util all of
** the input has been exhausted.  Note that it stops adding codes to the
** table after all of the possible codes have been defined.
*/
  while ((character=getc(input)) != (unsigned)EOF)
  {
    if (++i==1000)                         /* Print a * every 1000    */
    {                                      /* input characters.  This */
      i=0;                                 /* is just a pacifier.     */
      printf(".");
    }
    index=find_match(string_code,character);/* See if the string is in */
    if (code_value[index] != -1)            /* the table.  If it is,   */
      string_code=code_value[index];        /* get the code value.  If */
    else                                    /* the string is not in the*/
    {                                       /* table, try to add it.   */
      if (next_code <= MAX_CODE)
      {
        code_value[index]=next_code++;
        prefix_code[index]=string_code;
        append_character[index]=character;
      }
      output_code(output,string_code);  /* When a string is found  */
      string_code=character;            /* that is not in the table*/
    }                                   /* I output the last string*/
  }                                     /* after adding the new one*/
/*
** End of the main loop.
*/
  output_code(output,string_code); /* Output the last code               */
  output_code(output,MAX_VALUE);   /* Output the end of buffer code      */
  output_code(output,0);           /* This code flushes the output buffer*/
  printf("\n");

  /* cleanup... */
  fclose(input);
  fclose(output);

  free(code_value);
  free(prefix_code);
  free(append_character);

  return 1;
}

/*
** This is the hashing routine.  It tries to find a match for the prefix+char
** string in the string table.  If it finds it, the index is returned.  If
** the string is not found, the first available index in the string table is
** returned instead.
*/

int find_match(int hash_prefix,unsigned int hash_character)
{
int index;
int offset;

  index = (hash_character << HASHING_SHIFT) ^ hash_prefix;
  if (index == 0)
    offset = 1;
  else
    offset = TABLE_SIZE - index;
  while (1)
  {
    if (code_value[index] == -1)
      return(index);
    if ((int)prefix_code[index] == hash_prefix && 
        append_character[index] == hash_character)
      return(index);
    index -= offset;
    if (index < 0)
      index += TABLE_SIZE;
  }
}

/*******************************************************************
	LZW_Decompression()

  用LZW对文件进行解码                                     
********************************************************************/
int LZW_Decompression(char *in_filename, char *out_filename)
{
  unsigned int next_code;
  unsigned int new_code;
  unsigned int old_code;
  int character;
  int counter;
  unsigned char *string;
  FILE *input;
  FILE *output;

  /* allocate memory for decompression */
  prefix_code=malloc(TABLE_SIZE*sizeof(unsigned int));
  append_character=malloc(TABLE_SIZE*sizeof(unsigned char));
  if (prefix_code==NULL || append_character==NULL)
  {
    printf("Fatal error allocating table space!\n");
    return 0;
  }

  /* open files */
  input=fopen(in_filename,"rb");
  output=fopen(out_filename,"wb");
  if (input==NULL || output==NULL)
  {
    printf("Fatal error opening files.\n");
    return 0;
  };

  /* decompress... */

  next_code=256;           /* This is the next available code to define */
  counter=0;               /* Counter is used as a pacifier.            */
  printf("Decompress...\n");

  old_code=input_code(input);  /* Read in the first code, initialize the */
  character=old_code;          /* character variable, and send the first */
  putc(old_code,output);       /* code to the output file                */
/*
**  This is the main expansion loop.  It reads in characters from the LZW file
**  until it sees the special code used to inidicate the end of the data.
*/
  while ((new_code=input_code(input)) != (MAX_VALUE))
  {
    if (++counter==1000)   /* This section of code prints out     */
    {                      /* an asterisk every 1000 characters   */
      counter=0;           /* It is just a pacifier.              */
      printf(".");
    }
/*
** This code checks for the special STRING+CHARACTER+STRING+CHARACTER+STRING
** case which generates an undefined code.  It handles it by decoding
** the last code, and adding a single character to the end of the decode string.
*/
    if (new_code>=next_code)
    {
      *decode_stack=character;
      string=decode_string(decode_stack+1,old_code);
    }
/*
** Otherwise we do a straight decode of the new code.
*/
    else
      string=decode_string(decode_stack,new_code);
/*
** Now we output the decoded string in reverse order.
*/
    character=*string;
    while (string >= decode_stack)
      putc(*string--,output);
/*
** Finally, if possible, add a new code to the string table.
*/
    if (next_code <= MAX_CODE)
    {
      prefix_code[next_code]=old_code;
      append_character[next_code]=character;
      next_code++;
    }
    old_code=new_code;
  }
  printf("\n");

  /* cleanup... */
  fclose(input);
  fclose(output);

  free(prefix_code);
  free(append_character);

  return 1;
}

/*
** This routine simply decodes a string from the string table, storing
** it in a buffer.  The buffer can then be output in reverse order by
** the expansion program.
*/

char *decode_string(unsigned char *buffer,unsigned int code)
{
int i;

  i=0;
  while (code > 255)
  {
    *buffer++ = append_character[code];
    code=prefix_code[code];
    if (i++>=4094)
    {
      printf("Fatal error during code expansion.\n");
      exit(0);
    }
  }
  *buffer=code;
  return(buffer);
}

/*
** The following two routines are used to output variable length
** codes.  They are written strictly for clarity, and are not
** particularyl efficient.
*/

unsigned int input_code(FILE *input)
{
unsigned int return_value;
static int input_bit_count=0;
static unsigned long input_bit_buffer=0L;

  while (input_bit_count <= 24)
  {
    input_bit_buffer |= 
        (unsigned long) getc(input) << (24-input_bit_count);
    input_bit_count += 8;
  }
  return_value=input_bit_buffer >> (32-BITS);
  input_bit_buffer <<= BITS;
  input_bit_count -= BITS;
  return(return_value);
}

void output_code(FILE *output,unsigned int code)
{
static int output_bit_count=0;
static unsigned long output_bit_buffer=0L;

  output_bit_buffer |= (unsigned long) code << (32-BITS-output_bit_count);
  output_bit_count += BITS;
  while (output_bit_count >= 8)
  {
    putc(output_bit_buffer >> 24,output);
    output_bit_buffer <<= 8;
    output_bit_count -= 8;
  }
}

