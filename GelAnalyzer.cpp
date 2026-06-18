#include "GelAnalyzer.h"

GelAnalyzer::GelAnalyzer() {}

GelAnalyzer::~GelAnalyzer(){}

std::vector<std::vector<double>> GelAnalyzer::plotLanes(
    cv::Mat image,
    std::vector<cv::Rect> roi_list,
    double globalMin,
    double globalMax)
{
    // 转为单通道灰度图
    if (image.channels() > 1) {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }

    if (uncalibratedOD && image.depth() != CV_8U) {
        cv::normalize(image, image, 0, 255, cv::NORM_MINMAX, CV_8U);
    }

    if (invertPeaks) {
        // 只有 8位图 反相才有 255-x 的物理意义
        if (image.depth() == CV_8U) {
            cv::bitwise_not(image, image);
        }
    }

    std::vector<std::vector<double>> allProfiles;

    // 重置全局 OD 极值
    if (uncalibratedOD) {
        odMin = std::numeric_limits<double>::max();
        odMax = -std::numeric_limits<double>::max();
    }

    for (cv::Rect cvRoi : roi_list) {

        bool averageHorizontally = (cvRoi.height > cvRoi.width);

        ProfilePlot plot(image, cvRoi, averageHorizontally);
        std::vector<double> profile = plot.getProfile();

        if (profile.empty()) continue;

        if (uncalibratedOD) {
            profile = calculateOD(profile);
        }

        auto minmax = std::minmax_element(profile.begin(), profile.end());
        if (*minmax.first < globalMin) globalMin = *minmax.first;
        if (*minmax.second > globalMax) globalMax = *minmax.second;

        allProfiles.push_back(profile);
    }

    if (allProfiles.empty()) {
        return allProfiles;
    }

    return allProfiles;
}

std::vector<double> GelAnalyzer::calculateOD(const std::vector<double>& profile) {
    std::vector<double> odProfile(profile.size());

    for (size_t i = 0; i < profile.size(); ++i) {
        double pixelValue = std::min(profile[i], 254.999);

        double v = 0.434294481 * std::log(255.0 / (255.0 - pixelValue));

        if (v < odMin) odMin = v;
        if (v > odMax) odMax = v;

        odProfile[i] = v;
    }

    return odProfile;
}
