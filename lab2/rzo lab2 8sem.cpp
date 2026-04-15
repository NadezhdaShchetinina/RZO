#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>  
#include <ctime>    

using namespace cv;
using namespace std;

int main() {
    srand(time(nullptr));

    // Белый фон 
    Mat baseCanvas(600, 800, CV_8UC3, Scalar(255, 255, 255));

    line(baseCanvas, Point(0, 0), Point(799, 599), Scalar(255, 0, 0), 3);

    // Прямоугольник в левом верхнем углу
    rectangle(baseCanvas, Point(50, 50), Point(200, 150), Scalar(0, 255, 0), 2);

    // Круг в правом верхнем углу
    circle(baseCanvas, Point(700, 100), 70, Scalar(0, 0, 255), -1);

    // Эллипс в левом нижнем углу
    ellipse(baseCanvas, Point(150, 500), Size(120, 60), 30, 0, 360, Scalar(0, 255, 255), 2);

    // Многоугольник в правом нижнем углу
    vector<Point> pts = { Point(550, 450), Point(650, 500),
                         Point(700, 550), Point(600, 550),
                         Point(500, 500) };
    polylines(baseCanvas, pts, true, Scalar(255, 255, 0), 2);

    string text = "Shchetinina N.S. Group 22-PM-1";
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    Scalar textColor = Scalar(0, 0, 0); 
    int thickness = 2;

    int baseline = 0;
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);

    Point textPos(100, 100);

    int dx = 4;
    int dy = 3;

    cout << "Анимация запущена. Нажмите ESC для выхода..." << endl;
    cout << "Размер текста: ширина = " << textSize.width
        << ", высота = " << textSize.height << endl;
    cout << "Baseline = " << baseline << endl;

    while (true) {

        Mat frame = baseCanvas.clone();

        putText(frame, text, textPos, fontFace, fontScale, textColor, thickness);

        imshow("My Drawing Animation", frame);

        textPos.x += dx;
        textPos.y += dy;

        if (textPos.x <= 0) {
            textPos.x = 0;
            dx = abs(dx);
        }

        if (textPos.x + textSize.width >= frame.cols) {
            textPos.x = frame.cols - textSize.width;
            dx = -abs(dx);
        }

        if (textPos.y - textSize.height <= 0) {
            textPos.y = textSize.height;
            dy = abs(dy);
        }

        if (textPos.y >= frame.rows - baseline) {
            textPos.y = frame.rows - baseline;
            dy = -abs(dy);
        }

        int key = waitKey(30);

        if (key == 27) {
            break;
        }
    }

    Mat finalImage = baseCanvas.clone();
    putText(finalImage, text, textPos, fontFace, fontScale, textColor, thickness);
    imwrite("SavedFigure.jpg", finalImage);

    cout << "Анимация завершена. Изображение сохранено." << endl;

    return 0;
}