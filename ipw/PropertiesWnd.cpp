
#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "ipw.h"
#include "afxipw.h"
#include "ipwDoc.h"
#include "ImageMat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept
{
	m_nComboHeight = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_BN_CLICKED(IDU_BUTTON_PROCESS_NEW_WINDOW, OnButtonProcessNewWindow)
	ON_UPDATE_COMMAND_UI(IDU_BUTTON_PROCESS_NEW_WINDOW, OnUpdateButtonProcessNewWindow)
	ON_BN_CLICKED(IDU_BUTTON_PROCESS_EXISTING_WINDOW, OnButtonProcessExistingWindow)
	ON_UPDATE_COMMAND_UI(IDU_BUTTON_PROCESS_EXISTING_WINDOW, OnUpdateButtonProcessExistingWindow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int cyButton = 20;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight + cyTlb + 2 * cyButton), SWP_NOACTIVATE | SWP_NOZORDER);

	if (m_buttonProcessNewWindow.GetSafeHwnd() == nullptr)
	{
		m_buttonProcessNewWindow.Create(_T("Process in New Window"), WS_CHILD | WS_VISIBLE, CRect(rectClient.left, rectClient.Height() - 2 * cyButton, rectClient.Width(), rectClient.Height() - cyButton), this, IDU_BUTTON_PROCESS_NEW_WINDOW);
	}
	else
	{
		m_buttonProcessNewWindow.SetWindowPos(nullptr, rectClient.left, rectClient.Height() - 2 * cyButton, rectClient.Width(), cyButton, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	m_buttonProcessNewWindow.ShowWindow(SW_SHOW);
	m_buttonProcessNewWindow.EnableWindow(TRUE);

	if (m_buttonProcessExistingWindow.GetSafeHwnd() == nullptr)
	{
		m_buttonProcessExistingWindow.Create(_T("Process in Existing Window"), WS_CHILD | WS_VISIBLE, CRect(rectClient.left, rectClient.Height() - cyButton, rectClient.Width(), rectClient.Height()), this, IDU_BUTTON_PROCESS_EXISTING_WINDOW);
	}
	else
	{
		m_buttonProcessExistingWindow.SetWindowPos(nullptr, rectClient.left, rectClient.Height() - cyButton, rectClient.Width(), cyButton, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	m_buttonProcessExistingWindow.ShowWindow(SW_SHOW);
	m_buttonProcessExistingWindow.EnableWindow(TRUE);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Open"));
	m_wndObjectCombo.AddString(_T("Save"));
	m_wndObjectCombo.AddString(_T("Invert"));
	m_wndObjectCombo.AddString(_T("Add Intensity Linear"));
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


void CPropertiesWnd::OnButtonProcessNewWindow()
{
	// TODO: Add your implementation code here.
	CString command;
	m_wndObjectCombo.GetLBText(m_wndObjectCombo.GetCurSel(), command);

	std::vector<CString> titles = {};
	AfmGetChildWindowTitles(titles);

	if (command.Compare(_T("Open")) == 0)
	{
		//CString tt;
		//((CMDIFrameWnd*)AfxGetMainWnd())->GetActiveFrame()->GetWindowTextW(tt);
		//AfxMessageBox(tt);
		//CipwDoc* tt = AfmFindDocument(titles[2]);
		//cv::Mat aa = tt->GetImageMat()->GetMat();
		AfxMessageBox(AfmString(_T("NNNNNNNNNNNN")));

		//for (int i = 0; i < titles.size(); ++i)
		//{
		//	AfxMessageBox(titles[i]);
		//}
	}
}

void CPropertiesWnd::OnUpdateButtonProcessNewWindow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CPropertiesWnd::OnButtonProcessExistingWindow()
{
	// TODO: Add your implementation code here.
	CString command;
	m_wndObjectCombo.GetLBText(m_wndObjectCombo.GetCurSel(), command);

	std::vector<CString> titles = {};
	AfmGetChildWindowTitles(titles);

	if (command.Compare(_T("Open")) == 0)
	{
		//CString tt;
		//((CMDIFrameWnd*)AfxGetMainWnd())->GetActiveFrame()->GetWindowTextW(tt);
		//AfxMessageBox(tt);
		//CipwDoc* tt = AfmFindDocument(titles[2]);
		//cv::Mat aa = tt->GetImageMat()->GetMat();
		AfxMessageBox(AfmString(_T("EEEEEEEEEEEEEEEEEEEEE")));

		//for (int i = 0; i < titles.size(); ++i)
		//{
		//	AfxMessageBox(titles[i]);
		//}
	}
}

void CPropertiesWnd::OnUpdateButtonProcessExistingWindow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}


void CPropertiesWnd::SetCurrObjectCombo(CString item)
{
	// TODO: Add your implementation code here.
	m_wndObjectCombo.SelectString(0, item);
	//m_wndObjectCombo.SetCurSel(m_wndObjectCombo.FindStringExact(0, _T("Save")));
}


void CPropertiesWnd::SetOpenPropList(CString titleWnd, CString filepath)
{
	// TODO: Add your implementation code here.
	SetCurrObjectCombo(_T("Open"));
	m_wndPropList.RemoveAll();

	CMFCPropertyGridProperty* pInput = new CMFCPropertyGridProperty(_T("Input"));
	static const TCHAR szFilter[] = _T("Image Files(*.tif;*.jpg;*.bmp;*.pgm)|*.tif;*.jpg;*.bmp;*.pgm|All Files(*.*)|*.*||");
	pInput->AddSubItem(new CMFCPropertyGridFileProperty(_T("Image"), TRUE, filepath, _T("image"), 0, szFilter, _T("Filepath selected")));
	m_wndPropList.AddProperty(pInput);

	CMFCPropertyGridProperty* pOutput = new CMFCPropertyGridProperty(_T("Output"));
	pOutput->AddSubItem(new CMFCPropertyGridProperty(_T("Window"), (_variant_t)titleWnd, _T("Window title")));
	m_wndPropList.AddProperty(pOutput);
}

