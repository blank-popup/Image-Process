
// ipwView.cpp : implementation of the CipwView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ipw.h"
#endif

#include "MainFrm.h"
#include "ipwDoc.h"
#include "ipwView.h"
#include "ImageMat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "afxipw.h"

// CipwView

IMPLEMENT_DYNCREATE(CipwView, CScrollView)

BEGIN_MESSAGE_MAP(CipwView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CipwView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CipwView construction/destruction

CipwView::CipwView() noexcept
{
	// TODO: add construction code here

}

CipwView::~CipwView()
{
}

BOOL CipwView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CipwView drawing

void CipwView::OnDraw(CDC* pDC)
{
	CipwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	ImageMat* im = pDoc->GetImageMat();
	if (im->GetMat().cols != 0 && im->GetMat().rows != 0)
	{
		im->DrawMat(pDC);
		theApp.GetImageMat()->SetMat(cv::Mat());
	}
}

void CipwView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	ImageMat* pIm = GetDocument()->GetImageMat();
	if (pIm->GetMat().cols != 0 && pIm->GetMat().rows != 0)
	{
		sizeTotal.cx = pIm->GetMat().cols;
		sizeTotal.cy = pIm->GetMat().rows;
	}

	CipwDoc* pDoc = GetDocument();
	int countProcess = pDoc->GetProcessItem().id;
	CString filename = pDoc->GetProcessItem().name;
	CString titleWnd = AfmString(_T("%d _ %s"), countProcess, filename);
	GetParent()->SetWindowTextW(titleWnd);

	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(FALSE);
}


// CipwView printing


void CipwView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CipwView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CipwView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CipwView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CipwView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CipwView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CipwView diagnostics

#ifdef _DEBUG
void CipwView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CipwView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CipwDoc* CipwView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CipwDoc)));
	return (CipwDoc*)m_pDocument;
}
#endif //_DEBUG


// CipwView message handlers


void CipwView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt = point + GetScrollPosition();
	ShowImageInfo(pt);

	CScrollView::OnMouseMove(nFlags, point);
}


void CipwView::SetScrollSizeToFit()
{
	// TODO: Add your implementation code here.
}


void CipwView::ShowImageInfo(CPoint point)
{
	// TODO: Add your implementation code here.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ImageMat* pIm = GetDocument()->GetImageMat();
	int w = pIm->GetMat().cols;
	int h = pIm->GetMat().rows;
	int c = pIm->GetMat().channels();

	CString strText;
	if (point.x >= 0 && point.y >= 0 && point.x < w && point.y < h)
	{
		strText.Format(_T("(%d, %d)"), point.x, point.y);
		pFrame->m_wndStatusBar.SetPaneText(1, strText);
	}
	if (c > 0)
	{
		strText.Format(_T("w:%d h:%d c:%d"), w, h, c);
		pFrame->m_wndStatusBar.SetPaneText(2, strText);
	}
}
