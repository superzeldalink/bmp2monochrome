// LIBRARY FOR IMAGE READING AND PROCESSING

#include<stdio.h>
#include<malloc.h>
#include <iostream>
#define BM 19778
#define ushort unsigned short
#define uint unsigned int
#define uchar unsigned char

//Judge whether it is a bitmap, in 0-1 bytes
int IsBitMap(FILE *fp) {
	ushort s;
	fread(&s,1,2,fp);
	return s==BM ? 1 : 0;
}

//Get the width of the picture, in 18-21 bytes
int getWidth(FILE *fp) {
	int width;
	fseek(fp,18,SEEK_SET);
	fread(&width,1,4,fp);
	return width;
}

//Get the height of the picture, in 22-25 bytes
int getHeight(FILE *fp) {
	int height;
	fseek(fp,22,SEEK_SET);
	fread(&height,1,4,fp);
	return height;
}

//Get the number of bits of each pixel in 28-29 bytes
ushort getBpp(FILE *fp) {
	ushort bit;
	fseek(fp,28,SEEK_SET);
	fread(&bit,1,2,fp);
	return bit;
}

//Get the starting position of data, in 10-13 bytes
uint getOffSet(FILE *fp) {
	uint OffSet;
	fseek(fp,10L,SEEK_SET);
	fread(&OffSet,1,4,fp);
	return OffSet;
}

//Get the byte in a location
uchar getByte(FILE *fp, int location) {
	uchar byte;
	fseek(fp,location,SEEK_SET);
	fread(&byte,1,1,fp);
	return byte;
}

//Print the image to console
void printImage(FILE *fp, int width, int height, int bpp) {
	uint offset = getOffSet(fp);							// Get offset to pixel array

	int rowSize = int((bpp*width*1.0 + 31)/32)*4;			// Rowsize

	for(int i = height - 1; i >= 0; i--) {					// Loop from bottom up
		for(int j = 0; j < width; j++) {
			float gray = 0;
			uint red = 0, green = 0, blue = 0;
			if(bpp == 8) {
				uchar index = getByte(fp, offset + i*rowSize + j);	// Get the index color of color table
				uint location = 54 + index * 4;						// Get location of the color (54: start of color table, *4 since there are 4 bytes defined for every color
				blue = getByte(fp, location);
				green = getByte(fp, location + 1);
				red = getByte(fp, location + 2);
			} else if(bpp == 24) {
				blue = getByte(fp, offset + i*rowSize + j*3);
				green = getByte(fp, offset + i*rowSize + j*3 + 1);
				red = getByte(fp, offset + i*rowSize + j*3 + 2);
			}

			gray = 0.299*red + 0.587*green + 0.114*blue;	// Grayscale calculated using luminosity method
			if(gray <= 127)
				printf("0");
			else
				printf("1");
		}
		printf("\n");
	}
}

// Turn an array of 8 binary integers into a character (a byte)
char BitToByte(int *num) {
	char result = 0;
	for (int i = 0; i < 8; ++i )
		result |= (num[i] == 1) << (7 - i);

	return result;
}

// Convert the image into monochrome array
char *GetMonochromeData(FILE *fp, int width, int height, int bpp, unsigned long *dataSize) {
	uint offset = getOffSet(fp);

	int rowSize = int((bpp*width*1.0 + 31)/32)*4;						// Rowsize

	unsigned long newRowSizeInBits = int((1*width*1.0 + 31)/32)*4*8;	// New rowsize (in bits, bpp = 1; 1 byte = 8 bits)

//	char monochromeBits[newRowSizeInBits * height];						// Image data (in bits)
	char *monochromeBits = (char *)calloc(newRowSizeInBits * height, sizeof(char));	

	int progressCounter = 5;											// Progress bar will update every 5 percent (process will slow down if constantly checking for progress)
	system("cls");
	printf("Converting... Please wait... (Ctrl+C to break)\nProgress: 0 %%");
	for(int i = 0; i < height; i++) {									// Loop from top down
		int j;
		for(j = 0; j < width; j++) {									// Same as above
			float gray = 0;
			uchar red = 0, green = 0, blue = 0;
			if(bpp == 8) {
				uchar index = getByte(fp, offset + i*rowSize + j);
				uint location = 54 + index * 4;
				blue = getByte(fp, location);
				green = getByte(fp, location + 1);
				red = getByte(fp, location + 2);
			} else if(bpp == 24) {
				blue = getByte(fp, offset + i*rowSize + j*3);
				green = getByte(fp, offset + i*rowSize + j*3 + 1);
				red = getByte(fp, offset + i*rowSize + j*3 + 2);
			}

			gray = 0.299*red + 0.587*green + 0.114*blue;

			if(gray <= 127)												// Save bits into array
				*(monochromeBits + i*newRowSizeInBits + j) = 0;
			else
				*(monochromeBits + i*newRowSizeInBits + j) = 1;
		}
		for(int k = j; k < newRowSizeInBits; k++) {						// Set all 0 for padding
			*(monochromeBits + i*newRowSizeInBits + j) = 0;
		}
		
		// Show the progress
		int progress = i*100.0/height;
		if(progress % 5 >=4) {
			system("cls");
			printf("Converting... Please wait... (Ctrl+C to break)\nProgress: %d %%", progress);
		}
	}

	*dataSize = newRowSizeInBits*height/8;								// Size of pixel array

	char *monochromeData = (char *)calloc(*dataSize, sizeof(char));		// Image array (in bytes)
	for(unsigned long i = 0; i < *dataSize; i++) {						// For every 8 bits, pack into a byte, then store it
		int byte[8];
		for(unsigned long j = 0; j < 8; j++)
			byte[j] = monochromeBits[8*i+j];
		monochromeData[i] = BitToByte(byte);
	}

	return monochromeData;
}