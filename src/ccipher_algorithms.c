

/************************************************************************************
	Implementation of core column cipher functions
	Author:             Ashis Kumar Das
	ID:                 11301002
	Email:              akd.bracu@gmail.com
*************************************************************************************/



#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list.h>
#include "ccipher.h"




int ccipher_init(ColumnData *cData, unsigned int row, unsigned int col) {
	
	char *arraySpace;
	unsigned int arrayLength;
	
	arrayLength = row * col * sizeof(char);
	arraySpace = (char*) malloc(arrayLength);
	if (arraySpace == 0)
		return -1;
	
	cData->data = arraySpace;
	cData->rowSize = row;
	cData->columnSize = col;
	memset((void*) cData->data, 0, arrayLength);
	
	return 0;
}


int ccipher_destroy(ColumnData *cData) {
	
	free((void*) cData->data);
	memset((void*) cData, 0, sizeof(ColumnData));
	
	return 0;
}


int ccipher_writeRow(const char *inputText, int inputLen, ColumnData *cData) {
	
	register unsigned int index;
	char *location;
	
	index = 0;
	while (index < inputLen) {
		location = cData->data + index;
		*(location) = *(inputText + index);
		index = index + 1;
	}
	
	return 0;
}


int ccipher_writeColumn(const char *inputText, int inputLen, ColumnData *cData, List *columnSerial) {
	
	ListElem *elem;
	int *columnIndex, globalInputIndex;
	register unsigned int rowIndex;
	char *array, *writeLocation;
	unsigned int arrayLength;
	unsigned int noUseLength;
	char *noUseFirstIndex;
	
	array = cData->data;
	globalInputIndex = 0;
	elem = list_head(columnSerial);
	arrayLength = cData->rowSize * cData->columnSize;
	noUseLength = arrayLength - (unsigned int) inputLen;
	noUseFirstIndex = array + arrayLength - 1 - noUseLength + 1;
	
	while (elem != 0) {
		
		columnIndex = (int*) list_data(elem);
		rowIndex = 0;
		
		while (rowIndex < cData->rowSize
			   && globalInputIndex < inputLen) {
			   	
			writeLocation = array + (rowIndex * cData->columnSize) + *columnIndex - 1;
			if (writeLocation < noUseFirstIndex) {
				*writeLocation = *(inputText + globalInputIndex);
				globalInputIndex = globalInputIndex + 1;
			}
			rowIndex = rowIndex + 1;
		}
		
		elem = list_next(elem);
	}
	return 0;
}


int ccipher_readRow(ColumnData *cData, char *outputText, int *outputLen) {
	
	char charValue;
	register unsigned int index;
	unsigned int arrayLength;
	
	index = 0;
	arrayLength = cData->rowSize * cData->columnSize;
	
	LOOP_BEGIN:
		if (index == arrayLength)
			goto LOOP_END;

		charValue = *(cData->data + index);
		if (charValue == 0)
			goto LOOP_END;
			
		*(outputText + index) = charValue;
		index = index + 1;
		goto LOOP_BEGIN;
	
	LOOP_END:
	*outputLen = index;
	
	return 0;
}


int ccipher_readColumn(ColumnData *cData, List *columnSerial, char *outputText, int *outputLen) {
	
	ListElem *elem;
	int globalOutputIndex, *columnIndex;
	register unsigned int rowIndex;
	char *array, *pIndexValue;
	
	array = cData->data;
	globalOutputIndex = 0;
	elem = list_head(columnSerial);
	
	while (elem != 0) {
		
		columnIndex = (int*) list_data(elem);
		rowIndex = 0;
		
		while (rowIndex < cData->rowSize) {
			pIndexValue = array + (rowIndex * cData->columnSize) + *columnIndex - 1;
			if (*pIndexValue != 0) {
				*(outputText + globalOutputIndex) = *pIndexValue;
				globalOutputIndex = globalOutputIndex + 1;
			}
			rowIndex = rowIndex + 1;
		}
		
		elem = list_next(elem);
	}

	*outputLen = globalOutputIndex;
	return 0;
}




int ccipher_encrypt(const char *plainText, int plainLength,
						char *cipherText, int *cipherLength,
						unsigned int key, List *extendedKey) {


	unsigned int row, column;
	ColumnData cData;
	int opResult;
	
	column = key;
	row = (unsigned int) ceil((double) plainLength / (double) key);
	
	opResult = ccipher_init(&cData, row, column);
	if (opResult == -1)
		return -1;

	ccipher_writeRow(plainText, plainLength, &cData);
	ccipher_readColumn(&cData, extendedKey, cipherText, cipherLength);
	
	ccipher_destroy(&cData);

	return 0;
}


int ccipher_decrypt(const char *cipherText, int cipherLength,
						char *plainText, int *plainLength,
						unsigned int key, List *extendedKey) {


	unsigned int row, column;
	ColumnData cData;
	int opResult;

	column = key;
	row = (unsigned int) ceil((double) cipherLength / (double) key);
	
	opResult = ccipher_init(&cData, row, column);
	if (opResult == -1)
		return -1;

	ccipher_writeColumn(cipherText, cipherLength, &cData, extendedKey);
	ccipher_readRow(&cData, plainText, plainLength);
	
	ccipher_destroy(&cData);

	return 0;
}
