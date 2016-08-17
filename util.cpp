/*
noMeiryoUI (C) 2005,2012-2015 Tatsuhiko Shoji
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
 */
void readResourceItem(TCHAR *file, TCHAR *key)
{
	TCHAR buf[255];

	// INI�t�@�C����ǂݍ��ށBUnicode�ł�API�ł��t�@�C������Unicode�̏ꍇ��
	// �e����̕����R�[�h�̃t�@�C���Ƃ��ēǂ�ł����B
	GetPrivateProfileString(_T("RESOURCE"), key, _T(""), buf, 255, file);

	langResource.push_back(buf);
}

/**
 * ���\�[�X�̓ǂݍ��݂��J�n����B
 *
 * @param file ���\�[�X�t�@�C����
 */
void readResourceFile(TCHAR *file)
{
	readResourceItem(file, _T("FONT_FACE"));
	readResourceItem(file, _T("TITLE"));
	readResourceItem(file, _T("MENU_FILE"));
	readResourceItem(file, _T("MENU_FILE_LOAD"));
	readResourceItem(file, _T("MENU_FILE_SAVE"));
	readResourceItem(file, _T("MENU_FILE_SET_QUIT"));
	readResourceItem(file, _T("MENU_FILE_QUIT"));
	readResourceItem(file, _T("MENU_PRESET"));
	readResourceItem(file, _T("MENU_PRESET_8"));
	readResourceItem(file, _T("MENU_PRESET_10"));
	readResourceItem(file, _T("MENU_TOOLS"));
	readResourceItem(file, _T("MENU_TOOLS_THREAD"));
	readResourceItem(file, _T("MENU_TOOLS_SEVEN"));
	readResourceItem(file, _T("MENU_HELP"));
	readResourceItem(file, _T("MENU_HELP_HELP"));
	readResourceItem(file, _T("MENU_HELP_ABOUT"));

}

