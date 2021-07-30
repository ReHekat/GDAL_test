#include <iostream>
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "CSVreader.h"

using namespace std;

//************************************
// GDALGridTest:
// �Ӳ�������ɢ���ݵ�������񡢲������GRD�ļ�
//
//************************************
void GDALGridTest(char* inFile, char* outFile, GUInt32 nPoints)
{
	//const char* inFile = "C:\\Users\\xxz\\Desktop\\test\\xyz.xyz";
	//const char* outFile = "C:\\Users\\xxz\\Desktop\\test\\lout.grd";
	//GUInt32  nPoints = 9315;		//�����������xyz�ļ�����������
	
	//���������Сֵ
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

	////���ı��ļ��������鲢ͳ�Ƴ������Сֵ
	double x, y, z;
	double* X = (double*)malloc(sizeof(double) * nPoints);
	double* Y = (double*)malloc(sizeof(double) * nPoints);
	double* Z = (double*)malloc(sizeof(double) * nPoints);
	double** p = (double**)malloc(sizeof(double*) * 3);
	// ���ļ�  
	ifstream inFileStream(inFile, ios::in);
	string line;
	if (inFileStream.fail())
		cout << "��ȡ�ļ�ʧ��" << endl;
	int index = 0;
	while (getline(inFileStream, line))
	{
		string field;
		istringstream sin(line); //�������ַ���line���뵽�ַ�����sin��
		getline(sin, field, ',');
		x = atof(field.c_str());
		X[index] = x;

		getline(sin, field, ',');
		y = atof(field.c_str());
		Y[index] = y;

		getline(sin, field, ',');
		z = atof(field.c_str());
		Z[index] = z;   // ��ֵת��Ϊ��ֵ


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
	//����ͼ���С
	double pixResoultion = 0.5;	//���÷ֱ���Ϊ0.5
	GUInt32 nXSize = (dfXMax - dfXMin) / pixResoultion;
	GUInt32 nYSize = (dfYMax - dfYMin) / pixResoultion;
	
	// ��ɢ���ڲ巽����ʹ�÷�����Ȩ�ز�ֵ��
	GDALGridInverseDistanceToAPowerOptions* poOptions = new GDALGridInverseDistanceToAPowerOptions();
	poOptions->dfPower = 2;
	poOptions->dfRadius1 = 2;
	poOptions->dfRadius2 = 1.5;

	short int* pData = new short int[nXSize * nYSize];  //Zֵ���������ͣ�GDT_Int16��short int��Ӧ

	// ��ɢ���ڲ巽����ʹ�÷�����Ȩ�ز�ֵ����ʹ�������Ĳ�ֵ�㷨�����ﻻ�������ģ����������GDALGridCreate�����Ķ�Ӧ����
	GDALGridCreate(GGA_InverseDistanceToAPower, poOptions, nPoints, padfX, padfY, padfZ,
		dfXMin, dfXMax, dfYMin, dfYMax, nXSize, nYSize, GDT_Int16, pData, NULL, NULL);
	

	// ����������ݼ�����ʽΪGS7BG �C Golden Software Surfer 7��ʽ
	GDALDriver* pDriver = nullptr;
	pDriver = GetGDALDriverManager()->GetDriverByName("GS7BG");

	char** papszOptions = NULL;
	papszOptions = CSLSetNameValue(papszOptions, "ZMIN", "-890");	//Z��Сֵ
	papszOptions = CSLSetNameValue(papszOptions, "ZMAX", "-815");	//Z���ֵ
	GDALDataset* OutDs = pDriver->Create(
		outFile,    //@param pszFilename, ����ļ���
		nXSize,			//@param nXSize, �ļ����
		nYSize,			//@param nYSize, �ļ��߶�
		1,				//@param nBands, ������
		GDT_Int16,	//@param eType,  ��������
		papszOptions	//@param papszOptions, �������ã�����������õ����Բο�ÿ��driver���ĵ�
	);

	// ���÷���任������
	double adfGeoTransform[6] = { dfXMin, pixResoultion, 0 , dfYMax, 0, -pixResoultion };
	/*
	���������ֱ��ǣ�
		geos[0]  top left x ���Ͻ�x����
		geos[1]  w-e pixel resolution �����������طֱ���
		geos[2]  rotation, 0 if image is "north up" ��ת�Ƕȣ���������ʱΪ0
		geos[3]  top left y ���Ͻ�y����
		geos[4]  rotation, 0 if image is "north up" ��ת�Ƕȣ���������ʱΪ0
		geos[5]  n-s pixel resolution �ϱ������طֱ���
	*/
	OutDs->SetGeoTransform(adfGeoTransform);


	// д��Ӱ��
	OutDs->RasterIO(
		GF_Write,		//@param eRWFlag		GF_Read��GF_Write
		0, 0,			//@param nXOff,nYOff    ��ʼ��x,y����
		nXSize, nYSize, //@param nXSize,nYSize  �����ȡ(д��)����/�߶�
		pData,			//@param * pData        ����(д)����,ָ��
		nXSize, nYSize, //@param nBufXSize      һ���nXSize��nYSizeһ�£���������ͼ��
						//@param nBufYSize      ͼ�񽫰�nBufXSize/nXsize��x/y�߶����ţ����Զ��ز�����
		GDT_Int16,	//@param eBufType       ��pData��ʵ������һ��
		1,				//@param nBandCount		���ڶ�ȡ��д��Ĳ�������
		0,				//@param panBandMap		���ڶ� / д�� nBandCount ���α���б�
		0, 0, 0
		);

	// �ͷ���Դ �ر�ͼ��	
	delete poOptions;
	delete[]pData;
	GDALClose(OutDs);
	OutDs = NULL;
	free(X); free(Y); free(Z); free(p);
}

