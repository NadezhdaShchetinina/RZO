#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {

    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    cv::CascadeClassifier smileCascade;

    if (!faceCascade.load("haarcascade_frontalface_default.xml")) {
        std::cerr << "ERROR: Failed to load haarcascade_frontalface_default.xml" << std::endl;
        return -1;
    }
    if (!eyeCascade.load("haarcascade_eye.xml")) {
        std::cerr << "ERROR: Failed to load haarcascade_eye.xml" << std::endl;
        return -1;
    }
    if (!smileCascade.load("haarcascade_smile.xml")) {
        std::cerr << "ERROR: Failed to load haarcascade_smile.xml" << std::endl;
        return -1;
    }

    std::cout << "Все каскады успешно загружены" << std::endl;

    cv::VideoCapture cap("ZUA.mp4");
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Failed to open video file ZUA.mp4" << std::endl;
        std::cerr << "Проверьте, что файл находится в той же папке, что и программа" << std::endl;
        return -1;
    }

    std::cout << "Видео ZUA.mp4 успешно открыто" << std::endl;

    cv::Mat frame, gray;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cout << "Видео закончилось" << std::endl;
            break;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(60, 60));

        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);

            cv::Mat faceROI = gray(face);

            std::vector<cv::Rect> eyes;
            eyeCascade.detectMultiScale(faceROI, eyes, 1.3, 6, 0, cv::Size(20, 20));

            for (const auto& eye : eyes) {
                float eyeYcenter = eye.y + eye.height / 2.0f;
                float relativeY = eyeYcenter / face.height;

                if (relativeY < 0.45 && eye.width > 20 && eye.height > 20) {
                    cv::Point center(face.x + eye.x + eye.width / 2,
                        face.y + eye.y + eye.height / 2);
                    int radius = std::max(eye.width, eye.height) / 2;
                    cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 2);
                }
            }

            std::vector<cv::Rect> smiles;
            smileCascade.detectMultiScale(faceROI, smiles, 1.1, 15, 0, cv::Size(25, 25));

            for (const auto& smile : smiles) {
                float smileYcenter = smile.y + smile.height / 2.0f;
                float relativeY = smileYcenter / face.height;

                if (relativeY > 0.55 && smile.width > 30 && smile.height > 15) {
                    int smileX = face.x + smile.x;
                    int smileY = face.y + smile.y;
                    int smileW = smile.width;
                    int smileH = smile.height;

                    // Changed from ellipse to rectangle
                    cv::rectangle(frame,
                        cv::Point(smileX, smileY),
                        cv::Point(smileX + smileW, smileY + smileH),
                        cv::Scalar(0, 255, 255), 2);
                }
            }
        }

        cv::imshow("Detect: Face (square), Eyes (circle), Smile (rectangle)", frame);

        // Exit on ESC
        char key = cv::waitKey(30);
        if (key == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();

    std::cout << "Программа завершена" << std::endl;
    return 0;
}