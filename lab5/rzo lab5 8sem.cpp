#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cmath>

std::string detectShape(const std::vector<cv::Point>& contour) {
    double perimeter = cv::arcLength(contour, true);
    if (perimeter < 10) return "Unknown";

    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, 0.05 * perimeter, true);

    int vertices = approx.size();
    double area = cv::contourArea(contour);
    double circularity = 4 * CV_PI * area / (perimeter * perimeter);
    cv::Rect rect = cv::boundingRect(contour);
    double aspectRatio = (double)rect.width / rect.height;

    if (circularity > 0.9) return "Circle";
    else if (vertices == 3) return "Triangle";
    else if (vertices == 4) return "Square";
    else if (vertices >= 5 && vertices <= 8 && circularity > 0.65) return "Circle";

    return "Unknown";
}

int main() {
    cv::VideoCapture cap("Motion abstract geometric shapes.mkv");

    if (!cap.isOpened()) {
        std::cerr << "Failed open video" << std::endl;
        return -1;
    }

    cv::Mat frame, gray, blurred, edges, closed;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::medianBlur(gray, blurred, 5);


        cv::Canny(blurred, edges, 30, 90);


        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(edges, closed, cv::MORPH_CLOSE, kernel);


        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(closed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Mat result = frame.clone();

        for (const auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area < 500 || area > 50000) continue;

            std::string shapeName = detectShape(contour);
            if (shapeName == "Unknown") continue;

            cv::Rect bbox = cv::boundingRect(contour);

            cv::Scalar color;
            if (shapeName == "Circle") color = cv::Scalar(255, 0, 0);
            else if (shapeName == "Square") color = cv::Scalar(0, 255, 0);
            else if (shapeName == "Triangle") color = cv::Scalar(0, 0, 255);

            cv::rectangle(result, bbox, color, 2);
            cv::Point center(bbox.x + bbox.width / 2, bbox.y + bbox.height / 2);
            cv::putText(result, shapeName,
                cv::Point(center.x - 35, center.y - 15),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, color, 2, cv::LINE_AA);
        }

        cv::imshow("Shape Detection", result);
        cv::imshow("Closed Edges", closed);

        char key = cv::waitKey(30);
        if (key == 27) break;
        if (key == ' ') cv::waitKey(0);
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}