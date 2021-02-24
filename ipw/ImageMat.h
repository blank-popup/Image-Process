#pragma once
#include <afx.h>
class ImageMat :
    public CObject
{
public:
    ImageMat();
    ImageMat(cv::Mat& mat);
    ImageMat(const ImageMat& im);
    ~ImageMat();
    ImageMat& operator=(const ImageMat& mat);
protected:
    cv::Mat m_mat;
public:
    void DrawMat(CDC* pDC);
    void SetMat(cv::Mat& mat);
    cv::Mat GetMat();
};
