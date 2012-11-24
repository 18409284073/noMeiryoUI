#include "stdafx.h"

#include "commctrl.h"
#include "AppliBase.h"
#include "TwrListView.h"

/**
 * �R���X�g���N�^
 */
TwrListView::TwrListView() : TwrWnd()
{
}

/**
 * �R���X�g���N�^
 *
 * @param �I�u�W�F�N�g�Ɍ��ѕt����E�C���h�E�̃n���h��
 */
TwrListView::TwrListView(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}


/**
 * ���X�g�r���[���쐬���܂��B
 *
 * @return �쐬�������X�g�r���[�̃E�C���h�E�n���h��
 */
HWND TwrListView::create(void)
{
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		_T("LISTVIEW"),
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
 * ���X�g�r���[�ɗv�f��}�����܂��B
 *
 * @param index �}������ʒu
 * @param item �ǉ����镶����
 */
void TwrListView::insertItem(int index, TCHAR *item)
{
	LVITEM itemInfo;
	
	memset(&itemInfo, 0x00, sizeof(LVITEM));
	itemInfo.mask = LVIF_TEXT;
	itemInfo.iItem = index;
	itemInfo.pszText = item;
	itemInfo.cchTextMax = _tcslen(item) + 1;

	SendMessage(hWnd, LVM_INSERTITEM , (WPARAM)0, (LPARAM)&itemInfo);
}

/**
 * ���X�g�r���[�̃X�^�C����ݒ肵�܂��B
 *
 * @param newStyle �V�����X�^�C��
 * @return �ݒ茋��
 */
unsigned int TwrListView::SetExtendedStyle(unsigned int newStyle)
{
	return SendMessage(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)0, (LPARAM)newStyle);
}

/**
 * ���X�g�r���[�ɗ��}�����܂��B
 *
 * @param no ��ԍ�
 * @param title ��̃^�C�g��
 * @param nFormat �t�H�[�}�b�g
 * @param nWidth ��
 * @param nSubItem ��̃T�u�A�C�e���̃C���f�b�N�X
 * @return �}������
 */
int TwrListView::insertColumn(
	int no,
	TCHAR *title, 
	int nFormat,
	int nWidth,
	int nSubItem 
)
{
	LVCOLUMN columnInfo;
	
	columnInfo.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	columnInfo.pszText = title;
	columnInfo.fmt = nFormat;
	columnInfo.cx = nWidth;
	if (nSubItem > -1) {
		columnInfo.iSubItem = nSubItem;
	} else {
		columnInfo.iSubItem = 0;
	}
	
	return SendMessage(hWnd, LVM_INSERTCOLUMN, (WPARAM)no, (LPARAM)(&columnInfo));
}

/**
 * ���X�g�r���[�̃`�F�b�N��Ԃ�ύX���܂��B
 *
 * @param index �`�F�b�N��Ԃ�ύX����s
 * @param checked true:�`�F�b�N����Ă��� false:�`�F�b�N����Ă��Ȃ�
 */
void TwrListView::checkItem(int index, bool checked)
{
	ListView_SetCheckState(hWnd, index, (checked ? TRUE : FALSE));
}

/**
 * ���X�g�r���[�̍s��I�����܂��B
 *
 * @param index �I������s
 */
void TwrListView::selectItem(int index)
{
	LVITEM itemInfo;
	
	itemInfo.mask = LVIF_STATE;
	itemInfo.iItem = index;
	itemInfo.state = LVIS_SELECTED ;
	itemInfo.stateMask = LVIS_SELECTED ;

	SendMessage(hWnd, LVM_SETITEM, (WPARAM)0, (LPARAM)(&itemInfo));
}

/**
 * ���X�g�r���[�̍s�̃`�F�b�N��Ԃ��擾���܂��B
 *
 * @param index �`�F�b�N��Ԃ��擾����s
 * @return true:�`�F�b�N����Ă��� false:�`�F�b�N����Ă��Ȃ�
 */
bool TwrListView::isChecked(int index)
{
	BOOL state;
	
	state = ListView_GetCheckState(hWnd, index);

	return (state ? true : false);
}

