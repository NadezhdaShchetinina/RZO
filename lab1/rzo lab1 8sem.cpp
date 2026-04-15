#include <opencv2/opencv.hpp>
#include <iostream>

    int main()
{
    cv::Mat img = cv::imread("lab1.jpg");

    if (img.empty()) {
        std::cout << "Ошибка загрузки изображения!" << std::endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(), 0.5, 0.5);

    cv::Mat rotated;
    cv::rotate(img, rotated, cv::ROTATE_90_CLOCKWISE);

    cv::Mat blurred;
    cv::GaussianBlur(img, blurred, cv::Size(5, 5), 0);

    cv::Mat edges;
    cv::Canny(img, edges, 50, 150);

    cv::imwrite("gray.jpg", gray);
    cv::imwrite("rotated.jpg", rotated);

    cv::imshow("Оригинал", img);
    cv::imshow("Измененный размер", resized);
    cv::imshow("Повернутое изображение", rotated);
    cv::imshow("Размытое изображение", blurred);
    cv::imshow("Выделенная граница", edges);
    cv::imshow("Градация серого", gray);

    cv::waitKey(0);
    return 0;
}