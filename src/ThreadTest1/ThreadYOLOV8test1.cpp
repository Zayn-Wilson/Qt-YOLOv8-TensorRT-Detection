#include "ThreadYOLOV8test1.h"

ThreadYOLOV8test1::ThreadYOLOV8test1(QObject *parent)
	: QThread(parent)
{
	idelayTime = 100;
	initOK = false;
	initYoloV8();
}

ThreadYOLOV8test1::~ThreadYOLOV8test1()
{
	this->stop();
	this->wait(1000);
}

void ThreadYOLOV8test1::run()
{
	while (stopped) 
	{
		QString result;

		// Start time
		QDateTime dateTimeStart = QDateTime::currentDateTime();
		QString strDateStart = dateTimeStart.toString("yyyyMMdd-hh:mm:ss zzz");
		// Execute algorithm
		if (initOK == true)
		{
			taskmain2();
		}
		// End time
		QDateTime dateTimeStop = QDateTime::currentDateTime();
		QString strDateStop = dateTimeStop.toString("yyyyMMdd-hh:mm:ss zzz");
		
		// Time interval
		qint64 intervalTimeMS = dateTimeStart.msecsTo(dateTimeStop);
		
		result = "startTime:" + strDateStart + "  StopTime:" + strDateStop + "  Algorithm time:" + QString::number(intervalTimeMS) + "ms";
		if (!result.isEmpty()) {
			emit receiveResult(result);
		}
		msleep(idelayTime);
	}
}

void ThreadYOLOV8test1::setDelayTime(int tempDelayTime)
{
	idelayTime = tempDelayTime;
}

void ThreadYOLOV8test1::slots_start()
{
	stopped = true;
	this->start();
}
void ThreadYOLOV8test1::stop()
{
	stopped = false;
}

void ThreadYOLOV8test1::initYoloV8()
{
	setParameters(param);
	video_path = "";
	image_path = "path/to/your/image.bmp";
	model_path = "path/to/your/model.trt";
	camera_id = 0;

	QString str = QString::fromStdString(image_path);
	ReadImage(&m_hShowImg, QString(str).toLocal8Bit().constData());

	frame = cv::imread(image_path);

	source = utils::InputStream::IMAGE;

	size = -1;
	batch_size = 8;
	is_show = false;
	is_save = false;
	total_batches = 0;
	delay_time = 1;
	param.batch_size = 1;
	total_batches = 1;
	delay_time = 0;
	param.src_h = 1024;
	param.src_w = 1280;

	yolo = new YOLOV8(param);

	// Read model
	std::vector<unsigned char> trt_file = utils::loadModel(model_path);
	if (trt_file.empty())
	{
		return;
	}
	// Init model
	if (!yolo->init(trt_file))
	{
		return;
	}
	yolo->check();
	
	imgs_batch.reserve(param.batch_size);
	initOK = true;
}

void ThreadYOLOV8test1::setParameters(utils::InitParameter& initParameters)
{
	initParameters.class_names = utils::dataSets::coco80;
	initParameters.num_class = 5;

	initParameters.batch_size = 8;
	initParameters.dst_h = 640;
	initParameters.dst_w = 640;
	initParameters.input_output_names = { "images",  "output0" };
	initParameters.conf_thresh = 0.25f;
	initParameters.iou_thresh = 0.45f;
	initParameters.save_path = "";
}

void ThreadYOLOV8test1::task(YOLOV8& yolo, const utils::InitParameter& param, std::vector<cv::Mat>& imgsBatch, const int& delayTime, const int& batchi,
	const bool& isShow, const bool& isSave)
{
	utils::DeviceTimer d_t0; yolo.copy(imgsBatch);        float t0 = d_t0.getUsedTime();
	utils::DeviceTimer d_t1; yolo.preprocess(imgsBatch);  float t1 = d_t1.getUsedTime();
	utils::DeviceTimer d_t2; yolo.infer();                float t2 = d_t2.getUsedTime();
	utils::DeviceTimer d_t3; yolo.postprocess(imgsBatch); float t3 = d_t3.getUsedTime();
	sample::gLogInfo <<
		"preprocess time = " << t1 / param.batch_size << "; "
		"infer time = " << t2 / param.batch_size << "; "
		"postprocess time = " << t3 / param.batch_size << std::endl;

	// Display results
	for (int i = 0; i < yolo.getObjectss().size(); i++)
	{
		for (int y = 0; y < yolo.getObjectss().at(i).size(); y++)
		{
			sample::gLogInfo <<
				"i= " << i << "; "
				"y= " << y << "; "
				"label = " << yolo.getObjectss().at(i).at(y).label << "; "
				"left = " << yolo.getObjectss().at(i).at(y).left << "; "
				"right = " << yolo.getObjectss().at(i).at(y).right << "; "
				"top = " << yolo.getObjectss().at(i).at(y).top << "; "
				"bottom = " << yolo.getObjectss().at(i).at(y).bottom << "; "
				"confidence = " << yolo.getObjectss().at(i).at(y).confidence << "; "
				<< std::endl;
		}
	}

	if (isShow)
		utils::show(yolo.getObjectss(), param.class_names, delayTime, imgsBatch);
	if (isSave)
		utils::save(yolo.getObjectss(), param.class_names, param.save_path, imgsBatch, param.batch_size, batchi);
	yolo.reset();
}

