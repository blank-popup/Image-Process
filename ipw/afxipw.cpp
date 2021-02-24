#include "pch.h"
#include "ipw.h"
#include "MainFrm.h"

#include "afxipw.h"
#include "ImageMat.h"


void AfxDisplay(cv::Mat& mat)
{
	theApp.GetImageMat()->SetMat(mat);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void AfxInform(CString message)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndOutput.AddString(message);
}

void AfxInform(LPCTSTR lpszFormat, ...)
{
	CString message;

	va_list argList;
	va_start(argList, lpszFormat);
	message.FormatV(lpszFormat, argList);
	va_end(argList);

	AfxInform(message);
}

CString AfxString(LPCTSTR lpszFormat, ...)
{
	CString str;

	va_list argList;
	va_start(argList, lpszFormat);
	str.FormatV(lpszFormat, argList);
	va_end(argList);

	return str;
}
