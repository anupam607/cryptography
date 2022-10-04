#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR_IN_ENG 26
#define MAX_CIPHER_TEXT_SIZE 1000

/* This function calculates the relative freq of cipher text and puts in q[] buffer */
void calculate_relative_freq_of_cipher_text(char *cipher_text, double q[], int len);

/* This function finds key using coincidence method */
int get_key_using_coincidence_method(double p[], double q[]);

/* This function finds key using absolute distribution method */
int get_key_using_absolute_distribution(double p[], double q[]);

/* This function shifts the cipher text by key times and print original message */
void print_msg_using_key(char *cipher_text, char msg[], int len, int key);
/* table of relative frequency of english letter (a to z)*/

void main(void)
{
	/* Relative freq of English character */
	double p[MAX_CHAR_IN_ENG] = {0.0817, 0.0150, 0.0278, 0.0425, 
		0.1270, 0.0223, 0.0202, 0.0609, 
		0.0697, 0.0015, 0.0077, 0.0403,
		0.0241, 0.0675, 0.0751, 0.0193, 
		0.0010, 0.0599, 0.0633, 0.0906, 
		0.0276, 0.0098, 0.0236, 0.0015, 
		0.0197, 0.0007};
	/* buffer to hold permutation of letter in cipher key */
	double q[MAX_CHAR_IN_ENG] = {0.0};

	/* buffer to hold decrypted message */
	char msg[MAX_CIPHER_TEXT_SIZE];

	/* buffer to hold cipher text */
	char cipher_text[MAX_CIPHER_TEXT_SIZE];

	/* variable to store cipher text length and key */
	int length, key;

	/* get the cipher text form user as input */
	printf("Enter Shift Cipher Text:");
	gets(cipher_text);

	/*calculate cipher text length */
	length = strlen(cipher_text);

        /* calculate cipher key letter frequency */
	calculate_relative_freq_of_cipher_text(cipher_text, q, length);

	printf("Decreapting using Absolute Distribution-  ");

	/* calculate key using absolute distribution method */
	key = get_key_using_absolute_distribution(p,q);

	printf("Key: %d\n",key);
	
        /* print msg using key */
	print_msg_using_key(cipher_text, msg, length, key);

	printf("Decreapting using Coincidence Method- ");

	/* find key using coincidence method */
	key = get_key_using_coincidence_method(p,q);
	
	printf("Key: %d\n",key);

        /* print msg using key */
	print_msg_using_key(cipher_text, msg, length, key);


}

/* This function calculate the relative freq of cipher text and puts in q[] buffer */
void calculate_relative_freq_of_cipher_text(char *cipher_text, double q[], int len)
{
	int i;
	/* Count same charater and increament their count */
	for (int i = 0; i < len; i++)
		++q[cipher_text[i] - 'a'];


	/* Calculate the relative freq of cipher text  */
	for (int i = 0; i < MAX_CHAR_IN_ENG; i++)
        q[i] /= len;
}

/* This function finds key using absolute distribution method */
int get_key_using_absolute_distribution(double p[], double q[])
{
	double sum = 0.0 , low = (MAX_CHAR_IN_ENG * MAX_CHAR_IN_ENG) ;
	int i, j, key;
        /* calculate sigma for i = 0 to 25 */
	for (i = 0; i < MAX_CHAR_IN_ENG; i++)
	{
		sum = 0.0;
                /* calculate for |p-q0|, |p-q1|---------|p-q25| here q0(q0,q1---------q25)*/
		for (j = 0 ; j < MAX_CHAR_IN_ENG; j++)
			sum += (double)(((p[j]-q[(j+i)%MAX_CHAR_IN_ENG]) >= 0) ? (p[j]-q[(j+i)%MAX_CHAR_IN_ENG]) : -(p[j]-q[(j+i)%MAX_CHAR_IN_ENG])) ;
                /* check for least value */
		if ( sum < low)
		{
			/* update key and low if current sum is msaller than low */
			low = sum;
			key = i;
		}
	}
        /* return key */
	return key;
}

/* This function finds key using coincidence method */
int get_key_using_coincidence_method(double p[], double q[])
{
	int i, j , key = 0;
	double alpha = 0.0, beta =0.0, diff = (MAX_CHAR_IN_ENG * MAX_CHAR_IN_ENG) ;

	/* calculate alph = sigma i=0 to 25 pi sqr */
	for (i = 0; i < MAX_CHAR_IN_ENG; i++)
		alpha +=  q[i] * q[i];
	diff = alpha;

        /* calculate sigma for i = 0 to 25 for beta */
	for (i = 0; i < MAX_CHAR_IN_ENG; i++)
	{
		beta = 0.0;
		/* calculate pi*qi+j and assign to beta */
		for (int j = 0; j < MAX_CHAR_IN_ENG; j++)
			beta += q[i]*p[(i+j+1)%MAX_CHAR_IN_ENG];
		/* check which beta j is closest to alpha and assig j to key*/
		if (((alpha -beta) > 0 ? (alpha -beta) : -(alpha -beta) ) < diff)
		{
			diff = ((alpha -beta) > 0 ? (alpha -beta) : -(alpha -beta) );
			key = i;
		}
	}
	return key;
}
/* This function shifts the cipher text by key times and print original message */
void print_msg_using_key(char *cipher_text, char msg[], int len, int key)
{
	int i;

	/* Shifting chipher text by key times */
	for ( int i = 0; i< len; i++)
		msg[i] =  ((((cipher_text[i]- 'a') +  MAX_CHAR_IN_ENG - key)% MAX_CHAR_IN_ENG) + 'a') ;


	/* printing decrepted message */
	for (int i = 0; i< len; i++)
		printf("%c",msg[i]);
	printf("\n");

	/* clear buffer */
	for (int i = 0; i< len; i++)
		msg[i] = '\0';
}
