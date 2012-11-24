#include "stdafx.h"
#include "AppliBase.h"
#include "TwrGroupBox.h"

/**
 * �R���X�g���N�^
 */
TwrGroupBox::TwrGroupBox() : TwrWnd()
{
}

/**
 * �O���[�v�{�b�N�X���쐬���܂��B
 */
HWND TwrGroupBox::create()
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
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
 * �O���[�v�{�b�N�X���쐬���܂��B
 *
 * @param parent �e�E�C���h�E�n���h��
 */
HWND TwrGroupBox::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
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
