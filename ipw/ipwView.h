
// ipwView.h : interface of the CipwView class
//

#pragma once


class CipwView : public CScrollView
{
protected: // create from serialization only
	CipwView() noexcept;
	DECLARE_DYNCREATE(CipwView)

// Attributes
public:
	CipwDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CipwView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void SetScrollSizeToFit();
	void ShowImageInfo(CPoint point);
};

#ifndef _DEBUG  // debug version in ipwView.cpp
inline CipwDoc* CipwView::GetDocument() const
   { return reinterpret_cast<CipwDoc*>(m_pDocument); }
#endif

