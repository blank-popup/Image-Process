
// ipw.h : main header file for the ipw application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CipwApp:
// See ipw.cpp for the implementation of this class
//

class ImageMat;

class CipwApp : public CWinAppEx
{
public:
	CipwApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	ImageMat* m_pIm;
public:
	ImageMat* GetImageMat();
	void SetImageMat(ImageMat* im);
};

extern CipwApp theApp;
