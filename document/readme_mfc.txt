[MFC Executing Order]
CipwApp::CipwApp() noexcept
BOOL CipwApp::InitInstance()
CMainFrame::CMainFrame() noexcept
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
CipwDoc::CipwDoc() noexcept
CChildFrame::CChildFrame() noexcept
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
CipwView::CipwView() noexcept
BOOL CipwView::PreCreateWindow(CREATESTRUCT& cs)
BOOL CipwDoc::OnNewDocument()
void CipwView::OnDraw(CDC* /*pDC*/)

CipwView::~CipwView()
CChildFrame::~CChildFrame()
CipwDoc::~CipwDoc()
CMainFrame::~CMainFrame()
int CipwApp::ExitInstance()


[Open File]
CipwDoc::OnOpenDocument
CipwView::OnInitialUpdate
CipwView::OnDraw

[Image Process]
CipwDoc::OnImageProcess
CipwDoc::OnNewDocument
CipwView::OnInitialUpdate
CipwView::OnDraw



[Projects]
ip: cli
ipw: gui(MFC)
ipl: dll using in ip and ipw


Configure MFC Project
Application type options: UNcheck Tabbed documents
Project style: Visual Studio
Visual stype and colors: Visual Studio 2008
Use of MFC: Use MFC in a static library
Advanced frame panes: check Output docking pane
Generated classes: View
Base class: CScrollView


[Error]
warning C4190: 'read_image' has C-linkage specified, but returns UDT 'cv::Mat' which is incompatible with C
If return type is c++ variable in dll, for example cv::Mat, word extern "C" is not available
cv::Mat read_image(std::string filepath);
The solution is using call by reference
int read_image(cv::Mat& mat, std::string filepath);

error LNK2019: unresolved external symbol __imp_read_image referenced in function "public: virtual int __cdecl CipwDoc::OnOpenDocument(wchar_t const *)" (?OnOpenDocument@CipwDoc@@UEAAHPEB_W@Z)
project ipw - references - Add Reference : ipl

In debuging ipw, error occur
Expression: __acrt_first_block == header
https://stackoverflow.com/questions/35310117/debug-assertion-failed-expression-acrt-first-block-header
the final solution was actually was to have all of the projects (the exe, dll and the entire googleTest project) built in Multi-threaded Debug DLL (/MDd) (the GoogleTest projects are built in Multi-threaded debug(/MTd) by default)
ipw - Configuration Properties - C/C++ - Code Generation - Runtime Library : Multi-threaded Debug DLL (/MDd) (same as ipl)
But, error occur in build.
fatal error C1189: #error:  Building MFC application with /MD[d] (CRT dll version) requires MFC shared dll version. Please #define _AFXDLL or do not use /MD[d]
The solution is coding #define _AFXDLL in pch.h
#ifdef _DEBUG
#define _AFXDLL
#endif


[Coding]
ipw\OutputWnd.cpp(101,17): error C3861: 'max': identifier not found
max, min in stdlib.h are the standard of C
max, min -> __max, __min


// --M_M--  Do NOT open new document when starting application
BOOL CipwApp::InitInstance()
    ...
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
    {
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
    }
    ...


// --M_M--  Open File: Class Wizard - Virtual Function - OnOpenDocument
BOOL CipwDoc::OnOpenDocument(LPCTSTR lpszPathName)


// --M_M--  Convert CView to CScrollView
ipwView.h: class CipwView : public ClView -> class CipwView : public CScrollView
ipwView.cpp: IMPLEMENT_DYNCREATE(CipwView, CView) -> IMPLEMENT_DYNCREATE(CipwView, CScrollView)
ipwView.cpp: BEGIN_MESSAGE_MAP(CipwView, CView) -> BEGIN_MESSAGE_MAP(CipwView, CScrollView)
Class Wizard - Virtual Function - OnInitialUpdate



CString to string
case: Multi-byte character set
    CString cs;
    std::string s((LPCTSTR)cs);
case: Unicode charater set
    CString cs;
    CT2CA pszConvertedAnsiString(cs);
    std::string s(pszConvertedAnsiString);  

string to CString
    std::string s;
    CString cs(s.c_str());
