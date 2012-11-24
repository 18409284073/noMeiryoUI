#pragma once

#include <windows.h>
#include "TwrCore.h"
#include "BaseForm.h"
#include "../resource.h"

// �A�v���P�[�V�������ʒ�`

void addModelessDialog(HWND dialog);
void delModelessDialog(HWND dialog);

int doEvents();

class WindowAppliBase : public BaseForm {
public:
	virtual ~WindowAppliBase();
	virtual BaseForm *createBaseForm();
	virtual int OnAppliStart(TCHAR *lpCmdLine);
	virtual int OnWindowShow();
	virtual int OnAppliEnd();

};

// �E�C���h�E�A�v�����ʒ�`

#define MAX_LOADSTRING 100

extern TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

WindowAppliBase *createAppli();
ATOM	MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
