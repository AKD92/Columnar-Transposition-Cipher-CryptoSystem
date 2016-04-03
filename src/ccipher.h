

/************************************************************************************
	Program Interface of Columnar Transposition Cypher
	Author:             Ashis Kumar Das
	Email:              akd.bracu@gmail.com
	GitHub:				https://github.com/AKD92
*************************************************************************************/






#ifndef COLUMNCIPHER_AKD_H
#define COLUMNCIPHER_AKD_H


#include <list.h>





struct ColumnData_ {
	
	char *data;                                 /*	2D Array that will hold characters	*/
	
	unsigned int rowSize;                       /*	2D array total number of the rows	*/
	unsigned int columnSize;                    /*	2D array total number of the columns*/
	
};

typedef struct ColumnData_ ColumnData;




int ccipher_init(ColumnData *cData, unsigned int row, unsigned int column);

int ccipher_destroy(ColumnData *cData);

int ccipher_writeRow(const char *inputText, int inputLen, ColumnData *cData);

int ccipher_writeColumn(const char *inputText, int inputLen, ColumnData *cData, List *columnSerial);

int ccipher_readRow(ColumnData *cData, char *outputText, int *outputLen);

int ccipher_readColumn(ColumnData *cData, List *columnSerial, char *outputText, int *outputLen);


int ccipher_encrypt(const char *plainText, int plainLength,
						char *cipherText, int *cipherLength,
						unsigned int key, List *extendedKey);

int ccipher_decrypt(const char *cipherText, int cipherLength,
						char *plainText, int *plainLength,
						unsigned int key, List *extendedKey);



#endif

