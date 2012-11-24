#include "stdafx.h"
#include "AppliBase.h"
#include "TwrListbox.h"

/**
 * �R���X�g���N�^
 */
TwrListbox::TwrListbox() : TwrWnd()
{
}

/**
 * �R���X�g���N�^
 *
 * @param �I�u�W�F�N�g�Ɍ��ѕt����E�C���h�E�̃n���h��
 */
TwrListbox::TwrListbox(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}


/**
 * ���X�g�{�b�N�X���쐬���܂��B
 */
HWND TwrListbox::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("LISTBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		NULL,
		hInst,
		NULL);

	return hWnd;
}

/**
 * ���X�g�{�b�N�X�ɗv�f��ǉ����܂��B
 *
 * @param item �ǉ����镶����
 */
void TwrListbox::addItem(TCHAR *item)
{
	SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)item);
}

/**
 * �I�����ꂽ�v�f�̈ʒu���擾���܂��B
 *
 * @return -1:�I������Ă��Ȃ� -1�ȊO:�I�����ꂽ�ꏊ
 */
int TwrListbox::getSelectedIndex(void)
{
	LRESULT result = SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	if (result == LB_ERR) {
		return -1;
	} else {
		return (int)result;
	}
}

/**
 * ���X�g�{�b�N�X�ɗv�f��}�����܂��B
 *
 * @param index �}������ʒu
 * @param item �ǉ����镶����
 */
void TwrListbox::insertItem(int index, TCHAR *item)
{
	SendMessage(hWnd, LB_INSERTSTRING , (WPARAM)index, (LPARAM)item);
}

/**
 * ���X�g�{�b�N�X�̗v�f���폜���܂��B
 *
 * @param index �폜����v�f�̈ʒu
 */
void TwrListbox::removeItem(int index)
{
	SendMessage(hWnd, LB_DELETESTRING , (WPARAM)index, (LPARAM)0);
}

/**
 * ���X�g�{�b�N�X�̗v�f��I�����܂��B
 *
 * @param index �I������v�f�̈ʒu
 */
void TwrListbox::selectItem(int index)
{
	SendMessage(hWnd, LB_SETCURSEL , (WPARAM)index, (LPARAM)0);
}

