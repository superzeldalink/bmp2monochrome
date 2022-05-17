#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "bmpLib.h"
#include "bmpWrite.h"
#include "UILib.h"

char const *title = "                         ______\n\
 _________        .---\"\"\"      \"\"\"---.\n\
:______.-':      :  .--------------.  :\n\
| ______  |      | :                : |\n\
|:______B:|      | | Welcome to     | |\n\
|:______B:|      | |                | |\n\
|:______B:|      | | Image to       | |\n\
|         |      | |     Monochrome | |\n\
|:_____:  |      | |   Converter    | |\n\
|    ==   |      | :                : |\n\
|       O |      :  '--------------'  :\n\
|       o |      :'---...______...---'\n\
|       o |-._.-i___/'             \\._\n\
|'-.____o_|   '-.   '-...______...-'  `-._\n\
:_________:      `.____________________   `-.___.-.\n\
                 .'.eeeeeeeeeeeeeeeeee.'.      :___:\n\
               .'.eeeeeeeeeeeeeeeeeeeeee.'.\n\
              :____________________________:\n\n";

char const *end = "             .-\"\"\"-.\n\
            / .===. \\\n\
            \\/ 6 6 \\/\n\
            ( \\___/ )\n\
  ______ooo__\\_____/____________\n\
 /Thanks for using my program!  \\\n\
| Creator: Luong Trien Thang    |\n\
| Semester - Class: 212 - TT03  |\n\
| Lecturer: Dang Thanh Tin      |\n\
|           Tran Anh Khoa       |\n\
| 	          15 May 2022   |\n\
 \\___________________ooo________/\n\
             |  |  |\n\
             |_ | _|\n\
             |  |  |\n\
             |__|__|\n\
             /-'Y'-\\\n\
            (__/ \\__)";


int main() {
main: {
		SetConsoleTitle("Image to Monochrome");				// Set console title
		system("cls");										// Clear screen
		SetDefaultConsoleSize();							// Set default console size
		fflush(stdin);										// Clear input buffer(s)
		printf(title);										// Print title art

		char pathInput[265] = "";							// path string
		char path[260] = "";								// real path
		printf("Please input the file locaion (drag and drop is supported).\nType h or help to show the instructions.\nType q or quit to end the program.\n");
		gets(pathInput);									// get path string

		printf("\n");

		if(strcmp(pathInput, "h") == 0 || strcmp(pathInput, "help") == 0) {
			system("cls");
			printf(title);

			printf("INSTRUCTIONS:\n");
			printf("The program only accepts input of 8bpp and 24bpp bitmap image.\n");
			printf("\n");
			printf("For printing:\n");
			printf("\tConsole and font will be resized for better viewing.\n");
			printf("\tPress <Enter> once, then please maximize the window by clicking on the middle icon at the top right corner.\n");
			printf("\tPress <Enter> again to print monochrome version of the image.\n");
			printf("\tPress <Enter> after printing to restore console size and font.\n");
			printf("\n");
			printf("For converting:\n");
			printf("\tThe image will be converted to monochrome bitmap file and saved at the same location as the input.\n");
			system("pause");
			goto main;
		} else if(strcmp(pathInput, "q") == 0 || strcmp(pathInput, "quit") == 0) {
			goto end;
		}
		if(pathInput[0] == '"')
			strncpy(path, pathInput + 1, strlen(pathInput) - 2);		// If the path contains " (double quotes), remove it then copy to path
		else
			strcpy(path, pathInput);									// Else, just copy

		FILE *fp=fopen(path,"r");										// Open file

		if(fp == NULL) {												// If file not found
			printf("ERROR: File not found\n");
			goto restartPrompt;
		}

		if(!IsBitMap(fp)) {												// Check whether a bitmap file
			printf("ERROR: Format error. Or not a bitmap file!\n");
			fclose(fp);
			goto restartPrompt;
		}

		int width = getWidth(fp);										// Get width, height, bpp
		int height = getHeight(fp);
		int bpp = getBpp(fp);

		printf("File path: %s\nSize: %dpx x %dpx\nBit per pixel: %d\n", path, width, height, bpp);

		if(bpp == 1){													// If bpp = 1 (monochrome, binary image)
			printf("ERROR: The file is already in monochrome (1bpp).\n");
			fclose(fp);
			goto restartPrompt;
		}
		else if(bpp != 8 && bpp != 24) {								// Else if bpp is not 8 or 24
			printf("ERROR: The file is not in 8bpp or 24bpp.\n");
			fclose(fp);
			goto restartPrompt;
		}


		printf("Do you want to print image or convert to monochrome? (p: print, c: convert) ");
ask:
		char pc = getch();												// Get character from keyboard
		printf("\n");
		if(pc == 'p') {													// PRINT!
			system("pause");
			SetPrintSize(width, height);								// Set window and font size
			
			getch();
			printImage(fp, width, height, bpp);							// Print binary image to console
			
			getch();
			system("cls");												// Print end, clear screen
			SetDefaultConsoleSize();									// Revert console size
		} else if (pc == 'c') {											// CONVERT!
			unsigned long dataSize;
			char *dataPtr;
			dataPtr = GetMonochromeData(fp, width, height, bpp, &dataSize);	// Get monochrome data

			unsigned long fileSize = 62 + dataSize;						// Filesize = 62 (file header + DIB header) + dataSize
			char bitmap[fileSize];
			
			char outputLocation[260] = "";
			strncpy(outputLocation, path, strlen(path) - 4);			// Remove ".bmp" at the end of the path
			strcat(outputLocation, "_mono.bmp");						// Add "_mono.bmp" to the end of the path

			BMPmake(bitmap, width, height, dataPtr, dataSize);			// Make a bitmap file array
			BMPwrite(bitmap, fileSize, outputLocation);					// Write the array to the location

			free(dataPtr);												// Free the bitmap array
			system("cls");
			printf("Conversion completed. Your file is located at\n%s", outputLocation);
			printf("\n");
		} else {														// If not "p" or "c"
			printf("Please type p to print or c to convert. ");
			goto ask;
		}
	}
	
restartPrompt:{
		printf("Restart program? (y/[n]) ");
		if(getch() == 'y')
			goto main;
	}

end: {
		system("cls");
		printf(end);
	}

	return 0;
}