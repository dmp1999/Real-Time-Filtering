// Project 1 of CS 5330: Pattern Recognition and Computer Vision
// Created by Dhruvil Parikh

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main() {
    cv::Mat img;
    std::string path = "test.jpg";
    char key = cv::waitKey(1000);

    img = cv::imread(path, cv::IMREAD_COLOR);
    cv::resize(img, img, cv::Size(), 0.25, 0.25);
    cv::namedWindow("Test Image");
    cv::imshow("Test Image", img);

    std::cout << "Press 's' to save." << std::endl;
    std::cout << "Press 'q' to quit." << std::endl;

    int flag = 0;
    
    while (true) {

        switch(cv::waitKey(0)) {
            case 'q' :
                flag = 1;
                std::cout << "Destroying the Image and Exiting the Program..." << std::endl;
                cv::destroyWindow("Test Image");
                break;
            case 's' :
                std::cout << "Saving the Image..." << std::endl;
                cv::imwrite("Saved Image.jpg", img);
                break;
        }

        if (flag) {
            break;
        }

    }
    
    return 0;
}