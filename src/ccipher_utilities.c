

/************************************************************************************
	Utility routined used by column cipher project
	Author:             Ashis Kumar Das
	ID:                 11301002
	Email:              akd.bracu@gmail.com
*************************************************************************************/




#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <list.h>

#define BUFFERSIZE 10

#define cmp_char(c1, c2) ((*c1) - (*c2))




int util_convStringToInteger(const char *inputString, char endInputMark, List *integerList);

int util_transformExtendedKey(unsigned int key, List *extendedKey);

int util_convKeywordToExtendedKey(const char *keywordString, int keywordLen, List **extendedKey);






int util_convStringToInteger(const char *inputString, char endInputMark, List *integerList) {
	
	register unsigned int globalInputIndex;
	register unsigned int bufferIndex;
	int intValue, *pInteger;
	register char digit;
	char tempBuffer[BUFFERSIZE];
	int isDigit, isSpace, isEndReached, isBufferEmpty;
	int isIntegerAvailable;
	ListElem *elem_before;
	
	globalInputIndex = 0;
	bufferIndex = 0;
	list_init(integerList, free);
	memset((void*) tempBuffer, 0, BUFFERSIZE);
	
	LOOP_BEGIN:

	digit = *(inputString + globalInputIndex);
	
	isDigit = isdigit(digit) != 0 ? 1 : 0;
	isSpace = isspace(digit) != 0 ? 1 : 0;
	isEndReached = digit == endInputMark ? 1 : 0;
	isBufferEmpty = bufferIndex == 0 ? 1 : 0;
	isIntegerAvailable = (isSpace | isEndReached) & !(isBufferEmpty);
		
	if (isDigit == 1) {
		*(tempBuffer + bufferIndex) = digit;
		bufferIndex = bufferIndex + 1;
	}
	
	else if (isIntegerAvailable == 1) {
		
		intValue = (int) strtol((const char*) tempBuffer, 0, 10);
		pInteger = (int*) malloc(sizeof(int));
		*pInteger = intValue;
		elem_before = list_tail(integerList);
		list_ins_next(integerList, elem_before, (const void*) pInteger);
		memset((void*) tempBuffer, 0, bufferIndex);
		bufferIndex = 0;
		pInteger = 0;
	}
	
	if (isEndReached == 1)
		goto LOOP_END;

	globalInputIndex = globalInputIndex + 1;
	goto LOOP_BEGIN;
	
	LOOP_END:
	
	return 0;
}



int util_transformExtendedKey(unsigned int key, List *extendedKey) {
	
	int *tempArray;
	int *pValue;
	register unsigned int index;
	ListElem *elem;
	
	if (key != list_size(extendedKey))
		return -2;
	
	tempArray = (int*) malloc(sizeof(int) * key);
	if (tempArray == 0)
		return -1;
	
	index = 1;
	elem = list_head(extendedKey);
	
	while (elem != 0) {
		pValue = (int*) list_data(elem);
		*(tempArray + *pValue - 1) = (int) index;
		elem = list_next(elem);
		index = index + 1;
	}
	
	index = 0;
	elem = list_head(extendedKey);
	
	while (elem != 0) {
		pValue = (int*) list_data(elem);
		*pValue = *(tempArray + index);
		elem = list_next(elem);
		index = index + 1;
	}
	
	free((void*) tempArray);
	
	return 0;
}





int util_convKeywordToExtendedKey(const char *keywordString, int keywordLen, List **extendedKey) {
	

	List *keyList;
	ListElem *elem;
	register unsigned int outerIndex;
	register unsigned int innerIndex;
	const char *currentChar, *prevChar;
	int *newInt, *prevInt;
	int cmpResult;
	
	keyList = (List*) malloc(sizeof(List));
	list_init(keyList, free);
	newInt = (int*) malloc(sizeof(int));
	*newInt = 1;
	elem = list_tail(keyList);
	list_ins_next(keyList, elem, (const void*) newInt);
	outerIndex = 1;
	
	while (outerIndex < keywordLen) {
		
		currentChar = keywordString + outerIndex;
		newInt = (int*) malloc(sizeof(int));
		*newInt = 1;
		innerIndex = 0;
		elem = list_head(keyList);
		
		while (innerIndex < outerIndex) {
			prevChar = keywordString + innerIndex;
			prevInt = (int*) list_data(elem);
			cmpResult = cmp_char(prevChar, currentChar);
			if (cmpResult > 0)
				*prevInt = *prevInt + 1;
			else
				*newInt = *newInt + 1;
			innerIndex = innerIndex + 1;
			elem = list_next(elem);
		}
		
		elem = list_tail(keyList);
		list_ins_next(keyList, elem, (const void*) newInt);
		outerIndex = outerIndex + 1;
	}
	
	*extendedKey = keyList;
	
	return 0;
}
