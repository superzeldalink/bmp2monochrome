#include "bmpLib.cpp"

int IsBitMap(FILE *fp);
int getWidth(FILE *fp);
int getHeight(FILE *fp);
ushort getBit(FILE *fp);
uint getOffSet(FILE *fp);
uchar getByte(FILE *fp, int location);
int printImage(FILE *fp, int width, int height);