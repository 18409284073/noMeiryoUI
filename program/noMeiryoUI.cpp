/*
noMeiryoUI (C) 2005,2012-2016 Tatsuhiko Shoji
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
#include <locale.h>
#include <mbctype.h>
#include "noMeiryoUI.h"
#include "FontSel.h"
#include "NCFileDialog.h"
#include "util.h"
#include "resource.h"

#define MAX_LOADSTRING 100

//
// �_�C�A���O�x�[�X�A�v���P�[�V�����t���[�����[�N��
// ���[�U�[�����̃u���b�W���[�`��
//

// �A�v���P�[�V�����I�u�W�F�N�g
NoMeiryoUI *appObj;
static bool use7Compat = true;
bool useResource = false;
enum language {
	Japanese,
	SimplifiedChinese,
	English
};
enum language language;
bool has8Preset = true;
bool has10Preset = true;

/**
 * �A�v���P�[�V�����I�u�W�F�N�g���쐬���܂��B
 */
DialogAppliBase *createAppli()
{
	CoInitialize(NULL);
	TCHAR iniPath[MAX_PATH];
	TCHAR *p;

	::GetModuleFileName(NULL, iniPath, _MAX_PATH);
	p = _tcsrchr(iniPath, '\\');
	if (p != NULL) {
		*(p + 1) = '\0';
	}

	// ���P�[���̏�����
	char *localeName = setlocale(LC_ALL, "");
	_setmbcp(_MB_CP_LOCALE);

	//localeName = "aaa";
	int readResult;
	if (strstr(localeName, "Japanese_Japan") != NULL) {
		useResource = false;
		language = Japanese;
		setFontResourceJa8();
		setFontResourceJa10();
	} else if (strstr(localeName, "Chinese (Simplified)_China") != NULL) {
		useResource = true;
		_tcscat(iniPath, _T("ChineseSimplified.lng"));
		readResourceFile(iniPath);
		language = SimplifiedChinese;

		readResult = readFontResource8(iniPath);
		if (!readResult) {
			has8Preset = false;
		}
		readResult = readFontResource10(iniPath);
		if (!readResult) {
			has10Preset = false;
		}
	} else {
		useResource = true;
		_tcscat(iniPath, _T("English.lng"));
		readResourceFile(iniPath);
		language = English;

		readResult = readFontResource8(iniPath);
		if (!readResult) {
			has8Preset = false;
		}
		readResult = readFontResource10(iniPath);
		if (!readResult) {
			has10Preset = false;
		}
	}

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
	noTahoma = false;
	setOnStart = false;
	_tcscpy(settingFile, _T(""));
	verInfo = NULL;

	allFont = NULL;
	titleFont = NULL;
	iconFontHandle = NULL;
	paletteFont = NULL;
	hintFont = NULL;
	messageFont = NULL;
	menuFont = NULL;

	allFontTextBox = NULL;
	titleFontTextBox = NULL;
	iconFontTextBox = NULL;
	paletteFontTextBox = NULL;
	hintFontTextBox = NULL;
	messageFontTextBox = NULL;
	menuFontTextBox = NULL;

	DWORD dwVersion = GetVersion();

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if (major < 6) {
		// Windows XP or earlyer
		WIN8_SIZE = false;
		use7Compat = false;
	} else if (major == 6) {
		if (minor < 2) {
			// Windows Vista/7
			WIN8_SIZE = false;
			use7Compat = false;
		} else {
			// Windows 8/8.1
			WIN8_SIZE = true;
			use7Compat = true;
		}
	} else {
		// Windows 10 or later
		WIN8_SIZE = false;
		use7Compat = false;
	}

	// �I�v�V�������擾����B
	getOption(lpCmdLine);

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

	if (allFont != NULL) {
		DeleteObject(allFont);
	}
	if (titleFont != NULL) {
		DeleteObject(titleFont);
	}
	if (iconFontHandle != NULL) {
		DeleteObject(iconFontHandle);
	}
	if (paletteFont != NULL) {
		DeleteObject(paletteFont);
	}
	if (hintFont != NULL) {
		DeleteObject(hintFont);
	}
	if (messageFont != NULL) {
		DeleteObject(messageFont);
	}
	if (menuFont != NULL) {
		DeleteObject(menuFont);
	}

	if (allFontTextBox != NULL) {
		delete allFontTextBox;
	}
	if (titleFontTextBox != NULL) {
		delete titleFontTextBox;
	}
	if (iconFontTextBox != NULL) {
		delete iconFontTextBox;
	}
	if (paletteFontTextBox != NULL) {
		delete paletteFontTextBox;
	}
	if (hintFontTextBox != NULL) {
		delete hintFontTextBox;
	}
	if (messageFontTextBox != NULL) {
		delete messageFontTextBox;
	}
	if (menuFontTextBox != NULL) {
		delete menuFontTextBox;
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

	if (!use7Compat) {
		// Windows 7�ȑO�̏ꍇ�̓t�H���g�T�C�Y�̎�舵�����[�h��ύX�ł��Ȃ�����B
		appMenu->setEnabled(IDM_COMPAT7, false);
	}
	appMenu->CheckMenuItem(IDM_ANOTHER, true);

	if (useResource) {
		// �C�O�ł͏����ݒ�̃t�H���g���قȂ�̂Ńv���Z�b�g���j���[���
		// ����ꍇ�̂݃v���Z�b�g��L���ɂ���B
		appMenu->setEnabled(IDM_SET_8, has8Preset);
		appMenu->setEnabled(IDM_SET_10, has10Preset);
	}

	// Windows 8.1�ȑO�ł�Windows 10�ɂ���t�H���g���Ȃ��ꍇ������̂�
	// Windows 10�p�̃v���Z�b�g���g�p�s�Ƃ���B
	DWORD dwVersion = GetVersion();

	DWORD major = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	if (major < 10) {
		appMenu->setEnabled(IDM_SET_10, false);
	}


	// �t�H���g���擾�p�\���̂̏�����
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&metricsAll, sizeof(NONCLIENTMETRICS), 0x00);
	FillMemory(&iconFont, sizeof(LOGFONT), 0x00);
	FillMemory(&iconFontAll, sizeof(LOGFONT), 0x00);

	// ���݂̃t�H���g���擾����B
	if (settingFile[0] == _T('\0')) {
		getActualFont();
	} else {
		// ���݂̃t�H���g�ƕt�������ʊe���̕����̏����擾���Ă����B
		getActualFont();

		BOOL loadResult = loadFontInfo(settingFile);
		if (loadResult) {
			if (setOnStart) {
				// -set�I�v�V�������w�肳�ꂽ�ꍇ�̓t�H���g��ݒ肵�ă_�C�A���O�����B
				OnBnClickedOk();
				EndDialog(hWnd, 0);

				return (INT_PTR)FALSE;
			} else {
				// ���j���[�t�H���g�̏������ׂẴt�H���g�̊e�t�H���g�̏��ɂ��Ă�B
				metricsAll.lfMenuFont = metrics.lfMenuFont;
				metricsAll.lfStatusFont = metricsAll.lfMenuFont;
				metricsAll.lfMessageFont = metricsAll.lfMenuFont;
				metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
				metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
				iconFontAll = metricsAll.lfMenuFont;
			}
		} else {
			// �ǂݍ��ݎ��s���͔O�̂��ߍēx���݂̃t�H���g���̏���
			// �擾���Ă����B
			getActualFont();
		}
	}

	if (useResource) {
		// ���{��ȊO��OS�ŋN�����Ă���ꍇ��
		// UI����������\�[�X�ɍ��킹�ĕύX����B
		applyResource();
	}
	// ���C���_�C�A���O�̃o�[�W�����\�L�ݒ�
	TCHAR buf[64];
	TCHAR verString[32];
	const TCHAR *appName;
	LoadString(hInst, IDS_VERSION, verString, 32);
	if (useResource) {
		appName = langResource[1].c_str();
	} else {
		appName = _T("Meiryo UI��������炢!!");
	}
	_stprintf(buf, verString, appName);
	setChildText(IDC_STATIC_APP_TITLE, buf);


	// �t�H���g���\�����X�V����B
	updateDisplay();

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

	// �e�L�X�g�{�b�N�X����p�Ƀ_�C�A���O�̊e�e�L�X�g�{�b�N�X���擾����B
	allFontTextBox = GetDlgItem(IDC_EDIT_ALL);
	titleFontTextBox = GetDlgItem(IDC_EDIT_TITLE);
	iconFontTextBox = GetDlgItem(IDC_EDIT_ICON);
	paletteFontTextBox = GetDlgItem(IDC_EDIT_PALETTE);
	hintFontTextBox = GetDlgItem(IDC_EDIT_HINT);
	messageFontTextBox = GetDlgItem(IDC_EDIT_MESSAGE);
	menuFontTextBox = GetDlgItem(IDC_EDIT_MENU);

	return 0;
}

