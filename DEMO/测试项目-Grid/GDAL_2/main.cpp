#pragma once
#include <iostream>
#include "GridTest.h"
#include "ZmapTest.h"

using namespace std;

void GRIDZMapTest(char*, char*);
int main(int argc, char** argv) {
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  //֧������·��
	GDALAllRegister();		//ע��GDAL

	char* XYZ_filePath = "C:\\Workspace\\gdal2.2\\����\\������Ŀ-Grid\\in.xyz";
	//��ȷ�ϳ����Ŀ¼��д��Ȩ��
	char* GRD_outfilePath = "C:\\Workspace\\gdal2.2\\����\\������Ŀ-Grid\\GrdOut.grd";
	GUInt32 nPoints = 9315; //�����������xyz�ļ�����������
	GDALGridTest(XYZ_filePath, GRD_outfilePath, nPoints);

	char* GRD_filePath = GRD_outfilePath;
	//��ȷ�ϳ����Ŀ¼��д��Ȩ��
	char* ZMap_outfilePath = "C:\\Workspace\\gdal2.2\\����\\������Ŀ-Grid\\ZMapOut.zmap";
	GRIDZMapTest(GRD_filePath, ZMap_outfilePath);
	GDALDestroyDriverManager();		//Destroy GRID Driver
	return 0;
}