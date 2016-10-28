#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class CImgProcess
{
public:
	CImgProcess(void);
	~CImgProcess(void);
	int OtsuThreshold(const Mat & img);
	int OtsuThreshold2(IplImage* pImg);
	void BinImg(Mat img);
	void DrawContours(Mat img);
	void DrawHistGram(Mat img);

};

