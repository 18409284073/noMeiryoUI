/*
noMeiryoUI (C) 2005,2012-2016 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/

#include "util.h"

/** Windows 8�̃t�H���g�T�C�Y�Z�o�����g�p���邩�H */
bool WIN8_SIZE = true;
/** ���ꃊ�\�[�X */
std::vector<tstring> langResource;

/**
 * �t�H���g�̃s�N�Z�����ɑΉ�����|�C���g���𐮐��ŎZ�o����B(Windows 8)
 *
 * @param font �t�H���g���
 * @param hWnd �E�C���h�E�n���h��
 * @return �t�H���g�T�C�Y
 */
int getFontPointInt(LOGFONT *font, HWND hWnd)
{
	double point = getFontPoint(font, hWnd);

	if (WIN8_SIZE) {
		// Windows 8�f�B�X�v���C�R���g���[���p�l���݊�
		if ((point > 10) || (point < 8)) {
			return (int)point;
		} else {
			// 10pt�܂ł�Windows 7�Ɠ��l�Ɍv�Z����B
			// Windows 7�ȑO�݊� 
			if (point - abs((int)point) > 0.49) {
				return (int)point + 1;
			} else {
				return (int)point;
			}
		}
	} else {
		// Windows 7�ȑO�݊� 
		if (point - abs((int)point) > 0.49) {
			return (int)point + 1;
		} else {
			return (int)point;
		}
	}
}

/**
 * �t�H���g�̃s�N�Z�����ɑΉ�����|�C���g�����Z�o����B
 * (Windows 7/�R�����_�C�A���O�݊�)
 *
 * @param font �t�H���g���
 * @param hWnd �E�C���h�E�n���h��
 * @return �t�H���g�T�C�Y
 */
