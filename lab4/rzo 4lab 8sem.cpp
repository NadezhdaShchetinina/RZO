#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main()
{
    cv::Mat img = cv::imread("D:/open cv/rzo 4lab 8sem/rzo 4lab 8sem/Figure.png");
    if (img.empty()) {
        std::cout << "Error: Could not load image!" << std::endl;
        return -1;
    }

    cv::Mat gray, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);
    cv::Canny(gray, edges, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area < 500) continue;

        double perimeter = cv::arcLength(contour, true);

        cv::Moments m = cv::moments(contour);
        cv::Point2f center(m.m10 / m.m00, m.m01 / m.m00);

        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * perimeter, true);

        std::string shapeName;
        int vertices = approx.size();

        if (vertices == 3) {
            shapeName = "Triangle";
        }
        else if (vertices == 4) {
            cv::Rect rect = cv::boundingRect(contour);
            double aspect = (double)rect.width / rect.height;

            cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
            double angle = rotatedRect.angle;
            double areaRotated = rotatedRect.size.width * rotatedRect.size.height;
            double fillRatio = area / areaRotated;

            if (fillRatio > 0.7 && fillRatio < 0.9 && std::abs(angle) > 10 && std::abs(angle) < 80) {
                shapeName = "Rhombus";
            }
            else if (aspect > 0.95 && aspect < 1.05) {
                shapeName = "Square";
            }
            else {
                shapeName = "Rectangle";
            }
        }
        else if (vertices == 5) {
            shapeName = "Pentagon";
        }
        else if (vertices == 6) {
            shapeName = "Hexagon";
        }
        else if (vertices > 6) {
            cv::Point2f circleCenter;
            float radius;
            cv::minEnclosingCircle(contour, circleCenter, radius);
            double areaCircle = CV_PI * radius * radius;
            double circularity = area / areaCircle;

            if (circularity > 0.85) {
                shapeName = "Circle";
            }
            else {
                shapeName = "Ellipse";
            }
        }

        std::string text = shapeName + " P=" + std::to_string((int)perimeter);
        cv::putText(img, text, cv::Point(center.x - 60, center.y - 20),
            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

        // ОБВОДКА СИНИМ ЦВЕТОМ 
        cv::drawContours(img, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255, 0, 0), 2);
        cv::circle(img, center, 3, cv::Scalar(255, 0, 0), -1);
    }

    cv::imshow("Result", img);
    cv::imwrite("contour_result.png", img);
    cv::waitKey(0);

    return 0;
}