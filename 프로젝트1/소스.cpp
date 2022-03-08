#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<cmath>
#include<conio.h>
#include<process.h>
#include<time.h>
#include"resource1.h"
#define PI 3.14

typedef struct tagBITMAPHEADER {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
}BITMAPHEADER;

typedef struct tagBITMAPDRAWS {
	HBITMAP* bitmap;
	tagBITMAPDRAWS* next;
	int x;
	int y;
}BITMAPDRAWS;

BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, char* filename);

void DrawBitmap(HDC hdc, HBITMAP bitmap, int x, int y);
void DrawBitmapCursor(HDC hdc, HBITMAP bitmap);

struct DRAWPROCESS_ARG {
	HDC* hdc;
	BITMAPDRAWS* bitmap_draws;
	bool* running;
};

unsigned _stdcall draw_process(void* arg);
void frame(int w, int h);

void clearconsole();
void remove_scrollbar();



int main() {
	//자료입력
	//자료처리(연산)
	//자료출력
	
	//BITMAPHEADER header;
	//int imgSize;
	//BYTE* image = LoadBitmapFile(&header, &imgSize, (char*)"Flandre.bmp");


	//COLORREF* img = (COLORREF*) malloc(imgSize*sizeof(COLORREF));
	INPUT_RECORD InputRecord;
	DWORD Events;
	COORD coord;
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	system("mode con: cols=128 lines=48");
	remove_scrollbar();

	HWND myConsole = GetConsoleWindow();
	HDC mdc = GetDC(myConsole);

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(myConsole , GWLP_HINSTANCE);
	int pixels = 0;
	COLORREF COLOR = RGB(255, 0, 0);

	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, L"Flan_face.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//HBITMAP bitmap2 = (HBITMAP)LoadImage(NULL, L"Flan_face.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	CONSOLE_FONT_INFO presentFont;
	// 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), false, &presentFont);
	
	
	//frame(ceil(477.0 / presentFont.dwFontSize.X)+2, ceil(360.0 / presentFont.dwFontSize.Y)+2);
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {1,1});
	BITMAPDRAWS bit_draw = { &bitmap,NULL,4,4 };
	//void* thread_arg[2];
	//memcpy(thread_arg, &mdc, sizeof(HDC));
	//memcpy(thread_arg+sizeof(HDC), &bit_draw, sizeof(BITMAPDRAWS));
	//thread_arg+sizeof(HDC) = &bit_draw;
	//thread_arg[0] = & mdc;
	//thread_arg[1] = & bit_draw;

	bool* draw_running_flag = (bool*)malloc(sizeof(bool));
	*draw_running_flag = true;

	DRAWPROCESS_ARG* thread_arg = (DRAWPROCESS_ARG*)malloc(sizeof(DRAWPROCESS_ARG));
	thread_arg->hdc = &mdc;
	thread_arg->bitmap_draws = &bit_draw;
	thread_arg->running = draw_running_flag;


	_beginthreadex(NULL, 0, draw_process, thread_arg, 0, NULL);

	//DrawBitmap(mdc, bitmap);
	while (true)
	{
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) {
			if (InputRecord.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				printf("hello flan\n");
			}
		}
		else if (InputRecord.EventType == KEY_EVENT) {
			//DrawBitmap(mdc, bitmap2, 300, 400);
		}



	}
	
	/*
	for (double i = 0; i < PI * 4;i+=0.05) {
		SetPixel(mdc, pixels, (int)(50 + 25 * cos(i)), COLOR);
		
		pixels += 1;
	}
	*/
	
	/*
	for (int i = 0; i < imgSize; i++)
	{
		
		int colorIndex = i * 3;
		BYTE b = image[colorIndex];
		BYTE g = image[colorIndex + 1];
		BYTE r = image[colorIndex + 2];
		SetPixel(mdc, i% header.bi.biWidth, header.bi.biHeight - (int)(i / header.bi.biWidth), RGB(r, g, b));
	
		//printf("%i %i %i\n",r,g,b);
	}
	*/
	*draw_running_flag = false;
	


	ReleaseDC(myConsole, mdc);

	free(thread_arg);
	free(draw_running_flag);
	

	//free(image);

	_getch();
	return 0;
}

//박종득 교수 연락처
//1층 109호 (과사맞은편)
//010-9031-9221
//pjd@kongju.ac.kr





