// GameOfLife.exe --input .bmp --output directory --max_iter --dump_freq

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int EndFlag = 0;

#pragma pack(1)
typedef union BITMAPFILEHEADER
{
	char Buffer[14];
	struct Data
	{
		unsigned char bfType[2];
		unsigned int bfSize;
		unsigned short int bfReserved[2];
		unsigned int bfOffBits;
	} Data;
} BITMAPFILEHEADER;
#pragma pack(1)

typedef union BITMAPINFO
{
	char Buffer[48];
	struct Info
	{
		unsigned int biSize;
		unsigned int biWidth;
		unsigned int biHeight;
		unsigned short int biPlanes;
		unsigned short int biBitCount;
		unsigned int biCompression;
		unsigned int biSizeImage;
		unsigned char biUseless[24];
	} Info;
} BITMAPINFO;

typedef struct cell {
	unsigned short status : 1;	//0 - alive(black), 1 - dead(white)
}cell;

void copy(FILE* Input, FILE* Output) {
	fseek(Input, 0, SEEK_SET);
	fseek(Output, 0, SEEK_SET);
	unsigned Byte;
	while ((Byte = fgetc(Input)) != EOF) {
		fputc(Byte, Output);
	}
}

int EndGameCheck(cell** CurrentGen, cell*** PreviousGens, unsigned int Height, unsigned int Width, int iteration) {
	unsigned int coincidences = 0;
	for (int generationNumber = iteration - 1; generationNumber >= 0; generationNumber--)
	{
		for (int i = 0; i < Height; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				if (PreviousGens[i][j][generationNumber].status == CurrentGen[i][j].status)
				{
					coincidences++;
				}
			}
		}
		if (coincidences == Height * Width)
			return 1;
		coincidences = 0;
	}

	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			PreviousGens[i][j][iteration].status = CurrentGen[i][j].status;
		}
	}

	return 0;
}

void CreateNewGeneration(cell** CurrentField, unsigned int Height, unsigned int Width) {
	cell** NextField = (cell**)malloc(Height * sizeof(cell*));
	for (int i = 0; i < Height; i++)
		NextField[i] = (cell*)malloc(Width * sizeof(cell));

	for (int row = 0; row < Height; row++)
	{
		for (int column = 0; column < Width; column++)
		{
			int neighbors = 0;
			neighbors += CurrentField[row][(Width + column - 1) % Width].status;							//левый сосед
			neighbors += CurrentField[(Height + row - 1) % Height][(Width + column - 1) % Width].status;	//левый верхний сосед
			neighbors += CurrentField[(Height + row - 1) % Height][column].status;							//верхний сосед
			neighbors += CurrentField[(Height + row - 1) % Height][(Width + column + 1) % Width].status;	//правый верхний
			neighbors += CurrentField[row][(Width + column + 1) % Width].status;							//правый
			neighbors += CurrentField[(Height + row + 1) % Height][(Width + column + 1) % Width].status;	//правый нижний
			neighbors += CurrentField[(Height + row + 1) % Height][column].status;							//нижний
			neighbors += CurrentField[(Height + row + 1) % Height][(Width + column - 1) % Width].status;	//левый нижний

			neighbors = 8 - neighbors;
			switch (neighbors)
			{
			case 3:
				NextField[row][column].status = 0;
				break;
			case 2:
				NextField[row][column].status = CurrentField[row][column].status;
				break;
			default:
				NextField[row][column].status = 1;
				break;
			}
		}
	}
	for (int i = 0; i < Height; i++)
		for (int j = 0; j < Width; j++)
			CurrentField[i][j].status = NextField[i][j].status;

	//free
	for (int i = 0; i < Height; i++)
		free(NextField[i]);
	free(NextField);
}

char* new_OutPath(char* OutDirectory, int iteration) {
	char* newOutPath = (char*)malloc(strlen(OutDirectory) * sizeof(char) + 15 * sizeof(char) + 5 * sizeof(char) + sizeof(char));
	strcpy(newOutPath, OutDirectory);
	strcat(newOutPath, "\\Generation");
	char* GenerationNumber = (char*)malloc(10 * sizeof(char));
	_itoa(iteration, GenerationNumber, 10);
	strcat(newOutPath, GenerationNumber);
	strcat(newOutPath, ".bmp");
	return newOutPath;
}

