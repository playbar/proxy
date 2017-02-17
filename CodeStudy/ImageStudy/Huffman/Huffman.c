/***********************************************************************************************************
	Huffman.c

本演示程序提供了哈夫曼编码法的压缩和解压缩函数，并实现了对图象
文件的压缩和解压缩
**********************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 函数原型 */
int 	Huffman_Compression(char * infilename, char * outfilename);
int 	Huffman_Decompression(char * infilename, char * outfilename);

/* 内部函数 */
unsigned short  generate_code_table ();
void          build_code_tree ();
void			 build_initial_heap ();
void          compress_image ();
void          get_frequency_count ();
void			build_decomp_tree ();
void			decompress_image ();

/* 全局变量 */
short           father[512];
unsigned short  code[256], heap_length;
unsigned long   compress_charcount, file_size, heap[257];
unsigned char   code_length[256];
long            frequency_count[512];

short           decomp_tree[512];

FILE            *ifile, *ofile;

/* 主程序 */
void main(int argc, char *argv[])
{
	printf("Huffman compression and decompression utility\n");

	if (4 != argc) 
	{
		printf("\nUsage : huffman -c|d sourcefilename targetfilename\n");
		exit(0);
	}

	if (! strcmp(argv[1], "-c"))
	{
		printf("\nCompress...");
		Huffman_Compression(argv[2], argv[3]);
	}
	else if (! strcmp(argv[1], "-d"))
	{
		printf("\nDecompress...");
		Huffman_Decompression(argv[2], argv[3]);
	}
	else
		printf("\nUnknow command.\n");
}

/*********************************************************************************
  Huffman_Compression ()

  用Huffman编码压缩指定的文件，并将结果存为新的文件
 ********************************************************************************/
int Huffman_Compression(char * infilename, char * outfilename)
{
	if ((ifile = fopen (infilename, "rb")) != NULL)
	{
		fseek (ifile, 0L, 2);
		file_size = (unsigned long) ftell (ifile);

		fseek (ifile, 0L, 0);
		get_frequency_count ();

		build_initial_heap ();

		build_code_tree ();

		if (!generate_code_table ())
		{
			printf ("ERROR!  Code Value Out of Range. Cannot Compress.\n");
			return 0;
		}
		else
		{
			if ((ofile = fopen (outfilename, "wb")) != NULL)
			{
				fwrite (&file_size, sizeof (file_size), 1, ofile);
				fwrite (code, 2, 256, ofile);
				fwrite (code_length, 1, 256, ofile);

				fseek (ifile, 0L, 0);
				compress_image ();

				fclose (ofile);
			}
			else
			{
				printf("\nERROR: Couldn't create output file %s\n", outfilename);
				return 0;
			}
		}
		fclose (ifile);
	}
	else
	{
		printf ("\nERROR:  %s -- File not found!\n", infilename);
		return 0;
	}

	return 1;
}


/**************************************************************************
 COMPRESS_IMAGE ()

 本函数进行数据的压缩操作
 **************************************************************************/
void compress_image ()
{
   register unsigned int    thebyte = 0;
   register short           loop1;
   register unsigned short  current_code;
   register unsigned long   loop;

   unsigned short  current_length, dvalue;
   unsigned long   curbyte = 0;
   short           curbit = 7;


   for (loop = 0L; loop < file_size; loop++)
   {
      dvalue         = (unsigned short) getc (ifile);
      current_code   = code[dvalue];
      current_length = (unsigned short) code_length[dvalue];

      for (loop1 = current_length-1; loop1 >= 0; --loop1)
      {
         if ((current_code >> loop1) & 1)
            thebyte |= (char) (1 << curbit);

         if (--curbit < 0)
         {
            putc (thebyte, ofile);
            thebyte = 0;
            curbyte++;
            curbit = 7;
         }
      }
   }
   putc (thebyte, ofile);
   compress_charcount = ++curbyte;
}

/**************************************************************************
 GENERATE_CODE_TABLE ()

 本函数生成压缩码表
 **************************************************************************/
unsigned short  generate_code_table ()
{
   register unsigned short  loop;
   register unsigned short  current_length;
   register unsigned short  current_bit;

   unsigned short  bitcode;
   short           parent;


   for (loop = 0; loop < 256; loop++)
      if (frequency_count[loop])
      {
         current_length = bitcode = 0;
         current_bit = 1;
         parent = father[loop];

         while (parent)
         {
            if (parent < 0)
            {
               bitcode += current_bit;
               parent = -parent;
            }
            parent = father[parent];
            current_bit <<= 1;
            current_length++;
         }

         code[loop] = bitcode;

         if (current_length > 16)
            return (0);
         else
            code_length[loop] = (unsigned char) current_length;
      }
      else
         code[loop] = code_length[loop] = 0;

   return (1);
}


