/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ilya
 *
 * Created on 12 февраля 2019 г., 17:37
 */

#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>
#include <cstring>

using namespace cv;
using namespace std;

void GetFoldersImg(string& folder, vector<String>& filename, int counter, vector<Mat>& images) {
    glob(folder, filename);
    for (int i = 0; i < filename.size(); i++) {
        images.push_back(imread(filename[i]));
    }
    for (auto& item : images) {
        imshow(to_string(counter++), item);
    }
}

void PrintResultCoord(vector<Rect2d>& v_boxes, vector<Mat>& resultImg, Mat& frame, int& counter) {
    for (const auto& item : v_boxes) {
        cout << counter << ") " << item.x << " " << item.y << endl;
        resultImg.push_back(frame(item));
        counter++;
    }
}

void PrintCropImgResult(vector<Mat>& resultImg) {
    int imgCounter = 1;
    for (auto& itemS : resultImg) {
        imshow("Image" + to_string(imgCounter), itemS);
        imgCounter++;
    }
}

int CropImageExecution(vector<Mat>& images, vector<Rect2d>& crop_images, bool& showCrosshair, bool& fromCenter, vector<Mat>& resultImg, int counter) {
    for (int i = 0; i < images.size(); i++) {
        putText(images[i], "Counter: " + to_string(i), Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 4);
        putText(images[i], "Left: " + to_string(images.size() - 1), Point(50, 150), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 4);
        selectROI("MultiTracker" + to_string(i), images[i], crop_images, showCrosshair);
        if (crop_images.size() < 1) {
            return 0;
        } else {
            cout << "It's Okay" << endl;
        }
        PrintResultCoord(crop_images, resultImg, images[i], counter);
        cout << "The number of cropped images: " << resultImg.size() << endl;
        //PrintCropImgResult(resultImg);
        imwrite("./Robot2/Cropped/robot" + to_string(i) + ".jpg", resultImg[i]);
        destroyWindow("MultiTracker" + to_string(i));
    }
}

int main(int argc, char **arv) {
    
    vector<Rect2d> crop_images;
    vector<Mat> resultImg;
    vector<Mat> images;
    vector<String> filename;
    
    bool showCrosshair = false;
    bool fromCenter = false;
    
    int counter = 1;
    string folder = "./Robot2/*.jpg";
    cout << "Strat getting folder's images" << endl;
    GetFoldersImg(folder, filename, counter, images);
    cout << "End getting folder's images" << endl;
    CropImageExecution(images, crop_images, showCrosshair, fromCenter, resultImg, counter);
    waitKey(0);
    return 0;
}