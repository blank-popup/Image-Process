#ifndef __IPL_PIXEL__
#define __IPL_PIXEL__

#ifdef IPL_EXPORTS
#define IPL_PORT __declspec(dllexport)
#else
#define IPL_PORT __declspec(dllimport)
#endif

typedef struct _iplp_coefficient {
    int a;
    int b;
} iplp_coefficient;

typedef struct _iplp_linearity {
    int aa;
    int ab;
    int A;
    int ba;
    int bb;
    int B;
} iplp_linearity;

#ifdef __cplusplus
extern "C" {
#endif

    IPL_PORT int ipl_adjust_intensity(cv::Mat& mat_out, cv::Mat& mat_in, std::vector<std::vector<int>> ce);
    IPL_PORT int ipl_add_linear_intensity(cv::Mat& mat_out, cv::Mat& mat_in, std::vector<std::vector<int>> ce);




    IPL_PORT int ipl_adjust_intensity_bgra(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_coefficient b, iplp_coefficient g, iplp_coefficient r, iplp_coefficient a,
        int(*method)(uchar*, int, int, int, iplp_coefficient, iplp_coefficient, iplp_coefficient, iplp_coefficient));
    IPL_PORT int ipl_function_adjust_intensity_bgra(uchar* mat_data, int row, int column, int columns,
        iplp_coefficient b, iplp_coefficient g, iplp_coefficient r, iplp_coefficient a);
    IPL_PORT int ipl_adjust_intensity_bgr(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_coefficient b, iplp_coefficient g, iplp_coefficient r,
        int(*method)(uchar*, int, int, int, iplp_coefficient, iplp_coefficient, iplp_coefficient));
    IPL_PORT int ipl_function_adjust_intensity_bgr(uchar* mat_data, int row, int column, int columns,
        iplp_coefficient b, iplp_coefficient g, iplp_coefficient r);
    IPL_PORT int ipl_adjust_intensity_wk(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_coefficient wk,
        int(*method)(uchar*, int, int, int, iplp_coefficient));
    IPL_PORT int ipl_function_adjust_intensity_wk(uchar* mat_data, int row, int column, int columns,
        iplp_coefficient wk);

    IPL_PORT int ipl_add_linear_intensity_bgra(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_linearity b, iplp_linearity g, iplp_linearity r, iplp_linearity a,
        int(*method)(uchar*, int, int, int, iplp_linearity, iplp_linearity, iplp_linearity, iplp_linearity));
    IPL_PORT int ipl_function_add_linear_intensity_bgra(uchar* mat_data, int row, int column, int columns,
        iplp_linearity b, iplp_linearity g, iplp_linearity r, iplp_linearity a);
    IPL_PORT int ipl_add_linear_intensity_bgr(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_linearity b, iplp_linearity g, iplp_linearity r,
        int(*method)(uchar*, int, int, int, iplp_linearity, iplp_linearity, iplp_linearity));
    IPL_PORT int ipl_function_add_linear_intensity_bgr(uchar* mat_data, int row, int column, int columns,
        iplp_linearity b, iplp_linearity g, iplp_linearity r);
    IPL_PORT int ipl_add_linear_intensity_wk(cv::Mat& mat_out, cv::Mat& mat_in,
        iplp_linearity wk,
        int(*method)(uchar*, int, int, int, iplp_linearity));
    IPL_PORT int ipl_function_add_linear_intensity_wk(uchar* mat_data, int row, int column, int columns,
        iplp_linearity wk);

#ifdef __cplusplus
}
#endif

#endif // __IPL_PIXEL__
