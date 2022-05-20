//Archiver.exe --file arc1.arc --create a.txt b.txt

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MyArcData
{
	unsigned short NumberOfFiles;
	unsigned short NameSizes[10];
	unsigned int offBytes[10];
}MyArcData;

void insertInArchive(FILE* Input, FILE* Output) {
	unsigned Byte;
	while ((Byte = fgetc(Input)) != EOF) {
		fputc(Byte, Output);
	}
}

void createArchive(char* ArchiveName, char** FileNames, unsigned NumberOfFiles) {	
	MyArcData ArcData;
	ArcData.NumberOfFiles = NumberOfFiles;
	for (int i = 0; i < NumberOfFiles; i++)
	{
		ArcData.NameSizes[i] = strlen(FileNames[i]) + 1;
	}

	FILE* Archive = fopen(ArchiveName, "wb");
	fwrite(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);
	int zero = 0;
	fwrite(&zero, sizeof(int), 1, Archive);

	for (int i = 0; i < NumberOfFiles; i++)
	{
		ArcData.offBytes[i] = ftell(Archive);
		FILE* Input = fopen(FileNames[i], "rb");
		insertInArchive(Input, Archive);
		fclose(Input);
	}
	unsigned int ByteWhereNamesStart = ftell(Archive);
	fseek(Archive, 2, SEEK_SET);
	fwrite(&ByteWhereNamesStart, sizeof(int), 1, Archive);

	fseek(Archive, ByteWhereNamesStart, SEEK_SET);
	for (int i = 0; i < NumberOfFiles; i++)
	{
		fwrite(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		fwrite(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		fwrite(&ArcData.offBytes[i], sizeof(int), 1, Archive);
	}

	fclose(Archive);
}

void extractFile(FILE* Input, FILE* Output, unsigned int FileSize) {
	int Byte;
	for (int i = 0; i < FileSize; i++)
	{
		Byte = fgetc(Input);
		fputc(Byte, Output);
	}
}

void extract(char* ArchiveName) {
	MyArcData ArcData;
	ArcData.NumberOfFiles = 0;
	FILE* Archive = fopen(ArchiveName, "rb");
	fread(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);
	unsigned int ByteWhereNamesStart = 0;
	fread(&ByteWhereNamesStart, sizeof(int), 1, Archive);

	fseek(Archive, ByteWhereNamesStart, SEEK_SET);
	char* FileNames[10];
	int i;
	for (i = 0; i < ArcData.NumberOfFiles; i++)
	{
		fread(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		FileNames[i] = (char*)malloc(ArcData.NameSizes[i] * sizeof(char));
		fread(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		fread(&ArcData.offBytes[i], sizeof(int), 1, Archive);
	}
	ArcData.offBytes[i] = ByteWhereNamesStart;

	fseek(Archive, ArcData.offBytes[0], SEEK_SET);
	for (int i = 0; i < ArcData.NumberOfFiles; i++)
	{
		FILE* File = fopen(FileNames[i], "wb");
		extractFile(Archive, File, ArcData.offBytes[i + 1] - ArcData.offBytes[i]);
		fclose(File);
		free(FileNames[i]);
	}
	fclose(Archive);
}

void getList(char* ArchiveName) {
	FILE* Archive = fopen(ArchiveName, "rb");
	MyArcData ArcData;
	ArcData.NumberOfFiles = 0;
	fread(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);
	unsigned int ByteWhereNamesStart = 0;
	fread(&ByteWhereNamesStart, sizeof(int), 1, Archive);
	fseek(Archive, ByteWhereNamesStart, SEEK_SET);
	char* FileNames[10];
	for (int i = 0; i < ArcData.NumberOfFiles; i++)
	{
		fread(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		FileNames[i] = (char*)malloc(ArcData.NameSizes[i] * sizeof(char));
		fread(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		printf("%s\n", FileNames[i]);
		free(FileNames[i]);
		fread(&ArcData.offBytes[i], sizeof(int), 1, Archive);
	}
	fclose(Archive);
}

int main(int argc, char* argv[]) {
	char* ArchiveName = NULL; 
	short CreateFlag = 0, ExtractFlag = 0, ListFlag = 0, NumberOfFiles = 0;
	char** FileNames = malloc(10 * sizeof(char*));
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][2] == 'f')	//--file
		{
			ArchiveName = argv[++i];
			continue;
		}
		if (argv[i][0] == '-' && argv[i][2] == 'c')	//--create
		{
			 CreateFlag = 1;
			continue;
		}
		if (argv[i][0] == '-' && argv[i][2] == 'e')	//--extract
		{
			ExtractFlag = 1;
			continue;
		}
		if (argv[i][0] == '-' && argv[i][2] == 'l')	//--list
		{
			ListFlag = 1;
			continue;
		}
		FileNames[NumberOfFiles] = argv[i];
		NumberOfFiles++;
	}
	
	if (CreateFlag == 1)
	{
		createArchive(ArchiveName, FileNames, NumberOfFiles);
	}
	if (ExtractFlag == 1)
	{
		extract(ArchiveName);
	}
	if (ListFlag == 1)
	{
		getList(ArchiveName);
	}
	
}