void ThreadYOLOV8test1::taskmain1()
{
	int i = 0;
	int batchi = 0;
	int runtime = 0;

	while (1)
	{
		if (runtime > 0)
		{
			break;
		}
		if (batchi >= total_batches && source != utils::InputStream::CAMERA)
		{
			return;
		}
		if (imgs_batch.size() < param.batch_size)
		{
			if (source != utils::InputStream::IMAGE)
			{
				capture.read(frame);
			}
			else
			{
				frame = cv::imread(image_path);
			}

			if (frame.empty())
			{
				sample::gLogWarning << "no more video or camera frame" << std::endl;
				task(*yolo, param, imgs_batch, delay_time, batchi, is_show, is_save);
				imgs_batch.clear();
				batchi++;
				return;
			}
			else
			{
				for (int j = 0; j < 8; j++)
					imgs_batch.emplace_back(frame.clone());
			}
		}
		else
		{
			task(*yolo, param, imgs_batch, delay_time, batchi, is_show, is_save);
			imgs_batch.clear();
			batchi = 0;
			runtime++;
		}
	}
}

void ThreadYOLOV8test1::taskmain2()
{
	HObject ho_Image2, ho_Region1, AReg1, ho_Imagemain2;
	std::vector<cv::Mat> imgs_batch;
	imgs_batch.reserve(1);
	imgs_batch.emplace_back(frame.clone());
	std::vector<bbox_t> result_vec = task2(imgs_batch);
}

std::vector<bbox_t> ThreadYOLOV8test1::task2(std::vector<cv::Mat>& imgsBatch)
{
	std::vector<bbox_t> return_bbox_t;
	bool isShow = false;
	bool isSave = false;

	yolo->copy(imgsBatch);
	yolo->preprocess(imgsBatch);
	yolo->infer();
	yolo->postprocess(imgsBatch);

	// Get results
	for (int i = 0; i < yolo->getObjectss().size(); i++)
	{
		for (int y = 0; y < yolo->getObjectss().at(i).size(); y++)
		{
			bbox_t temp;
			temp.x = (unsigned int)yolo->getObjectss().at(i).at(y).left;
			temp.y = (unsigned int)yolo->getObjectss().at(i).at(y).top;
			temp.w = (unsigned int)yolo->getObjectss().at(i).at(y).right - temp.x;
			temp.h = (unsigned int)yolo->getObjectss().at(i).at(y).bottom - temp.y;
			temp.obj_id = (unsigned int)yolo->getObjectss().at(i).at(y).label;
			temp.prob = yolo->getObjectss().at(i).at(y).confidence;
			return_bbox_t.push_back(temp);
		}
	}
	yolo->reset();
	return return_bbox_t;
}

cv::Mat ThreadYOLOV8test1::HImageToMat(HObject &imgHalcon)
{
	HTuple channels;
	HString cType;
	cv::Mat Image;
	ConvertImageType(imgHalcon, &imgHalcon, "byte");
	CountChannels(imgHalcon, &channels);
	Hlong width = 0;
	Hlong height = 0;
	if (channels[0].I() == 1)
	{
		HImage hImg(imgHalcon);
		void *ptr = hImg.GetImagePointer1(&cType, &width, &height);
		int W = width;
		int H = height;
		Image.create(H, W, CV_8UC1);
		unsigned char *pdata = static_cast<unsigned char *>(ptr);
		memcpy(Image.data, pdata, W*H);
	}
	else if (channels[0].I() == 3)
	{
		void *Rptr;
		void *Gptr;
		void *Bptr;
		HImage hImg(imgHalcon);
		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &width, &height);
		int W = width;
		int H = height;
		Image.create(H, W, CV_8UC3);
		vector<cv::Mat> VecM(3);
		VecM[0].create(H, W, CV_8UC1);
		VecM[1].create(H, W, CV_8UC1);
		VecM[2].create(H, W, CV_8UC1);
		unsigned char *R = (unsigned char *)Rptr;
		unsigned char *G = (unsigned char *)Gptr;
		unsigned char *B = (unsigned char *)Bptr;
		memcpy(VecM[2].data, R, W*H);
		memcpy(VecM[1].data, G, W*H);
		memcpy(VecM[0].data, B, W*H);
		cv::merge(VecM, Image);
	}
	return Image;
}
