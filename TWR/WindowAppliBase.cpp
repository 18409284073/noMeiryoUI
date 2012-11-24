// AppliBase.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "commctrl.h"

#include "TwrCore.h"
#include "WindowAppliBase.h"
#include "ModelessDialog.h"

// �O���[�o���ϐ�:
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
BaseForm *baseForm;

// �A�v���P�[�V�����ŗL�̃O���[�o���ϐ�
WindowAppliBase *appObj;

extern void initModelessCallback(void);

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
int runApplication(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow);
BOOL				InitInstance(HINSTANCE, int);
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

	// �����ŃA�v���P�[�V�����N���X�̃C���X�^���X��ݒ肵�܂��B
	appObj = createAppli();
	baseForm = appObj->createBaseForm();

	// �E�C���h�E�\���O�̃A�v���P�[�V�����̏��������s���B
	appObj->OnAppliStart(lpCmdLine);

	int result = runApplication(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// �A�v���P�[�V�����̌�n�����s���B
	appObj->OnAppliEnd();

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
	LoadString(hInstance, IDS_APP_CLASS, szWindowClass, MAX_LOADSTRING);
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

/**
 * �f�X�g���N�^
 */
WindowAppliBase::~WindowAppliBase()
{
}

/**
 * �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X�𐶐����܂��B
 *
 * @return �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X
 */
BaseForm *WindowAppliBase::createBaseForm()
{
	return new BaseForm();
}

/**
 * �A�v���P�[�V�����̊J�n���������s���܂��B
 *
 * @return �\��
 */
int WindowAppliBase::OnAppliStart(TCHAR *lpCmdLine)
{
	// ���̊֐����I�[�o�[���C�h���ăA�v���ŗL�̏��������s���܂��B
	return 0;
}

/**
 * �E�C���h�E���\�����ꂽ�Ƃ��̏��������s���܂��B
 *
 * @return �\��
 */
int WindowAppliBase::OnWindowShow()
{
	// ���̊֐����I�[�o�[���C�h���āA����̕\�����̏������s���܂��B
	return 0;
}

/**
 * �A�v���P�[�V�����̏I�����������s���܂��B
 *
 * @return �\��
 */
int WindowAppliBase::OnAppliEnd()
{
	// ���̊֐����I�[�o�[���C�h���ăA�v���ŗL�̌㏈�����s���܂��B
	return 0;
}
