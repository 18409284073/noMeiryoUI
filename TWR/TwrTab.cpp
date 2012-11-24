#include "stdafx.h"
#include <string.h>
#include "CommCtrl.h"

#include "AppliBase.h"
#include "TwrTab.h"

/**
 * �R���X�g���N�^
 */
TwrTab::TwrTab() : TwrWnd()
{
}

/**
 * �^�u�R���g���[�����쐬���܂��B
 *
 * @return �쐬�����^�u�R���g���[���̃E�C���h�E�n���h��
 */
HWND TwrTab::create()
{
	hWnd = CreateWindowEx(
		WS_EX_LEFT,
		WC_TABCONTROL,
		title.c_str(),
		WS_CHILD|WS_VISIBLE,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		(HMENU)IDC_STATIC,
		hInst,
		NULL);

	return hWnd;
}

/**
 * �^�u�R���g���[�����쐬���܂��B
 *
 * @param parent �e�E�C���h�E�n���h��
 * @return �쐬�����^�u�R���g���[���̃E�C���h�E�n���h��
 */
HWND TwrTab::create(HWND parent)
{
	hWnd = CreateWindowEx(
		WS_EX_LEFT,
		WC_TABCONTROL,
		title.c_str(),
		WS_CHILD|WS_VISIBLE,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		(HMENU)IDC_STATIC,
		hInst,
		NULL);

	return hWnd;
}

/**
 * �^�u�R���g���[����ǉ����܂��B
 *
 * @param title �^�u�̃^�C�g��
 */
void TwrTab::add(TCHAR *title)
{
	TCITEM item;

	memset(&item, 0, sizeof(TCITEM));
	item.mask = TCIF_TEXT;
	item.pszText = title;
	item.cchTextMax = (int)((_tcslen(title) + 1) * sizeof(TCHAR));

	INT_PTR count = SendMessage(hWnd, TCM_GETITEMCOUNT, 0 ,0);
	::SendMessage(hWnd, TCM_INSERTITEM, (WPARAM)count, (LPARAM)&item);
}

/**
 * �I�����ꂽ�^�u�̈ʒu��Ԃ��܂��B
 *
 * @return > -1:�I�����ꂽ�^�u�̈ʒu -1:�I������Ă��Ȃ�
 */
int TwrTab::getSelectedIndex(void)
{
	int count = (int)SendMessage(hWnd, TCM_GETCURSEL, 0, 0);

	return count;
}

/**
 * �^�u��I�����܂��B
 *
 * @param index �I������^�u�̈ʒu
 */
void TwrTab::select(int index)
{
	SendMessage(hWnd, TCM_SETCURSEL, (WPARAM)index, 0);

}

