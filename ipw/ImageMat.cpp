#include "pch.h"
#include "ipw.h"
#include "ImageMat.h"


ImageMat::ImageMat()
{
	m_mat = cv::Mat();
}

ImageMat::ImageMat(cv::Mat& mat)
{
    m_mat = mat.clone();
}

ImageMat::ImageMat(const ImageMat& im)
{
    m_mat = im.m_mat.clone();
}

ImageMat::~ImageMat()
{
}

ImageMat& ImageMat::operator=(const ImageMat& im)
{
	if (this == &im)
	{
		return *this;
	}

    ImageMat* newim = new ImageMat();
    newim->m_mat = im.m_mat.clone();
    return *newim;
}

void ImageMat::DrawMat(CDC* pDC)
{
    // TODO: Add your implementation code here.
    if (m_mat.empty())
    {
        return;
    }

	cv::Mat mat;
	switch (m_mat.channels())
	{
	case 1:
		cv::cvtColor(m_mat, mat, cv::COLOR_GRAY2BGRA);
		break;
	case 3:
		cvtColor(m_mat, mat, cv::COLOR_BGR2BGRA);
		break;
	default:
		break;
	}

	int nPixelBytes = mat.channels() * (mat.depth() + 1);
	
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * nPixelBytes;
	bitInfo.bmiHeader.biWidth = mat.cols;
	bitInfo.bmiHeader.biHeight = - mat.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, mat.cols, mat.rows,
		0, 0, mat.cols, mat.rows,
		mat.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}


void ImageMat::SetMat(cv::Mat& mat)
{
    // TODO: Add your implementation code here.
    m_mat = mat;
}


cv::Mat ImageMat::GetMat()
{
    // TODO: Add your implementation code here.
    return m_mat;
}
