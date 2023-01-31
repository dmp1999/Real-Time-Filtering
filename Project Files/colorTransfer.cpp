// Project 1 of CS 5330: Pattern Recognition and Computer Vision
// Created by Dhruvil Parikh

#include "filter.hpp"

int colorTransfer(cv::Mat &src, cv::Mat &dst, cv::Mat &result) {

    cv::Mat src_lab, dst_lab, result_lab;

    cvtColor(src, src_lab, cv::COLOR_BGR2Lab);
    cvtColor(dst, dst_lab, cv::COLOR_BGR2Lab);
    cvtColor(result, result_lab, cv::COLOR_BGR2Lab);

    src_lab.convertTo(src_lab, CV_32F);
    dst_lab.convertTo(dst_lab, CV_32F);
    result_lab.convertTo(result_lab, CV_32F);

    std::vector<cv::Mat> src_channels;
    std::vector<cv::Mat> dst_channels;
    std::vector<cv::Mat> result_channels;

    cv::split(src_lab, src_channels);
    cv::split(dst_lab, dst_channels);
    cv::split(result_lab, result_channels);

    float mean1, mean2, mean3, mean4,mean5, mean6;
    float std_dev1, std_dev2, std_dev3, std_dev4, std_dev5, std_dev6;
    cv::Scalar mean_src, mean_dst, std_dev_src, std_dev_dst;

    cv::meanStdDev(src_lab, mean_src, std_dev_src, cv::Mat());
    mean1 = mean_src.val[0];
    mean2 = mean_src.val[1];
    mean3 = mean_src.val[2];
    std_dev1 = std_dev_src.val[0];
    std_dev2 = std_dev_src.val[1];
    std_dev3 = std_dev_src.val[2];

    cv::meanStdDev(dst_lab, mean_dst, std_dev_dst, cv::Mat());
    mean4 = mean_dst.val[0];
    mean5 = mean_dst.val[1];
    mean6 = mean_dst.val[2];
    std_dev4 = std_dev_dst.val[0];
    std_dev5 = std_dev_dst.val[1];
    std_dev6 = std_dev_dst.val[2];

    result_channels[0] = dst_channels[0] - mean4;
    result_channels[1] = dst_channels[1] - mean5;
    result_channels[2] = dst_channels[2] - mean6;

    result_channels[0] = result_channels[0] * (std_dev1/ std_dev4);
    result_channels[1] = result_channels[1] * (std_dev2/ std_dev5);
    result_channels[2] = result_channels[2] * (std_dev3/ std_dev6);

    result_channels[0] = result_channels[0] + mean1;
    result_channels[1] = result_channels[1] + mean2;
    result_channels[2] = result_channels[2] + mean3;

    cv::merge(result_channels, result_lab);

    result_lab.convertTo(result_lab, CV_8UC3);

    cvtColor(result_lab, result, cv::COLOR_Lab2BGR);

    return 0;

}

int main(){

    cv::Mat src;
    cv::Mat dst;

    src = cv::imread("Scenery4.jpg", cv::IMREAD_COLOR);
    dst = cv::imread("Scenery3.jpg", cv::IMREAD_COLOR);
    
    cv::resize(src, src, cv::Size(512, 384));
    cv::resize(dst, dst, cv::Size(512, 384));

    cv::Mat result;
    dst.copyTo(result);

    int result_colorTransfer = 1;

    result_colorTransfer = colorTransfer(src, dst, result);


    cv::namedWindow("Original Image", 1);
    cv::namedWindow("Destination Image", 1);
    cv::namedWindow("Color Transferred Image", 1);

    cv::imshow("Original Image", src);
    cv::imshow("Destination Image", dst);
    cv::imshow("Color Transferred Image", result);

    char key = cv::waitKey(0);
    if(key == 's') {
        cv::imwrite("Color Transferred Image.jpg", result);
        std::cout << "Saving and Exiting..." << std::endl;
    }    

    cv::destroyAllWindows();

    return 0;
}