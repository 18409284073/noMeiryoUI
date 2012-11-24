#include "stdafx.h"
#include "AppliBase.h"
#include "TwrCheckbox.h"

/**
 * �R���X�g���N�^
 */
TwrCheckbox::TwrCheckbox() : TwrWnd()
{
}

/**
 * �`�F�b�N�{�b�N�X���쐬���܂��B
 */
HWND TwrCheckbox::create(void)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|WS_TABSTOP,
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
 * �`�F�b�N�{�b�N�X���쐬���܂��B
 *
 * @param parent �e�E�C���h�E�n���h��
 */
HWND TwrCheckbox::create(HWND parent)
{
	hWnd = CreateWindow(
		_T("BUTTON"),
		title.c_str(),
		WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|WS_TABSTOP,
		left,	// X
		top,	// Y
		width,	// Width
		height,	// Height
		parent,	// Parent
		NULL,
		hInst,
		NULL);

	return hWnd;
}

/**
 * �`�F�b�N��Ԃ�Ԃ��܂��B
 *
 * @return true:�`�F�b�N���ꂽ false:�`�F�b�N����Ă��Ȃ�
 */
bool TwrCheckbox::isChecked(void)
{
	LRESULT result = SendMessage(hWnd, BM_GETCHECK, 0, 0);
	if (result == BST_CHECKED) {
		return true;
	} else {
		return false;
	}
}

/**
 * �`�F�b�N��Ԃ�ݒ肵�܂��B
 *
 * @param checked true:�`�F�b�N���ꂽ false:�`�F�b�N����Ă��Ȃ�
 */
void TwrCheckbox::setChecked(bool checked)
{
	if (checked) {
		SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
	} else {
		SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
	}
}
