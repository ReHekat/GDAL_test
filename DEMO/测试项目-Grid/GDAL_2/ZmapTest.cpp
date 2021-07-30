#pragma once
#include <iostream>
#include "gdal_priv.h"

using namespace std;

//************************************
// GRIDZMapTest:
// ��GRD�ļ��������ݡ��������ZMap�ļ�
//
//************************************
void GRIDZMapTest(char* inFile, char* outFile) 
{
	GDALDataset* ds = (GDALDataset*)GDALOpen(inFile, GA_ReadOnly);
	GDALDataset* OutDs;
	GDALDriver* pDriver = GetGDALDriverManager()->GetDriverByName("ZMAP");

	char** papszOptions = NULL;
	CSLSetNameValue(papszOptions, "APPEND_SUBDATASET", "YES");
	//����ѹ�����洢��ʽ�ȣ�ÿ�ָ�ʽ��ͬ������ֱ������ΪNULL

	OutDs = pDriver->CreateCopy(outFile, ds, FALSE, papszOptions, NULL, NULL);
	/*
	CreateCopy����:
	pszFilename �C �����ݼ������ơ�UTF-8 ���롣
	poSrcDS �C �����Ƶ����ݼ���
	bStrict �C ������������ϸ��Ч����ΪTRUE�����߸��������� FALSE����ʾ�������Ը��������ʽ����Ҫ���е�����
	papszOptions �C ��������ļ������ĸ��Ӹ�ʽ���ѡ�����ָ�� APPEND_SUBDATASET=YES ѡ���Ա���Ԥ���ƻ��������ݼ���
	pfnProgress �C ���ڱ��渱�����ȵĺ�����
	pProgressData �C ���ݸ����Ⱥ�����Ӧ�ó������ݡ�
	*/

	// �ǵ��ͷ��ڴ�
	GDALClose(ds);
	GDALClose(OutDs);
	free(papszOptions);
}
