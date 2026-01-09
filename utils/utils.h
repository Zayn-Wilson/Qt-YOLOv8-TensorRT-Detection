#pragma once
#include"common_include.h"

namespace utils 
{
    namespace dataSets
    {
        const std::vector<std::string> coco80 = {
            "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
            "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
            "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
            "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
            "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
            "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
            "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
            "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
            "hair drier", "toothbrush"
        };
        const std::vector<std::string> voc20 = {
            "aeroplane","bicycle","bird","boat","bottle","bus","car","cat","chair","cow","diningtable",
            "dog","horse","motorbike","person","pottedplant","sheep","sofa","train","tvmonitor"
        };
    }

    struct InitParameter
    {
        int num_class{ 80 };
        std::vector<std::string> class_names;
        std::vector<std::string> input_output_names;

        bool dynamic_batch{ true };
        size_t batch_size;
        int src_h, src_w;
        int dst_h, dst_w;

        float scale{ 255.f };
        float means[3] = { 0.f, 0.f, 0.f };
        float stds[3] = { 1.f, 1.f, 1.f };

        float iou_thresh;
        float conf_thresh;

        int topK{ 1000 };
        std::string save_path;
    };

    struct Box
    {
        float left, top, right, bottom, confidence;
        int label;
        std::vector<cv::Point2i> land_marks;

        Box() = default;
        Box(float left, float top, float right, float bottom, float confidence, int label) :
            left(left), top(top), right(right), bottom(bottom), confidence(confidence), label(label) {}
    };

    enum class InputStream { IMAGE, VIDEO, CAMERA };
    enum class ColorMode { RGB, GRAY };

    struct AffineMat
    {
        float v0, v1, v2;
        float v3, v4, v5;
    };

    void saveBinaryFile(float* vec, size_t len, const std::string& file);
    std::vector<uint8_t> readBinaryFile(const std::string& file);
    std::vector<unsigned char> loadModel(const std::string& file);
    std::string getSystemTimeStr();

    bool setInputStream(const InputStream& source, const std::string& imagePath, const std::string& videoPath, const int& cameraID,
        cv::VideoCapture& capture, int& totalBatches, int& delayTime, InitParameter& param);

    void show(const std::vector<std::vector<Box>>& objectss, const std::vector<std::string>& classNames,
        const int& cvDelayTime, std::vector<cv::Mat>& imgsBatch);

    void save(const std::vector<std::vector<Box>>& objectss, const std::vector<std::string>& classNames,
        const std::string& savePath, std::vector<cv::Mat>& imgsBatch, const int& batchSize, const int& batchi);

    class HostTimer
    {
    public:
        HostTimer();
        float getUsedTime();
        ~HostTimer();
    private:
        std::chrono::steady_clock::time_point t1;
        std::chrono::steady_clock::time_point t2;
    };

    class DeviceTimer
    {
    public:
        DeviceTimer();
        float getUsedTime();
        DeviceTimer(cudaStream_t ctream);
        float getUsedTime(cudaStream_t ctream);
        ~DeviceTimer();
    private:
        cudaEvent_t start, end;
    };
}
