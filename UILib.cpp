#include <windows.h>
#include <stdio.h>
#include <cwchar>
#include "wtypes.h"

// Get screen resolution
void GetDesktopResolution(int& horizontal, int& vertical) {
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

// char to wchar
static wchar_t* charToWChar(const char* text)
{
    const size_t size = strlen(text) + 1;
    wchar_t* wText = new wchar_t[size];
    mbstowcs(wText, text, size);
    return wText;
}

// Get current console font width
int GetCurrentFontWidth(){
	CONSOLE_FONT_INFO fontInfo;
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
	
	return fontInfo.dwFontSize.X;
}

// Set console window and font size
void SetConsoleSize(int width, int height, int textWidth, int textHeight, char* fontName){
	int defaultTextSize[3][2] = {{4, 6}, {6, 8}, {8, 8}};
	
	COORD coord;
	coord.X = width + 2; 								// Defining X and
	coord.Y = height + 2;  								//   Y size for buffer.
	
	SMALL_RECT rect;

	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = coord.Y-1; // height for window
	rect.Right = coord.X-1;  // width for window

	HANDLE hwnd = GetStdHandle(STD_OUTPUT_HANDLE);		// get handle
	SetConsoleScreenBufferSize(hwnd, coord);       		// set buffer size
	SetConsoleWindowInfo(hwnd, TRUE, &rect);       		// set window size

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = textWidth;                   	// Width of each character in the font
	cfi.dwFontSize.Y = textHeight;           			// Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, charToWChar(fontName));
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	
	if(GetCurrentFontWidth() != textWidth){
		int i = 0;
		while(textWidth > defaultTextSize[i][0] && i < 3) i++;
		cfi.dwFontSize.X = defaultTextSize[i][0];       // Width of each character in the font
		cfi.dwFontSize.Y = defaultTextSize[i][1];  
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
}

// Set print size
void SetPrintSize(int width, int height){
	int horizontal, vertical;
	GetDesktopResolution(horizontal, vertical);
	
	SetConsoleSize(width, height, horizontal/width, horizontal/width, "Raster Fonts");
}

//Set default size (window width, height: 120, 30 ; font width, height: 8, 16 ; font name: Consolas)
void SetDefaultConsoleSize(){
	SetConsoleSize(120, 30, 8, 16, "Consolas");
}
