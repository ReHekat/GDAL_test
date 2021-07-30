#include <iostream>  
#include <string>  
#include <fstream>  
#include <sstream>  

using namespace std;


double** CSVreader(string filepath,int lineNum)
{
	double x, y, z;
	double* X = (double*)malloc(sizeof(double) * lineNum);
	double* Y = (double*)malloc(sizeof(double) * lineNum);
	double* Z = (double*)malloc(sizeof(double) * lineNum);
	double** p = (double**)malloc(sizeof(double*) * 3);
	// 读文件  
	ifstream inFile(filepath, ios::in);
	string line;
	if (inFile.fail())
		cout << "读取文件失败" << endl;
	int index = 0;
	while (getline(inFile, line))
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
		Z[index] = z;

		++index;
	}
	p[0] = X; p[1] = Y; p[2] = Z;

	return p;
}

//void main()
//{
//	double** p = nullptr;
//	p = CSVreader(9315);
//	cout << p[2][3] << endl;
//}