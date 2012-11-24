// AppliBase.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "commctrl.h"

#include "TwrCore.h"
#include "AppliBase.h"
#include "ModelessDialog.h"
#include "BaseForm.h"

// �A�v���P�[�V�����ŗL�̃w�b�_�t�@�C��
#include "../MyAppli.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
BaseForm *baseForm;

// �A�v���P�[�V�����ŗL�̃O���[�o���ϐ�
MyAppli *appObj;

extern void initModelessCallback(void);

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow);
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
bool handleDialogMessage(LPMSG lpMsg);

/**
 * �A�v���P�[�V�������C�����[�`��
 *
 * @param hInstance �A�v���P�[�V�����̃C���X�^���X�n���h��
 * @param hPrevInstance ���g�p
 * @param lpCmdLine �R�}���h���C��
 * @param nCmdShow �\�����
 * @return �v���O�����I���R�[�h
 */
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = ICC_TAB_CLASSES;
	if (!InitCommonControlsEx(&iccx)) {
         return FALSE;
	}

	initModelessDialog();
	initModelessCallback();

	// TODO: �����ɃA�v���P�[�V�����N���X�̃C���X�^���X��ݒ肵�Ă��������B
	appObj = new MyAppli();
	baseForm = appObj->createBaseWindow();

	// �E�C���h�E�\���O�̃A�v���P�[�V�����̏��������s���B
	appObj->OnAppliStart(lpCmdLine);

	int result = runApplication(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// �A�v���P�[�V�����̌�n�����s���B
	appObj->OnAppliEnd();

	delete baseForm;
	delete appObj;
	return result;
}

/**
 * �A�v���P�[�V�����J�n
 *
 * @param hInstance �A�v���P�[�V�����̃C���X�^���X�n���h��
 * @param hPrevInstance ���g�p
 * @param lpCmdLine �R�}���h���C��
 * @param nCmdShow �\�����
 * @return �v���O�����I���R�[�h
 */
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BOOKSYNC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APP_ACCEL));
	doEvents();

	// �E�C���h�E�\������ɍs���������s���B
	appObj->OnWindowShow();

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (handleDialogMessage(&msg)) {
			// ���b�Z�[�W�����[�h���X�_�C�A���O�̂��̂������ꍇ��
			// �������Ȃ��B
			continue;
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.lpszClassName	= szWindowClass;

	// �A�v���P�[�V�����ŗL�̏��
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	// wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BOOKSYNC);
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   baseForm->setClass(szWindowClass);
   baseForm->setTitle(szTitle);
   hWnd = baseForm->Create(hInstance);
   if (!hWnd)
   {
      return FALSE;
   }

   baseForm->ShowWindow(nCmdShow);
   baseForm->UpdateWindow();

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = baseForm->WndProc(hWnd, message, wParam, lParam);

	return lResult;
}

/**
 * Windows�̃C�x���g����������B
 *
 * @return �C�x���g��������
 */
int doEvents()
{
	MSG msg;
    // ���b�Z�[�W�̏���
    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet > 0){
			if (handleDialogMessage(&msg)) {
				// ���b�Z�[�W�����[�h���X�_�C�A���O�̂��̂������ꍇ��
				// �������Ȃ��B
				continue;
			} else {
				::TranslateMessage(&msg);
				::DispatchMessage (&msg);
			}
		} else {
			break;
		}
    }
	return 0;
}

