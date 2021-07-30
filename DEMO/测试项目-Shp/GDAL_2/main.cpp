#pragma once
#include "ogrsf_frmts.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
int main()
{
	char* inFilePath = "C:\\Workspace\\gdal2.2\\����\\������Ŀ-Shp\\in.txt";
	char* outFilePath = "C:\\Workspace\\gdal2.2\\����\\������Ŀ-Shp\\point_out.shp";
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //֧������·��
	//ע��Driver
	const char* pszDriverName = "ESRI Shapefile";
	GDALDriver* poDriver;
	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}

	//����Shapefile Driver
	GDALDataset* poDS;
	poDS = poDriver->Create(outFilePath, 0, 0, 0, GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	//����Layer  "point_out"ΪLayer��
	OGRLayer* poLayer;
	poLayer = poDS->CreateLayer("point_out", NULL, wkbPoint, NULL);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}
	
	//���������ֶ� "Z"Ϊ�ֶ�����OFTRealΪ�ֶ�����double
	OGRFieldDefn oField("Z", OFTReal);
	oField.SetWidth(32);
	if (poLayer->CreateField(&oField) != OGRERR_NONE)
	{
		printf("Creating Name field failed.\n");
		exit(1);
	}

	//��ȡ��x,y,Z����ʽ���У�����������
	double x, y, z;
	char* filepath = inFilePath;
	ifstream inFileStream(filepath, ios::in);
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

		getline(sin, field, ',');
		y = atof(field.c_str());

		getline(sin, field, ',');
		z = atof(field.c_str());

		++index;

		OGRFeature* poFeature;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		poFeature->SetField("Z", z);
		OGRPoint pt;

		pt.setX(x);
		pt.setY(y);

		poFeature->SetGeometry(&pt);
		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			exit(1);
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDS);
}