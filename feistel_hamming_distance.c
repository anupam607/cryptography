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

/* This function represents Feistel cipher block data in binary data 1 and 0 */
void print_binary_representation_of_feistel_cipher(char *feistel_cypther , int len);

/* This function returns hamming distance between two Feistel cipher */
int get_hamming_distance_of_two_feistel_cipher(char *feistel_cypther1,char *feistel_cypther2 );

void main(void)
{
    /* buffer to hold Encryption and Decryption key */
    int enc_key[MAX_KEY_ROUND], dec_key[MAX_KEY_ROUND];

    /* buffer to hold orignal, fiestel encrypted and decrypted message */
    char org_msg[MAX_CIPHER_TEXT_SIZE] = {0, }, enc_msg1[MAX_CIPHER_TEXT_SIZE] ={0, }, enc_msg2[MAX_CIPHER_TEXT_SIZE]= {0,};

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
    encrypt_decrypt_msg_using_feistel_cipher(org_msg, length, enc_key, MAX_KEY_ROUND, enc_msg1);
    
    /* Toggle last bit of original message */
    org_msg[BLOCK_SIZE-1] = (org_msg[BLOCK_SIZE-1] & (1 << 7)) ?  (org_msg[BLOCK_SIZE-1] & (~(1<< 7))) : (org_msg[BLOCK_SIZE -1] | (1<<7));
    
    /* Encrypt last bit modified orginal message using Fiestel Cypher */
    encrypt_decrypt_msg_using_feistel_cipher(org_msg, length, enc_key, MAX_KEY_ROUND, enc_msg2);
    printf("\n");
    
             /* reprsent fieste2 cipher1 block data in binary data 1 and 0 */
    printf("Feistel: Encrypted Cipher- 1 Text-       ");
    print_binary_representation_of_feistel_cipher(enc_msg1, BLOCK_SIZE);
    printf("\n");

    printf("Feistel: last bit modified Cipher Text-  ");
    print_binary_representation_of_feistel_cipher(enc_msg2, BLOCK_SIZE);
    printf("\n");

    /* get hamming distance between two fiestel cipher */
    printf("Hamming distance between these cipher is -  %d\n",get_hamming_distance_of_two_feistel_cipher(enc_msg1, enc_msg2));
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

        /* apply key from keycaps by round times */
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

/* This function returns hamming distance between two Feistel cipher */
int get_hamming_distance_of_two_feistel_cipher(char *feistel_cypther1,char *feistel_cypther2 )
{
    int i,j, hamming_distance = 0;

    /* Calculate hamming distance for one block */
    for(i = 0 ; i < BLOCK_SIZE; i++)
    {
        /* Count diff bit of two cipher byte by byte */
        for(j = 0; j < 8; j++)
        {
            /* increment the hamming distance count when bit is diff */
            if ( ((feistel_cypther1[i] >> j) & 0x1) != ((feistel_cypther2[i] >> j) & 0x1))
            {
                hamming_distance++;
            }
        }
    }
    /* return hamming distance */
    return hamming_distance;
}

/* This function represents Feistel cipher block data in binary data 1 and 0 */
void print_binary_representation_of_feistel_cipher(char *feistel_cypther, int len )
{
    int i,j;

    /* represent feistel_cipher in binary format */
    for(i = 0 ; i < len; i++)
    {
        for(j = 0; j < 8; j++)
        {
            printf("%d",((feistel_cypther[i] >> j) & 0x1));

        }
    }

}

/* This is arbitrary function f1  */
int arbitrary_function_f1 (int right_half, int key)
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

