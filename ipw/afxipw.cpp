#include "pch.h"
#include "afxipw.h"


void AfmDisplayNew(cv::Mat& mat)
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

void AfmGetChildWindowTitles(std::vector<CString>& titles)
{
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

CChildFrame* AfmGetActiveFrame()
{
	//In sub thread, use "AfxGetApp()->m_pMainWnd" instead of "AfxGetMainWnd()"
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	return (CChildFrame*)pMainFrame->GetActiveFrame();
}

CipwView* AfmGetActiveView()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	return (CipwView*)pChildFrame->GetActiveView();
}

CipwDoc* AfmGetActiveDocument()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CChildFrame* pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	return (CipwDoc*)pChildFrame->GetActiveDocument();
}


CipwDoc* AfmFindDocument(int id)
{
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
				if (id == AfmGetId(pView))
				{
					return (CipwDoc*)pView->GetDocument();
				}
			}
		}
	}

	return nullptr;
}


int AfmGetId(CipwView* pView)
{
	CString title;
	pView->GetParentFrame()->GetWindowTextW(title);
	return _ttoi(title.Mid(0, title.Find(_T(" _ "), 0)));
}

int AfmGetId(CipwDoc* pDoc)
{
	return pDoc->GetProcessItem().id;
}
