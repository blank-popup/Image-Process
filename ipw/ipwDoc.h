
// ipwDoc.h : interface of the CipwDoc class
//


#pragma once


typedef struct _process_item {
	CString name;
	int number;
	CString process;
	int previous;
	int next;
	void* option;
} process_item;

class CipwDoc : public CDocument
{
protected: // create from serialization only
	CipwDoc() noexcept;
	DECLARE_DYNCREATE(CipwDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CipwDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
protected:
	ImageMat* m_pIm;
public:
	ImageMat* GetImageMat();
	void SetImageMat(ImageMat* im);
protected:
	process_item m_processItem;
public:
	process_item GetProcessItem();
	void SetProcessItem(process_item item);
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnImageInvert();
	afx_msg void OnImageAddintensitylinear();
};
