#pragma once
#ifndef LOADIMAGE_H

class PrintBitMap
{
public:
	HBITMAP om_buffer, os_buffer, bg_buffer;
	HDC main_buffer, sub_buffer;
    int xPos, yPos;

	//Double buffer
	HDC sub2_buffer;
	HBITMAP Test_hbit, sub_backGround;

	//스타팅 비트맵 로드 함수입니다.
	void Load_Starting_Logo(HWND& hwnd, HDC& hdc, HDC& mainDC, HBITMAP& Main_oj, HBITMAP& Sub_oj, int xpos, int ypos);

	//배경 위에 오브젝트 출력
	void Load_Object(HINSTANCE& hInst, HDC& hdc, HDC& Main_DC, HDC& OJ_DC, HBITMAP& Main_object, HBITMAP& Main_BG, HBITMAP& h_Text, RECT& Size);
	
	//배경 화면을 되돌립니다.
	void Reset_BackGround(HINSTANCE& hInst, HDC& hdc, HDC& MainDC, HBITMAP& Main_BG);

	//점프 스케어 출현
	void Jump_Scare(HINSTANCE& hInst, HDC& hdc, int time);

	//저장된 오브젝트를 삭제합니다.
	void Reset_Object(HBITMAP& Main_oj, HBITMAP& Sub_oj, HBITMAP& text_oj);

	//애니메이션
	void Animation(HINSTANCE& hInst, HDC& hdc, HDC& main_dc, int count);

	//더블 버퍼링
	void Double_buffering(HWND& hwnd, HINSTANCE& hInst, HDC& hdc, HBITMAP& object, HBITMAP& Main_bg, RECT& size, int yPos);

	//배경과 이미지만 출력합니다.
	void BackGround_Text(HINSTANCE& hInst, HDC& hdc, HBITMAP& Main_BG, HBITMAP& h_text);
};
#endif // !LOADIMAGE_H
