// noMeiryoUI.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <windows.h>
#include <commdlg.h>
#include "noMeiryoUI.h"

#define MAX_LOADSTRING 100

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����t���[�����[�N��
// ���[�U�[�����̃u���b�W���[�`��
//

// �A�v���P�[�V�����I�u�W�F�N�g
NoMeiryoUI *appObj;

/**
 * �A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
 */
DialogAppliBase *createAppli()
{
	// �����Ń��[�U�[�̃A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
	appObj = new NoMeiryoUI();
	return appObj;
}

/**
 * �_�C�A���O�v���[�V�W��
 *
 */
INT_PTR CALLBACK MainDialogProc(
  HWND hwndDlg,  // �_�C�A���O�{�b�N�X�̃n���h��
  UINT uMsg,     // ���b�Z�[�W
  WPARAM wParam, // �ŏ��̃��b�Z�[�W�p�����[�^
  LPARAM lParam  // 2 �Ԗڂ̃��b�Z�[�W�p�����[�^
  )
{
	// ���[�U�[�A�v���P�[�V�����̃_�C�A���O�v���[�V�W���Ɋۓ������܂��B
	return appObj->dialogProc(hwndDlg, uMsg,wParam,lParam);
}

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����t���[�����[�N�̎�����
//

/**
 * �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X�𐶐����܂��B
 *
 * @return �A�v���P�[�V�����E�C���h�E�N���X�̃C���X�^���X
 */
BaseDialog *NoMeiryoUI::createBaseDialog()
{
	return appObj;
}

/**
 * �A�v���P�[�V�����̊J�n���������s���܂��B
 *
 * @param lpCmdLine �R�}���h���C��
 * @return �\��
 */
int NoMeiryoUI::OnAppliStart(TCHAR *lpCmdLine)
{
	// ���̊֐����I�[�o�[���C�h���ăA�v���ŗL�̏��������s���܂��B
	// �{�A�v���P�[�V�����ł͓��ɏ����Ȃ��B
	return 0;
}

/**
 * �E�C���h�E���\�����ꂽ�Ƃ��̏��������s���܂��B<br>
 * �E�C���h�E�x�[�X�A�v���ƃC���^�t�F�[�X�����킹�邽�߂ɗp�ӂ��Ă��܂��B
 *
 * @return �\��
 */
int NoMeiryoUI::OnWindowShow()
{

	// ���̊֐����I�[�o�[���C�h���āA����̕\�����̏������s���܂��B
	// ���̃^�C�~���O�Ń_�C�A���O�����݂���̂ŁA�����ɏ��������邱�Ƃ�
	// �_�C�A���O�������ԂŋN�����̏������������s�����Ƃ��ł��܂��B

	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&metricsAll,sizeof(NONCLIENTMETRICS),0x00);

	// �A�C�R���ȊO�̃t�H���g�����擾����B
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		0);

	metricsAll.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		0);

	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		0);

	allFontName = metricsAll.lfMenuFont.lfFaceName;
	titleFontName = metrics.lfCaptionFont.lfFaceName;
	iconFontName = iconFont.lfFaceName;
	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	hintFontName = metrics.lfStatusFont.lfFaceName;
	messageFontName = metrics.lfMessageFont.lfFaceName;
	// ���j���[�ƑI������
	menuFontName = metrics.lfMenuFont.lfFaceName;

	UpdateData(false);

	return 0;
}

/**
 * �A�v���P�[�V�����̏I�����������s���܂��B
 *
 * @return �\��
 */
int NoMeiryoUI::OnAppliEnd()
{
	// ���̊֐����I�[�o�[���C�h���ăA�v���ŗL�̌㏈�����s���܂��B
	// �{�A�v���P�[�V�����ł͓��ɏ����Ȃ��B
	return 0;
}

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����Ƃ��Ă�
// �A�v���P�[�V�����ŗL�����̎�������
//

/**
 * �_�C�A���O����������
 *
 * @return TRUE: FALSE:�t�H�[�J�X��ݒ肵��
 */
INT_PTR NoMeiryoUI::OnInitDialog()
{
	// �e�N���X�̃_�C�A���O�������������ĂԁB
	DialogAppliBase::OnInitDialog();

	HICON hIcon;

    hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0);
    SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);


	UpdateData(false);

	return (INT_PTR)FALSE;
}

/**
 * �_�C�A���O�R���g���[���ƃI�u�W�F�N�g�̓��e�̓��������B
 *
 * @param toObj true:�R���g���[�����I�u�W�F�N�g false:�I�u�W�F�N�g���R���g���[��
 */
