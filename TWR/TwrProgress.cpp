#include <windows.h>

#include "AppliBase.h"
#include "TwrProgress.h"

/**
 * �R���X�g���N�^
 */
TwrProgress::TwrProgress() : TwrWnd()
{
}

/**
 * �R���X�g���N�^
 *
 * @param newHwnd �I�u�W�F�N�g�Ɍ��ѕt����E�C���h�E�n���h��
 */
TwrProgress::TwrProgress(HWND newHwnd) : TwrWnd()
{
	hWnd = newHwnd;
}

/**
 * �v���O���X�o�[�͈̔͂�ݒ肷��B
 *
 * @param min �ŏ��l
 * @param max �ő�l
 */
void TwrProgress::setRange(unsigned int min, unsigned int max)
{
	::SendMessage(hWnd, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(min, max));
}

/**
 * �v���O���X�o�[�̐i�ޕ���ݒ肷��B
 *
 * @param step �i�ޕ�
 */
void TwrProgress::setStep(int step)
{
	::SendMessage(hWnd, PBM_SETSTEP, (WPARAM)step, (LPARAM)0);
}

/**
 * �v���O���X�o�[�̈ʒu��ݒ肷��B
 *
 * @param pos �ʒu
 */
void TwrProgress::setPos(int pos)
{
	::SendMessage(hWnd, PBM_SETPOS, (WPARAM)pos, (LPARAM)0);
}

/**
 * �v���O���X�o�[�̒l��i�߂�B
 */
void TwrProgress::stepIt(void)
{
	::SendMessage(hWnd, PBM_STEPIT, (WPARAM)0, (LPARAM)0);
}

