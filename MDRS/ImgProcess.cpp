#include "StdAfx.h"
#include "ImgProcess.h"


CImgProcess::CImgProcess(void)
{
}


CImgProcess::~CImgProcess(void)
{
}

//�������Ӧ��ֵ����
int CImgProcess::OtsuThreshold(const Mat & img)  
{  
	int T = 0;			//��ֵ  
	int height = img.rows;  
	int width  = img.cols;  
	int step   = img.step;  
	int channels  = img.channels();  
	uchar* data  = /*(uchar*)*/img.data;  
	double gSum0;		//��һ��Ҷ���ֵ  
	double gSum1;		//�ڶ���Ҷ���ֵ  
	double N0 = 0;		//ǰ��������  
	double N1 = 0;		//����������  
	double u0 = 0;		//ǰ������ƽ���Ҷ�  
	double u1 = 0;		//��������ƽ���Ҷ�  
	double w0 = 0;		//ǰ�����ص���ռ����ͼ��ı���Ϊ��0  
	double w1 = 0;		//�������ص���ռ����ͼ��ı���Ϊ��1  
	double u = 0;		//��ƽ���Ҷ�  
	double tempg = -1;	//��ʱ��䷽��  
	double g = -1;		//��䷽��  
	double Histogram[256]={0};	// = new double[256];//�Ҷ�ֱ��ͼ  
	double N = width*height;	//��������  
	for(int i=0; i<height; i++)  
	{//����ֱ��ͼ  
		for(int j=0; j<width; j++)  
		{  
			double temp = data[i*step + j*3]*0.114 + data[i*step + j*3 + 1]*0.587 + data[i*step + j*3 + 2]*0.299;  
			temp = temp<0 ? 0:temp;  
			temp = temp>255 ? 255:temp;  
			Histogram[(int)temp]++;  
		}   
	}  
	//������ֵ  
	for (int i=0; i<256; i++)  
	{  
		gSum0 = 0;  
		gSum1 = 0;  
		N0 += Histogram[i];           
		N1 = N-N0;  
		if(0==N1)break;		//������ǰ�������ص�ʱ������ѭ��  
		w0 = N0/N;  
		w1 = 1-w0;  
		for (int j = 0;j<=i;j++)  
		{  
			gSum0 += j*Histogram[j];  
		}  
		u0 = gSum0/N0;  
		for(int k = i+1;k<256;k++)  
		{  
			gSum1 += k*Histogram[k];  
		}  
		u1 = gSum1/N1;  
		u = w0*u0 + w1*u1;  
		g = w0*w1*(u0-u1)*(u0-u1);  
		if (tempg<g)  
		{  
			tempg = g;  
			T = i;  
		}  
	}  
	return T;   
}  
//�������Ӧ��ֵ��
int CImgProcess::OtsuThreshold2(IplImage* pImg)
{
	int height=pImg->height;    
	int width=pImg->width;        
	long size = height * width;   
	//histogram    
	float histogram[256] = {0};    
	for(int m=0; m < height; m++)  
	{    
		unsigned char* p=(unsigned char*)pImg->imageData + pImg->widthStep * m;    
		for(int n = 0; n < width; n++)   
		{    
			histogram[int(*p++)]++;    
		}    
	}    

	int threshold;      
	long sum0 = 0, sum1 = 0; //�洢ǰ���ĻҶ��ܺͺͱ����Ҷ��ܺ�  
	long cnt0 = 0, cnt1 = 0; //ǰ�����ܸ����ͱ������ܸ���  
	double w0 = 0, w1 = 0; //ǰ���ͱ�����ռ����ͼ��ı���  
	double u0 = 0, u1 = 0;  //ǰ���ͱ�����ƽ���Ҷ�  
	double variance = 0; //�����䷽��  
	int i, j;  
	double u = 0;  
	double maxVariance = 0;  
	for(i = 1; i < 256; i++) //һ�α���ÿ������  
	{    
		sum0 = 0;  
		sum1 = 0;   
		cnt0 = 0;  
		cnt1 = 0;  
		w0 = 0;  
		w1 = 0;  
		for(j = 0; j < i; j++)  
		{  
			cnt0 += (long)histogram[j];  
			sum0 += long(j * histogram[j]);  
		}  

		u0 = (double)sum0 /  cnt0;   
		w0 = (double)cnt0 / size;  

		for(j = i ; j <= 255; j++)  
		{  
			cnt1 += (long)histogram[j];  
			sum1 += long(j * histogram[j]);  
		}  

		u1 = (double)sum1 / cnt1;  
		w1 = 1 - w0; // (double)cnt1 / size;  

		u = u0 * w0 + u1 * w1; //ͼ���ƽ���Ҷ�  
		//		printf("u = %f\n", u);  
		//variance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);  
		variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);  
		if(variance > maxVariance)   
		{    
			maxVariance = variance;    
			threshold = i;    
		}   
	}    
	//	printf("threshold = %d\n", threshold);  
	return threshold;    
}

void CImgProcess::BinImg(Mat img)
{

}

void CImgProcess::DrawContours(Mat img)
{

}

void CImgProcess::DrawHistGram(Mat img)
{

}