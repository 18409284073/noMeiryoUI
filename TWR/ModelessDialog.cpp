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
#include "ModelessDialog.h"

HWND modelessDialogHandle[10];
int modelessDialogHandles;

/**
 * ���[�h���X�_�C�A���O�֘A�̏��������s���B
 */
void initModelessDialog(void)
{
	for (int i = 0; i < 10; i++) {
		modelessDialogHandle[i] = NULL;
	}
	modelessDialogHandles = 0;
}

/**
 * ���[�h���X�_�C�A���O��ǉ�����B
 *
 * @param dialog �_�C�A���O�̃E�C���h�E�n���h��
 */
void addModelessDialog(HWND dialog)
{
	bool found = false;
	int empty = -1;
	for (int i = 0; i < 10; i++) {
		if (modelessDialogHandle[i] == dialog) {
			return;
		}
		if ((modelessDialogHandle[i] == NULL) && (empty == -1)){
			empty = i;
		}
	}
	if (empty > -1) {
		modelessDialogHandle[empty] = dialog;
		modelessDialogHandles++;
	} else {
		modelessDialogHandle[modelessDialogHandles] = dialog;
		modelessDialogHandles++;
	}
}

/**
 * ���[�h���X�_�C�A���O���A�v���P�[�V�����ɂ�鐧�䂩��O���B
 *
 * @param dialog �_�C�A���O�̃E�C���h�E�n���h��
 */
void delModelessDialog(HWND dialog)
{
	bool found = false;
	int empty = -1;
	for (int i = 0;i < 10; i++) {
		if (modelessDialogHandle[i] == dialog) {
			modelessDialogHandle[i] = NULL;
			modelessDialogHandles--;
			return;
		}
	}
}

/**
 * ���[�h���X�_�C�A���O�̃_�C�A���O���b�Z�[�W�n���h���[
 *
 * @param lpMsg �_�C�A���O���b�Z�[�W
 * @return true:���b�Z�[�W���������� false:���b�Z�[�W���������Ȃ�
 */
bool handleDialogMessage(LPMSG lpMsg)
{
	HWND hDlg;

	for (int i = 0; i < 10; i++) {
		hDlg = modelessDialogHandle[i];
		if (hDlg == NULL) {
			continue;
		}
		if (IsDialogMessage(hDlg, lpMsg)) {
			return true;
		}
	}
	return false;
}

