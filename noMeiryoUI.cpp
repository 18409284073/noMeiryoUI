/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
// noMeiryoUI.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <process.h>
#include <Objbase.h>
#include <shellapi.h>
#ifdef DEBUG
#include <vld.h>
#endif
#include "noMeiryoUI.h"
#include "FontSel.h"
#include "NCFileDialog.h"

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
	CoInitialize(NULL);

	// �����Ń��[�U�[�̃A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
	appObj = new NoMeiryoUI();
	return appObj;
}

/**
 * �_�C�A���O�v���[�V�W��
 *
 * @return TRUE:�������� FALSE:�������Ȃ�
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
	// �A�v���ŗL�̏��������s���܂��B
	noMeiryoUI = false;
	verInfo = NULL;

	if (_tcsstr(lpCmdLine, _T("noMeiryoUI")) != NULL) {
		noMeiryoUI = true;
	}

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
	if (verInfo != NULL) {
		delete verInfo;
	}
	return 0;
}

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����Ƃ��Ă�
// �A�v���P�[�V�����ŗL�����̎�������
//

/**
 * �_�C�A���O����������
 *
 * @return TRUE:�t�H�[�J�X��ݒ肵�� FALSE:�t�H�[�J�X��ݒ肵�Ȃ�
 */
INT_PTR NoMeiryoUI::OnInitDialog()
{
	// �e�N���X�̃_�C�A���O�������������ĂԁB
	DialogAppliBase::OnInitDialog();

	// �A�v���P�[�V�����A�C�R���̐ݒ�
	HICON hIcon;

    hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDC_MYICON), IMAGE_ICON, 16, 16, 0);
    SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	appMenu = new TwrMenu(this->hWnd);

	return (INT_PTR)FALSE;
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

	SetWinVer();

	// �t�H���g���擾�p�\���̂̏�����
	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&metricsAll,sizeof(NONCLIENTMETRICS),0x00);
	FillMemory(&iconFont,sizeof(LOGFONT),0x00);
	FillMemory(&iconFontAll,sizeof(LOGFONT),0x00);

	//
	// �ʂ̃t�H���g�p�̏��擾
	//

	// �A�C�R���ȊO�̃t�H���g�����擾����B
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	// �A�C�R���̃t�H���g�����擾����B
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT),
		&iconFont,
		0);

	double point = getFontPoint(&(metrics.lfCaptionFont));

	//
	// ���ׂẴt�H���g�p�̏��擾
	//

	// �A�C�R���ȊO�̃t�H���g�����擾����B
	metricsAll.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metricsAll,
		0);

	// �A�C�R���̃t�H���g�����擾����B
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

	metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	metricsAll.lfMessageFont = metricsAll.lfMenuFont;
	metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
	metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
	iconFontAll = metricsAll.lfMenuFont;

	UpdateData(false);

	return 0;
}

/**
 * �t�H���g�T�C�Y���Z�o����B
 *
 * @param font
 * @return �t�H���g�T�C�Y
 */
