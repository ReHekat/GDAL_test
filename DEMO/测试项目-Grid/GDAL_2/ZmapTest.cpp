#pragma once
#include <iostream>
#include "gdal_priv.h"

using namespace std;

//************************************
// GRIDZMapTest:
// 从GRD文件读入数据、并输出到ZMap文件
//
//************************************
void GRIDZMapTest(char* inFile, char* outFile) 
{
	GDALDataset* ds = (GDALDataset*)GDALOpen(inFile, GA_ReadOnly);
	GDALDataset* OutDs;
	GDALDriver* pDriver = GetGDALDriverManager()->GetDriverByName("ZMAP");

	char** papszOptions = NULL;
	CSLSetNameValue(papszOptions, "APPEND_SUBDATASET", "YES");
	//设置压缩、存储方式等，每种格式不同，可以直接设置为NULL

	OutDs = pDriver->CreateCopy(outFile, ds, FALSE, papszOptions, NULL, NULL);
	/*
	CreateCopy参数:
	pszFilename – 新数据集的名称。UTF-8 编码。
	poSrcDS – 被复制的数据集。
	bStrict – 如果副本必须严格等效，则为TRUE，或者更常见的是 FALSE，表示副本可以根据输出格式的需要进行调整。
	papszOptions – 控制输出文件创建的附加格式相关选项。可以指定 APPEND_SUBDATASET=YES 选项以避免预先破坏现有数据集。
	pfnProgress – 用于报告副本进度的函数。
	pProgressData – 传递给进度函数的应用程序数据。
	*/

	// 记得释放内存
	GDALClose(ds);
	GDALClose(OutDs);
	free(papszOptions);
}
