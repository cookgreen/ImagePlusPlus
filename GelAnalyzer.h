#ifndef GELANALYZER_H
#define GELANALYZER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "ProfilePlot.h"

class GelAnalyzer
{
public:
    GelAnalyzer();
    ~GelAnalyzer();
    std::vector<std::vector<double>> plotLanes(
        cv::Mat image, std::vector<cv::Rect> roi_list,
        double globalMin,
        double globalMax);
    std::vector<double> calculateOD(const std::vector<double>& profile);

    double odMin;
    double odMax;
    bool uncalibratedOD;
    bool invertPeaks;
};

#endif // GELANALYZER_H
