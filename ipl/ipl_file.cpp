#include "pch.h"
#include "ipl_file.h"

int ipl_display_cv_window(std::string title, cv::Mat mat, int flags) {
    cv::namedWindow(title, flags);
    cv::imshow(title, mat);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}

int ipl_read_image(cv::Mat& mat, std::string filepath, int flags) {
    mat = cv::imread(filepath, flags);

    return 0;
}

int ipl_write_image(std::string filepath, cv::Mat& mat, const std::vector<int>& params) {
    cv::imwrite(filepath, mat, params);

    return 0;
}

int ipl_say_greeting(std::string greeting) {
	cv::namedWindow("image", 1);

	cv::Mat testMat = cv::Mat::zeros(200, 900, CV_8UC3);
	cv::putText(testMat, greeting, cvPoint(100, 100), cv::FONT_HERSHEY_PLAIN, 3, cvScalar(0, 0, 255), 4);

	cv::imshow("image", testMat);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}

int ipl_blend(cv::Mat& mat, std::string file1, std::string file2, double alpha) {
    cv::Mat src1 = cv::imread(file1);
    cv::Mat src2 = cv::imread(file2);
    cv::addWeighted(src1, alpha, src2, 1.0 - alpha, 0.0, mat);

    return 0;
}
