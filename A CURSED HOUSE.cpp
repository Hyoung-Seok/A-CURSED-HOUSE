#include<Windows.h>
#include<tchar.h>
#include"resource.h"
#include"LoadImage.h"

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//이미지 관련 변수
	static HDC hdc, MainDC, Object_dc;
	static HBITMAP Main_BG, Main_Object, Sub_Object;
	PAINTSTRUCT ps;
	RECT Size = { 0,0,0,0 }; //이미지의 좌우 길이, 시작지점 좌표를 등록합니다.

	//더블 버퍼링 (클래스에서 관리합니다.)
	//static HDC back_buffer,sub_buffer;
	//static HBITMAP old_b_buffer, old_s_buffer;

	//텍스트용 비트맵
	static HBITMAP h_text;
	
	//메뉴바 관련
	static HMENU hMenu, hSubMenu;

	//시작 페이드 아웃
	static int Start;
	static int xPos, yPos;

	//마우스 좌표
	int mx, my;

	//오브젝트 카운트
	static int count_oj;
	static int TV_count;
	static int Time;
	static int NUM;
	static int Jump;
	static BOOL Select;
	static BOOL Select_NUM;
	static BOOL TV_NUM;
	
	PrintBitMap printBitMap;

	switch (iMsg)
	{
	case WM_CREATE:
		//메인메뉴 이미지 로드
		Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MENU_BIT));
		//메뉴바 로드
		hMenu = GetMenu(hwnd);
		hSubMenu = GetSubMenu(hMenu, 0);
		//텍스트
		xPos = 1910;
		yPos = -150;

		//카운트
		Start = 0;
		count_oj = 0;
		Select = FALSE;
		Select_NUM = FALSE;
		NUM = 0;
		Jump = -200;
		TV_NUM = FALSE;
		TV_count = 0;
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Object_dc = CreateCompatibleDC(hdc);
		//메인 메뉴 이미지 로드
		MainDC = CreateCompatibleDC(hdc);
		SelectObject(MainDC, Main_BG);
		BitBlt(hdc, 0, 0, 1920, 1080, MainDC, 0, 0, SRCCOPY);
		SelectObject(MainDC, Main_BG);

		//게임을 시작하면 로고를 이동시킵니다.
		if (Start == 1)
		{
			//메인 백그라운드를 검은색으로 변경합니다.
			Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FAIDOUT));
			Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DAY_LOGO));
			Sub_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POSTION_LOGO));
			printBitMap.Load_Starting_Logo(hwnd, hdc, MainDC, Main_Object, Sub_Object, xPos, yPos);
		}

		if (Start >= 2)
		{
			//메인 백그라운드를 1스테이지 백그라운드로 변경합니다.
			printBitMap.Reset_BackGround(hInst, hdc, MainDC, Main_BG);
		
			switch(count_oj)
			{
			case 0:
				//배경화면을 되돌립니다.
				if (Time == 2)
					printBitMap.Jump_Scare(hInst, hdc, Time);
				else if (Time > 4)
					printBitMap.Jump_Scare(hInst, hdc, Time);
				else
					printBitMap.Reset_BackGround(hInst, hdc, MainDC, Main_BG);
				break;
				
			case 1:
				//십자가 사진을 로드합니다.
				Size = { 540,910,690,0 };
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				//표백제를 획득하면 십자가에 사용할수 있습니다.
				if (Select_NUM)
				{
					Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_CROSS));
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUM1_TEXT));
					if (NUM == 1)
					{
						Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_CROSSNUM));
						h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USEOX_TEXT));
						NUM++;
						Select_NUM = FALSE;
					}
				}
				//표백제를 사용하면 더이상 인카운트가 발생하지 않습니다.
				else if (NUM == 2)
				{
					Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_CROSSNUM));
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NOTHING_TEXT));
				}
				//기본 십자가 이미지 입니다.
				else
				{
					Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_CROSS));
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CROSS_TEXT1));
				}
				printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 2:
				//쪽지 사진을 로드합니다.
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Size = { 1023,682,442,199 };
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_PAPER));
				printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 3:
				//독병 사진을 로드합니다.
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Size = { 630,856,639,12 };
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_POSION));
				h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POSION_TEXT));
				printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 4:
				//오래된 상자 사진을 로드합니다.
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Size = { 900,486,504,288 };
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_BOX));
				//표백제를 획득하면 상자인카운트가 사라집니다
				if (Select)
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NOTHING_TEXT));
				//기본 상자 이미지 입니다.
				else
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOX_TEXT));
				printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 5:
				//표백제 이미지 로드합니다.
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Size = { 855,802,526,39 };
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_OX));
				h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_OX_TEXT));
				printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 6:
				//오래된 tv를 로드합니다.
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Size = { 1228,854,340,13 };
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_TV));
				if (Select == TRUE && NUM == 2)
				{
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TVNUMS_TEXT));
					if (TV_NUM)
					{
						SetTimer(hwnd, 2, 1000, NULL);
						DeleteObject(h_text);
						Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_OUT));
						SelectObject(MainDC, Main_BG);
						printBitMap.Animation(hInst, hdc, MainDC, TV_count);
						if (TV_count == 8)
						{
							KillTimer(hwnd, 2);
							printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
							Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_OUT));
							h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HEAD_TEXT1));
							printBitMap.BackGround_Text(hInst, hdc, Main_BG, h_text);

							if (Start == 3)
							{
								printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
								count_oj = 7;
								TV_count++;
								InvalidateRgn(hwnd, NULL, TRUE);
							}
						}
					}
				}
				else
					h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TV_TEXT));
					printBitMap.Load_Object(hInst, hdc, MainDC, Object_dc, Main_Object, Main_BG, h_text, Size);
				break;

			case 7:
				//인형 머리를 떨어트립니다.
				DeleteDC(MainDC);
				Size = { 860,693,0,0 };
				SetTimer(hwnd, 3, 3, NULL);
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_HEAD));
				printBitMap.Double_buffering(hwnd, hInst, hdc, Main_Object, Main_BG, Size, Jump);
				if (Start >= 5)
				{
					count_oj = 8;
				}
				break;

			case 8:
				//마지막 엔딩입니다.
				Jump = 0;
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Main_BG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_1STAGE_OUT));
				h_text = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_END_TEXT));
				printBitMap.BackGround_Text(hInst, hdc, Main_BG, h_text);

				if (Start >= 7)
				{
					count_oj = 9;
				}
				break;
			case 9:
				printBitMap.Reset_Object(Main_Object, Sub_Object, h_text);
				Main_Object = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FINAL1));
				SelectObject(MainDC, Main_Object);
				BitBlt(hdc, 0, -50, 1920, 1080, MainDC, 0, 0, SRCCOPY);
				if (Start >= 9)
					PostQuitMessage(0);
				break;
			}
		}
		DeleteDC(MainDC);
		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			Start++;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//힌트 메뉴 처리
		case ID_HINT:
			switch (count_oj)
			{
			case 0:
				MessageBox(hwnd, _T("물건을 확인했을때만 볼수 있을꺼 같아..."), _T("힌트 수첩"), MB_OK);
				break;
			case 1:
				if(Select_NUM)
					MessageBox(hwnd, _T("여기에 표백제를 사용하면 될꺼같아.."), _T("힌트 수첩"), MB_OK);
				else if(NUM == 1)
					MessageBox(hwnd, _T("고민할 필요가 있나?"), _T("힌트 수첩"), MB_OK);
				else if(NUM == 2)
					MessageBox(hwnd, _T("설마 여기에 뭐가 더 있을꺼라고 생각하는건 아니지??"), _T("힌트 수첩"), MB_OK);
				else
					MessageBox(hwnd, _T("주변에 뭔가 있을꺼야.. 숨기고 싶어하는것 같으니 상자 안에 있을려나..?"), _T("힌트 수첩"), MB_OK);
				break;
			case 2:
				MessageBox(hwnd, _T("이 집주인의 성격을 알것같군..편집증에..물건을 숨기는걸 좋아하는건가??"), _T("힌트 수첩"), MB_OK);
				break;
			case 3:
				MessageBox(hwnd, _T("괜찮아 굳이 숫자를 기억할 필요는 없어"), _T("힌트 수첩"), MB_OK);
				break;
			case 4:
				if(Select)
					MessageBox(hwnd, _T("설마 여기에 뭐가 더 있을꺼라고 생각하는건 아니지??"), _T("힌트 수첩"), MB_OK);
				else
					MessageBox(hwnd, _T("고민하지 말고 열어보자..뭐가 튀어나오기라도 하겠어?"), _T("힌트 수첩"), MB_OK);
				break;
			case 5:
				MessageBox(hwnd, _T("이걸 십자가에 사용할 수 있을꺼 같아."), _T("힌트 수첩"), MB_OK);
				break;
			case 6:
				MessageBox(hwnd, _T("이 티비에도 뭔가를 숨겨두었을꺼 같아..뭔지 찾아봐야겠어"), _T("힌트 수첩"), MB_OK);
				break;
			}

			break;

		//프로그램 종료 메뉴 처리
		case ID_EXIT:
			int s = MessageBox(hwnd, _T("정말 도망칠꺼야?"), _T("겁쟁이"), MB_OKCANCEL);
			if (s == IDOK)
			{
				PostQuitMessage(0);
				break;
			}
			else
				InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		break;

	case WM_TIMER:
		if (Start == 1)
		{
			xPos -= 20;
			yPos += 5;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		if (count_oj == 6)
			TV_count++;
		if (count_oj == 7)
			Jump += 50;
		if (count_oj == 8 && Start >= 7)
			Jump += 50;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		//return 좌표 위치
		if (count_oj > 0)
		{
			if (0 <= mx && mx <= 380)
				if (0 <= my && my <= 80)
				{
					count_oj = 0;
					Time++;
				}
		}
		//귀신 좌표 위치
		if (880 <= mx && mx <= 1082)
			if (253 <= my && my <= 458)
				Time++;
		//십자가 좌표 위치
		if (720 <= mx && mx <= 770)
			if (300 <= my && my <= 400)
				count_oj = 1;
		//쪽지 좌표 위치
		if (1228 <= mx && mx <= 1254)
			if (795 <= my && my <= 817)
				count_oj = 2;
		//독병 좌표 위치
		if (115 <= mx && mx <= 303)
			if (429 <= my && my <= 549)
				count_oj = 3;
		//상자 좌표 위치
		if (193 <= mx && mx <= 409)
			if (870 <= my && my <= 1001)
				count_oj = 4;
		//상자 열기 선택 좌표 위치
		if (count_oj == 4)
		{
			//Yes 선택
			if (1291 <= mx && mx <= 1430)
				if (898 <= my && my <= 1005)
				{
					count_oj = 5;
					Select_NUM = Select = TRUE;
				}
			//no 선택
			if (1559 <= mx && mx <= 1684)
				if (903 <= my && my <= 1001)
					count_oj = 0;
		}
		//표백제 사용 선택 좌표 위치
		if (count_oj == 1)
		{
			//Yes 선택
			if (1388 <= mx && mx <= 1521)
				if (900 <= my && my <= 983)
				{
					NUM++;
					Select_NUM = TRUE;
				}
			//NO 선택
			if (1608 <= mx && mx <= 1735)
				if (906 <= my && my <= 988)
					count_oj = 0;
		}
		//tv좌표 위치
		if (1593 <= mx && mx <= 1866)
			if (559 <= my && my <= 769)
				count_oj = 6;
		//tv다이얼 선택 좌표 위치
		if (count_oj == 6)
		{	
			//yes 선택
			if (1228 <= mx && mx <= 1364)
				if (906 <= my && my <= 995)
					TV_NUM = TRUE;
			//NO 선택
			if (1455 <= mx && mx <= 1574)
				if (908 <= my && my <= 988)
					count_oj = 0;		
		}
			
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		DeleteObject(Main_Object);
		DeleteObject(Sub_Object);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLien, int iCmdShow)
{
	HWND hwnd;
	MSG msg;
	HACCEL hAcc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN_AC));
	hInst = hInstance;
	WNDCLASSEX ws;
	ws.cbSize = sizeof(ws);
	ws.style = CS_HREDRAW | CS_VREDRAW;
	ws.lpfnWndProc = WndProc;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hInstance = hInstance;
	ws.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ws.hCursor = LoadCursor(NULL, IDC_ARROW);
	ws.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	ws.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
	ws.lpszClassName = _T("MyFirstGame");
	ws.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&ws);

	hwnd = CreateWindow(
		_T("MyFirstGame"),
		_T("A CRUSED HOUSE"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1920,
		1080,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAcc, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}