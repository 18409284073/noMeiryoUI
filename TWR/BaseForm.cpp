#include "stdafx.h"
#include "BaseForm.h"

/**
 * �E�C���h�E�̃x�[�X�N���X
 */

/**
 * �R���X�g���N�^
 */
BaseForm::BaseForm()
{
	windowClass = _T("");
	title = _T("");
	style = WS_OVERLAPPEDWINDOW;
	left = CW_USEDEFAULT;
	top = CW_USEDEFAULT;
	width = CW_USEDEFAULT;
	height = 0;
}

/**
 * �f�X�g���N�^
 */
BaseForm::~BaseForm()
{
}

/**
 * �E�C���h�E���쐬���܂��B
 *
 * @param �C���X�^���X�n���h��
 * @return �E�C���h�E�n���h��
 */
HWND BaseForm::Create(HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	hWnd = CreateWindow(
		windowClass,
		title,
		style,
		left,
		top,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hWnd;
}

/**
 * �E�C���h�E�v���[�V�W��
 *
 * @param hDlg �E�C���h�E�n���h��
 * @param message ���b�Z�[�W
 * @param wParam WPARAM
 * @param lParam LPARAM
 * @return ��������
 */
LRESULT CALLBACK BaseForm::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	LPNMHDR nmhdr;

	this->hWnd = hWnd;
	this->message = message;
	this->wParam = wParam;
	this->lParam = lParam;

	switch (message)
	{
		case WM_CREATE:
			return OnCreate();
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);

			return OnCommand(wmId, wmEvent);
		case WM_NOTIFY:
			nmhdr = (LPNMHDR)lParam;

			return OnNotify(nmhdr);
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: �`��R�[�h�������ɒǉ����Ă�������...
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			return OnDestroy();
		case WM_SYSCOLORCHANGE:	// �V�X�e���F���ύX�ɂȂ����B
			// ��N���C�A���g�̈�(���j���[�o�[��^�C�g���o�[)���X�V���邽�߁A������g���B
			RedrawWindow(hWnd,NULL,NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/**
 * �E�C���h�E�X�^�C����ݒ肵�܂��B
 *
 * @param newStyle �E�C���h�E�X�^�C��
 */
void BaseForm::setStyle(DWORD newStyle)
{
	style = newStyle;
	if (hWnd != NULL) {
		SetWindowLongPtr(hWnd, GWL_STYLE, newStyle);
	}
}

/**
 * �E�C���h�E�ƃE�C���h�E�I�u�W�F�N�g�̃f�[�^�̓������Ƃ�܂��B
 *
 * @param toObj true:�_�C�A���O�̓��e���I�u�W�F�N�g�Ɋi�[����B
 *              false:�I�u�W�F�N�g�̓��e���_�C�A���O�Ɋi�[����B
 */
void BaseForm::UpdateData(bool toObj)
{
}

/**
 * WM_COMMAND���b�Z�[�W���������̏������s���܂��B
 *
 * @param wmId 
 * @param wmEvent
 * @return ���b�Z�[�W��������
 */
LRESULT BaseForm::OnCommand(int wmId, int wmEvent)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
 * �E�C���h�E�̏�ɏ��E�C���h�E�I�u�W�F�N�g��ǉ����܂��B
 *
 * @param �E�C���h�E�I�u�W�F�N�g
 * @return �e�ɂȂ����E�C���h�E�̃n���h��
 */
HWND BaseForm::add(TwrWnd *obj)
{
	return ::SetParent(obj->getHwnd(), hWnd);
}

/**
 * �E�C���h�E�̏�ɏ��_�C�A���O�I�u�W�F�N�g��ǉ����܂��B
 *
 * @param �_�C�A���O�I�u�W�F�N�g
 * @return �e�ɂȂ����E�C���h�E�̃n���h��
 */
HWND BaseForm::add(BaseDialog *obj)
{
	obj->setParent(hWnd);
	return ::SetParent(obj->getHwnd(), hWnd);
}

/**
 * �E�C���h�E���쐬���ꂽ�Ƃ��ɌĂяo����܂��B
 *
 * @return 0
 */
LRESULT BaseForm::OnCreate()
{
	return 0;
}

/**
 * �R�����R���g���[���ŃC�x���g�����������Ƃ��ɌĂяo����܂��B
 *
 * @param nmhdr �ʒm���b�Z�[�W���
 * @return ���b�Z�[�W��������
 */
LRESULT BaseForm::OnNotify(LPNMHDR nmhdr)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**
 * �E�C���h�E�����ۂɌĂяo����܂��B
 *
 * @return 0
 */
LRESULT BaseForm::OnDestroy()
{
	PostQuitMessage(0);

	return 0;
}

