#pragma once
#include "ogrsf_frmts.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
int main()
{
	char* inFilePath = "C:\\Workspace\\gdal2.2\\其他\\测试项目-Shp\\in.txt";
	char* outFilePath = "C:\\Workspace\\gdal2.2\\其他\\测试项目-Shp\\point_out.shp";
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径
	//注册Driver
	const char* pszDriverName = "ESRI Shapefile";
	GDALDriver* poDriver;
	GDALAllRegister();
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}

	//创建Shapefile Driver
	GDALDataset* poDS;
	poDS = poDriver->Create(outFilePath, 0, 0, 0, GDT_Unknown, NULL);
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	//创建Layer  "point_out"为Layer名
	OGRLayer* poLayer;
	poLayer = poDS->CreateLayer("point_out", NULL, wkbPoint, NULL);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}
	
	//创建属性字段 "Z"为字段名，OFTReal为字段类型double
	OGRFieldDefn oField("Z", OFTReal);
	oField.SetWidth(32);
	if (poLayer->CreateField(&oField) != OGRERR_NONE)
	{
		printf("Creating Name field failed.\n");
		exit(1);
	}

	//读取“x,y,Z”形式的行，并解析它们
	double x, y, z;
	char* filepath = inFilePath;
	ifstream inFileStream(filepath, ios::in);
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