#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <iostream>
#include<cmath>

#define B 0.114     // COMPOSITION OF COLOR BLUE IN GRAYSCALE
#define G 0.587     // COMPOSITION OF COLOR GREEN IN GRAYSCALE
#define R 0.299     // COMPOSITION OF COLOR RED IN GRAYSCALE

// GRAYSCALE
int grayscale(const cv::Mat &src, cv::Mat &dst) {

    int i = 0, j = 0;

    for (i = 0; i < src.rows; i++) {
        for (j = 0; j < src.cols; j++) {
            dst.at<uchar>(i, j) = B*src.at<cv::Vec3b>(i, j)[0] + G*src.at<cv::Vec3b>(i, j)[1] + R*src.at<cv::Vec3b>(i, j)[2];
        }
    }
    
    return 0;
}

// GAUSSIAN BLUR 5x5
cv::Vec3b applyHorizontalBlur(const cv::Mat &src, int i, int j)
{
    cv::Vec3i sum = {0, 0, 0};
    cv::Vec3b final_sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 5, filter_cols = 5;

    for (c = 0; c < 3; c++)
    {

        sum[c] = 1 * src.at<cv::Vec3b>(i - 2, j)[c] + 2 * src.at<cv::Vec3b>(i - 1, j)[c] + 4 * src.at<cv::Vec3b>(i, j)[c] +
                 2 * src.at<cv::Vec3b>(i + 1, j)[c] + 1 * src.at<cv::Vec3b>(i + 2, j)[c];
        sum[c] /= 10;
        final_sum[c] = (unsigned char)sum[c];
    }

    return (final_sum);
}

cv::Vec3b applyVerticalBlur(const cv::Mat &src, int i, int j)
{
    cv::Vec3i sum = {0, 0, 0};
    cv::Vec3b final_sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 5, filter_cols = 5;

    for (c = 0; c < 3; c++)
    {
        sum[c] = 1 * src.at<cv::Vec3b>(i, j - 2)[c] + 2 * src.at<cv::Vec3b>(i, j - 1)[c] + 4 * src.at<cv::Vec3b>(i, j)[c] +
                 2 * src.at<cv::Vec3b>(i, j + 1)[c] + 1 * src.at<cv::Vec3b>(i, j + 2)[c];
        sum[c] /= 10;
        final_sum[c] = (unsigned char)sum[c];
    }

    return (final_sum);
}

int blur5x5(const cv::Mat &src, cv::Mat &dst)
{

    int i = 0, j = 0, c = 0;
    cv::Vec3b sum = {0, 0, 0};
    cv::Mat temp;
    src.copyTo(temp);

    for (i = 2; i < src.rows - 2; i++)
    {
        for (j = 2; j < src.cols - 2; j++)
        {
            temp.at<cv::Vec3b>(i, j) = applyHorizontalBlur(src, i, j);
        }
    }

    for (i = 2; i < src.rows - 2; i++)
    {
        for (j = 2; j < src.cols - 2; j++)
        {
            dst.at<cv::Vec3b>(i, j) = applyVerticalBlur(temp, i, j);
        }
    }

    return 0;
}

// SOBELX3x3
cv::Vec3s applyHorizontalSobelX(const cv::Mat &src, int i, int j)
{
    cv::Vec3s sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 3, filter_cols = 3;

    for (c = 0; c < 3; c++)
    {

        sum[c] = (-1) * src.at<cv::Vec3b>(i, j - 1)[c] + 0 * src.at<cv::Vec3b>(i, j)[c] + 1 * src.at<cv::Vec3b>(i, j + 1)[c];
    }

    return (sum);
}

cv::Vec3s applyVerticalSobelX(const cv::Mat &src, int i, int j)
{
    cv::Vec3s sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 3, filter_cols = 3;

    for (c = 0; c < 3; c++)
    {
        sum[c] = 1 * src.at<cv::Vec3s>(i - 1, j)[c] + 2 * src.at<cv::Vec3s>(i, j)[c] + 1 * src.at<cv::Vec3s>(i + 1, j)[c];
        sum[c] /= 4;
    }

    return (sum);
}