void NoMeiryoUI::UpdateData(bool toObj)
{
	// �����Ƀ_�C�A���O�̃R���g���[���Ɠ�������郁���o�ϐ����L�q���܂��B
	DDX_Text(toObj,IDC_EDIT_ALL, allFontName);
	DDX_Text(toObj,IDC_EDIT_TITLE, titleFontName);
	DDX_Text(toObj,IDC_EDIT_ICON, iconFontName);
	DDX_Text(toObj,IDC_EDIT_PALETTE, paletteFontName);
	DDX_Text(toObj,IDC_EDIT_HINT, hintFontName);
	DDX_Text(toObj,IDC_EDIT_MESSAGE, messageFontName);
	DDX_Text(toObj,IDC_EDIT_MENU, menuFontName);
}

INT_PTR NoMeiryoUI::OnCommand(WPARAM wParam)
{
	switch (LOWORD(wParam)) {
		case ID_SEL_ALL:
			selectFont(all);
			return (INT_PTR)TRUE;
		case ID_SEL_TITLE:
			selectFont(title);
			return (INT_PTR)TRUE;
		case ID_SEL_ICON:
			selectFont(icon);
			return (INT_PTR)TRUE;
		case ID_SEL_PALETTE:
			selectFont(palette);
			return (INT_PTR)TRUE;
		case ID_SEL_HINT:
			selectFont(hint);
			return (INT_PTR)TRUE;
		case ID_SEL_MESSAGE:
			selectFont(message);
			return (INT_PTR)TRUE;
		case ID_SEL_MENU:
			selectFont(menu);
			return (INT_PTR)TRUE;
		case ID_SET_ALL:
			OnBnClickedAll();
			return (INT_PTR)TRUE;
		case IDOK:
			OnBnClickedOk();
			break;
	}
	return BaseDialog::OnCommand(wParam);

}

void NoMeiryoUI::selectFont(enum fontType type)
{
	CHOOSEFONT font;
	int result;
	LOGFONT logfont;	// �擾�����t�H���g�̏�������\����

	// �ݒ肵�����t�H���g��I������B
	FillMemory(&font,sizeof(CHOOSEFONT),0x00);
	
	font.lStructSize = sizeof(CHOOSEFONT);
	font.hwndOwner = NULL;
	font.lpLogFont = &logfont;
	font.Flags = CF_SCREENFONTS;
	result = ChooseFont(&font);

	if (!result){
		return;
	}
	if (logfont.lfFaceName[0] == _T('\0')) {
		return;
	}
	switch (type) {
		case all:
			metricsAll.lfMenuFont = logfont;
			metricsAll.lfStatusFont = logfont;
			metricsAll.lfMessageFont = logfont;
			metricsAll.lfCaptionFont = logfont;
			metricsAll.lfSmCaptionFont = logfont;
			iconFontAll = logfont;

			allFontName = logfont.lfFaceName;
			break;

		case title:
			metrics.lfCaptionFont = logfont;
			titleFontName = logfont.lfFaceName;
			break;

		case icon:
			iconFont = logfont;
			iconFontName = logfont.lfFaceName;
			break;

		case palette:
			metrics.lfSmCaptionFont = logfont;
			paletteFontName = logfont.lfFaceName;
			break;

		case hint:
			metrics.lfStatusFont = logfont;
			hintFontName = logfont.lfFaceName;
			break;

		case message:
			metrics.lfMessageFont = logfont;
			messageFontName = logfont.lfFaceName;
			break;

		case menu:
			// ���j���[�ƑI������
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;
			break;
	}
	UpdateData(false);
}

void NoMeiryoUI::OnBnClickedOk()
{
	// �A�C�R���ȊO�̃t�H���g�ݒ�
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// �A�C�R���̃t�H���g�ݒ�
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// �F���Đݒ肷�邱�Ƃŉ�ʂ����t���b�V������B
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);
}

void NoMeiryoUI::OnBnClickedAll()
{

	// �A�C�R���ȊO�̃t�H���g�ݒ�
	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// �A�C�R���̃t�H���g�ݒ�
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFontAll,
		SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

	// �F���Đݒ肷�邱�Ƃŉ�ʂ����t���b�V������B
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);

	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	iconFontName = iconFontAll.lfFaceName;
	paletteFontName = metricsAll.lfCaptionFont.lfFaceName;
	hintFontName = metricsAll.lfCaptionFont.lfFaceName;
	messageFontName = metricsAll.lfCaptionFont.lfFaceName;
	// ���j���[�ƑI������
	menuFontName = metricsAll.lfCaptionFont.lfFaceName;

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));

	UpdateData(false);
}