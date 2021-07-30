#include <iostream>
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "CSVreader.h"

using namespace std;

//************************************
// GDALGridTest:
// 从不规则离散数据点计算网格、并输出到GRD文件
//
//************************************
void GDALGridTest(char* inFile, char* outFile, GUInt32 nPoints)
{
	//const char* inFile = "C:\\Users\\xxz\\Desktop\\test\\xyz.xyz";
	//const char* outFile = "C:\\Users\\xxz\\Desktop\\test\\lout.grd";
	//GUInt32  nPoints = 9315;		//网格点数，即xyz文件的数据行数
	
	//计算最大最小值
	double  dfXMin;
	double  dfXMax;
	double  dfYMin;
	double  dfYMax;
	double  dfZMin;
	double  dfZMax;

	double* padfX = nullptr;
	double* padfY = nullptr;
	double* padfZ = nullptr;

	//double** p = CSVreader(inFile, nPoints);

	////将文本文件读入数组并统计出最大最小值
	double x, y, z;
	double* X = (double*)malloc(sizeof(double) * nPoints);
	double* Y = (double*)malloc(sizeof(double) * nPoints);
	double* Z = (double*)malloc(sizeof(double) * nPoints);
	double** p = (double**)malloc(sizeof(double*) * 3);
	// 读文件  
	ifstream inFileStream(inFile, ios::in);
	string line;
	if (inFileStream.fail())
		cout << "读取文件失败" << endl;
	int index = 0;
	while (getline(inFileStream, line))
	{
		string field;
		istringstream sin(line); //将整行字符串line读入到字符串流sin中
		getline(sin, field, ',');
		x = atof(field.c_str());
		X[index] = x;

		getline(sin, field, ',');
		y = atof(field.c_str());
		Y[index] = y;

		getline(sin, field, ',');
		z = atof(field.c_str());
		Z[index] = z;   // 负值转换为正值


		if (index == 0)
		{
			dfXMin = x;
			dfXMax = x;
			dfYMin = y;
			dfYMax = y;
			dfZMin = z;
			dfZMax = z;
		}

		dfXMin = (x < dfXMin) ? x : dfXMin;
		dfXMax = (x > dfXMax) ? x : dfXMax;
		dfYMin = (y < dfYMin) ? y : dfYMin;
		dfYMax = (y > dfYMax) ? y : dfYMax;
		dfZMin = (z < dfZMin) ? z : dfZMin;
		dfZMax = (z > dfZMax) ? z : dfZMax;

		++index;
	}
	p[0] = X; p[1] = Y; p[2] = Z;

	padfX = p[0]; padfY = p[1]; padfZ = p[2];
	//计算图像大小
	double pixResoultion = 0.5;	//设置分辨率为0.5
	GUInt32 nXSize = (dfXMax - dfXMin) / pixResoultion;
	GUInt32 nYSize = (dfYMax - dfYMin) / pixResoultion;
	
	// 离散点内插方法，使用反距离权重插值法
	GDALGridInverseDistanceToAPowerOptions* poOptions = new GDALGridInverseDistanceToAPowerOptions();
	poOptions->dfPower = 2;
	poOptions->dfRadius1 = 2;
	poOptions->dfRadius2 = 1.5;

	short int* pData = new short int[nXSize * nYSize];  //Z值的数据类型：GDT_Int16与short int对应

	// 离散点内插方法，使用反距离权重插值法。使用其他的插值算法，这里换成其他的，还有下面的GDALGridCreate函数的对应参数
	GDALGridCreate(GGA_InverseDistanceToAPower, poOptions, nPoints, padfX, padfY, padfZ,
		dfXMin, dfXMax, dfYMin, dfYMax, nXSize, nYSize, GDT_Int16, pData, NULL, NULL);
	

	// 创建输出数据集，格式为GS7BG – Golden Software Surfer 7格式
	GDALDriver* pDriver = nullptr;
	pDriver = GetGDALDriverManager()->GetDriverByName("GS7BG");

	char** papszOptions = NULL;
	papszOptions = CSLSetNameValue(papszOptions, "ZMIN", "-890");	//Z最小值
	papszOptions = CSLSetNameValue(papszOptions, "ZMAX", "-815");	//Z最大值
	GDALDataset* OutDs = pDriver->Create(
		outFile,    //@param pszFilename, 输出文件名
		nXSize,			//@param nXSize, 文件宽度
		nYSize,			//@param nYSize, 文件高度
		1,				//@param nBands, 波段数
		GDT_Int16,	//@param eType,  数据类型
		papszOptions	//@param papszOptions, 参数设置，具体可以设置的属性参考每个driver的文档
	);

	// 设置仿射变换六参数
	double adfGeoTransform[6] = { dfXMin, pixResoultion, 0 , dfYMax, 0, -pixResoultion };
	/*
	六个参数分别是：
		geos[0]  top left x 左上角x坐标
		geos[1]  w-e pixel resolution 东西方向像素分辨率
		geos[2]  rotation, 0 if image is "north up" 旋转角度，正北向上时为0
		geos[3]  top left y 左上角y坐标
		geos[4]  rotation, 0 if image is "north up" 旋转角度，正北向上时为0
		geos[5]  n-s pixel resolution 南北向像素分辨率
	*/
	OutDs->SetGeoTransform(adfGeoTransform);


	// 写入影像
	OutDs->RasterIO(
		GF_Write,		//@param eRWFlag		GF_Read或GF_Write
		0, 0,			//@param nXOff,nYOff    起始点x,y坐标
		nXSize, nYSize, //@param nXSize,nYSize  所需读取(写入)块宽度/高度
		pData,			//@param * pData        所读(写)数据,指针
		nXSize, nYSize, //@param nBufXSize      一般跟nXSize，nYSize一致，用于缩放图像，
						//@param nBufYSize      图像将按nBufXSize/nXsize在x/y尺度缩放（会自动重采样）
		GDT_Int16,	//@param eBufType       与pData的实际类型一致
		1,				//@param nBandCount		正在读取或写入的波段数。
		0,				//@param panBandMap		正在读 / 写的 nBandCount 波段编号列表
		0, 0, 0
		);

	// 释放资源 关闭图像	
	delete poOptions;
	delete[]pData;
	GDALClose(OutDs);
	OutDs = NULL;
	free(X); free(Y); free(Z); free(p);
}

