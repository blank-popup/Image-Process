
// ipwDoc.cpp : implementation of the CipwDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ipw.h"
#endif

#include "ipwDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "afxipw.h"
#include "MainFrm.h"
#include "ImageMat.h"
#include "ipl.h"

// CipwDoc

IMPLEMENT_DYNCREATE(CipwDoc, CDocument)

BEGIN_MESSAGE_MAP(CipwDoc, CDocument)
	ON_COMMAND(ID_IMAGE_INVERT, &CipwDoc::OnImageInvert)
	ON_COMMAND(ID_IMAGE_ADDINTENSITYLINEAR, &CipwDoc::OnImageAddintensitylinear)
END_MESSAGE_MAP()


// CipwDoc construction/destruction

CipwDoc::CipwDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pIm = new ImageMat();
}

CipwDoc::~CipwDoc()
{
	if (m_pIm != nullptr)
	{
		m_pIm->GetMat().release();
		delete m_pIm;
	}
}

ImageMat* CipwDoc::GetImageMat()
{
	// TODO: Add your implementation code here.
	return m_pIm;
}


void CipwDoc::SetImageMat(ImageMat* im)
{
	// TODO: Add your implementation code here.
	m_pIm = im;
}


process_item CipwDoc::GetProcessItem()
{
	// TODO: Add your implementation code here.
	return m_processItem;
}


void CipwDoc::SetProcessItem(process_item item)
{
	// TODO: Add your implementation code here.
	m_processItem = item;
}


// CipwDoc commands


BOOL CipwDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	ImageMat* im = theApp.GetImageMat();
	if (im != nullptr)
	{
		if (im->GetMat().cols != 0 && im->GetMat().rows != 0)
		{
			m_pIm->SetMat(im->GetMat());
		}
		else
		{
			cv::Mat mat(256, 256, CV_8UC3, cv::Scalar(127, 127, 127));
			m_pIm->SetMat(mat);
		}
	}

	return TRUE;
}




// CipwDoc serialization

void CipwDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CipwDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CipwDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CipwDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CipwDoc diagnostics

#ifdef _DEBUG
void CipwDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CipwDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


BOOL CipwDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	CT2CA pszConvertedAnsiString(lpszPathName);
	std::string filepath(pszConvertedAnsiString);

	// error in debug: __acrt_first_block == header
	cv::Mat mat;
	ipl_read_image(mat, filepath, -1);
	//AfxInform(_T("%d"), mat.channels());
	//AfxInform(_T("%hs"), __FUNCTION__);
	//ipl_display_cv_window("TEST WINDOW", mat);

	CString filename = CString(lpszPathName).Mid(CString(lpszPathName).ReverseFind('\\') + 1);
	int countProcess = ((CipwApp*)AfxGetApp())->GetCountProcess();
	SetProcessItem({filename, countProcess, _T("Open"), {}, {}, {}});
	((CipwApp*)AfxGetApp())->IncreaseCountProcess();

	CMainFrame* mainFrame = (CMainFrame*)AfxGetMainWnd();
	CString titleWnd = AfxString(_T("%s : %02d"), filename, countProcess);
	mainFrame->m_wndProperties.SetOpenPropList(titleWnd, CString(lpszPathName));
	mainFrame->m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	mainFrame->DockPane(&mainFrame->m_wndProperties);

	m_pIm->SetMat(mat);

	return TRUE;
}


void CipwDoc::OnImageInvert()
{
	// TODO: Add your command handler code here
	cv::Mat mat;
	if (m_pIm->GetMat().channels() == 1)
	{
		ipl_adjust_intensity_wk(mat, m_pIm->GetMat(), {-1, 255}, ipl_function_adjust_intensity_wk);
	}
	else if (m_pIm->GetMat().channels() == 3)
	{
		ipl_adjust_intensity_bgr(mat, m_pIm->GetMat(), {-1, 255}, {-1, 255}, {-1, 255}, ipl_function_adjust_intensity_bgr);
	}
	else if (m_pIm->GetMat().channels() == 4)
	{
		ipl_adjust_intensity_bgra(mat, m_pIm->GetMat(), {-1, 255}, {-1, 255}, {-1, 255}, {-1, 255}, ipl_function_adjust_intensity_bgra);
	}
	else
	{
		return;
	}

	AfxDisplay(mat);
}


void CipwDoc::OnImageAddintensitylinear()
{
	// TODO: Add your command handler code here
	cv::Mat mat;
	if (m_pIm->GetMat().channels() == 1)
	{
		ipl_add_linear_intensity_wk(mat, m_pIm->GetMat(), {20, 200, -100, 240, 10, 400}, ipl_function_add_linear_intensity_wk);
	}
	else if (m_pIm->GetMat().channels() == 3)
	{
		ipl_add_linear_intensity_bgr(mat, m_pIm->GetMat(), {20, 200, -100, 240, 10, 400}, {20, 200, -100, 240, 10, 400}, {20, 200, -100, 240, 10, 400}, ipl_function_add_linear_intensity_bgr);
	}
	else if (m_pIm->GetMat().channels() == 4)
	{
		ipl_add_linear_intensity_bgra(mat, m_pIm->GetMat(), {20, 200, -100, 240, 10, 400}, {20, 200, -100, 240, 10, 400}, {20, 200, -100, 240, 10, 400}, {20, 200, -100, 240, 10, 400}, ipl_function_add_linear_intensity_bgra);
	}
	else
	{
		return;
	}

	AfxDisplay(mat);
}
