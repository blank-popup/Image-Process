#include "pch.h"
#include "ipl_common.h"
#include "ipl_pixel.h"


int ipl_adjust_intensity(cv::Mat& mat_out, cv::Mat& mat_in, std::vector<std::vector<int>> ce) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int r = 0; r < mat_out.rows; ++r) {
        for (int c = 0; c < mat_out.cols; ++c) {
            int i = (r * mat_out.cols + c) * ce.size();
            for (int ch = 0; ch < ce.size(); ++ch) {
                mat_data[i + ch] = limit_intensity(
                                        ce[ch][0] * mat_data[i + ch] + ce[ch][1]
                                    );
            }
        }
    }

    return 0;
}


int ipl_add_linear_intensity(cv::Mat& mat_out, cv::Mat& mat_in, std::vector<std::vector<int>> ce) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int r = 0; r < mat_out.rows; ++r) {
        for (int c = 0; c < mat_out.cols; ++c) {
            int i = (r * mat_out.cols + c) * ce.size();
            for (int ch = 0; ch < ce.size(); ++ch) {
                double px = (
                                (ce[ch][4] - ce[ch][1]) * (ce[ch][4] - ce[ch][1]) * ce[ch][0]
                                + (ce[ch][3] - ce[ch][0]) * (ce[ch][3] - ce[ch][0]) * c
                                - (ce[ch][1] - r) * (ce[ch][3] - ce[ch][0]) * (ce[ch][4] - ce[ch][1])
                            )
                        / 
                            static_cast<double>(
                                (ce[ch][4] - ce[ch][1]) * (ce[ch][4] - ce[ch][1])
                                + (ce[ch][3] - ce[ch][0]) * (ce[ch][3] - ce[ch][0])
                            );
                double w = (ce[ch][5] - ce[ch][2]) * (px - ce[ch][0])
                        /
                            static_cast<double>(
                                ce[ch][3] - ce[ch][0] + ce[ch][2]
                            );
                mat_data[i + ch] = limit_intensity(
                                        mat_data[i + ch] + w
                                    );
            }
        }
    }

    return 0;
}






int ipl_adjust_intensity_bgra(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_coefficient b, iplp_coefficient g, iplp_coefficient r, iplp_coefficient a,
    int(*method)(uchar*, int, int, int, iplp_coefficient, iplp_coefficient, iplp_coefficient, iplp_coefficient)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, b, g, r, a);
        }
    }

    return 0;
}

int ipl_function_adjust_intensity_bgra(uchar* mat_data, int row, int column, int columns,
    iplp_coefficient b, iplp_coefficient g, iplp_coefficient r, iplp_coefficient a) {
    int index = (row * columns + column) * 4;
    mat_data[index]     = limit_intensity(b.a * mat_data[index] + b.b);
    mat_data[index + 1] = limit_intensity(g.a * mat_data[index + 1] + g.b);
    mat_data[index + 2] = limit_intensity(r.a * mat_data[index + 2] + r.b);
    mat_data[index + 3] = limit_intensity(a.a * mat_data[index + 3] + a.b);

    return 0;
}

int ipl_adjust_intensity_bgr(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_coefficient b, iplp_coefficient g, iplp_coefficient r,
    int(*method)(uchar*, int, int, int, iplp_coefficient, iplp_coefficient, iplp_coefficient)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, b, g, r);
        }
    }

    return 0;
}

int ipl_function_adjust_intensity_bgr(uchar* mat_data, int row, int column, int columns,
    iplp_coefficient b, iplp_coefficient g, iplp_coefficient r) {
    int index = (row * columns + column) * 3;
    mat_data[index]     = limit_intensity(b.a * mat_data[index] + b.b);
    mat_data[index + 1] = limit_intensity(g.a * mat_data[index + 1] + g.b);
    mat_data[index + 2] = limit_intensity(r.a * mat_data[index + 2] + r.b);

    return 0;
}

int ipl_adjust_intensity_wk(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_coefficient wk,
    int(*method)(uchar*, int, int, int, iplp_coefficient)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, wk);
        }
    }

    return 0;
}

int ipl_function_adjust_intensity_wk(uchar* mat_data, int row, int column, int columns,
    iplp_coefficient wk) {
    int index = row * columns + column;
    mat_data[index] = limit_intensity(wk.a * mat_data[index] + wk.b);

    return 0;
}


int ipl_add_linear_intensity_bgra(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_linearity b, iplp_linearity g, iplp_linearity r, iplp_linearity a,
    int(*method)(uchar*, int, int, int, iplp_linearity, iplp_linearity, iplp_linearity, iplp_linearity)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, b, g, r, a);
        }
    }

    return 0;
}