/**
 * �R�}���h���C���I�v�V�������擾����B
 *
 * @param lpCmdLine �R�}���h���C��
 */
void NoMeiryoUI::getOption(TCHAR *lpCmdLine)
{
	TCHAR *p;
	TCHAR *paramStart;
	bool firstCommand = false;
	bool capturing = false;
	TCHAR delimiter;
	int argCount = 0;

	p = lpCmdLine;
	while (*p != _T('\0')) {
		if (*p == _T('\"')) {
			if (!capturing) {
				capturing = true;
				delimiter = _T('\"');
				// ���̕�������p�����[�^�J�n
				paramStart = p + 1;
			} else {
				if (delimiter == _T('\"')) {
					// ��͒��ŋ�؂蕶�����_�u���N�H�[�e�[�V�����̏ꍇ
					// �p�����[�^�I���Ƃ���B
					capturing = false;
					*p = _T('\0');
					argCount++;
					// �����Ńp�����[�^�̌��ɉ������������s���B
					parseOption(paramStart, argCount);
				}
			}
		} else if (_istspace(*p)) {
			// �󔒂̏ꍇ
			if (capturing) {
				if (delimiter != _T('\"')) {
					// ��͒��ŋ�؂蕶�����_�u���N�H�[�e�[�V�����łȂ��ꍇ
					// �p�����[�^�I���Ƃ���B
					capturing = false;
					*p = _T('\0');
					argCount++;
					// �����Ńp�����[�^�̌��ɉ������������s���B
					parseOption(paramStart, argCount);
				}
			}
		} else {
			if (!capturing) {
				// �p�����[�^�J�n
				capturing = true;
				paramStart = p;
				delimiter = _T(' ');
			}
		}
		p++;
	}
	if (capturing) {
		// �܂��R�}���h���C����͂������Ă����炱���܂ł��R�}���h���C���Ƃ���B
		argCount++;
		// �����Ńp�����[�^�̌��ɉ������������s���B
		parseOption(paramStart, argCount);
	}

}