double getFontPoint(LOGFONT *font, HWND hWnd)
{
	// �t�H���g���쐬����B
	HFONT hFont = CreateFontIndirect(font);
	// ���g�̃E�C���h�E�n���h������쐬�����f�o�C�X�R���e�L�X�g��
	// �t�H���g��ݒ肷��B
	HDC dc = GetDC(hWnd);
	SelectFont(dc, hFont);

	// �f�o�C�X�R���e�L�X�g����TEXTMETRIC���擾����B
	TEXTMETRIC metric;
	GetTextMetrics(dc, &metric);

	int logPixelY = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(hWnd, dc);
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
 * ���\�[�X�̓ǂݍ��݂��s���B
 *
 * @param file ���\�[�X�t�@�C����
 * @param key �L�[��
 * @param �ǂݍ��߂Ȃ��������̕�����
 */
void readResourceItem(TCHAR *file, TCHAR *key, TCHAR *fallback)
{
	TCHAR buf[255];
	int len;

	// INI�t�@�C����ǂݍ��ށBUnicode�ł�API�ł��t�@�C������Unicode�̏ꍇ��
	// �e����̕����R�[�h�̃t�@�C���Ƃ��ēǂ�ł����B
	len = GetPrivateProfileString(_T("RESOURCE"), key, _T(""), buf, 255, file);
	if (len > 0) {
		langResource.push_back(buf);
	} else {
		langResource.push_back(fallback);
	}

}

/**
 * ���\�[�X�̓ǂݍ��݂��J�n����B
 *
 * @param file ���\�[�X�t�@�C����
 */
void readResourceFile(TCHAR *file)
{
	readResourceItem(file, _T("FONT_FACE"),
		_T("System")
	);
	readResourceItem(file, _T("TITLE"),
		_T("No!! Meiryo UI")
	);
	readResourceItem(file, _T("MENU_FILE"),
		_T("&File")
	);
	readResourceItem(file, _T("MENU_FILE_LOAD"),
		_T("L&oad font settings...")
	);
	readResourceItem(file, _T("MENU_FILE_SAVE"),
		_T("S&ave font settings...")
	);
	readResourceItem(file, _T("MENU_FILE_SET_QUIT"),
		_T("&Set system fonts and quit")
	);
	readResourceItem(file, _T("MENU_FILE_QUIT"),
		_T("E&xit")
	);
	readResourceItem(file, _T("MENU_PRESET"),
		_T("&Preset")
	);
	readResourceItem(file, _T("MENU_PRESET_8"),
		_T("Windows 8/8.1")
	);
	readResourceItem(file, _T("MENU_PRESET_10"),
		_T("Windows 10")
	);
	readResourceItem(file, _T("MENU_TOOLS"),
		_T("&Tools")
	);
	readResourceItem(file, _T("MENU_TOOLS_THREAD"),
		_T("Set system fonts in separated thread")
	);
	readResourceItem(file, _T("MENU_TOOLS_SEVEN"),
		_T("Calculate font size same as Windows 7")
	);
	readResourceItem(file, _T("MENU_HELP"),
		_T("&Help")
	);
	readResourceItem(file, _T("MENU_HELP_HELP"),
		_T("&Help")
	);
	readResourceItem(file, _T("MENU_HELP_ABOUT"),
		_T("&About No!! Meiryo UI")
	);
	readResourceItem(file, _T("DLG_ALL_FONT"),
		_T("All fonts")
	);
	readResourceItem(file, _T("DLG_TITLE_BAR"),
		_T("Title bar")
	);
	readResourceItem(file, _T("DLG_ICON"),
		_T("Icon")
	);
	readResourceItem(file, _T("DLG_PALETTE"),
		_T("Palette title")
	);
	readResourceItem(file, _T("DLG_HINT"),
		_T("Hint")
	);
	readResourceItem(file, _T("DLG_MESSAGE"),
		_T("Message box")
	);
	readResourceItem(file, _T("DLG_MENU"),
		_T("Menu")
	);
	readResourceItem(file, _T("DLG_SELECT"),
		_T("Select...")
	);
	readResourceItem(file, _T("DLG_SET_ALL"),
		_T("Set all")
	);
	readResourceItem(file, _T("DLG_SET"),
		_T("Set")
	);
	readResourceItem(file, _T("DLG_EXIT"),
		_T("Exit")
	);
	readResourceItem(file, _T("DLG_FONT_SEL"),
		_T("Font Selection")
	);
	readResourceItem(file, _T("DLG_FONT_NAME"),
		_T("&Font name")
	);
	readResourceItem(file, _T("DLG_STYLE"),
		_T("St&yle")
	);
	readResourceItem(file, _T("DLG_SIZE"),
		_T("&Size")
	);
	readResourceItem(file, _T("DLG_UNDERLINE"),
		_T("&Under line")
	);
	readResourceItem(file, _T("DLG_STRIKE"),
		_T("S&trikethrough")
	);
	readResourceItem(file, _T("DLG_CHARSET"),
		_T("&Character set")
	);
	readResourceItem(file, _T("DLG_OK"),
		_T("OK")
	);
	readResourceItem(file, _T("DLG_CANCEL"),
		_T("Cancel")
	);

	readResourceItem(file, _T("DLG_CHARSET_ANSI"),
		_T("Ansi")
	);
	readResourceItem(file, _T("DLG_CHARSET_BALTIC"),
		_T("Baltic")
	);
	readResourceItem(file, _T("DLG_CHARSET_BIG5"),
		_T("Traditional Chinese")
	);
	readResourceItem(file, _T("DLG_CHARSET_DEFAULT"),
		_T("Default")
	);
	readResourceItem(file, _T("DLG_CHARSET_EASTEUROPE"),
		_T("East Europe")
	);
	readResourceItem(file, _T("DLG_CHARSET_GB2312"),
		_T("Simplified Chinese")
	);
	readResourceItem(file, _T("DLG_CHARSET_GREEK"),
		_T("Greek")
	);
	readResourceItem(file, _T("DLG_CHARSET_HANGUL"),
		_T("Hangul")
	);
	readResourceItem(file, _T("DLG_CHARSET_MAC"),
		_T("Mac")
	);
	readResourceItem(file, _T("DLG_CHARSET_OEM"),
		_T("OEM")
	);
	readResourceItem(file, _T("DLG_CHARSET_RUSSIAN"),
		_T("Russian")
	);
	readResourceItem(file, _T("DLG_CHARSET_SHIFTJIS"),
		_T("Japanese")
	);
	readResourceItem(file, _T("DLG_CHARSET_SYMBOL"),
		_T("Symbol")
	);
	readResourceItem(file, _T("DLG_CHARSET_TURKISH"),
		_T("Turkish")
	);
	readResourceItem(file, _T("DLG_CHARSET_VIETNAMESE"),
		_T("Vietnamese")
	);
	readResourceItem(file, _T("DLG_CHARSET_JOHAB"),
		_T("Johab Korean")
	);
	readResourceItem(file, _T("DLG_CHARSET_ARABIC"),
		_T("Arabic")
	);
	readResourceItem(file, _T("DLG_CHARSET_HEBREW"),
		_T("Hebrew")
	);
	readResourceItem(file, _T("DLG_CHARSET_THAI"),
		_T("Thai")
	);

	readResourceItem(file, _T("DLG_STYLE_NORMAL"),
		_T("Normal")
	);
	readResourceItem(file, _T("DLG_STYLE_ITALIC"),
		_T("Italic")
	);
	readResourceItem(file, _T("DLG_STYLE_BOLD"),
		_T("Bold")
	);
	readResourceItem(file, _T("DLG_STYLE_BOLD_ITALIC"),
		_T("Bold Italic")
	);

	readResourceItem(file, _T("MSG_SETTING_FILE"),
		_T("Font settings file(*.ini)")
	);
	readResourceItem(file, _T("MSG_ALL_FILE"),
		_T("All file(*.*)")
	);
	readResourceItem(file, _T("MSG_LOAD_FAIL"),
		_T("Font settings file load failed.")
	);
	readResourceItem(file, _T("MSG_SAVE_FAIL"),
		_T("Font settings file save failed.")
	);
	readResourceItem(file, _T("MSG_ERROR"),
		_T("Error")
	);
	readResourceItem(file, _T("MSG_ABOUT"),
		_T("About No!! Meiryo UI")
	);
	readResourceItem(file, _T("MSG_NO_FONT"),
		_T("Please select font.")
	);
	readResourceItem(file, _T("MSG_NO_STYLE"),
		_T("Please select style.")
	);
	readResourceItem(file, _T("MSG_NO_SIZE"),
		_T("Please select size.")
	);
	readResourceItem(file, _T("MSG_NO_CHARSET"),
		_T("Please select charset.")
	);
	readResourceItem(file, _T("MSG_TRANSLATE"),
		_T("Tatsuhiko Shoji(Tatsu)")
	);
}

