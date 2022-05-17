#include <stdio.h>

// Integer to four bytes (characters)
void ToFourBytes(unsigned long n, char bytes[4]){
	bytes[0] = (n >> 24) & 0xFF;
	bytes[1] = (n >> 16) & 0xFF;
	bytes[2] = (n >> 8) & 0xFF;
	bytes[3] = (n >> 0) & 0xFF;
}
	
// Make bitmap file (as an array)
void BMPmake(char *bitmap, int width, int height, char *data, int dataSize) {	
	// -- FILE HEADER -- //

	// bitmap signature
	bitmap[0] = 'B';
	bitmap[1] = 'M';

	// file size
	unsigned long fileSize = 62 + dataSize;
	char fileSizeBytes[4];
	ToFourBytes(fileSize, fileSizeBytes);
	bitmap[2] = fileSizeBytes[3];					// 40 (dib) + 14 (header) + 12 (data)
	bitmap[3] = fileSizeBytes[2];
	bitmap[4] = fileSizeBytes[1];
	bitmap[5] = fileSizeBytes[0];

	// reserved field (in hex. 00 00 00 00)
	for(int i = 6; i < 10; i++) bitmap[i] = 0;

	// offset of pixel data inside the image
	bitmap[10] = 62;
	for(int i = 11; i < 14; i++) bitmap[i] = 0;

	// -- DIB HEADER -- //

	// header size
	bitmap[14] = 40;
	for(int i = 15; i < 18; i++) bitmap[i] = 0;

	// width of the image
	char widthBytes[4];
	ToFourBytes(width, widthBytes);
	bitmap[18] = widthBytes[3];
	bitmap[19] = widthBytes[2];
	bitmap[20] = widthBytes[1];
	bitmap[21] = widthBytes[0];
	
	// height of the image
	char heightBytes[4];
	ToFourBytes(height, heightBytes);
	bitmap[22] = heightBytes[3];
	bitmap[23] = heightBytes[2];
	bitmap[24] = heightBytes[1];
	bitmap[25] = heightBytes[0];

	// reserved
	bitmap[26] = 1;
	bitmap[27] = 0;

	// number of bits per pixel
	bitmap[28] = 1; // 1 bit
	bitmap[29] = 0;

	// compression method (no compression here)
	for(int i = 30; i < 34; i++) bitmap[i] = 0;

	// size of pixel data
	char dataSizeBytes[4];
	ToFourBytes(dataSize, dataSizeBytes);
	bitmap[34] = dataSizeBytes[3];
	bitmap[35] = dataSizeBytes[2];
	bitmap[36] = dataSizeBytes[1];
	bitmap[37] = dataSizeBytes[0];
	
	// unimportant
	for(int i = 38; i < 54; i++) bitmap[i] = 0;

	// color table 0x00000000 (black), 0xffffff00 (white)
	bitmap[54] = 0;
	bitmap[55] = 0;
	bitmap[56] = 0;
	bitmap[57] = 0;

	bitmap[58] = 0xff;
	bitmap[59] = 0xff;
	bitmap[60] = 0xff;
	bitmap[61] = 0;
	
	// -- PIXEL DATA -- //
	for(unsigned long i = 62; i < fileSize; i++) {
		bitmap[i] = data[i - 62];
	}
}

// Make a bitmap binary image file array
void BMPwrite(char *bitmap, unsigned long fileSize, char *location) {
	FILE *file;
	file = fopen(location, "wb");
	for(unsigned long i = 0; i < fileSize; i++) {						// Write all bytes into file
		fputc(bitmap[i], file);
	}
	fclose(file);
}