/**
 * �I�v�V�����̈ʒu�ɉ�������͂��s���B
 *
 * @param param �p�����[�^
 * @param argCount �I�v�V�����̌�
 */
void NoMeiryoUI::parseOption(TCHAR *param, int argCount)
{
	switch (argCount) {
		case 1:
			// �ݒ�t�@�C����
			if (_tcscmp(_T("--"), param)) {
				_tcscpy(settingFile, param);
			}
			break;
		default:
			if (!_tcscmp(param, _T("-set"))) {
				setOnStart = true;
			} else if (!_tcscmp(param, _T("noMeiryoUI"))) {
				noMeiryoUI = true;
			} else if (!_tcscmp(param, _T("noTahoma"))) {
				noTahoma = true;
			}

			break;
	}
}

/**
 * ���ݐݒ肳��Ă���t�H���g���擾����B
 */
void NoMeiryoUI::getActualFont(void)
{
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

	// ���j���[�t�H���g�̏��𑼂̃t�H���g�̏��ɂ��Ă�B
	metricsAll.lfStatusFont = metricsAll.lfMenuFont;
	metricsAll.lfMessageFont = metricsAll.lfMenuFont;
	metricsAll.lfCaptionFont = metricsAll.lfMenuFont;
	metricsAll.lfSmCaptionFont = metricsAll.lfMenuFont;
	iconFontAll = metricsAll.lfMenuFont;
}

/**
 * �t�H���g��񂩂�t�H���g�̃n���h�����쐬����B
 *
 * @param font �t�H���g�̏��
 * @return �t�H���g�n���h��
 */
