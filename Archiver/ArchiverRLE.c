//Archiver.exe --file arc1.arc --create a.txt b.txt

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MyArcData
{
	unsigned short NumberOfFiles;
	unsigned short NameSizes[10];
	unsigned short originalSizes[10];
}MyArcData;

unsigned short sizeOfFile(FILE* File) {
	fseek(File, 0, 0);
	int size = 0;
	int c;
	while ((c = fgetc(File)) != EOF);
	size = ftell(File);
	fseek(File, 0, 0);
	return size;
}

void createArchive(char* ArchiveName, char** FileNames, unsigned NumberOfFiles) {
	MyArcData ArcData;
	ArcData.NumberOfFiles = NumberOfFiles;
	for (int i = 0; i < NumberOfFiles; i++)
	{
		ArcData.NameSizes[i] = strlen(FileNames[i]) + 1;
	}

	char* Buffer = NULL;
	int SizeOfBuffer = 0;
	for (int i = 0; i < NumberOfFiles; i++)
	{
		FILE* Input = fopen(FileNames[i], "rb");
		ArcData.originalSizes[i] = sizeOfFile(Input);
		SizeOfBuffer += ArcData.originalSizes[i];
		Buffer = (char*)realloc(Buffer, SizeOfBuffer * sizeof(char));
		for (int k = SizeOfBuffer - ArcData.originalSizes[i]; k < SizeOfBuffer; k++)
			Buffer[k] = fgetc(Input);
		fclose(Input);
	}

	char BufferRLE[10000];
	int j = 0;
	for (int i = 0; i < SizeOfBuffer; )
	{
		char c = Buffer[i];
		char TypeLength = 1;

		if (i == (SizeOfBuffer - 1))
		{
			BufferRLE[j] = TypeLength, BufferRLE[j + 1] = c;
			j += 2;
			i++;
		}

		if (i < (SizeOfBuffer - 1) && Buffer[i] == Buffer[i + 1])
		{
			TypeLength |= 0x80;
			while (i < (SizeOfBuffer - 1) && Buffer[i] == Buffer[i + 1] && TypeLength != 0xff)
			{
				TypeLength++;
				i++;
			}
			BufferRLE[j] = TypeLength, BufferRLE[j + 1] = c;
			j += 2;
			i++;
			continue;
		}

		if (i < (SizeOfBuffer - 1) && Buffer[i] != Buffer[i + 1])
		{
			int TL = j;
			BufferRLE[TL] = 0;
			while (i < (SizeOfBuffer - 1) && Buffer[i] != Buffer[i + 1] && BufferRLE[TL] != 0x7f)
			{
				BufferRLE[TL]++;
				BufferRLE[++j] = Buffer[i++];
			}
			if (i == (SizeOfBuffer - 1) && Buffer[i] != Buffer[i - 1] && BufferRLE[TL] != 0x7f)
			{
				BufferRLE[TL]++;
				BufferRLE[++j] = Buffer[i++];
			}
			j++;
		}
	}

	free(Buffer);

	FILE* Archive = fopen(ArchiveName, "wb");

	fwrite(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);

	for (int i = 0; i < NumberOfFiles; i++)
	{
		fwrite(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		fwrite(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		fwrite(&ArcData.originalSizes[i], sizeof(short), 1, Archive);
	}

	for (int i = 0; i < j; i++)
		fputc(BufferRLE[i], Archive);

	fclose(Archive);
}

void extractFile(FILE* Input, FILE* Output, unsigned short FileSize) {
	for (int i = 0, SizeCounter = 0; SizeCounter < FileSize; i++)
	{
		char TypeLength;
		TypeLength = fgetc(Input);
		int Type = (TypeLength & 0x80) >> 7;
		int Length = TypeLength & 0x7f;

		if (Type == 1)
		{
			char Byte = fgetc(Input);
			for (int i = 0; i < Length; i++)
			{
				fputc(Byte, Output);
				SizeCounter++;
			}
		}
		else
		{
			char Byte;
			for (int i = 0; i < Length; i++)
			{
				Byte = fgetc(Input);
				fputc(Byte, Output);
				SizeCounter++;
			}
		}

	}
}

void extract(char* ArchiveName) {
	MyArcData ArcData;
	ArcData.NumberOfFiles = 0;
	FILE* Archive = fopen(ArchiveName, "rb");
	fread(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);

	char* FileNames[10];
	int i;
	for (i = 0; i < ArcData.NumberOfFiles; i++)
	{
		fread(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		FileNames[i] = (char*)malloc(ArcData.NameSizes[i] * sizeof(char));
		fread(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		fread(&ArcData.originalSizes[i], sizeof(short), 1, Archive);
	}

	for (int i = 0; i < ArcData.NumberOfFiles; i++)
	{
		FILE* File = fopen(FileNames[i], "wb");
		extractFile(Archive, File, ArcData.originalSizes[i]);
		fclose(File);
		free(FileNames[i]);
	}
	fclose(Archive);
}

void getList(char* ArchiveName) {
	FILE* Archive = fopen(ArchiveName, "rb");
	MyArcData ArcData;
	fread(&ArcData.NumberOfFiles, sizeof(short), 1, Archive);
	char* FileNames[10];
	for (int i = 0; i < ArcData.NumberOfFiles; i++)
	{
		fread(&ArcData.NameSizes[i], sizeof(short), 1, Archive);
		FileNames[i] = (char*)malloc(ArcData.NameSizes[i] * sizeof(char));
		fread(FileNames[i], sizeof(char), ArcData.NameSizes[i], Archive);
		printf("%s\n", FileNames[i]);
		free(FileNames[i]);
		fseek(Archive, sizeof(short), SEEK_CUR);
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