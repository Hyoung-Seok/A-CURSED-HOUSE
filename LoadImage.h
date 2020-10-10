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

	//��Ÿ�� ��Ʈ�� �ε� �Լ��Դϴ�.
	void Load_Starting_Logo(HWND& hwnd, HDC& hdc, HDC& mainDC, HBITMAP& Main_oj, HBITMAP& Sub_oj, int xpos, int ypos);

	//��� ���� ������Ʈ ���
	void Load_Object(HINSTANCE& hInst, HDC& hdc, HDC& Main_DC, HDC& OJ_DC, HBITMAP& Main_object, HBITMAP& Main_BG, HBITMAP& h_Text, RECT& Size);
	
	//��� ȭ���� �ǵ����ϴ�.
	void Reset_BackGround(HINSTANCE& hInst, HDC& hdc, HDC& MainDC, HBITMAP& Main_BG);

	//���� ���ɾ� ����
	void Jump_Scare(HINSTANCE& hInst, HDC& hdc, int time);

	//����� ������Ʈ�� �����մϴ�.
	void Reset_Object(HBITMAP& Main_oj, HBITMAP& Sub_oj, HBITMAP& text_oj);

	//�ִϸ��̼�
	void Animation(HINSTANCE& hInst, HDC& hdc, HDC& main_dc, int count);

	//���� ���۸�
	void Double_buffering(HWND& hwnd, HINSTANCE& hInst, HDC& hdc, HBITMAP& object, HBITMAP& Main_bg, RECT& size, int yPos);

	//���� �̹����� ����մϴ�.
	void BackGround_Text(HINSTANCE& hInst, HDC& hdc, HBITMAP& Main_BG, HBITMAP& h_text);
};
#endif // !LOADIMAGE_H
