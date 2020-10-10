#include<iostream>
#include<Windows.h>
#include"resource.h"
#include"LoadImage.h"
#pragma comment(lib, "msimg32.lib")

//이 함수는 처음 년도와 위치를 표시할때만 사용합니다.
void PrintBitMap::Load_Starting_Logo(HWND& hwnd, HDC& hdc, HDC& mainDC, HBITMAP& Main_oj, HBITMAP& Sub_oj, int xpos, int ypos)
{
	SetTimer(hwnd, 1, 100, NULL);
	if (ypos >= 100 && xpos <= 1000)
		KillTimer(hwnd, 1);

	main_buffer = CreateCompatibleDC(mainDC);
	sub_buffer = CreateCompatibleDC(mainDC);

	om_buffer = (HBITMAP)SelectObject(main_buffer, Main_oj);
	os_buffer = (HBITMAP)SelectObject(sub_buffer, Sub_oj);

	BitBlt(mainDC, 100, ypos, 600, 200, main_buffer, 0, 0, SRCCOPY);
	BitBlt(mainDC, xpos, 600, 600, 200, sub_buffer, 0, 0, SRCCOPY);
	BitBlt(hdc, 0, 0, 1920, 1080, mainDC, 0, 0, SRCCOPY);

	SelectObject(main_buffer, om_buffer);
	SelectObject(sub_buffer, os_buffer);
	DeleteDC(main_buffer);
	DeleteDC(sub_buffer);  

}

//
//배경 위에 오브젝트를 출력합니다. RECT 구조체 안에 이미지의 크기, 시작좌표를 차례대로 저장합니다.
//
void PrintBitMap::Load_Object(HINSTANCE& hInst, HDC& hdc, HDC& Main_DC, HDC& OJ_DC, HBITMAP& Main_object, HBITMAP& Main_BG, HBITMAP& h_Text, RECT& Size)
{
	main_buffer = CreateCompatibleDC(Main_DC);
	//오브젝트를 선택하면 배경을 어둡게 변경합니다.
	Main_BG = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_DARK));
	SelectObject(Main_DC, Main_BG);

	om_buffer = (HBITMAP)SelectObject(OJ_DC, Main_object);	//오브젝트
	os_buffer = (HBITMAP)SelectObject(main_buffer, h_Text);	//텍스트
	
	
	BitBlt(Main_DC, Size.right, Size.bottom, Size.left, Size.top, OJ_DC, 0, 0, SRCCOPY);	//오브젝트 출력
	BitBlt(Main_DC, 0, 900, 1920, 100, main_buffer, 0, 0, SRCCOPY);	//텍스트 출력
	BitBlt(hdc, 0, 0, 1920, 1080, Main_DC, 0, 0, SRCCOPY);	//오브젝트 + 텍스트 출력
	
	SelectObject(OJ_DC, om_buffer);
	SelectObject(main_buffer, os_buffer);
	
	DeleteDC(OJ_DC);
	DeleteDC(main_buffer);
}

//
//배경화면을 1stage 배경화면으로 되돌립니다.
//
void PrintBitMap::Reset_BackGround(HINSTANCE& hInst, HDC& hdc, HDC& MainDC, HBITMAP& Main_BG)
{
	Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_BG));
	SelectObject(MainDC, Main_BG);
	BitBlt(hdc, 0, 0, 1920, 1080, MainDC, 0, 0, SRCCOPY);
	SelectObject(MainDC, Main_BG);
}

//
//점프 스케어를 출현시킵니다.
//
void PrintBitMap::Jump_Scare(HINSTANCE& hInst, HDC& hdc, int time)
{
	main_buffer = CreateCompatibleDC(hdc);
	if (time == 2)
		om_buffer = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_GOHST));
	else if (time > 5)
		om_buffer = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_HELPBG));
	SelectObject(main_buffer, om_buffer);
	BitBlt(hdc, 0, 0, 1920, 1080, main_buffer, 0, 0, SRCCOPY);
	SelectObject(main_buffer, om_buffer);
	DeleteDC(main_buffer);
	DeleteObject(om_buffer);
}
//
//애니메이션
//
void PrintBitMap::Animation(HINSTANCE& hInst, HDC& hdc, HDC& main_dc, int count)
{
	main_buffer = CreateCompatibleDC(main_dc);
	sub_buffer = CreateCompatibleDC(main_buffer);
	HBITMAP RunBit[8];
	RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV0)); // for(i = 0; i < 8; i++)
	RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV1));
	RunBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV2));
	RunBit[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV3));
	RunBit[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV4));
	RunBit[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV5));
	RunBit[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV6));
	RunBit[7] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV7));

	if(count < 8)
	{
		os_buffer = (HBITMAP)SelectObject(sub_buffer, RunBit[count]);
		BitBlt(main_dc, 340, 13, 1228, 854, sub_buffer, 0, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, 1920, 1080, main_dc, 0, 0, SRCCOPY);
		SelectObject(main_buffer, RunBit[count]);
		DeleteObject(os_buffer);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			DeleteObject(RunBit[i]);
		}
	}
	DeleteDC(main_dc);
}
//
//더블 버퍼링을 사용한 점프 스케어 입니다.
//
void PrintBitMap::Double_buffering(HWND& hwnd, HINSTANCE& hInst, HDC& hdc, HBITMAP& object, HBITMAP& Main_bg, RECT& size, int yPos)
{
	sub_buffer = CreateCompatibleDC(hdc); //배경	
	sub2_buffer = CreateCompatibleDC(sub_buffer); //오브젝트

	sub_backGround = (HBITMAP)SelectObject(sub_buffer, Main_bg);
	os_buffer = (HBITMAP)SelectObject(sub2_buffer, object);

	TransparentBlt(sub_buffer, 300, yPos, size.left, size.top, sub2_buffer, 0, 0, size.left, size.top, RGB(0, 0, 0));
	BitBlt(hdc, 0, 0, 1920, 1080, sub_buffer, 0, 0, SRCCOPY);

	SelectObject(sub_buffer, Main_bg);
	SelectObject(sub2_buffer, object);
	DeleteDC(sub2_buffer);
	DeleteDC(sub_buffer);

	if (yPos >= 1773)
	{
		KillTimer(hwnd, 3);
	}
	
}
//
//배경과 이미지만 출력합니다.
//
void PrintBitMap::BackGround_Text(HINSTANCE& hInst, HDC& hdc, HBITMAP& Main_BG, HBITMAP& h_text)
{
	main_buffer = CreateCompatibleDC(hdc);
	sub_buffer = CreateCompatibleDC(main_buffer);

	om_buffer = (HBITMAP)SelectObject(main_buffer, Main_BG);
	os_buffer = (HBITMAP)SelectObject(sub_buffer, h_text);

	BitBlt(main_buffer, 0, 900, 1920, 100, sub_buffer, 0, 0, SRCCOPY);
	BitBlt(hdc, 0, 0, 1920, 1080, main_buffer, 0, 0, SRCCOPY);
	SelectObject(main_buffer, Main_BG);
	SelectObject(sub_buffer, h_text);
	DeleteDC(main_buffer);
	DeleteDC(sub_buffer);
}

//
//오브젝트를 삭제합니다.
//
void PrintBitMap::Reset_Object(HBITMAP& Main_oj, HBITMAP& Sub_oj, HBITMAP& text_oj)
{
	DeleteObject(Main_oj);
	DeleteObject(Sub_oj);
	DeleteObject(text_oj);
}