double NoMeiryoUI::getFontPoint(LOGFONT *font)
{
	// �t�H���g���쐬����B
	HFONT hFont = CreateFontIndirect(font);
	// ���g�̃E�C���h�E�n���h������쐬�����f�o�C�X�R���e�L�X�g��
	// �t�H���g��ݒ肷��B
	HDC dc = GetDC(this->getHwnd());
	SelectFont(dc, hFont);

	// �f�o�C�X�R���e�L�X�g����TEXTMETRIC���擾����B
	TEXTMETRIC metric;
	GetTextMetrics(dc, &metric);

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(this->getHwnd(), dc);
	DeleteObject(hFont);

	int height;
	if (font->lfHeight < 0) {
		// ���̏ꍇ��lfHeight�̓t�H���g���̂̍����B
		height = 0 - font->lfHeight;
	} else if (font->lfHeight > 0) {
		// ���̏ꍇ�͂��ł�Internal Leading���܂�ł���̂ł��̕��������B
		height = font->lfHeight - metric.tmInternalLeading;
	} else {
		// 0�̏ꍇ�̓f�t�H���g�̑傫�����쐬�����t�H���g����擾����B
		height = metric.tmAscent + metric.tmDescent - metric.tmInternalLeading;
	}

	double point = (double)height * 72 / logPixelY;

	return point;
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

/**
 * �e����ɑ΂��鏈���̕���
 *
 * @param wParam WPARAM
 * @return 0:���b�Z�[�W�����������B 0�ȊO:���b�Z�[�W���������Ȃ������B
 */
INT_PTR NoMeiryoUI::OnCommand(WPARAM wParam)
{
	INT_PTR result;

	switch (LOWORD(wParam)) {
		case ID_SEL_ALL:
			selectFont(all);
			return (INT_PTR)0;
		case ID_SEL_TITLE:
			selectFont(title);
			return (INT_PTR)0;
		case ID_SEL_ICON:
			selectFont(icon);
			return (INT_PTR)0;
		case ID_SEL_PALETTE:
			selectFont(palette);
			return (INT_PTR)0;
		case ID_SEL_HINT:
			selectFont(hint);
			return (INT_PTR)0;
		case ID_SEL_MESSAGE:
			selectFont(message);
			return (INT_PTR)0;
		case ID_SEL_MENU:
			selectFont(menu);
			return (INT_PTR)0;
		case ID_SET_ALL:
			OnBnClickedAll();
			return (INT_PTR)0;
		case IDM_OPEN:
			OnLoad();
			return (INT_PTR)0;
		case IDM_SAVE:
			OnSave();
			return (INT_PTR)0;
		case IDOK:
			result = OnBnClickedOk();
			if (!result) {
				return (INT_PTR)0;
			}
			break;
		case IDM_EXIT:
			EndDialog(hWnd, LOWORD(wParam));
			break;
		case IDM_ANOTHER:
			if (appMenu->isChecked(IDM_ANOTHER)) {
				appMenu->CheckMenuItem(IDM_ANOTHER, false);
			} else {
				appMenu->CheckMenuItem(IDM_ANOTHER, true);
			}
			return (INT_PTR)0;
		case IDM_HELPTOPIC:
			showHelp();
			return (INT_PTR)0;
		case IDM_ABOUT:
			MessageBox(hWnd, 
				_T("Meiryo UI��������炢!! Version 2.14\n\nBy Tatsuhiko Syoji(Tatsu) 2005,2012-2015"),
				_T("Meiryo UI��������炢!!�ɂ���"),
				MB_OK | MB_ICONINFORMATION);

			return (INT_PTR)0;
	}
	return BaseDialog::OnCommand(wParam);

}

/**
 * �t�H���g��I������B
 *
 * @param type �ݒ肷��t�H���g�̎��
 */
void NoMeiryoUI::selectFont(enum fontType type)
{

	CHOOSEFONT font;
	INT_PTR result;
	LOGFONT logfont;	// �擾�����t�H���g�̏�������\����

	// �ݒ肵�����t�H���g��I������B
	FillMemory(&font,sizeof(CHOOSEFONT),0x00);
	FillMemory(&logfont,sizeof(LOGFONT),0x00);
	
	font.lStructSize = sizeof(CHOOSEFONT);
	font.hwndOwner = this->hWnd;
	font.hDC = NULL;
	font.lpLogFont = &logfont;
	font.Flags = CF_SCREENFONTS;
	font.hInstance = hInst;
	font.lpTemplateName = _T("");
	font.lpszStyle = _T("");
	font.nFontType = SCREEN_FONTTYPE;
	font.nSizeMax = 72;
	// font.rgbColors = rgbCurrent;

	try {
		// result = ChooseFont(&font);
		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}

		result = selector->showModal();
		if (result != IDOK){
			delete []selector;
			return;
		}
		logfont = selector->getSelectedFont();
		if (logfont.lfFaceName[0] == _T('\0')) {
			delete []selector;
			return;
		}

		delete []selector;
	} catch (...) {
		MessageBox(this->hWnd,
			_T("�t�H���g�I���_�C�A���O���œ����G���[���������܂����B"),
			_T("�����G���["),
			MB_OK | MB_ICONEXCLAMATION);
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

/**
 * �t�H���g�ݒ��ۑ������I���������̓���
 */
void NoMeiryoUI::OnLoad()
{
	NCFileDialog *dlg = new NCFileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�ݒ�t�@�C��(*.ini)\0*.ini\0���ׂẴt�@�C��(*.*)\0*.*\0\0"),
		this->getHwnd(),
		0);

	int result = dlg->DoModal();
	if (result != IDOK) {
		delete dlg;
		return;
	}

	BOOL loadResult;
	loadResult = startLoadFont(dlg->GetPathName());
	if (!loadResult) {
		MessageBox(
			this->getHwnd(),
			_T("�t�H���g�ݒ�̓ǂݍ��݂Ɏ��s���܂����B"),
			_T("�G���["),
			MB_OK | MB_ICONEXCLAMATION);
	}


	delete dlg;
}

/**
 * �t�H���g���ۑ����J�n����B
 *
 * @param filename ini�t�@�C����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
 */
BOOL NoMeiryoUI::startLoadFont(TCHAR *filename)
{
	BOOL loadResult;
	LOGFONT captionFont;
	LOGFONT newIconFont;
	LOGFONT smCaptionFont;
	LOGFONT statusFont;
	LOGFONT messageFont;
	LOGFONT menuFont;

	loadResult = loadFont(filename, _T("TitleFont"), &captionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("IconFont"), &newIconFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("PaletteFont"), &smCaptionFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("HintFont"), &statusFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("MessageFont"), &messageFont);
	if (!loadResult) {
		return FALSE;
	}
	loadResult = loadFont(filename, _T("MenuFont"), &menuFont);
	if (!loadResult) {
		return FALSE;
	}

	metrics.lfCaptionFont = captionFont;
	iconFont = newIconFont;
	metrics.lfSmCaptionFont = smCaptionFont;
	metrics.lfStatusFont = statusFont;
	metrics.lfMessageFont = messageFont;
	metrics.lfMenuFont = menuFont;

	titleFontName = metrics.lfCaptionFont.lfFaceName;
	iconFontName = iconFont.lfFaceName;
	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	hintFontName = metrics.lfStatusFont.lfFaceName;
	messageFontName = metrics.lfMessageFont.lfFaceName;
	menuFontName = metrics.lfMenuFont.lfFaceName;
	UpdateData(false);

	return TRUE;
}

/**
 * �t�H���g����ۑ�����B
 *
 * @param filename ini�t�@�C����
 * @param category �ۑ��Ώۃt�H���g��ini�t�@�C���Z�N�V������
 * @param font �ۑ��Ώۃt�H���g��LOGFONT�\����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
 */
BOOL NoMeiryoUI::loadFont(TCHAR *filename, TCHAR *section, LOGFONT *font)
{
	TCHAR buf[32];
	DWORD result;

	result = GetPrivateProfileString(section,
		_T("FaceName"),
		_T(""),
		font->lfFaceName,
		32,
		filename);
	if (!result) {
		return FALSE;
	}

	result = GetPrivateProfileString(section,
		_T("Height"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfHeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Width"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWidth = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Escapement"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfEscapement = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Orientation"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOrientation = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Weight"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfWeight = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Italic"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfItalic = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Underline"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfUnderline = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("StrikeOut"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfStrikeOut = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("CharSet"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfCharSet = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("OutPrecision"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfOutPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("ClipPrecision"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfClipPrecision = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("Quality"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfQuality = _ttoi(buf);

	result = GetPrivateProfileString(section,
		_T("PitchAndFamily"),
		_T(""),
		buf,
		32,
		filename);
	if (!result) {
		return FALSE;
	}
	font->lfPitchAndFamily = _ttoi(buf);

	return TRUE;
}


/**
 * �t�H���g�ݒ��ۑ������I���������̓���
 */
void NoMeiryoUI::OnSave()
{
	NCFileDialog *dlg = new NCFileDialog(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�ݒ�t�@�C��(*.ini)\0*.ini\0���ׂẴt�@�C��(*.*)\0*.*\0\0"),
		this->getHwnd(),
		0);

	int result = dlg->DoModal();
	if (result != IDOK) {
		delete dlg;
		return;
	}

	BOOL saveResult;
	saveResult = startSaveFont(dlg->GetPathName());
	if (!saveResult) {
		MessageBox(
			this->getHwnd(),
			_T("�t�H���g�ݒ�̕ۑ��Ɏ��s���܂����B"),
			_T("�G���["),
			MB_OK | MB_ICONEXCLAMATION);
	}


	delete dlg;
}

/**
 * �t�H���g���ۑ����J�n����B
 *
 * @param filename ini�t�@�C����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
 */
BOOL NoMeiryoUI::startSaveFont(TCHAR *filename)
{
	BOOL saveResult;

	saveResult = saveFont(filename, _T("TitleFont"), &metrics.lfCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("IconFont"), &iconFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("PaletteFont"), &metrics.lfSmCaptionFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("HintFont"), &metrics.lfStatusFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("MessageFont"), &metrics.lfMessageFont);
	if (!saveResult) {
		return FALSE;
	}
	saveResult = saveFont(filename, _T("MenuFont"), &metrics.lfMenuFont);
	if (!saveResult) {
		return FALSE;
	}

	return TRUE;
}

/**
 * �t�H���g����ۑ�����B
 *
 * @param filename ini�t�@�C����
 * @param category �ۑ��Ώۃt�H���g��ini�t�@�C���Z�N�V������
 * @param font �ۑ��Ώۃt�H���g��LOGFONT�\����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
 */
BOOL NoMeiryoUI::saveFont(TCHAR *filename, TCHAR *section, LOGFONT *font)
{
	TCHAR buf[32];
	BOOL result;

	result = WritePrivateProfileString(section,
		_T("FaceName"),
		font->lfFaceName,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfHeight);
	result = WritePrivateProfileString(section,
		_T("Height"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfWidth);
	result = WritePrivateProfileString(section,
		_T("Width"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfEscapement);
	result = WritePrivateProfileString(section,
		_T("Escapement"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfOrientation);
	result = WritePrivateProfileString(section,
		_T("Orientation"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfWeight);
	result = WritePrivateProfileString(section,
		_T("Weight"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfItalic);
	result = WritePrivateProfileString(section,
		_T("Italic"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfUnderline);
	result = WritePrivateProfileString(section,
		_T("Underline"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfStrikeOut);
	result = WritePrivateProfileString(section,
		_T("StrikeOut"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfCharSet);
	result = WritePrivateProfileString(section,
		_T("CharSet"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfOutPrecision);
	result = WritePrivateProfileString(section,
		_T("OutPrecision"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfClipPrecision);
	result = WritePrivateProfileString(section,
		_T("ClipPrecision"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfQuality);
	result = WritePrivateProfileString(section,
		_T("Quality"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	_stprintf(buf, _T("%ld"), font->lfPitchAndFamily);
	result = WritePrivateProfileString(section,
		_T("PitchAndFamily"),
		buf,
		filename);
	if (!result) {
		return FALSE;
	}

	return TRUE;
}

/**
 * OK�{�^���������̓���(�I�������t�H���g��ݒ肷��B)
 *
 * @return TRUE:�t�H���g��ݒ肵���Ƃ� FALSE:�t�H���g�ݒ���L�����Z�������Ƃ�
 */
INT_PTR NoMeiryoUI::OnBnClickedOk()
{
	// ����ďc�����p�t�H���g���w�肵�Ȃ��悤�₢���킹���s���B
	bool hasVerticalFont = false;
	if (metrics.lfCaptionFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfSmCaptionFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfStatusFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfMessageFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}
	if (metrics.lfMenuFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;	
	}
	if (iconFont.lfFaceName[0] == _T('@')) {
		hasVerticalFont = true;
	}

	if (hasVerticalFont) {
		int answer = MessageBox(hWnd,
			_T("�c�����p�t�H���g(���O��@�Ŏn�܂�t�H���g)��\n�w�肳��Ă��܂�����낵���ł����H"),
			_T("�m�F"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return (INT_PTR)FALSE;
		}
	}

	// �t�H���g�ύX�����{����B
	setFont(&metrics, &iconFont);

	return (INT_PTR)TRUE;
}

/**
 * �ꊇ�ݒ�{�^���������̓���(���ׂẴt�H���g�őI�������t�H���g��ݒ肷��B)
 *
 */
void NoMeiryoUI::OnBnClickedAll()
{
	// ����ďc�����p�t�H���g���w�肵�Ȃ��悤�₢���킹���s���B
	if (metricsAll.lfMenuFont.lfFaceName[0] == _T('@')) {
		int answer = MessageBox(hWnd,
			_T("�c�����p�t�H���g(���O��@�Ŏn�܂�t�H���g)��\n�w�肳��Ă��܂�����낵���ł����H"),
			_T("�m�F"),
			MB_ICONQUESTION | MB_YESNO);
		if (answer != IDYES) {
			return;
		}
	}

	// �t�H���g�ύX�����{����B
	setFont(&metricsAll, &iconFontAll);

	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	titleFontName = metricsAll.lfCaptionFont.lfFaceName;
	iconFontName = iconFontAll.lfFaceName;
	paletteFontName = metricsAll.lfCaptionFont.lfFaceName;
	hintFontName = metricsAll.lfCaptionFont.lfFaceName;
	messageFontName = metricsAll.lfCaptionFont.lfFaceName;
	// ���j���[�ƑI������
	menuFontName = metricsAll.lfCaptionFont.lfFaceName;

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));
	memcpy(&iconFont, &iconFontAll,sizeof(LOGFONT));

	UpdateData(false);
}

NONCLIENTMETRICS *s_fontMetrics;

unsigned _stdcall setOnThread(void *p)
{
	DWORD_PTR ptr;
	LRESULT messageResult;

	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		s_fontMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	_endthreadex(0);
	return 0;
}


/**
 * ��ʊe���̃t�H���g��ݒ肷��B
 *
 * @param fontMetrics �A�C�R���ȊO�̃t�H���g�w��pNONCLIENTMETRICS
 * @param iconLogFont �A�C�R���̃t�H���g
 */
void NoMeiryoUI::setFont(
	NONCLIENTMETRICS *fontMetrics,
	LOGFONT *iconLogFont
) {

	DWORD_PTR ptr;
	LRESULT messageResult;

	// �A�C�R���̃t�H���g�ݒ�
	SystemParametersInfo(SPI_SETICONTITLELOGFONT,
		sizeof(LOGFONT),
		iconLogFont,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETICONTITLELOGFONT,
		0, // (LPARAM)_T("Environment"),
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			MessageBox(this->getHwnd(), 
				_T("SendMessage timeout."),
				_T("Information"),
				MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// �A�C�R���ȊO�̃t�H���g�ݒ�
	if (appMenu->isChecked(IDM_ANOTHER)) {
		// UI�ƕʃX���b�h��SystemParametersInfo(SPI_SETNONCLIENTMETRICS��
		// ���s����B
		s_fontMetrics = fontMetrics;

		HANDLE handle;

		handle = (HANDLE)_beginthreadex(NULL,0,setOnThread,NULL,0,NULL);

		// �ꉞ5�b�قǑ҂�
		WaitForSingleObject( handle, 5000 );
		CloseHandle(handle);
	} else {
		// UI�Ɠ����X���b�h��SystemParametersInfo(SPI_SETNONCLIENTMETRICS��
		// ���s����B
		SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
			sizeof(NONCLIENTMETRICS),
			fontMetrics,
			SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);
	}

	messageResult = SendMessageTimeout(
		HWND_BROADCAST,
		WM_SETTINGCHANGE,
		SPI_SETNONCLIENTMETRICS,
		(LPARAM)_T("WindowMetrics"),
		SMTO_ABORTIFHUNG,
		5000,
		&ptr);
	if (messageResult == 0) {
		if (GetLastError() == ERROR_TIMEOUT) {
			MessageBox(this->getHwnd(), 
				_T("SendMessage timeout."),
				_T("Information"),
				MB_OK | MB_ICONEXCLAMATION);
		}
	}

	// �F���Đݒ肷�邱�Ƃŉ�ʂ����t���b�V������B
	// �̂����AIObit StartMenu 8���N�����Ă���Ƃ���SetSysColors��
	// �Ăяo���Ɖ������Ȃ��Ȃ�̂ŌĂяo�����s��Ȃ����Ƃɂ����B
#if 0
	DWORD btnColor;
	btnColor = GetSysColor(COLOR_BTNTEXT);

	INT colorItems[1];
	colorItems[0] = COLOR_BTNTEXT;
	COLORREF colors[1];
	colors[0] = btnColor;
	SetSysColors(1,colorItems,colors);
#endif

}

/**
 * Windows�̃o�[�W�������擾���āA��ʂɐݒ肷��B
 */
void NoMeiryoUI::SetWinVer()
{
	// Windows�̓����o�[�W�����𒲂ׂ�B
	DWORD dwVersion = GetVersion();
	TCHAR buf[128];

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// �T�[�o�[���ǂ����̒ǉ������擾���邽��
	// GetVersionEx��OSVERSIONINFOEX��n���ČĂяo���B
	// Windows 98/Me,NT4�ȑO�͍l�����Ȃ��̂ŌĂѕ����͂Ȃ��B
	OSVERSIONINFOEX infoEx;
	memset(&infoEx, 0, sizeof(OSVERSIONINFOEX));
	infoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&infoEx);

	switch(major) {
		case 5:
			switch (minor) {
				case 0:
					_stprintf(buf,
						_T("Windows 2000 (%d.%d)"),
						major,minor);
					break;
				case 1:
					_stprintf(buf,
						_T("Windows XP (%d.%d)"),
						major,minor);
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows XP 64bit (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2003 (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		case 6:
			switch (minor) {
				case 0:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Vista (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2008 (%d.%d)"),
							major,minor);
					}
					break;
				case 1:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 7 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2008 R2 (%d.%d)"),
							major,minor);
					}
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 8 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2012 (%d.%d)"),
							major,minor);
					}
					break;
				case 3:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows 8.1 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Server 2012 R2 (%d.%d)"),
							major,minor);
					}
					break;
				default:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Future Windows Client (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Future Windows Server (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		default:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("Future Windows Client (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("Future Windows Server (%d.%d)"),
					major,minor);
			}
			break;
	}

	verInfo = GetDlgItem(IDC_STATIC_VERNO);
	verInfo->setText(buf);
}

/**
 * �h�L�������g�t�@�C����\������B
 *
 */
void NoMeiryoUI::showHelp(void)
{
	// ���s�t�@�C���̏��𓾂邽�߂̃o�b�t�@�Q
	TCHAR path[_MAX_PATH+1],drive[_MAX_DRIVE+1],dir[_MAX_DIR+1],helpFile[_MAX_PATH+1];

	// ���s�t�@�C���̂���Ƃ����BShelp.html�̃p�X���𐶐�����B
	::GetModuleFileName(NULL,path,_MAX_PATH);
	::_tsplitpath(path,drive,dir,NULL,NULL);
	::_stprintf(helpFile,_T("%s%s%s"),drive,dir,_T("noMeiryoUI.html"));
	
	// �֘A�t����ꂽ�A�v���Ńh�L�������g�t�@�C����\������B
	ShellExecute(hWnd,_T("open"),helpFile,NULL,NULL,SW_SHOW);
}

