/*
This file is part of noMeiryoUI
Copyright (C) 2005,2012,2013 Tatsuhiko Shoji

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "stdafx.h"
#include "AppliBase.h"
#include "TwrCombobox.h"

/**
 * �R���X�g���N�^
 */
TwrCombobox::TwrCombobox() : TwrWnd()
{
	style = CBS_DROPDOWNLIST;
}

/**
 * �R���X�g���N�^
 *
 * @param �I�u�W�F�N�g�Ɍ��ѕt����E�C���h�E�̃n���h��
 */
TwrCombobox::TwrCombobox(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}

/**
 * �R���{�{�b�N�X���쐬���܂��B
 */
HWND TwrCombobox::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("COMBOBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|style,
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
 * �R���{�{�b�N�X���쐬���܂��B
 *
 * @param parent �e�E�C���h�E�n���h��
 */
HWND TwrCombobox::create(HWND parent)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("COMBOBOX"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|style,
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
 * �R���{�{�b�N�X�ɗv�f��ǉ����܂��B
 *
 * @param item �ǉ����镶����
 */
void TwrCombobox::addItem(const TCHAR *item)
{
	SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)item);
}

/**
 * �I�����ꂽ�v�f�̈ʒu���擾���܂��B
 *
 * @return -1:�I������Ă��Ȃ� -1�ȊO:�I�����ꂽ�ꏊ
 */
int TwrCombobox::getSelectedIndex(void)
{
	LRESULT result = SendMessage(hWnd, CB_GETCURSEL, 0, 0);
	if (result == CB_ERR) {
		return -1;
	} else {
		return (int)result;
	}
}

/**
 * �R���{�{�b�N�X���N���A���܂��B
 */
void TwrCombobox::clear(void)
{
	SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
}

/**
 * �w�肵���ʒu�̗v�f��I�����܂��B
 *
 * @param index �I������ʒu
 */
void TwrCombobox::setSelectedIndex(int index)
{
	SendMessage(hWnd, CB_SETCURSEL, index, 0);
}

/**
 * �I�������ʒu�̕������Ԃ��܂��B
 *
 * @return �I�������ʒu�̕�����(�I������ĂȂ��ꍇ�͋�)
 */
tstring TwrCombobox::getSelectedText(void)
{
	TCHAR *buf;
	tstring ret = _T("");
	LRESULT len;

	int item = getSelectedIndex();
	if (item < 0) {
		return ret;
	}

	len = SendMessage(hWnd, CB_GETLBTEXTLEN, item, 0);
	try {
		buf = new TCHAR[len + 1];
		SendMessage(hWnd, CB_GETLBTEXT, item, (LPARAM)buf);

		ret = buf;
		delete []buf;
	} catch (...) {
	}

	return ret;	
}

