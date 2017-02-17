/***********************************************************************************************************
	RLE.c

本演示程序提供了游程长度编码法的压缩和解压缩函数，并实现了对图象
文件的压缩和解压缩
**********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN          (0x7f)       /* maximum length for runs or sequences    */
#define MAX_RUN_HEADER  (0xff)       /* bit 7 == 1 : run follows                */
                                  /* bit 6 - 0  : length of run              */
#define MAX_SEQ_HEADER (0x7f)         /* bit 7 == 0 : unencode sequence follows  */
                                  /* bit 6 - 0  : length of sequence         */
#define RUN (0x80)                    /* bit 7 == 1 : run follows                */
#define SEQ (0x00)                    /* bit 7 == 0 : unencoded sequence follows */

/* 函数原型 */
int RLE_Compression(char * infile_name, char * outfile_name);
int RLE_Decompression(char * infile_name, char * outfile_name);

/* 主程序 */
void main(int argc, char *argv[])
{
	printf("RLE compression and decompression utility\n");

	if (4 != argc) 
	{
		printf("\nUsage : rle -c|d sourcefilename targetfilename\n");
		exit(0);
	}

	if (! strcmp(argv[1], "-c"))
	{
		printf("\nCompress...");
		RLE_Compression(argv[2], argv[3]);
	}
	else if (! strcmp(argv[1], "-d"))
	{
		printf("\nDecompress...");
		RLE_Decompression(argv[2], argv[3]);
	}
	else
		printf("\nUnknow command.\n");
}

/**************************************************************************
 RLE_Compression ()

 本函数用RLE算法对文件进行压缩
 **************************************************************************/
int RLE_Compression(char * infile_name, char * outfile_name)
{
    register int cur_char;                /* a character                    */
    register unsigned int i;              /* generic index variable         */
    register unsigned short run_len = 0;  /* length of character run so far */
    int run_char;                         /* which char run is of           */
    unsigned int j;                       /* another index variable         */
    unsigned short seq_len=0;             /* length of non-run sequence     */

    char scratch_space[256];              /* string scratch space           */
    char seq[MAX_LEN];                    /* buffer for uncompressible data */

    FILE *infile;                           /* file ptr to input file (uncompressed)    */
    FILE *outfile;                          /* file ptr to output file (compressed)     */


    if ((infile=fopen(infile_name, "rb")) == NULL)
    {
        strcpy(scratch_space, "Uable to open ");
        strcat(scratch_space, infile_name);
        puts(scratch_space);
        return 1;
    }

    if ((outfile=fopen(outfile_name, "wb")) == NULL)
    {
        strcpy(scratch_space, "Uable to open ");
        strcat(scratch_space, outfile_name);
        puts(scratch_space);
        return 1;
    }

    while (!feof(infile))
    {
        cur_char = fgetc(infile);

        if (feof(infile))
            continue;

        if (seq_len ==0)                /* haven't got a sequence yet   */
        {
            if (run_len == 0)           /* start a new run              */
            {
                run_char = cur_char;
                ++run_len;
                continue;
            }

            if (run_char == cur_char)   /* got another char in the run  */
                if (++run_len == MAX_LEN)
                {
                    fputc((int)MAX_RUN_HEADER, outfile);
                    fputc((int) run_char, outfile);
                    run_len = 0;
                    continue;
                }

                                   /* got a different character     */
                                   /* than the run we were building */
            if (run_len > 2)       /* so write out the run and      */
                                   /* start a new one of the new    */
                                   /* character.                    */
            {
                fputc((int)(RUN | run_len), outfile);
                fputc((int)run_char, outfile);
                run_len = 1;
                run_char   = cur_char;
                continue;
            }

            /* run was only one or two chars, make a seq out of it instead       */

            for (j = 0; j < run_len; j++);    /* copy 1 or 2 char run to seq[]   */
            {
                seq[seq_len] = run_char;
                ++seq_len;
                if (seq_len == MAX_LEN)       /* if seq[] is full, write to disk */
                {
                    fputc((int)MAX_SEQ_HEADER, outfile);
                    for (i = 0; i < seq_len; i++)
                        fputc((int)seq[i], outfile);
                    seq_len = 0;
                }
            }

            run_len = 0;
            seq[seq_len++] = cur_char;
            if (seq_len == MAX_LEN)        /* if seq[] is full, write to disk */
            {
                fputc((int)MAX_SEQ_HEADER, outfile);
                for (i = 0; i < seq_len; i++)
                    fputc((int)seq[i], outfile);
                seq_len = 0;
            }
        }
        else    /* a sequence exists */
        {
            if (run_len != 0)           /* if a run exists */
            {
                if (cur_char == run_char )  /* add to run!  Yay.  */
                {
                    ++run_len;
                    if (run_len == MAX_LEN)  /* if run is full */
                    {
                        /* write sequence that precedes run */

                        fputc((int)(SEQ | seq_len), outfile);

                        for (i = 0; i < seq_len; i++)
                            fputc((int)seq[i], outfile);

                        /* write run                        */

                        fputc((int)(RUN | run_len), outfile);
                        fputc((int)run_char, outfile);

                        /* and start out fresh              */
                        seq_len = run_len = 0;
                    }  /* end write full run with existing sequence */

                    continue;
                }  /* end add to run for sequence exists */

                /* we couldn't add to the run, and a preceding sequence */
                /* exists, so write the sequence and the run, and       */
                /* try starting a new run with the current character.   */
                /* write sequence that precedes run */

                fputc((int)(SEQ | seq_len), outfile);

                for (i = 0; i < seq_len; i++)
                    fputc((int)seq[i], outfile);

                /* write run                        */

                fputc((int)(RUN | run_len), outfile);
                fputc((int)run_char, outfile);

                /* and start a new run w/ cur_char  */

                seq_len = 0;
                run_len = 1;
                run_char = cur_char;
                continue;

            }    /* end can't add to existing run, and preceding seq exists */

            /* no run exists, but a sequences does.  Try to create a run    */
            /* by looking at cur_char and the last char of the sequence.    */
            /* if that fails, add the char to the sequence.                 */
            /* if the sequence is full, write it to disk.  (Slightly non    */
            /* optimal; we could wait one more char.  A small thing to fix  */
            /* if someone gets the urge...                                  */

            if (seq[seq_len - 1] == cur_char)       /* if we can make a run */
            {
                run_char = cur_char;
                run_len = 2;
                --seq_len;
                continue;
            }

            /* couldn't make a run, add char to seq.  Maybe next time       */
            /* around...                                                    */

            seq[seq_len++] = cur_char;

            if (seq_len == MAX_LEN) /* if the sequence is full, write out   */
            {
                fputc((int)MAX_SEQ_HEADER, outfile);

                for (i = 0; i < MAX_LEN; i++)
                    fputc((int)seq[i], outfile);
                seq_len = 0;
            }
        }  /* end branch on sequence exists */
    } /* done with whole file */

    /* there may be stuff left that hasn't been written yet; if so, write it */

    if (seq_len != 0)  /* write sequence that precedes run */
    {
        fputc((int)(SEQ | seq_len), outfile);

        for (i = 0; i < seq_len; i++)
            fputc((int)seq[i], outfile);
    }

    if (run_len != 0)  /* write run */
    {
        fputc((int)(RUN | run_len), outfile);
        fputc((int)run_char, outfile);
    }

    fclose(infile);
    fclose (outfile);

    return 0;

}  /* end RLE_Compression() */


