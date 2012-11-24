#include "stdafx.h"
#include "AppliBase.h"
#include "TwrButton.h"

/**
 * �R���X�g���N�^
 */
TwrButton::TwrButton() : TwrWnd()
{
}

/**
 * �{�^�����쐬���܂��B
 *
 * @param id ���j���[ID
 */
HWND TwrButton::create(HMENU id)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_MULTILINE|BS_CENTER|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		GetDesktopWindow(),	// Parent
		id,
		hInst,
		NULL);

	return hWnd;
}

/**
 * �{�^�����쐬���܂��B
 *
 * @param parent �e�E�C���h�E�n���h��
 * @param id ���j���[ID
 */
HWND TwrButton::create(HWND parent, HMENU id)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_MULTILINE|BS_CENTER|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		id,
		hInst,
		NULL);

	return hWnd;
}