/**************************************************************************
 BUILD_CODE_TREE ()

 建立压缩编码树
 **************************************************************************/
void build_code_tree ()
{
   void    reheap ();

   register unsigned short  findex;
   register unsigned long   heap_value;


   while (heap_length != 1)
   {
      heap_value = heap[1];
      heap[1]    = heap[heap_length--];

      reheap (1);
      findex = heap_length + 255;

      frequency_count[findex] = frequency_count[heap[1]] +
                                frequency_count[heap_value];
      father[heap_value] =  findex;
      father[heap[1]]    = -findex;
      heap[1]            =  findex;

      reheap (1);
   }

   father[256] = 0;
}


/**************************************************************************
 REHEAP ()

 从当前的堆树结构中建立逻辑堆结构
 **************************************************************************/
void reheap (heap_entry)
unsigned short  heap_entry;
{
   register unsigned short  index;
   register unsigned short  flag = 1;

   unsigned long   heap_value;

   heap_value = heap[heap_entry];

   while ((heap_entry <= (heap_length >> 1)) && (flag))
   {
      index = heap_entry << 1;

      if (index < heap_length)
         if (frequency_count[heap[index]] >= frequency_count[heap[index+1]])
            index++;

      if (frequency_count[heap_value] < frequency_count[heap[index]])
	 flag--;
      else
      {
         heap[heap_entry] = heap[index];
         heap_entry       = index;
      }
   }

   heap[heap_entry] = heap_value;
}


/**************************************************************************
 BUILD_INITIAL_HEAP ()

 本函数从初始的频率统计数据中建立堆结构
 **************************************************************************/
void build_initial_heap ()
{
   void    reheap ();

   register unsigned short  loop;


   heap_length = 0;

   for (loop = 0; loop < 256; loop++)
      if (frequency_count[loop])
         heap[++heap_length] = (unsigned long) loop;

   for (loop = heap_length; loop > 0; loop--)
      reheap (loop);
}


/**************************************************************************
 GET_FREQUENCY_COUNT ()

 本函数对需要进行压缩的数据进行频率统计
 **************************************************************************/
void get_frequency_count ()
{
   register unsigned long  loop;


   for (loop = 0; loop < file_size; loop++)
      frequency_count[getc (ifile)]++;
}


/**************************************************************************
 Huffman_Decompression ()

 本函数进行Huffman解码
 **************************************************************************/
int  Huffman_Decompression(char * infilename, char * outfilename)
{
	if ((ifile = fopen (infilename, "rb")) != NULL)
	{
		fread (&file_size, sizeof (file_size), 1, ifile);
		fread (code, 2, 256, ifile);
		fread (code_length, 1, 256, ifile);

		build_decomp_tree ();

		if ((ofile = fopen (outfilename, "wb")) != NULL)
		{
			decompress_image();
			fclose (ofile);
		}
		else
		{
			printf ("\nERROR:  Couldn't create output file %s\n", outfilename);
			return 0;
		}

		fclose (ifile);
	}
	else
	{
		printf ("\nERROR:  %s -- File not found!\n", infilename);
		return 0;
	}

	return 1;
}


/**************************************************************************
 BUILD_DECOMP_TREE ()

 创建解压缩树
 **************************************************************************/
void  build_decomp_tree ()
{
   register unsigned short  loop1;
   register unsigned short  current_index;

   unsigned short  loop;
   unsigned short  current_node = 1;


   decomp_tree[1] = 1;

   for (loop = 0; loop < 256; loop++)
   {
      if (code_length[loop])
      {
	 current_index = 1;
	 for (loop1 = code_length[loop] - 1; loop1 > 0; loop1--)
	 {
	    current_index = (decomp_tree[current_index] << 1) +
			    ((code[loop] >> loop1) & 1);
	    if (!(decomp_tree[current_index]))
	       decomp_tree[current_index] = ++current_node;
	 }
	 decomp_tree[(decomp_tree[current_index] << 1) +
	   (code[loop] & 1)] = -loop;
      }
   }
}


/**************************************************************************
 DECOMPRESS_IMAGE ()

 进行解压缩操作
 **************************************************************************/
void  decompress_image ()
{
   register unsigned short  cindex = 1;
   register char            curchar;
   register short           bitshift;

   unsigned long  charcount = 0L;


   while (charcount < file_size)
   {
      curchar = (char) getc (ifile);

      for (bitshift = 7; bitshift >= 0; --bitshift)
      {
	 cindex = (cindex << 1) + ((curchar >> bitshift) & 1);

	 if (decomp_tree[cindex] <= 0)
	 {
	    putc ((int) (-decomp_tree[cindex]), ofile);

	    if ((++charcount) == file_size)
               bitshift = 0;
            else
               cindex = 1;
	 }
	 else
	    cindex = decomp_tree[cindex];
      }
   }
}
