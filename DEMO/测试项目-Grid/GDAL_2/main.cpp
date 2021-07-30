#pragma once
#include <iostream>
#include "GridTest.h"
#include "ZmapTest.h"

using namespace std;

void GRIDZMapTest(char*, char*);
int main(int argc, char** argv) {
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //支持中文路径
	GDALAllRegister();		//注册GDAL

	char* XYZ_filePath = "C:\\Workspace\\gdal2.2\\其他\\测试项目-Grid\\in.xyz";
	//请确认程序对目录有写入权限
	char* GRD_outfilePath = "C:\\Workspace\\gdal2.2\\其他\\测试项目-Grid\\GrdOut.grd";
	GUInt32 nPoints = 9315; //网格点数，即xyz文件的数据行数
	GDALGridTest(XYZ_filePath, GRD_outfilePath, nPoints);

	char* GRD_filePath = GRD_outfilePath;
	//请确认程序对目录有写入权限
	char* ZMap_outfilePath = "C:\\Workspace\\gdal2.2\\其他\\测试项目-Grid\\ZMapOut.zmap";
	GRIDZMapTest(GRD_filePath, ZMap_outfilePath);
	GDALDestroyDriverManager();		//Destroy GRID Driver
	return 0;
}