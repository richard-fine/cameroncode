//
//  encoder.c
//  
//
//  Created by Richard Fine on 13/01/2015.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isDecoding = 0;
char* dbFile = 0;

long dbSize = 0;
char* dbData = 0;
unsigned int dbEntries = 0;
char** dbIndices;

void readDbFile()
{
	FILE* fp = fopen(dbFile, "r");
	if(!fp) { fprintf(stderr, "Could not open dbfile %s\n", dbFile); exit(-2); }
	
	fseek(fp, 0, SEEK_END);
	dbSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	dbData = malloc(dbSize + 1);
	fread(dbData, dbSize, 1, fp);
	dbData[dbSize + 1] = 0;
	fclose(fp);
}

int isWhitespace(char ch)
{
	return (ch == ' ') || (ch == '\t') || (ch == '\n');
}

void parseDb()
{
	int e; char* ptr; long i;
	dbEntries = 0;
	// Do this in multiple passes cos I'm lazy
	for(i = 0; i < dbSize; ++i)
	{
		if(dbData[i] == '.' && ((i + 1 >= dbSize) || isWhitespace(dbData[i+1])))
		{
			dbData[i] = 0;
			++dbEntries;
		}
	}
	dbIndices = malloc(dbEntries * sizeof(char*));
	
	ptr = dbData;
	for(e = 0; e < dbEntries; ++e)
	{
		dbIndices[e] = ptr;
		ptr += strlen(ptr) + 1;
	}
}

void compressDb()
{
	for(int i = 0; i < dbEntries; ++i)
	{
		// Trim beginning
		while(isWhitespace(*dbIndices[i]) && *dbIndices[i] != 0)
			dbIndices[i]++;
		
		// Trim end
		int len = strlen(dbIndices[i]);
		while(len > 0 && isWhitespace(dbIndices[i][len - 1]))
		{
			dbIndices[i][--len] = 0;
		}
		
		// Check that we've not already seen it - this is O(n^2) - oh well
		for(int j = 0; j < i; ++j)
		{
			if(strcmp(dbIndices[i], dbIndices[j]) == 0)
			{
				*dbIndices[i] = 0;
				len = 0;
				break;
			}
		}
		
		// Eliminate zero-length strings
		if(len == 0)
		{
			dbIndices[i] = dbIndices[--dbEntries];
			--i;
			continue;
		}
	}
}

void encode(FILE* input, FILE* output, int wordSize)
{
	unsigned int buf = 0;
	unsigned char bitsQueued = 0;
	unsigned int bitsMask = (1 << wordSize) - 1;
	while(!feof(input))
	{
		unsigned int c = 0;
		fread(&c, 1, 1, input);
		c <<= bitsQueued;
		buf |= c;
		bitsQueued += 8;
		
		while(bitsQueued > wordSize)
		{
			int bits = buf & bitsMask;
			fprintf(output, "%s. ", dbIndices[bits]);
			buf >>= wordSize;
			bitsQueued -= wordSize;
		}
	}
	
	if(bitsQueued > 0)
	{
		fprintf(output, "%s.\n", dbIndices[buf]);
	}
}

int main(int argc, char* argv[])
{
	for(int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-decode") == 0)
		{
			isDecoding = 1;
			continue;
		}
		if (strcmp(argv[i], "-db") == 0)
		{
			if(i+1 < argc) dbFile = argv[i+1];
			continue;
		}
	}
	
	if(dbFile == 0)
	{
		fprintf(stderr, "Usage: %s [-decode] -db dbfile\n", argv[0]);
		exit(-1);
	}
	
	readDbFile();
	parseDb();
	compressDb();
	
	unsigned int wordSize = 32;
	while(((1ul << wordSize) - 1) > dbEntries && wordSize > 0) wordSize--;
	if(wordSize == 0) { fprintf(stderr, "DB is too small\n"); exit(-3); }
	
	if(isDecoding)
	{
		fprintf(stderr, "Decoding is not implemented for security reasons\n");
		exit(-5);
	}
	else
	{
		encode(stdin, stdout, wordSize);
	}
	
	free(dbIndices);
	free(dbData);
	return 0;
}