HFONT NoMeiryoUI::createFont(LOGFONT *font)
{
	HDC hDC = GetDC(this->hWnd);

	HFONT newFont = CreateFont(
		-MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0,
		0,
		0,
		font->lfWeight,
		font->lfItalic,
		font->lfUnderline,
		font->lfStrikeOut,
		font->lfCharSet,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		font->lfFaceName);

	ReleaseDC(this->hWnd, hDC);

	return newFont;
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
 * ���\�[�X���e���ڂɐݒ肷��B
 */
void NoMeiryoUI::applyResource()
{
	HDC hDC = GetDC(this->hWnd);

	HFONT newFont = CreateFont(
		-MulDiv(9, GetDeviceCaps(hDC, LOGPIXELSY), 72),
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, // CLEARTYPE_QUALITY,
		FIXED_PITCH | FF_MODERN,
		langResource[0].c_str());

	ReleaseDC(this->hWnd, hDC);


	// �A�v���^�C�g��
	setText(langResource[1].c_str());

	// ���j���[������ύX
	appMenu->setText(0, langResource[2].c_str(), TRUE);
	appMenu->setText(IDM_OPEN, langResource[3].c_str(), FALSE);
	appMenu->setText(IDM_SAVE, langResource[4].c_str(), FALSE);
	appMenu->setText(IDOK, langResource[5].c_str(), FALSE);
	appMenu->setText(IDM_EXIT, langResource[6].c_str(), FALSE);
	appMenu->setText(1, langResource[7].c_str(), TRUE);
	appMenu->setText(IDM_SET_8, langResource[8].c_str(), FALSE);
	appMenu->setText(IDM_SET_10, langResource[9].c_str(), FALSE);
	appMenu->setText(2, langResource[10].c_str(), TRUE);
	appMenu->setText(IDM_ANOTHER, langResource[11].c_str(), FALSE);
	appMenu->setText(IDM_COMPAT7, langResource[12].c_str(), FALSE);
	appMenu->setText(3, langResource[13].c_str(), TRUE);
	appMenu->setText(IDM_HELPTOPIC, langResource[14].c_str(), FALSE);
	appMenu->setText(IDM_ABOUT, langResource[15].c_str(), FALSE);

	setChildText(IDC_STATIC_ALL_FONT, langResource[16].c_str());
	setChildFont(IDC_STATIC_ALL_FONT, newFont);
	setChildText(IDC_STATIC_TITLE_BAR, langResource[17].c_str());
	setChildFont(IDC_STATIC_TITLE_BAR, newFont);
	setChildText(IDC_STATIC_ICON, langResource[18].c_str());
	setChildFont(IDC_STATIC_ICON, newFont);
	setChildText(IDC_STATIC_PALETTE_TITLE, langResource[19].c_str());
	setChildFont(IDC_STATIC_PALETTE_TITLE, newFont);
	setChildText(IDC_STATIC_HINT, langResource[20].c_str());
	setChildFont(IDC_STATIC_HINT, newFont);
	setChildText(IDC_STATIC_MESSAGE, langResource[21].c_str());
	setChildFont(IDC_STATIC_MESSAGE, newFont);
	setChildText(IDC_STATIC_MENU, langResource[22].c_str());
	setChildFont(IDC_STATIC_MENU, newFont);

	setChildText(ID_SEL_ALL, langResource[23].c_str());
	setChildFont(ID_SEL_ALL, newFont);
	setChildText(ID_SEL_TITLE, langResource[23].c_str());
	setChildFont(ID_SEL_TITLE, newFont);
	setChildText(ID_SEL_ICON, langResource[23].c_str());
	setChildFont(ID_SEL_ICON, newFont);
	setChildText(ID_SEL_PALETTE, langResource[23].c_str());
	setChildFont(ID_SEL_PALETTE, newFont);
	setChildText(ID_SEL_HINT, langResource[23].c_str());
	setChildFont(ID_SEL_HINT, newFont);
	setChildText(ID_SEL_MESSAGE, langResource[23].c_str());
	setChildFont(ID_SEL_MESSAGE, newFont);
	setChildText(ID_SEL_MENU, langResource[23].c_str());
	setChildFont(ID_SEL_MENU, newFont);
	setChildText(ID_SET_ALL, langResource[24].c_str());
	setChildFont(ID_SET_ALL, newFont);
	setChildText(IDOK, langResource[25].c_str());
	setChildFont(IDOK, newFont);
	setChildText(IDCANCEL, langResource[26].c_str());
	setChildFont(IDCANCEL, newFont);

	setChildFont(IDC_STATIC_APP_TITLE, newFont);

	setChildFont(IDC_STATIC_VERNO, newFont);
	setChildFont(IDC_STATIC_AUTHOR, newFont);


	DeleteObject(newFont);
}


/**
 * �t�H���g�\�����X�V����B
 */
void NoMeiryoUI::updateDisplay(void)
{
	// �t�H���g���A�|�C���g���\����������쐬����B
	int point;
	TCHAR buf[16];

	allFontName = metricsAll.lfMenuFont.lfFaceName;
	point = getFontPointInt(&(metricsAll.lfMenuFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	allFontName = allFontName + buf;

	titleFontName = metrics.lfCaptionFont.lfFaceName;
	point = getFontPointInt(&(metrics.lfCaptionFont), this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	titleFontName = titleFontName + buf;

	iconFontName = iconFont.lfFaceName;
	point = getFontPointInt(&iconFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	iconFontName = iconFontName + buf;

	paletteFontName = metrics.lfSmCaptionFont.lfFaceName;
	point = getFontPointInt(&metrics.lfSmCaptionFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	paletteFontName = paletteFontName + buf;

	hintFontName = metrics.lfStatusFont.lfFaceName;
	point = getFontPointInt(&metrics.lfStatusFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	hintFontName = hintFontName + buf;

	messageFontName = metrics.lfMessageFont.lfFaceName;
	point = getFontPointInt(&metrics.lfMessageFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	messageFontName = messageFontName + buf;

	// ���j���[�ƑI������
	menuFontName = metrics.lfMenuFont.lfFaceName;
	point = getFontPointInt(&metrics.lfMenuFont, this->getHwnd());
	_stprintf(buf, _T(" %3dpt"), point);
	menuFontName = menuFontName + buf;

	UpdateData(false);


	// �I�������t�H���g���e�L�X�g�{�b�N�X�ɐݒ肷��B
	if (allFont != NULL) {
		DeleteObject(allFont);
	}
	allFont = createFont(&metricsAll.lfMenuFont);
	allFontTextBox->setFont(allFont);

	if (titleFont != NULL) {
		DeleteObject(titleFont);
	}
	titleFont = createFont(&metrics.lfCaptionFont);
	titleFontTextBox->setFont(titleFont);

	if (iconFontHandle != NULL) {
		DeleteObject(iconFontHandle);
	}
	iconFontHandle = createFont(&iconFont);
	iconFontTextBox->setFont(iconFontHandle);

	if (paletteFont != NULL) {
		DeleteObject(paletteFont);
	}
	paletteFont = createFont(&metrics.lfSmCaptionFont);
	paletteFontTextBox->setFont(paletteFont);

	if (hintFont != NULL) {
		DeleteObject(hintFont);
	}
	hintFont = createFont(&metrics.lfStatusFont);
	hintFontTextBox->setFont(hintFont);

	if (messageFont != NULL) {
		DeleteObject(messageFont);
	}
	messageFont = createFont(&metrics.lfMessageFont);
	messageFontTextBox->setFont(messageFont);

	if (menuFont != NULL) {
		DeleteObject(menuFont);
	}
	menuFont = createFont(&metrics.lfMenuFont);
	menuFontTextBox->setFont(menuFont);

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
		case IDM_SET_8:
			OnSet8();
			return (INT_PTR)0;
		case IDM_SET_10:
			OnSet10();
			return (INT_PTR)0;
		case IDM_ANOTHER:
			if (appMenu->isChecked(IDM_ANOTHER)) {
				appMenu->CheckMenuItem(IDM_ANOTHER, false);
			} else {
				appMenu->CheckMenuItem(IDM_ANOTHER, true);
			}
			return (INT_PTR)0;
		case IDM_COMPAT7:
			if (appMenu->isChecked(IDM_COMPAT7)) {
				appMenu->CheckMenuItem(IDM_COMPAT7, false);
				WIN8_SIZE = true;
			} else {
				appMenu->CheckMenuItem(IDM_COMPAT7, true);
				WIN8_SIZE = false;
			}
			updateDisplay();

			return (INT_PTR)0;
		case IDM_HELPTOPIC:
			showHelp();
			return (INT_PTR)0;
		case IDM_ABOUT:
			showVersion();
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

	INT_PTR result;
	LOGFONT logfont;	// �擾�����t�H���g�̏�������\����

	FillMemory(&logfont,sizeof(LOGFONT),0x00);
	
	try {
		LOGFONT *target;
		switch (type) {
			case all:
				target = &metricsAll.lfMenuFont;
				break;

			case title:
				target = &metrics.lfCaptionFont;
				break;

			case icon:
				target = &iconFont;
				break;

			case palette:
				target = &metrics.lfSmCaptionFont;
				break;

			case hint:
				target = &metrics.lfStatusFont;
				break;

			case message:
				target = &metrics.lfMessageFont;
				break;

			case menu:
				// ���j���[�ƑI������
				target = &metrics.lfMenuFont;
				break;
		}

		FontSel *selector = new FontSel(this->hWnd, IDD_DIALOG_FONTSEL);
		if (noMeiryoUI) {
			selector->setNoMeiryoUI();
		}
		if (noTahoma) {
			selector->setNoTahoma();
		}
		// �I�����Ă����t�H���g���t�H���g�I���_�C�A���O�ɐݒ肷��B
		selector->setPreviousFont(target);

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
			_T("Internal error in font selection dialog."),
			_T("Error"),
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

			DeleteObject(allFont);
			allFont = createFont(&metricsAll.lfMenuFont);
			allFontTextBox->setFont(allFont);

			break;

		case title:
			metrics.lfCaptionFont = logfont;
			titleFontName = logfont.lfFaceName;

			DeleteObject(titleFont);
			titleFont = createFont(&metrics.lfCaptionFont);
			titleFontTextBox->setFont(titleFont);
			break;

		case icon:
			iconFont = logfont;
			iconFontName = logfont.lfFaceName;

			DeleteObject(iconFontHandle);
			iconFontHandle = createFont(&iconFont);
			iconFontTextBox->setFont(iconFontHandle);
			break;

		case palette:
			metrics.lfSmCaptionFont = logfont;
			paletteFontName = logfont.lfFaceName;

			DeleteObject(paletteFont);
			paletteFont = createFont(&metrics.lfSmCaptionFont);
			paletteFontTextBox->setFont(paletteFont);
			break;

		case hint:
			metrics.lfStatusFont = logfont;
			hintFontName = logfont.lfFaceName;

			DeleteObject(hintFont);
			hintFont = createFont(&metrics.lfStatusFont);
			hintFontTextBox->setFont(hintFont);
			break;

		case message:
			metrics.lfMessageFont = logfont;
			messageFontName = logfont.lfFaceName;

			DeleteObject(messageFont);
			messageFont = createFont(&metrics.lfMessageFont);
			messageFontTextBox->setFont(messageFont);
			break;

		case menu:
			// ���j���[�ƑI������
			metrics.lfMenuFont = logfont;
			menuFontName = logfont.lfFaceName;

			DeleteObject(menuFont);
			menuFont = createFont(&metrics.lfMenuFont);
			menuFontTextBox->setFont(menuFont);
			break;
	}

	// �\�����X�V����B
	updateDisplay();
}

/**
 * �t�@�C���_�C�A���O�p�̃t�@�C���}�X�N������쐬
 *
 * @param �t�@�C���}�X�N������o�b�t�@
 * @param �t�@�C���̎�ޖ�
 * @param �t�@�C���̃}�X�N
 * @param ���ׂẴt�@�C���̎�ޖ�
 * @param ���ׂẴt�@�C���̃}�X�N
 */
void setFileMask(
	TCHAR *buf,
	const TCHAR *fileMsg,
	const TCHAR *fileExt,
	const TCHAR *allMsg,
	const TCHAR *allExt)
{
	TCHAR *pDst = buf;
	int len;

	len = _tcslen(fileMsg);
	for (int i = 0; i < len; i++) {
		*pDst = fileMsg[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(fileExt);
	for (int i = 0; i < len; i++) {
		*pDst = fileExt[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(allMsg);
	for (int i = 0; i < len; i++) {
		*pDst = allMsg[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	len = _tcslen(allExt);
	for (int i = 0; i < len; i++) {
		*pDst = allExt[i];
		pDst++;
	}
	*pDst = _T('\0');
	pDst++;

	*pDst = _T('\0');
	pDst++;
}


/**
 * �t�H���g�ݒ��ۑ������I���������̓���
 */
void NoMeiryoUI::OnLoad()
{
	TCHAR buf[128];

	if (useResource) {
		setFileMask(
			buf,
			langResource[59].c_str(),
			_T("*.ini"),
			langResource[60].c_str(),
			_T("*.*")
		);
	} else {
		setFileMask(
			buf,
			_T("�ݒ�t�@�C��"),
			_T("*.ini"),
			_T("���ׂẴt�@�C��"),
			_T("*.*")
		);
	}

	NCFileDialog *dlg = new NCFileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		buf,
		this->getHwnd(),
		0);

	int result = dlg->DoModal();
	if (result != IDOK) {
		delete dlg;
		return;
	}

	BOOL loadResult;
	loadResult = loadFontInfo(dlg->GetPathName());
	if (!loadResult) {
		const TCHAR *message;
		const TCHAR *title;

		if (useResource) {
			message = langResource[61].c_str();
			title = langResource[63].c_str();
		} else {
			message = _T("�t�H���g�ݒ�̓ǂݍ��݂Ɏ��s���܂����B");
			title = _T("�G���[");
		}
		MessageBox(
			this->getHwnd(),
			message,
			title,
			MB_OK | MB_ICONEXCLAMATION);
	} else {
		// �t�H���g�ݒ�̓ǂݍ��݂ɐ���������e�L�X�g�{�b�N�X�ɐݒ肷��B
		updateDisplay();
	}


	delete dlg;
}

/**
 * �t�H���g����ǂݍ��ށB
 *
 * @param filename ini�t�@�C����
 * @result TRUE:�ۑ����� FALSE:�ۑ����s
 */
BOOL NoMeiryoUI::loadFontInfo(TCHAR *filename)
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

	return TRUE;
}

/**
 * �t�H���g����ǂݍ��ށB
 *
 * @param filename ini�t�@�C����
 * @param category �ǂݍ��ݑΏۃt�H���g��ini�t�@�C���Z�N�V������
 * @param font �ǂݍ��ݑΏۃt�H���g��LOGFONT�\����
 * @result TRUE:�ǂݍ��ݐ��� FALSE:�ǂݍ��ݎ��s
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
	TCHAR buf[128];

	if (useResource) {
		setFileMask(
			buf,
			langResource[59].c_str(),
			_T("*.ini"),
			langResource[60].c_str(),
			_T("*.*")
		);
	} else {
		setFileMask(
			buf,
			_T("�ݒ�t�@�C��"),
			_T("*.ini"),
			_T("���ׂẴt�@�C��"),
			_T("*.*")
		);
	}

	NCFileDialog *dlg = new NCFileDialog(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		buf,
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
		const TCHAR *message;
		const TCHAR *title;

		if (useResource) {
			message = langResource[62].c_str();
			title = langResource[63].c_str();
		} else {
			message = _T("�t�H���g�ݒ�̕ۑ��Ɏ��s���܂����B");
			title = _T("�G���[");
		}
		MessageBox(
			this->getHwnd(),
			message,
			title,
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
#if 0
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
#endif

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
#if 0
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
#endif

	// �t�H���g�ύX�����{����B
	setFont(&metricsAll, &iconFontAll);

	memcpy(&metrics, &metricsAll,sizeof(NONCLIENTMETRICS));
	memcpy(&iconFont, &iconFontAll,sizeof(LOGFONT));

	// �\�����X�V����B
	updateDisplay();
}

/**
 * �V�X�e����DPI���擾����B
 *
 * @return �擾����DPI
 */
int NoMeiryoUI::getDPI(void)
{
	// DPI���擾����B
	int dpiY = 96;
	HDC hdc = GetDC(NULL);

	if (hdc) {
		dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		ReleaseDC(NULL, hdc);
	}

	return dpiY;
}

/**
 * Windows 8/8.1�̏ꍇ�̃v���Z�b�g�l��ݒ肷��B
 */
void NoMeiryoUI::OnSet8(void)
{
	// DPI���擾����B
	int dpiY = getDPI();

	// �t�H���g�ȊO��NONCLIENTMETRICS�̌��ݒl��ێ����邽�߁A
	// NONCLIENTMETRICS�̓��e���擾���Ȃ����B
	FillMemory(&metrics,sizeof(NONCLIENTMETRICS),0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	memset(&metrics.lfCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfCaptionFont.lfFaceName, fontFaces8[0].c_str());
	metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes8[0],dpiY,72);
	metrics.lfCaptionFont.lfWeight = 400;
	metrics.lfCaptionFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfCaptionFont.lfQuality = 5;

	memset(&iconFont, 0, sizeof(LOGFONTW));
	_tcscpy(iconFont.lfFaceName, fontFaces8[1].c_str());
	iconFont.lfHeight = -MulDiv(fontSizes8[1], dpiY, 72);
	iconFont.lfWeight = 400;
	iconFont.lfCharSet = DEFAULT_CHARSET;
	iconFont.lfQuality = 5;

	memset(&metrics.lfSmCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfSmCaptionFont.lfFaceName, fontFaces8[2].c_str());
	metrics.lfSmCaptionFont.lfHeight = -MulDiv(fontSizes8[2], dpiY, 72);
	metrics.lfSmCaptionFont.lfWeight = 400;
	metrics.lfSmCaptionFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfSmCaptionFont.lfQuality = 5;

	memset(&metrics.lfStatusFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfStatusFont.lfFaceName, fontFaces8[3].c_str());
	metrics.lfStatusFont.lfHeight = -MulDiv(fontSizes8[3], dpiY, 72);
	metrics.lfStatusFont.lfWeight = 400;
	metrics.lfStatusFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfStatusFont.lfQuality = 5;

	memset(&metrics.lfMessageFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMessageFont.lfFaceName, fontFaces8[4].c_str());
	metrics.lfMessageFont.lfHeight = -MulDiv(fontSizes8[4], dpiY, 72);
	metrics.lfMessageFont.lfWeight = 400;
	metrics.lfMessageFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfMessageFont.lfQuality = 5;

	memset(&metrics.lfMenuFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMenuFont.lfFaceName, fontFaces8[5].c_str());
	metrics.lfMenuFont.lfHeight = -MulDiv(fontSizes8[5], dpiY, 72);
	metrics.lfMenuFont.lfWeight = 400;
	metrics.lfMenuFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfMenuFont.lfQuality = 5;

	// �\�����X�V����B
	updateDisplay();

}

/**
 * Windows 10�̏ꍇ�̃v���Z�b�g�l��ݒ肷��B
 */
void NoMeiryoUI::OnSet10(void)
{
	// DPI���擾����B
	int dpiY = getDPI();

	// �t�H���g�ȊO��NONCLIENTMETRICS�̌��ݒl��ێ����邽�߁A
	// NONCLIENTMETRICS�̓��e���擾���Ȃ����B
	FillMemory(&metrics, sizeof(NONCLIENTMETRICS), 0x00);
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		&metrics,
		0);

	memset(&metrics.lfCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfCaptionFont.lfFaceName, fontFaces10[0].c_str());
	metrics.lfCaptionFont.lfHeight = -MulDiv(fontSizes10[0], dpiY, 72);
	metrics.lfCaptionFont.lfWeight = 400;
	metrics.lfCaptionFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfCaptionFont.lfQuality = 5;

	memset(&iconFont, 0, sizeof(LOGFONTW));
	_tcscpy(iconFont.lfFaceName, fontFaces10[1].c_str());
	iconFont.lfHeight = -MulDiv(fontSizes10[1], dpiY, 72);
	iconFont.lfWeight = 400;
	iconFont.lfCharSet = DEFAULT_CHARSET;
	iconFont.lfQuality = 5;

	memset(&metrics.lfSmCaptionFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfSmCaptionFont.lfFaceName, fontFaces10[2].c_str());
	metrics.lfSmCaptionFont.lfHeight = -MulDiv(fontSizes10[2], dpiY, 72);
	metrics.lfSmCaptionFont.lfWeight = 400;
	metrics.lfSmCaptionFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfSmCaptionFont.lfQuality = 5;

	memset(&metrics.lfStatusFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfStatusFont.lfFaceName, fontFaces10[3].c_str());
	metrics.lfStatusFont.lfHeight = -MulDiv(fontSizes10[3], dpiY, 72);
	metrics.lfStatusFont.lfWeight = 400;
	metrics.lfStatusFont.lfCharSet = 1;
	metrics.lfStatusFont.lfQuality = 5;

	memset(&metrics.lfMessageFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMessageFont.lfFaceName, fontFaces10[4].c_str());
	metrics.lfMessageFont.lfHeight = -MulDiv(fontSizes10[4], dpiY, 72);
	metrics.lfMessageFont.lfWeight = 400;
	metrics.lfMessageFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfMessageFont.lfQuality = 5;

	memset(&metrics.lfMenuFont, 0, sizeof(LOGFONTW));
	_tcscpy(metrics.lfMenuFont.lfFaceName, fontFaces10[5].c_str());
	metrics.lfMenuFont.lfHeight = -MulDiv(fontSizes10[5], dpiY, 72);
	metrics.lfMenuFont.lfWeight = 400;
	metrics.lfMenuFont.lfCharSet = DEFAULT_CHARSET;
	metrics.lfMenuFont.lfQuality = 5;

	// �\�����X�V����B
	updateDisplay();

}

NONCLIENTMETRICS *s_fontMetrics;

/**
 * �X���b�h�ŃA�C�R���ȊO�̃t�H���g��ݒ肷��B
 *
 * @param p �X���b�h�ɓn���p�����[�^(���g�p)
 * @return 0
 */
unsigned _stdcall setOnThread(void *p)
{
	DWORD_PTR ptr;
	LRESULT messageResult;

	SystemParametersInfo(SPI_SETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS),
		s_fontMetrics,
		SPIF_UPDATEINIFILE); // | SPIF_SENDCHANGE);

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

		// �t�H���g�ݒ�����s����X���b�h���J�n����B
		uintptr_t startResult = _beginthreadex(NULL, 0, setOnThread, NULL, 0, NULL);
		if (startResult != 0) {
			// ����ɃX���b�h���J�n������X���b�h�I����ҋ@����B
			HANDLE handle;
			handle = (HANDLE)startResult;

			// �ꉞ5�b�قǑ҂�
			DWORD waitResult = WaitForSingleObject(handle, 5000);
			if (waitResult == WAIT_TIMEOUT) {
				// �X���b�h���I�����Ȃ��ꍇ�͂ǂ����悤���Ȃ��̂ŃX���b�h���I������B
				TerminateThread(handle, 0);
			}
			CloseHandle(handle);
		}
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
						_T("Windows Version:Windows 2000 (%d.%d)"),
						major,minor);
					break;
				case 1:
					_stprintf(buf,
						_T("Windows Version:Windows XP (%d.%d)"),
						major,minor);
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Version:Windows XP 64bit (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Windows Server 2003 (%d.%d)"),
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
							_T("Windows Version:Windows Vista (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Windows Server 2008 (%d.%d)"),
							major,minor);
					}
					break;
				case 1:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Version:Windows 7 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Windows Server 2008 R2 (%d.%d)"),
							major,minor);
					}
					break;
				case 2:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Version:Windows 8 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Windows Server 2012 (%d.%d)"),
							major,minor);
					}
					break;
				case 3:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Version:Windows 8.1 (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Windows Server 2012 R2 (%d.%d)"),
							major,minor);
					}
					break;
				default:
					if (infoEx.wProductType == VER_NT_WORKSTATION) {
						_stprintf(buf,
							_T("Windows Version:Future Windows Client (%d.%d)"),
							major,minor);
					} else {
						_stprintf(buf,
							_T("Windows Version:Future Windows Server (%d.%d)"),
							major,minor);
					}
					break;
			}
			break;
		case 10:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("Windows Version:Windows 10 (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("Windows Version:Windows Server 2016 (%d.%d)"),
					major,minor);
			}
			break;
		default:
			if (infoEx.wProductType == VER_NT_WORKSTATION) {
				_stprintf(buf,
					_T("Windows Version:Future Windows Client (%d.%d)"),
					major,minor);
			} else {
				_stprintf(buf,
					_T("Windows Version:Future Windows Server (%d.%d)"),
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
	if (language == Japanese) {
		::_stprintf(helpFile, _T("%s%s%s"), drive, dir, _T("noMeiryoUI_ja-jp.chm"));
	} else {
		::_stprintf(helpFile, _T("%s%s%s"), drive, dir, _T("noMeiryoUI_en.chm"));
	}
	
	// �֘A�t����ꂽ�A�v���Ńh�L�������g�t�@�C����\������B
	ShellExecute(hWnd,_T("open"),helpFile,NULL,NULL,SW_SHOW);
}

/**
 * �o�[�W�����ԍ���\������B
 *
 */
void NoMeiryoUI::showVersion(void)
{
	TCHAR aboutContent[256];
	TCHAR version[64];
	TCHAR transAuthor[64];
	TCHAR title[64];
	const TCHAR *appName;
	TCHAR verString[32];

	if (useResource) {
		appName = langResource[1].c_str();
		_stprintf(title, _T("%s"),
			langResource[64].c_str());
		_tcscpy(transAuthor, langResource[69].c_str());
	} else {
		appName = _T("Meiryo UI��������炢!!");
		_stprintf(title, _T("Meiryo UI��������炢!!�ɂ���"));
		_tcscpy(transAuthor, _T("Tatsuhiko Syoji(Tatsu)"));
	}
	LoadString(hInst, IDS_VERSION, verString, 32);

	_stprintf(version, verString, appName);
	_stprintf(aboutContent,
		_T("%s\n\nProgrammed By Tatsuhiko Syoji(Tatsu) 2005,2012-2016\nTranslated by %s"),
		version, transAuthor);

	MessageBox(hWnd,
		aboutContent,
		title,
		MB_OK | MB_ICONINFORMATION);
}

