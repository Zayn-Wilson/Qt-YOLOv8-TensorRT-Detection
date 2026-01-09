#pragma once

#include <QThread>
#include <QMutex>
#include <QStringList>
#include <QDateTime>

//Halcon
#include "HalconCpp.h"
#pragma comment(lib, "halconcpp.lib")
using namespace HalconCpp;

#include"../utils/yolo.h"
#include"yolov8.h"

using namespace std;

#pragma execution_character_set("utf-8")

struct bbox_t {
	unsigned int x, y, w, h;       // (x,y) - top-left corner, (w, h) - width & height of bounded box
	float prob;                    // confidence - probability that the object was found correctly
	unsigned int obj_id;           // class of object - from range [0, classes-1]
	unsigned int track_id;         // tracking id for video (0 - untracked, 1 - inf - tracked object)
	unsigned int frames_counter;   // counter of frames on which the object was detected
	float x_3d, y_3d, z_3d;        // center of object (in Meters) if ZED 3D Camera is used
};

struct image_t {
	int h;                        // height
	int w;                        // width
	int c;                        // number of chanels (3 - for RGB)
	float *data;                  // pointer to the image data
};

class ThreadYOLOV8test1  : public QThread
{
	Q_OBJECT

public:
	ThreadYOLOV8test1(QObject *parent);
	~ThreadYOLOV8test1();

private:
	QMutex mutex;
	volatile bool stopped;

private:
	// parameters
	utils::InitParameter param;

	std::string model_path;
	std::string video_path;
	std::string image_path;
	// camera' id
	int camera_id;
	// get input
	utils::InputStream source;

	// update params from command line parser
	int size; // w or h
	int batch_size;
	bool is_show;
	bool is_save;
	int total_batches;
	int delay_time;
	cv::VideoCapture capture;

	cv::Mat frame;
	cv::Mat frame1;
	cv::Mat frame2;
	std::vector<cv::Mat> imgs_batch;
	YOLOV8* yolo;

	bool initOK;
	void initYoloV8();
	void setParameters(utils::InitParameter& initParameters);
	void task(YOLOV8& yolo, const utils::InitParameter& param, std::vector<cv::Mat>& imgsBatch, const int& delayTime, const int& batchi,
		const bool& isShow, const bool& isSave);
	void taskmain1();
	void taskmain2();
	// Prediction interface
	std::vector<bbox_t> task2(std::vector<cv::Mat>& imgsBatch);
	cv::Mat HImageToMat(HObject &imgHalcon);
	HObject m_hShowImg;
	int idelayTime;
signals:
	void receiveResult(const QString &result);

public slots:
	void setDelayTime(int);
	void slots_start();
	void stop();


protected:
	void run();
};
