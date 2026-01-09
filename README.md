# Qt-YOLOv8-TensorRT-Detection

基于 Qt 的 YOLOv8 目标检测应用，使用 TensorRT 进行 GPU 加速推理。

## 功能特性

- 🚀 **TensorRT 加速**: 使用 NVIDIA TensorRT 进行高性能 GPU 推理
- 🖼️ **Qt GUI**: 友好的图形用户界面，支持多线程检测
- 📷 **多输入源**: 支持图片、视频和摄像头输入
- 🎯 **YOLOv8 检测**: 实现 YOLOv8 目标检测算法
- ⚡ **CUDA 优化**: 自定义 CUDA 核函数进行图像预处理
- 📊 **批量处理**: 支持批量图像推理

## 技术栈

- **GUI 框架**: Qt 5.x
- **深度学习推理**: NVIDIA TensorRT
- **GPU 计算**: CUDA
- **图像处理**: OpenCV 4.5.5
- **视觉库**: Halcon 12 (可选)
- **开发环境**: Visual Studio 2019/2022

## 系统要求

- Windows 10/11
- NVIDIA GPU (支持 CUDA)
- CUDA Toolkit 11.x+
- TensorRT 8.x+
- Qt 5.x
- OpenCV 4.5.5
- Visual Studio 2019/2022

## 项目结构

```
QtWidgetsApplication1/
├── main.cpp                    # 程序入口
├── QtWidgetsApplication1.h/cpp # 主窗口类
├── src/
│   ├── apiTensorRTYOLOV8/     # YOLOv8 TensorRT API
│   ├── frmmain/               # 主界面框架
│   └── ThreadTest1/           # 多线程测试
├── utils/
│   ├── common_include.h       # 公共头文件
│   ├── kernel_function.cu/h   # CUDA 核函数
│   ├── utils.cpp/h            # 工具函数
│   └── yolo.cpp/h             # YOLO 基类
└── platforms/                  # Qt 平台插件
```

## 核心功能

### 1. TensorRT 推理引擎
- 加载 TensorRT 序列化模型 (.engine/.trt)
- 支持动态批量大小
- 自动计算仿射变换矩阵

### 2. CUDA 图像预处理
- GPU 加速的图像缩放 (双线性插值)
- BGR 到 RGB 颜色空间转换
- 图像归一化
- HWC 到 CHW 格式转换

### 3. 后处理
- GPU 加速的解码
- 快速 NMS (非极大值抑制)
- 坐标变换回原图

## 支持的数据集

- COCO 80 类
- COCO 91 类
- VOC 20 类

## 使用方法

1. 准备 YOLOv8 TensorRT 模型文件
2. 配置模型参数 (输入尺寸、类别数等)
3. 运行程序，选择输入源
4. 点击"开始"按钮进行检测

## 编译说明

1. 使用 Visual Studio 打开 `QtWidgetsApplication1.sln`
2. 配置项目属性:
   - 添加 CUDA、TensorRT、OpenCV、Qt 的包含目录和库目录
   - 配置 `.props` 文件中的路径
3. 选择 Release x64 配置
4. 编译运行

## 配置文件

- `OpenCV4.5.5.props` - OpenCV 配置
- `TensorRTYoloV8.props` - TensorRT 配置
- `Halcon12.props` - Halcon 配置 (可选)

## 许可证

MIT License

## 致谢

- [Ultralytics YOLOv8](https://github.com/ultralytics/ultralytics)
- [NVIDIA TensorRT](https://developer.nvidia.com/tensorrt)
- [OpenCV](https://opencv.org/)
- [Qt](https://www.qt.io/)