int ipl_function_add_linear_intensity_bgra(uchar* mat_data, int row, int column, int columns,
    iplp_linearity b, iplp_linearity g, iplp_linearity r, iplp_linearity a) {
    double positionB = ((b.bb - b.ab) * (b.bb - b.ab) * b.aa + (b.ba - b.aa) * (b.ba - b.aa) * column - (b.ab - row) * (b.ba - b.aa) * (b.bb - b.ab)) / static_cast<double>((b.bb - b.ab) * (b.bb - b.ab) + (b.ba - b.aa) * (b.ba - b.aa));
    double value_b_add = (b.B - b.A) * (positionB - b.aa) / static_cast<double>(b.ba - b.aa) + b.A;
    double positionG = ((g.bb - g.ab) * (g.bb - g.ab) * g.aa + (g.ba - g.aa) * (g.ba - g.aa) * column - (g.ab - row) * (g.ba - g.aa) * (g.bb - g.ab)) / static_cast<double>((g.bb - g.ab) * (g.bb - g.ab) + (g.ba - g.aa) * (g.ba - g.aa));
    double value_g_add = (g.B - g.A) * (positionG - g.aa) / static_cast<double>(g.ba - g.aa) + g.A;
    double positionR = ((r.bb - r.ab) * (r.bb - r.ab) * r.aa + (r.ba - r.aa) * (r.ba - r.aa) * column - (r.ab - row) * (r.ba - r.aa) * (r.bb - r.ab)) / static_cast<double>((r.bb - r.ab) * (r.bb - r.ab) + (r.ba - r.aa) * (r.ba - r.aa));
    double value_r_add = (r.B - r.A) * (positionR - r.aa) / static_cast<double>(r.ba - r.aa) + r.A;
    double positionA = ((a.bb - a.ab) * (a.bb - a.ab) * a.aa + (a.ba - a.aa) * (a.ba - a.aa) * column - (a.ab - row) * (a.ba - a.aa) * (a.bb - a.ab)) / static_cast<double>((a.bb - a.ab) * (a.bb - a.ab) + (a.ba - a.aa) * (a.ba - a.aa));
    double value_a_add = (a.B - a.A) * (positionR - a.aa) / static_cast<double>(a.ba - a.aa) + a.A;

    int index = (row * columns + column) * 4;
    mat_data[index] = limit_intensity(mat_data[index] + value_b_add);
    mat_data[index + 1] = limit_intensity(mat_data[index + 1] + value_g_add);
    mat_data[index + 2] = limit_intensity(mat_data[index + 2] + value_r_add);
    mat_data[index + 3] = limit_intensity(mat_data[index + 3] + value_r_add);

    return 0;
}

int ipl_add_linear_intensity_bgr(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_linearity b, iplp_linearity g, iplp_linearity r,
    int(*method)(uchar*, int, int, int, iplp_linearity, iplp_linearity, iplp_linearity)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, b, g, r);
        }
    }

    return 0;
}

int ipl_function_add_linear_intensity_bgr(uchar* mat_data, int row, int column, int columns,
    iplp_linearity b, iplp_linearity g, iplp_linearity r) {
    double positionB = ((b.bb - b.ab) * (b.bb - b.ab) * b.aa + (b.ba - b.aa) * (b.ba - b.aa) * column - (b.ab - row) * (b.ba - b.aa) * (b.bb - b.ab)) / static_cast<double>((b.bb - b.ab) * (b.bb - b.ab) + (b.ba - b.aa) * (b.ba - b.aa));
    double value_b_add = (b.B - b.A) * (positionB - b.aa) / static_cast<double>(b.ba - b.aa) + b.A;
    double positionG = ((g.bb - g.ab) * (g.bb - g.ab) * g.aa + (g.ba - g.aa) * (g.ba - g.aa) * column - (g.ab - row) * (g.ba - g.aa) * (g.bb - g.ab)) / static_cast<double>((g.bb - g.ab) * (g.bb - g.ab) + (g.ba - g.aa) * (g.ba - g.aa));
    double value_g_add = (g.B - g.A) * (positionG - g.aa) / static_cast<double>(g.ba - g.aa) + g.A;
    double positionR = ((r.bb - r.ab) * (r.bb - r.ab) * r.aa + (r.ba - r.aa) * (r.ba - r.aa) * column - (r.ab - row) * (r.ba - r.aa) * (r.bb - r.ab)) / static_cast<double>((r.bb - r.ab) * (r.bb - r.ab) + (r.ba - r.aa) * (r.ba - r.aa));
    double value_r_add = (r.B - r.A) * (positionR - r.aa) / static_cast<double>(r.ba - r.aa) + r.A;

    int index = (row * columns + column) * 3;
    mat_data[index]     = limit_intensity(mat_data[index] + value_b_add);
    mat_data[index + 1] = limit_intensity(mat_data[index + 1] + value_g_add);
    mat_data[index + 2] = limit_intensity(mat_data[index + 2] + value_r_add);

    return 0;
}

int ipl_add_linear_intensity_wk(cv::Mat& mat_out, cv::Mat& mat_in,
    iplp_linearity wk,
    int(*method)(uchar*, int, int, int, iplp_linearity)) {
    mat_out = mat_in.clone();

    uchar* mat_data = mat_out.data;
    for (int row = 0; row < mat_out.rows; ++row) {
        for (int column = 0; column < mat_out.cols; ++column) {
            uchar* mat_data = mat_out.data;
            method(mat_data, row, column, mat_out.cols, wk);
        }
    }

    return 0;
}

int ipl_function_add_linear_intensity_wk(uchar* mat_data, int row, int column, int columns,
    iplp_linearity wk) {
    double positionwk = ((wk.bb - wk.ab) * (wk.bb - wk.ab) * wk.aa + (wk.ba - wk.aa) * (wk.ba - wk.aa) * column - (wk.ab - row) * (wk.ba - wk.aa) * (wk.bb - wk.ab)) / static_cast<double>((wk.bb - wk.ab) * (wk.bb - wk.ab) + (wk.ba - wk.aa) * (wk.ba - wk.aa));
    double value_wk_add = (wk.B - wk.A) * (positionwk - wk.aa) / static_cast<double>(wk.ba - wk.aa) + wk.A;

    int index = row * columns + column;
    mat_data[index] = limit_intensity(mat_data[index] + value_wk_add);

    return 0;
}
