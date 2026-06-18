#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QList>
#include <opencv2/core.hpp>

#include "GelAnalyzer.h"

class LaneGraphicsView;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void analyze();

private:
    LaneGraphicsView *view;
    QGraphicsScene *scene;
    QList<QGraphicsRectItem*> lanes; // 保存泳道矩形
    cv::Mat cvImage;                 // OpenCV 原图
    QImage qtImage;                  // Qt 显示用图
};

#endif