#include "stdafx.h"
#include "AppliBase.h"
#include "TwrText.h"

/**
 * �R���X�g���N�^
 */
TwrText::TwrText() : TwrWnd()
{
}

/**
 * �X�^�e�B�b�N�e�L�X�g���쐬���܂��B
 *
 * @return �쐬�����X�^�e�B�b�N�e�L�X�g�̃E�C���h�E�n���h��
 */
HWND TwrText::create()
{
	hWnd = CreateWindow(
		_T("STATIC"),
		title.c_str(),
		WS_CHILD | WS_VISIBLE,
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
 * �X�^�e�B�b�N�e�L�X�g���쐬���܂��B
 *
 * @param parent �e�E�C���h�E�̃n���h��
 * @return �쐬�����X�^�e�B�b�N�e�L�X�g�̃E�C���h�E�n���h��
 */
HWND TwrText::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("STATIC"),
		title.c_str(),
		WS_CHILD | WS_VISIBLE,
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
