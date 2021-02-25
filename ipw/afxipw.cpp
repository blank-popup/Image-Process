#include "pch.h"
#include "ipw.h"
#include "MainFrm.h"

#include "afxipw.h"
#include "ImageMat.h"


void AfmDisplay(cv::Mat& mat)
{
	theApp.GetImageMat()->SetMat(mat);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfmInform(CString message)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndOutput.AddString(message);
}

void AfmInform(LPCTSTR lpszFormat, ...)
{
	CString message;

	va_list argList;
	va_start(argList, lpszFormat);
	message.FormatV(lpszFormat, argList);
	va_end(argList);

	AfmInform(message);
}

CString AfmString(LPCTSTR lpszFormat, ...)
{
	CString str;

	va_list argList;
	va_start(argList, lpszFormat);
	str.FormatV(lpszFormat, argList);
	va_end(argList);

	return str;
}
