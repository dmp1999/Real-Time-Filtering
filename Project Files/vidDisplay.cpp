// Project 1 of CS 5330: Pattern Recognition and Computer Vision
// Created by Dhruvil Parikh

#include "filter.hpp"

int main(){

    cv::VideoCapture *cap;
    cap = new cv::VideoCapture(0);    

    if( !cap->isOpened() ) {
        printf("Unable to open video device\n");
        return(-1);
    }

    cv::Size refS( (int) cap->get(cv::CAP_PROP_FRAME_WIDTH ), (int) cap->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::Mat frame;
    cv::Mat dst_gray;
    cv::Mat dst_gray_frame;
    cv::Mat intermediate_blur_frame;
    cv::Mat dst_blur_frame;
    cv::Mat dst_magnitude_frame;
    cv::Mat dst_blur_quantized_frame;
    cv::Mat dst_cartoonized_frame;
    cv::Mat dst_negative;
    cv::Mat dst_sketch;
    cv::Mat dst_sharp;
    cv::Mat dst_kelvin_filtered;

    cv::namedWindow("Video Stream", 1);

    int cnt_save = 0;
    int cnt_blur = 0;
    bool flag_save = 0;
    bool flag_exit = 0;
    bool flag_gray = 0;
    bool flag_manual_gray = 0;
    bool flag_blur = 0;
    bool flag_sobel_x = 0;
    bool flag_sobel_y = 0;
    bool flag_magnitude = 0;
    bool flag_blur_quantized = 0;
    bool flag_cartoonized = 0;
    bool flag_negative = 0;
    bool flag_sketch = 0;
    bool flag_sharp = 0;
    bool flag_kelvin_filtered = 0;
    bool result_gray = 1;
    bool result_blur = 1;
    bool result_sobel_x = 1;
    bool result_sobel_y = 1;
    bool result_magnitude = 1;
    bool result_blur_quantized = 1;
    bool result_cartoonized = 1;
    bool result_negative = 0;
    bool result_sketch = 0;
    bool result_sharp = 0;
    bool result_kelvin_filtered = 0;

    std::cout << "Usage:" << std::endl;
    std::cout << "Pressing a key will open the corresponding window. Pressing the same key again will close that window." << std::endl;
    std::cout << "1) Press \"q\" to quit the program." << std::endl;
    std::cout << "2) Press \"s\" to save the image." << std::endl;
    std::cout << "3) Press \"g\" to display the grayscale image." << std::endl;
    std::cout << "4) Press \"h\" to display the custom grayscale image." << std::endl;
    std::cout << "5) Press \"b\" to display the blurred image." << std::endl;
    std::cout << "6) Press \"x\" to display the sobelX image." << std::endl;
    std::cout << "7) Press \"y\" to display the sobelY image." << std::endl;
    std::cout << "8) Press \"m\" to display the magnitude gradient image." << std::endl;
    std::cout << "9) Press \"i\" to display the blurred and quantized image." << std::endl;
    std::cout << "10) Press \"c\" to display the cartoonized image." << std::endl;
    std::cout << "11) Press \"n\" to display the negative image." << std::endl;
    std::cout << "12) Press \"a\" to display the image as a sketch." << std::endl;
    std::cout << "13) Press \"w\" to display the sharpened image." << std::endl;
    std::cout << "13) Press \"k\" to display the kelvin filtered image." << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    cv::waitKey(0);

    while (true) {

        int cnt = flag_manual_gray + flag_blur + flag_sobel_x + flag_sobel_y + flag_magnitude + flag_blur_quantized + flag_cartoonized
                  + flag_negative + flag_sketch + flag_sharp + flag_kelvin_filtered;

        if(cnt > 2) {
            std::cout << "Warning!!!" << std::endl;
            std::cout << "You have more than 3 Windows opened. Performance might be affected..." << std::endl;
        } else {
            std::cout << "You have " + std::to_string(cnt + 1) + " windows running at present." << std::endl;
        }

        *cap >> frame;
        if( frame.empty() ) {
            printf("Frame is empty.\n");
            break;
        }

        cv::imshow("Video Stream", frame);
        if(flag_save) {
            cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", frame);
            cnt_save++;
        }

        if(flag_blur) {
            frame.copyTo(dst_blur_frame);
            result_blur = blur5x5(frame, dst_blur_frame);
            cv::imshow("Video Stream: Gaussian Blur 5x5", dst_blur_frame);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_blur_frame);
                cnt_save++;
            }
        }

        if(flag_sobel_x || flag_sobel_y || flag_magnitude){
            
            cv::Mat dst_Sobel_X(frame.rows, frame.cols, CV_16SC3);
            cv::Mat dst_Sobel_Y(frame.rows, frame.cols, CV_16SC3);
            
            if (flag_sobel_x || flag_magnitude) {
                result_sobel_x = sobelX3x3(frame, dst_Sobel_X);
                if (flag_sobel_x) {
                    cv::imshow("Video Stream: SobelX 3x3", dst_Sobel_X);
                    if(flag_save) {
                        cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_Sobel_X);
                        cnt_save++;
                    }
                }
            }

            if (flag_sobel_y || flag_magnitude) {
                result_sobel_y = sobelY3x3(frame, dst_Sobel_Y);
                if (flag_sobel_y) {
                    cv::imshow("Video Stream: SobelY 3x3", dst_Sobel_Y);
                    if(flag_save) {
                        cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_Sobel_Y);
                        cnt_save++;
                    }
                }
            }

            if (flag_magnitude) {
                frame.copyTo(dst_magnitude_frame);
                result_magnitude = magnitude(dst_Sobel_X, dst_Sobel_Y, dst_magnitude_frame);
                cv::imshow("Video Stream: Gradient Magnitude Image", dst_magnitude_frame);
                if(flag_save) {
                    cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_magnitude_frame);
                    cnt_save++;
                }
            }

        }

        if (flag_blur_quantized) {
            frame.copyTo(dst_blur_quantized_frame);
            int levels = 7;
            result_blur_quantized = blurQuantize(frame, dst_blur_quantized_frame, levels);
            cv::imshow("Video Stream: Blurred and Quantized Image", dst_blur_quantized_frame);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_blur_quantized_frame);
                cnt_save++;
            }
        }

        if (flag_cartoonized) {
            frame.copyTo(dst_cartoonized_frame);
            int levels = 10;
            int magThreshold = 15;
            result_cartoonized = cartoon(frame, dst_cartoonized_frame, levels, magThreshold);
            cv::imshow("Video Stream: Cartoonized Image", dst_cartoonized_frame);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_cartoonized_frame);
                cnt_save++;
            }
        }

        if (flag_negative) {
            frame.copyTo(dst_negative);
            result_negative = negative(frame, dst_negative);
            cv::imshow("Video Stream: Negative Image", dst_negative);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_negative);
                cnt_save++;
            }
        }

        if (flag_sketch) {
            frame.copyTo(dst_sketch);
            result_sketch = sketch(frame, dst_sketch);
            cv::imshow("Video Stream: Sketch from Image", dst_sketch);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_sketch);
                cnt_save++;
            }
        }

        if (flag_sharp) {
            frame.copyTo(dst_sharp);
            result_sharp = sharpImage(frame, dst_sharp);
            cv::imshow("Video Stream: Sharpened Image", dst_sharp);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_sharp);
                cnt_save++;
            }
        }

        if (flag_kelvin_filtered) {
            frame.copyTo(dst_kelvin_filtered);
            result_kelvin_filtered = kelvinFilter(frame, dst_kelvin_filtered);
            cv::imshow("Video Stream: Kelvin Filtered Image", dst_kelvin_filtered);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_kelvin_filtered);
                cnt_save++;
            }
        }

        if (flag_manual_gray) {
            cv::Mat dst_gray_frame(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0));
            result_gray = grayscale(frame, dst_gray_frame);
            cv::imshow("Video Stream: Custom Grayscale", dst_gray_frame);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_gray_frame);
                cnt_save++;
            }
        }

        if (flag_gray) {
            cv::cvtColor(frame, dst_gray, cv::COLOR_BGR2GRAY);
            cv::imshow("Video Stream: In-built Grayscale", dst_gray);
            if(flag_save) {
                cv::imwrite("Image_" + std::to_string(cnt_save) + ".jpg", dst_gray);
                cnt_save++;
            }
        }

        switch(cv::waitKey(10)) {

            case 'q':
                std::cout << "Exiting the Program..." << std::endl;
                flag_exit = 1;
                break;

            case 's':
                flag_save = 1;
                std::cout << "Saving the current frames displayed..." << std::endl;
                break;

            case 'g':
                flag_gray = !flag_gray;
                if (flag_gray){
                    cv::namedWindow("Video Stream: In-built Grayscale", 1);
                } else {
                    cv::destroyWindow("Video Stream: In-built Grayscale");
                }
                break;

            case 'h':
                flag_manual_gray = !flag_manual_gray;
                if (flag_manual_gray){
                    cv::namedWindow("Video Stream: Custom Grayscale", 1);
                } else {
                    cv::destroyWindow("Video Stream: Custom Grayscale");
                }
                break;

            case 'b':
                flag_blur = !flag_blur;
                if (flag_blur){
                    cv::namedWindow("Video Stream: Gaussian Blur 5x5", 1);
                } else {
                    cv::destroyWindow("Video Stream: Gaussian Blur 5x5");
                }
                break;

            case 'x':
                flag_sobel_x = !flag_sobel_x;
                if (flag_sobel_x){
                    cv::namedWindow("Video Stream: SobelX 3x3", 1);
                } else {
                    cv::destroyWindow("Video Stream: SobelX 3x3");
                } 
                break;

            case 'y':
                flag_sobel_y = !flag_sobel_y;
                if (flag_sobel_y){
                    cv::namedWindow("Video Stream: SobelY 3x3", 1);
                } else {
                    cv::destroyWindow("Video Stream: SobelY 3x3");
                } 
                break;

            case 'm':
                flag_magnitude = !flag_magnitude;
                if (flag_magnitude) {
                    cv::namedWindow("Video Stream: Gradient Magnitude Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Gradient Magnitude Image");
                }
                break;

            case 'i':
                flag_blur_quantized = !flag_blur_quantized;
                if (flag_blur_quantized) {
                    cv::namedWindow("Video Stream: Blurred and Quantized Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Blurred and Quantized Image");
                }
                break;

            case 'c':
                flag_cartoonized = !flag_cartoonized;
                if (flag_cartoonized) {
                    cv::namedWindow("Video Stream: Cartoonized Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Cartoonized Image");
                }
                break;

            case 'n':
                flag_negative = !flag_negative;
                if (flag_negative) {
                    cv::namedWindow("Video Stream: Negative Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Negative Image");
                }
                break;

            case 'a':
                flag_sketch = !flag_sketch;
                if (flag_sketch) {
                    cv::namedWindow("Video Stream: Sketch from Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Sketch from Image");
                }
                break;

            case 'w':
                flag_sharp = !flag_sharp;
                if (flag_sharp) {
                    cv::namedWindow("Video Stream: Sharpened Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Sharpened Image");
                }
                break;

            case 'k':
                flag_kelvin_filtered = !flag_kelvin_filtered;
                if (flag_kelvin_filtered) {
                    cv::namedWindow("Video Stream: Kelvin Filtered Image", 1);
                } else {
                    cv::destroyWindow("Video Stream: Kelvin Filtered Image");
                }
                break;

            default:
                flag_save = 0;

        }

        if (flag_exit) {
            break;
        }

    }

    delete cap;
    return 0;

}