/*
*************************
CS6530 : Assignment -2
Name : Anupam Kumar
Roll No : CS22M507
*************************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAX_KEY_ROUND 16
#define MAX_CIPHER_TEXT_SIZE 1000
#define BLOCK_SIZE 8
#define int unsigned int
#define char unsigned char

/* This function encrypts and decrypt message using Feistel cipher */
void encrypt_decrypt_msg_using_feistel_cipher(char *msg, int len, int enc_key[], int round, char *feistel_cypther);

/* This is arbitrary function f1  */
int arbitrary_function_f1 (int right_half, int key);

/* This function prints message */
void print_msg(char *msg, int len);

void main(void)
{
    /* buffer to hold Encryption and Decryption key */
    int enc_key[MAX_KEY_ROUND], dec_key[MAX_KEY_ROUND];

    /* buffer to hold orignal, fiestel encrypted and decrypted message */
    char org_msg[MAX_CIPHER_TEXT_SIZE] = {0, }, enc_msg[MAX_CIPHER_TEXT_SIZE] ={0, }, dec_msg[MAX_CIPHER_TEXT_SIZE]= {0,};

    /* variable to store original msg length */
    int length, i;

    /* Generate random Key */
    srand(clock());
    for (i = 0; i < MAX_KEY_ROUND; i++)
        enc_key[i] = dec_key[(MAX_KEY_ROUND - 1) - i] = rand();

    /* get the input message form user as input */
    printf("Enter Message Text:");
    gets(org_msg);

    /*calculate message length */
    length = strlen(org_msg);

    /*Add pad byte */
    if(length % BLOCK_SIZE)
        length += (BLOCK_SIZE - (length % BLOCK_SIZE));

    /* Encrypt message using Feistel Cypher */
    encrypt_decrypt_msg_using_feistel_cipher(org_msg, length, enc_key, MAX_KEY_ROUND, enc_msg);

    printf("Fiestel: Encrypted Cipher Text-  ");
    print_msg(enc_msg, length);

    /* Decrypt cypher using Fiestel Cypher */
    encrypt_decrypt_msg_using_feistel_cipher(enc_msg, length, dec_key, MAX_KEY_ROUND, dec_msg);

    printf("Fiestel : Decrypted message Text-  ");
    print_msg(dec_msg, length);

}

/* This function encrypts and decrypt message using Feistel cipher */
void encrypt_decrypt_msg_using_feistel_cipher(char *msg, int len, int key_space[], int round, char *feistel_cypther)
{
    int i, m, temp, block_msg, key, left_half, right_half;

    /*Apply Feistel cipher algo on block by block */
    for(m = 0; m < len ; m += BLOCK_SIZE)
    {
        /* copy message into block */
        block_msg = msg[m];
        block_msg |= msg[m+1]<<8;
        block_msg |= msg[m+2]<<16;
        block_msg |= msg[m+3]<<24;

        /* copy half block data in left and right half variable */
        right_half = block_msg & 0xFFFFFFFF;
        /* copy message into block */
        block_msg = msg[m +4];
        block_msg |= msg[m+5]<<8;
        block_msg |= msg[m+6]<<16;
        block_msg |= msg[m+7]<<24;
        left_half = block_msg & 0xFFFFFFFF;

        /* apply key from keysapce by round times */
        for(i = 0; i < round; i++)
        {
            /*get key Ki from key space */
            key = key_space[i];

            /* store right half since we need to copy it into left half */
            temp = right_half;

            /* apply Li = Ri Xor F(Ri, Ki) */
            right_half = (left_half ^ arbitrary_function_f1 (right_half, key));

            /* Li = Ri */
            left_half = temp;

        }

        /* block swap after round complete */
        temp = right_half;
        right_half = left_half;
        left_half = temp;

        /* strore converted block message */
        feistel_cypther[m] = right_half  & 0xFF;
        feistel_cypther[m + 1] = (right_half >> 8)  & 0xFF;
        feistel_cypther[m + 2] = (right_half >> 16)  & 0xFF;
        feistel_cypther[m + 3] = (right_half >> 24)  & 0xFF;
        feistel_cypther[m + 4] = left_half  & 0xFF;
        feistel_cypther[m + 5] = (left_half >> 8)  & 0xFF;
        feistel_cypther[m + 6] = (left_half >> 16)  & 0xFF;
        feistel_cypther[m + 7] = (left_half >> 24)  & 0xFF;
    }
}
/* This is arbitrary function f1  */
int arbitrary_function_f1(int right_half, int key)
{
    /* simple xor operation between right half data and key */
    right_half ^= ((key) & 0xFFFFFFFF);
    /* return key */
    return right_half;
}

/* This function prints message */
void print_msg(char *msg, int len)
{
    int i;
    /* printing message */
    for (int i = 0; i< len; i++)
        printf("%c",msg[i]);
    printf("\n");
}

