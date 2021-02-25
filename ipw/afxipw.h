#ifndef __APX_IPW__
#define __APX_IPW__

void AfmDisplay(cv::Mat& mat);
void AfmInform(CString message);
void AfmInform(LPCTSTR lpszFormat, ...);
CString AfmString(LPCTSTR lpszFormat, ...);
void GetChildWindowTitles(std::vector<CString>& titles);

#endif // __APX_IPW__
