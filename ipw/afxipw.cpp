#include "pch.h"
#include "ipw.h"
#include "MainFrm.h"

#include "afxipw.h"
#include "ipwDoc.h"
#include "ipwView.h"
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

void GetChildWindowTitles(std::vector<CString>& titles)
{
	// TODO: Add your implementation code here.
	CipwApp* pApp = (CipwApp*)AfxGetApp();
	POSITION positionDocTemplate = pApp->GetFirstDocTemplatePosition();
	while (positionDocTemplate != nullptr)
	{
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(positionDocTemplate);
		POSITION positionDoc = pDocTemplate->GetFirstDocPosition();
		while (positionDoc != nullptr)
		{
			CipwDoc* pDoc = (CipwDoc*)pDocTemplate->GetNextDoc(positionDoc);
			POSITION positionView = pDoc->GetFirstViewPosition();
			while (positionView != nullptr)
			{
				CipwView* pView = (CipwView*)pDoc->GetNextView(positionView);
				CString title;
				pView->GetParentFrame()->GetWindowTextW(title);
				titles.push_back(title);
			}
		}
	}
}

