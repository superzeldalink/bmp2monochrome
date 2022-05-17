#include <stdio.h>
#include "bmpWrite.cpp"

void BMPmake(char *bitmap, int width, int height, char *data, int dataSize);
void BMPwrite(char *bitmap, int fileSize);
void WriteFile(char *data);