int sobelX3x3(const cv::Mat &src, cv::Mat &dst)
{

    int i = 0, j = 0;

    cv::Mat temp(src.rows, src.cols, CV_16SC3);

    for (i = 1; i < src.rows - 1; i++)
    {
        for (j = 1; j < src.cols - 1; j++)
        {
            temp.at<cv::Vec3s>(i, j) = applyHorizontalSobelX(src, i, j);
        }
    }

    for (i = 1; i < src.rows - 1; i++)
    {
        for (j = 1; j < src.cols - 1; j++)
        {
            dst.at<cv::Vec3s>(i, j) = applyVerticalSobelX(temp, i, j);
        }
    }

    cv::convertScaleAbs(dst, dst);

    return 0;
}

// SOBELY3x3
cv::Vec3s applyHorizontalSobelY(const cv::Mat &src, int i, int j)
{
    cv::Vec3s sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 3, filter_cols = 3;

    for (c = 0; c < 3; c++)
    {

        sum[c] = 1 * src.at<cv::Vec3b>(i, j - 1)[c] + 2 * src.at<cv::Vec3b>(i, j)[c] + 1 * src.at<cv::Vec3b>(i, j + 1)[c];
        sum[c] /= 4;
    }

    return (sum);
}

cv::Vec3s applyVerticalSobelY(const cv::Mat &src, int i, int j)
{
    cv::Vec3s sum = {0, 0, 0};
    int c = 0;
    int filter_rows = 3, filter_cols = 3;

    for (c = 0; c < 3; c++)
    {
        sum[c] = 1 * src.at<cv::Vec3s>(i - 1, j)[c] + 0 * src.at<cv::Vec3s>(i, j)[c] + (-1) * src.at<cv::Vec3s>(i + 1, j)[c];
    }

    return (sum);
}

int sobelY3x3(const cv::Mat &src, cv::Mat &dst)
{

    int i = 0, j = 0;

    cv::Mat temp(src.rows, src.cols, CV_16SC3);

    for (i = 1; i < src.rows - 1; i++)
    {
        for (j = 1; j < src.cols - 1; j++)
        {
            temp.at<cv::Vec3s>(i, j) = applyHorizontalSobelY(src, i, j);
        }
    }

    for (i = 1; i < src.rows - 1; i++)
    {
        for (j = 1; j < src.cols - 1; j++)
        {
            dst.at<cv::Vec3s>(i, j) = applyVerticalSobelY(temp, i, j);
        }
    }

    cv::convertScaleAbs(dst, dst);

    return 0;
}

// MAGNITUDE GRADIENT IMAGE
cv::Vec3b computeMagnitude (cv::Mat &sx, cv::Mat &sy, int i, int j) {

    cv::Vec3b mag = {0, 0, 0};
    int c = 0;
    int sx_squared = 0, sy_squared = 0;

    for (c = 0; c < 3; c++ ) {
        
        mag[c] = sqrt(sx.at<cv::Vec3b>(i, j)[c] * sx.at<cv::Vec3b>(i, j)[c] + sy.at<cv::Vec3b>(i, j)[c] * sy.at<cv::Vec3b>(i, j)[c]);

    }

    return(mag);

}

int magnitude (cv::Mat &sx, cv::Mat &sy, cv::Mat &dst) {

    int i = 0, j = 0;

    for (i = 0; i < dst.rows ; i++) {
        for (j = 0; j < dst.cols; j++) {

            dst.at<cv::Vec3b>(i, j) = computeMagnitude(sx, sy, i, j);

        }
    }

    return 0;

}

// BLUR QUANTIZE
cv::Vec3b quantize(const cv::Mat &src, int b, int i, int j) {

    cv::Vec3b quantized_value = {0, 0, 0};
    int c = 0;
    int x = 0, xt = 0, xf = 0;

    for (c = 0; c < 3; c++) {
        x = src.at<cv::Vec3b>(i, j)[c];
        xt = x/b;
        xf = xt*b;

        quantized_value[c] = xf;
    }

    return(quantized_value);

}