int main(int argc, char* argv[]) {
	char* InputFileName = "gunwitheater.bmp";
	char* OutDirectory = "Output";
	unsigned int Iterations = UINT_MAX, Freq = 1;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][2] == 'i')	//--input
		{
			InputFileName = argv[++i];
			continue;
		}
		if (argv[i][2] == 'o')	//--output
		{
			OutDirectory = argv[++i];
			continue;
		}
		if (argv[i][2] == 'm')	//--max_iter
		{
			Iterations = atoi(argv[++i]);
			continue;
		}
		if (argv[i][2] == 'd')	//--dump_freq
		{
			Freq = atoi(argv[++i]);
			continue;
		}
	}

	errno = 0;
	FILE* FileInput = fopen(InputFileName, "rb");
	if (errno != 0) { printf("%d", errno); }

	BITMAPFILEHEADER FileHeader;
	fread(FileHeader.Buffer, sizeof(char), 14, FileInput);
	BITMAPINFO FileInfo;
	fread(FileInfo.Buffer, sizeof(char), 48, FileInput);

	char* OutPath = new_OutPath(OutDirectory, 0);

	FILE* FileZeroGen = fopen(OutPath, "wb");
	copy(FileInput, FileZeroGen);
	fclose(FileZeroGen);

	unsigned int FieldHeight = FileInfo.Info.biHeight;
	unsigned int FieldWidth = FileInfo.Info.biWidth;

	cell** field = (cell**)malloc(FieldHeight * sizeof(cell*));
	for (int i = 0; i < FieldHeight; i++)
		field[i] = (cell*)malloc(FieldWidth * sizeof(cell));

	unsigned int PixelLineTrueSize = ceil((double)FieldWidth / 8);
	unsigned int ExtraBytes = (4 - PixelLineTrueSize % 4) % 4;
	unsigned int PixelLineSizePlusExtraBytes = PixelLineTrueSize + ExtraBytes;

	for (int i = 0; i < FieldHeight; i++)
	{
		fseek(FileInput, FileHeader.Data.bfOffBits + i * PixelLineSizePlusExtraBytes, SEEK_SET);

		for (int j = 0; j < FieldWidth; j += 8)
		{
			unsigned char pixelByte = fgetc(FileInput);

			for (int k = 0; k < 8 && (j + k) < FieldWidth; k++)
				field[i][j + k].status = pixelByte >> (7 - k);
		}
	}
	fclose(FileInput);

	cell*** PreviousGens = (cell***)malloc(FieldHeight * sizeof(cell**));
	for (int i = 0; i < FieldHeight; i++)
		PreviousGens[i] = (cell**)malloc(FieldWidth * sizeof(cell*));
	for (int i = 0; i < FieldHeight; i++)
	{
		for (int j = 0; j < FieldWidth; j++)
		{
			PreviousGens[i][j] = (cell*)malloc(3000 * sizeof(cell));
		}
	}

	for (int i = 0; i < FieldHeight; i++)
	{
		for (int j = 0; j < FieldWidth; j++)
		{
			PreviousGens[i][j][0].status = field[i][j].status;
		}
	}

	for (int iteration = 1; (iteration < Iterations) && (EndFlag != 1); iteration++)		//New Generation Builder
	{
		CreateNewGeneration(field, FieldHeight, FieldWidth);								//field with current generation -> field with next generation 

		EndFlag = EndGameCheck(field, PreviousGens, FieldHeight, FieldWidth, iteration);

		if (iteration % Freq == 0)
		{
			free(OutPath);
			OutPath = new_OutPath(OutDirectory, iteration);
			FILE* FileNextGen = fopen(OutPath, "wb");
			fwrite(FileHeader.Buffer, sizeof(char), 14, FileNextGen);
			fwrite(FileInfo.Buffer, sizeof(char), 48, FileNextGen);

			for (int i = 0; i < FieldHeight; i++)
			{
				for (int j = 0; j < FieldWidth; j += 8)
				{
					unsigned char EightCells = 0x00;
					for (int k = 0; k < 8 && k + j < FieldWidth; k++)
						EightCells |= field[i][k + j].status << (7 - k);
					fputc(EightCells, FileNextGen);
				}
				unsigned char extra = 0x00;
				for (int k = 0; k < ExtraBytes; k++)
					fputc(extra, FileNextGen);
			}
			fclose(FileNextGen);
		}
	}

	//free data
	for (int i = 0; i < FieldHeight; i++)
	{
		free(field[i]);
	}
	free(field);
	free(OutPath);
	for (int i = 0; i < FieldHeight; i++)
	{
		for (int j = 0; j < FieldWidth; j++)
		{
			free(PreviousGens[i][j]);
		}
	}
	for (int i = 0; i < FieldHeight; i++)
		free(PreviousGens[i]);
	free(PreviousGens);
}