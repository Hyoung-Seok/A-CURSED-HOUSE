#include<iostream>
#include<Windows.h>
#include"resource.h"
#include"LoadImage.h"
#pragma comment(lib, "msimg32.lib")

//�� �Լ��� ó�� �⵵�� ��ġ�� ǥ���Ҷ��� ����մϴ�.
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
//��� ���� ������Ʈ�� ����մϴ�. RECT ����ü �ȿ� �̹����� ũ��, ������ǥ�� ���ʴ�� �����մϴ�.
//
void PrintBitMap::Load_Object(HINSTANCE& hInst, HDC& hdc, HDC& Main_DC, HDC& OJ_DC, HBITMAP& Main_object, HBITMAP& Main_BG, HBITMAP& h_Text, RECT& Size)
{
	main_buffer = CreateCompatibleDC(Main_DC);
	//������Ʈ�� �����ϸ� ����� ��Ӱ� �����մϴ�.
	Main_BG = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_DARK));
	SelectObject(Main_DC, Main_BG);

	om_buffer = (HBITMAP)SelectObject(OJ_DC, Main_object);	//������Ʈ
	os_buffer = (HBITMAP)SelectObject(main_buffer, h_Text);	//�ؽ�Ʈ
	
	
	BitBlt(Main_DC, Size.right, Size.bottom, Size.left, Size.top, OJ_DC, 0, 0, SRCCOPY);	//������Ʈ ���
	BitBlt(Main_DC, 0, 900, 1920, 100, main_buffer, 0, 0, SRCCOPY);	//�ؽ�Ʈ ���
	BitBlt(hdc, 0, 0, 1920, 1080, Main_DC, 0, 0, SRCCOPY);	//������Ʈ + �ؽ�Ʈ ���
	
	SelectObject(OJ_DC, om_buffer);
	SelectObject(main_buffer, os_buffer);
	
	DeleteDC(OJ_DC);
	DeleteDC(main_buffer);
}

//
//���ȭ���� 1stage ���ȭ������ �ǵ����ϴ�.
//
void PrintBitMap::Reset_BackGround(HINSTANCE& hInst, HDC& hdc, HDC& MainDC, HBITMAP& Main_BG)
{
	Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_BG));
	SelectObject(MainDC, Main_BG);
	BitBlt(hdc, 0, 0, 1920, 1080, MainDC, 0, 0, SRCCOPY);
	SelectObject(MainDC, Main_BG);
}

//
//���� ���ɾ ������ŵ�ϴ�.
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
//�ִϸ��̼�
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
//���� ���۸��� ����� ���� ���ɾ� �Դϴ�.
//
void PrintBitMap::Double_buffering(HWND& hwnd, HINSTANCE& hInst, HDC& hdc, HBITMAP& object, HBITMAP& Main_bg, RECT& size, int yPos)
{
	sub_buffer = CreateCompatibleDC(hdc); //���	
	sub2_buffer = CreateCompatibleDC(sub_buffer); //������Ʈ

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
//���� �̹����� ����մϴ�.
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
//������Ʈ�� �����մϴ�.
//
void PrintBitMap::Reset_Object(HBITMAP& Main_oj, HBITMAP& Sub_oj, HBITMAP& text_oj)
{
	DeleteObject(Main_oj);
	DeleteObject(Sub_oj);
	DeleteObject(text_oj);
}