BYTE* LoadBitmapFile(BITMAPHEADER* bitmapHeader, int* imgSize, char* filename)
{
	FILE* fp = fopen(filename, "rb");	//파일을 이진읽기모드로 열기
	BYTE trash[16];
	int trash_width = 0;
	if (fp == NULL)
	{
		printf("파일로딩에 실패했습니다.\n");	//fopen에 실패하면 NULL값을 리턴
		return NULL;
	}
	else
	{
		fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//비트맵파일헤더 읽기
		fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//비트맵인포헤더 읽기

		int imgSizeTemp = bitmapHeader->bi.biWidth * bitmapHeader->bi.biHeight;	//이미지 사이즈 계산
		*imgSize = imgSizeTemp;	// 이미지 사이즈를 상위 변수에 할당

		BYTE* image = (BYTE*)malloc(sizeof(BYTE) * 3 * imgSizeTemp);	//이미지크기만큼 메모리할당
		//fread(image, sizeof(BYTE) , imgSizeTemp*3, fp);//이미지 크기만큼 파일에서 읽어오기
		trash_width = 4 - (bitmapHeader->bi.biWidth * 3 % 4);
		if (trash_width == 4) {
			fread(image, sizeof(BYTE) * imgSizeTemp, 3, fp);
		}
		else {
			for (int i = 0; i < bitmapHeader->bi.biHeight; i++) {
				fread(image + (bitmapHeader->bi.biWidth * i * 3), bitmapHeader->bi.biWidth * 3, 1, fp);
				fread(trash, trash_width, 1, fp);
			}
		}

		fclose(fp);
		return image;
	}
}

void DrawBitmap(HDC hdc, HBITMAP bitmap, int x, int y) {
	BITMAP bitmap_info;
	GetObject(bitmap, sizeof(BITMAP), &bitmap_info);

	//COORD cur = { bitmap_info.bmWidth / presentFont.dwFontSize.X + presentCur.dwCursorPosition.X + 1 , bitmap_info.bmHeight / presentFont.dwFontSize.Y + presentCur.dwCursorPosition.Y };


	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);

	HDC memDC = CreateCompatibleDC(hdc);

	HBITMAP old_bitmap = (HBITMAP)SelectObject(memDC, bitmap);

	GdiTransparentBlt(hdc, x, y, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, NULL);

	SelectObject(memDC, old_bitmap);

	DeleteDC(memDC);
}

void DrawBitmapCursor(HDC hdc, HBITMAP bitmap) {
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	CONSOLE_FONT_INFO presentFont;
	// 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), false, &presentFont);

	BITMAP bitmap_info;
	GetObject(bitmap, sizeof(BITMAP), &bitmap_info);

	DrawBitmap(hdc, bitmap, presentCur.dwCursorPosition.X * presentFont.dwFontSize.X, presentCur.dwCursorPosition.Y * presentFont.dwFontSize.Y);

}



unsigned _stdcall draw_process(void* arg) {
	RECT window_rect;
	DRAWPROCESS_ARG* draw_process_arg = (DRAWPROCESS_ARG*)arg;
	HDC* hdc = draw_process_arg->hdc;
	BITMAPDRAWS* bitmap_draws = draw_process_arg->bitmap_draws;
	bool* running = draw_process_arg->running;

	//HWND console = GetConsoleWindow();
	
	//GetWindowRect(console, &window_rect);
	//int width = window_rect.right;
	//int height = window_rect.bottom;

	while (*running) {
		BITMAPDRAWS* head = bitmap_draws;
		while (head != NULL) {
			HBITMAP* bitmap = head->bitmap;
			int x = head->x, y = head->y;
			DrawBitmap(*hdc, *bitmap, x, y);
			head = head->next;
		}
		//GetWindowRect(console, &window_rect);
		//MoveWindow(console, window_rect.left, window_rect.top, width, height, true);
	}
	return 0;
}

void frame(int w, int h) {
	//printf("┌─┐\n│┼│\n└─┘");
	printf("┌");
	for (int i = 0; i < w - 2; i++) {
		printf("─");
	}
	printf("┐\n");

	for (int j = 0; j < h - 2; j++) {
		printf("│");
		for (int i = 0; i < w - 2; i++) {
			printf(" ");
		}
		printf("│\n");
	}


	printf("└");
	for (int i = 0; i < w - 2; i++) {
		printf("─");
	}
	printf("┘\n");
}


void clearconsole() {
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 80 * 25, Coor, &dw); // 콘솔창 화면을 지운다.

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coor);
}

void remove_scrollbar()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}