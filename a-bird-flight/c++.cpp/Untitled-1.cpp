#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    const int N = 2000;
    const int W = 800, H = 600;
    // white background
    Mat img(H, W, CV_8UC3, Scalar(255,255,255));

    for (int i = 1; i <= N; i++) {
        double t = 2.0 * CV_PI * i / N;
        double x1 = 3.0 * std::pow(std::sin(t), 3.0);
        double y1 = -std::cos(8.0 * t);
        double x2 = 1.5 * std::pow(std::sin(t), 3.0);
        double y2 = -0.5 * std::cos(6.0 * t);

        // map to image pixels
        int px1 = int((x1 + 3.0) / 6.0 * W);
        int py1 = int((1 - (y1 + 1) / 2) * H);
        int px2 = int((x2 + 3) / 6 * W);
        int py2 = int((1 - (y2 + 1) / 2) * H);

        line(img, Point(px1, py1), Point(px2, py2), Scalar(0,0,0), 1, LINE_AA);
    }

    imshow("Bird in Flight", img);
    waitKey(0);
    return 0;
}
