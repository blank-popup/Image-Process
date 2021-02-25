#ifndef __APX_IPW__
#define __APX_IPW__

void AfmDisplay(cv::Mat& mat);
void AfmInform(CString message);
void AfmInform(LPCTSTR lpszFormat, ...);
CString AfmString(LPCTSTR lpszFormat, ...);

#endif // __APX_IPW__