/**************************************************************************
 RLE_Decompression ()

 本函数用RLE算法对文件进行解压缩
 **************************************************************************/
int RLE_Decompression(char * infile_name, char * outfile_name)
{
    register int byte;
    register unsigned short i;
    register unsigned short length;
    int packet_hdr;
    char scratch_space[134];

    FILE *infile, *outfile;

    if ((infile=fopen(infile_name, "rb")) == NULL)

    {

        strcpy(scratch_space, "Unable to open ");

        strcat(scratch_space, infile_name);

        puts(scratch_space);

        return 1;

    }

    if ((outfile=fopen(outfile_name, "wb")) == NULL)

    {
        strcpy(scratch_space, "Unable to open ");
        strcat(scratch_space, outfile_name);
        puts(scratch_space);
        return 1;
    }

    while (!feof(infile))
    {
        packet_hdr = fgetc(infile);

        if (feof(infile))
            continue;

        length = MAX_LEN & packet_hdr;

        if (packet_hdr & RUN)  /* if it's a run... */
        {
            byte = fgetc(infile);

            for (i = 0; i < length; i++)
                if (fputc(byte, outfile)== EOF)
                {
                    strcpy(scratch_space, "Error writing to ");
                    strcat(scratch_space, outfile_name);
                    puts(scratch_space);
                    fclose(infile);
                    fclose(outfile);
                    return 1;
                }
        }
        else /* it's a sequence */
            for (i = 0; i < length; i++)
                if (fputc(fgetc(infile), outfile)==EOF)
                {
                    strcpy(scratch_space, "Error writing to ");
                    strcat(scratch_space, outfile_name);
                    puts(scratch_space);
                    fclose(infile);
                    fclose(outfile);
                    return 1;
                }
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}  /* end RLE_Uncompression() */
