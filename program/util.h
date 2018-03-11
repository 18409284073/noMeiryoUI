/*
noMeiryoUI (C) 2005,2012-2017 Tatsuhiko Shoji
The sources for noMeiryoUI are distributed under the MIT open source license
*/
#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <vector>
#include "TWR/tstring.h"

#define PRESET_SECTION "PRESET"

extern bool WIN8_SIZE;
extern std::vector<tstring> langResource;
/** �t�H���g��(Windows 8.x) */
extern std::vector<tstring> fontFaces8;
/** �t�H���g�T�C�Y(Windows 8.x) */
extern std::vector<int> fontSizes8;
/** �t�H���g�����Z�b�g(Windows 8.x) */
extern std::vector<int> fontCharset8;
/** �t�H���g��(Windows 10) */
extern std::vector<tstring> fontFaces10;
/** �t�H���g�T�C�Y(Windows 8.x) */
extern std::vector<int> fontSizes10;
/** �t�H���g�����Z�b�g(Windows 10) */
extern std::vector<int> fontCharset10;
/** �R�[�h�y�[�W */
extern int codePage;

int getFontPointInt(LOGFONT *font, HWND hWnd);
double getFontPoint(LOGFONT *font, HWND hWnd);
void readResourceFile(TCHAR *file);
int readFontResource8(TCHAR *file);
int readFontResource10(TCHAR *file);
int setFontResourceJa8(void);
int setFontResourceJa10(void);
void adjustCenter(RECT parentRect, HWND parentHWnd, HWND myHWnd);

#endif