int blurQuantize(const cv::Mat &src, cv::Mat &dst, int levels = 15) {

    int i = 0, j = 0;
    int temp = 0, b = 0;

    temp = blur5x5(src, dst);
    b = 255/levels;

    for (i = 0; i < src.rows; i++) {
        for (j = 0; j < src.cols; j++) {

            dst.at<cv::Vec3b>(i, j) = quantize(src, b, i, j);

        }
    }

    return 0;

}

// CARTOON
void borderCartoon(cv::Mat &dst, cv::Mat &grad_mag, int magThreshold, int i, int j) {

    int c = 0;

    for (c = 0; c < 3; c++) {

        if (grad_mag.at<cv::Vec3b>(i, j)[c] > magThreshold) {
            dst.at<cv::Vec3b>(i, j)[c] = 0;
        }

    }

}

int cartoon(const cv::Mat &src, cv::Mat &dst, int levels = 15, int magThreshold = 15){

    cv::Mat grad_mag;
    src.copyTo(grad_mag);
    cv::Mat sx(src.rows, src.cols, CV_16SC3); 
    cv::Mat sy(src.rows, src.cols, CV_16SC3);
    int temp_sobelX, temp_sobelY, temp_magnitude, temp_blurQuantized;
    int i = 0, j = 0;

    temp_sobelX = sobelX3x3(src, sx);
    temp_sobelY = sobelY3x3(src, sy);
    temp_magnitude = magnitude(sx, sy, grad_mag);
    temp_blurQuantized = blurQuantize(src, dst, levels);

    for (i = 0; i < src.rows; i++) {
        for (j = 0; j < src.cols; j++) {

            borderCartoon(dst, grad_mag, magThreshold, i, j);

        }
    }

    return 0;

}

// NEGATIVE
int negative(cv::Mat &src, cv::Mat &dst) {
    
    int i = 0, j = 0, c = 0;

    for (i = 0; i < src.rows; i++) {
        for (j = 0; j < src.cols; j++) {
            for (c = 0; c < 3; c++) {
                dst.at<cv::Vec3b>(i,j)[c] = 255 - src.at<cv::Vec3b>(i, j)[c];
            }
        }
    }
    
    return 0;
}

// SKETCH FROM IMAGE
int sketch(cv::Mat &src, cv::Mat &dst) {

    cv::Mat src_gray;
    cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    cv::Mat src_gray_inv = 255 - src_gray;

    cv::GaussianBlur(src_gray_inv, src_gray_inv, cv::Size(21, 21), 0);

    cv::divide(src_gray, 255-src_gray_inv, dst, 256.0);

    return 0;

}

// SHARP IMAGE
int sharpImage(cv::Mat &src, cv::Mat &dst) {

    cv::Mat kernel;
    kernel = (cv::Mat_<double>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(src, dst, -1, kernel);

    return 0;

}

// KELVIN FILTER
void interpolation(uchar* lut, float* curve, float* originalValue){
    
    for(int i = 0; i < 256; i++){
    
        int j = 0;
        float a = i;
        
        while (a > originalValue[j]){
            j++;
        }

        if(a == originalValue[j]){
            lut[i] = curve[j];
            continue;
        }

        float slope = ((float)(curve[j] - curve[j-1]))/((float)(originalValue[j] - originalValue[j-1]));
        float constant = curve[j] - slope * originalValue[j];
        
        lut[i] = slope * a + constant;
    
    }
}

int kelvinFilter(cv::Mat &src, cv::Mat &dst) {

    std::vector<cv::Mat> channels;
    cv::split(dst, channels);

    float red_values_original[] = {0, 60, 110, 150, 235, 255};
    float red_values[] =         {0, 102, 185, 220, 245, 245};
    float green_values_original[] = {0, 68, 105, 190, 255};
    float green_values[] =         {0, 68, 120, 220, 255};
    float blue_values_original[] = {0, 88, 145, 185, 255};
    float blue_values[] =         {0, 12, 140, 212, 255};

    cv::Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();

    interpolation(lut, blue_values, blue_values_original);
    cv::LUT(channels[0], lookupTable, channels[0]);

    interpolation(lut, green_values, green_values_original);
    cv::LUT(channels[1], lookupTable, channels[1]);

    interpolation(lut, red_values, red_values_original);
    cv::LUT(channels[2], lookupTable, channels[2]);

    merge(channels, dst);

    return 0;

}