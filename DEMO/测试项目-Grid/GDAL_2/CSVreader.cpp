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
	// ���ļ�  
	ifstream inFile(filepath, ios::in);
	string line;
	if (inFile.fail())
		cout << "��ȡ�ļ�ʧ��" << endl;
	int index = 0;
	while (getline(inFile, line))
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