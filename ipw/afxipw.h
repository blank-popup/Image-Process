#ifndef __APX_IPW__
#define __APX_IPW__

#include "pch.h"
#include "ipw.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "ipwDoc.h"
#include "ipwView.h"
#include "ImageMat.h"

void AfmDisplayNew(cv::Mat& mat);
void AfmInform(CString message);
void AfmInform(LPCTSTR lpszFormat, ...);
CString AfmString(LPCTSTR lpszFormat, ...);
void AfmGetChildWindowTitles(std::vector<CString>& titles);
CChildFrame* AfmGetActiveFrame();
CipwView* AfmGetActiveView();
CipwDoc* AfmGetActiveDocument();
CipwDoc* AfmFindDocument(int id);

int AfmGetId(CipwView* pView);
int AfmGetId(CipwDoc* pDoc);

#endif // __APX_IPW__
