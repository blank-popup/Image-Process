#ifndef __IPL_FILE__
#define __IPL_FILE__

#ifdef IPL_EXPORTS
#define IPL_PORT __declspec(dllexport)
#else
#define IPL_PORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	IPL_PORT int ipl_display_cv_window(std::string title, cv::Mat mat, int flags = 1);
	IPL_PORT int ipl_read_image(cv::Mat& mat, std::string filepath, int flags = 1);
	IPL_PORT int ipl_write_image(std::string filepath, cv::Mat& mat, const std::vector<int>& params = std::vector<int>());
	IPL_PORT int ipl_say_greeting(std::string greeting);
	IPL_PORT int ipl_blend(cv::Mat& mat, std::string file1, std::string file2, double alpha);

#ifdef __cplusplus
}
#endif

#endif // __IPL_FILE__
