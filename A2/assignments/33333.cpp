// Assignmnet2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;
#define NameLength 25
#define ArratSize 800

struct city {
	string CityName;
	string CountryName;
	float latitude = 0;
	float longitude = 0;
};
vector<string> split(const string& str, const string& pattern) {
	vector<string> result;
	string strs = str;
	if (strs.substr(strs.length()-pattern.length()+1, strs.length()) != pattern)
		strs = strs + pattern;
	if (str == "") {
		return result;
	}
	size_t pos = strs.find(pattern);
	while (pos != strs.npos) {
		string temp = strs.substr(0, pos);
		result.push_back(temp);
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(pattern);
	}
	return result;
}

void setCity(city* city, string str, bool &outOfLength) {
	vector<string> strs = split(str, ",");
		city->CityName = strs.at(0);
		city->CountryName = strs.at(2);
		city->latitude = atof(strs.at(3).c_str());
		city->longitude = atof(strs.at(4).c_str());
		if (city->CityName.length() > NameLength)
			outOfLength = true;
}

int enterCity(city* City,int length) {
	string inputCity;
	getline(cin,inputCity);
	city* theCity = City;

	vector<int> allNumbers;
	vector<string> allNames;
	while (true) {
		if (inputCity == "bye") {
			return -1;
		}
		if (inputCity.length() < 3) {
			cout << "The name need to be longer than 3 letters, please enter again:";
			//cin.ignore();
			getline(cin, inputCity);
			continue;
		}
		for (int i = 0; i < length;i++) {
			if (i != 0) {
				theCity++;
			}
			if (theCity->CityName.find(inputCity)!=inputCity.npos) {
				allNumbers.push_back(i);
				allNames.push_back(theCity->CityName);
			}
		}
		if (allNumbers.size() == 0) {
			cout << "The name is not founded, please enter again:";
			//cin.ignore();
			getline(cin, inputCity);
			theCity = City;
			continue;
		}
		else if (allNumbers.size() == 1) {
			return allNumbers.at(0);
		}
		else{
			cout << "please choose one city below(using number):\n";
			int j = 0;
			for (; j < allNumbers.size(); j++) {
				cout << j+1 << ":" << allNames.at(j)<<endl;
			}
			string res;
			getline(cin,res);
			if (res=="bye"){
				return -1;
			}
			int num = atoi(res.c_str());
			while (!(num>=1&&num<=j)){
				cout << "please enter a nunber that less than "<<j+2<<" and bigger than 0,please enter again:";
				getline(cin, res);
				if (res == "bye") {
					return -1;
				}
				num = atoi(res.c_str());
			}
			return allNumbers.at(num-1);
		}
	}
}

int main()
{
	fstream f1("world_cities.csv", ios::in );
	if (!f1.good()) {
		cout << "the file is missing\n";
		exit (-1);
	}
	city allCitys[ArratSize];
	int i = 0;
	f1.seekg(0, ios_base::end);
	const int last= f1.tellg();
	f1.seekg(0, ios_base::beg);
	bool outOfLength = false;
	for (; (!f1.eof())&& f1.tellg()!=last;i++) {
		if (i >= ArratSize) {
			cout << "Warning: data is more than " << ArratSize << endl;
			break;
		}
		string line;
		getline(f1, line);
		setCity(&(allCitys[i]), line,outOfLength);
	}
	if (outOfLength){
		cout << "Warning: some city's name is longer than "<< NameLength << endl;
	}
	if (i < ArratSize) {
		cout << "Warning: data is less than "<<ArratSize << endl;
	}
	const int length = i;
	while (true) {

		cout << "The first city: ";
		const int first = enterCity(allCitys, length);
		if (first == -1) {
			break;
		}

		cout << "The second city: ";
		const int second = enterCity(allCitys, length);
		if (second == -1) {
			break;
		}

		const double pi = 3.14159;//取pi为3.14159
		const double phi1 = (90 - allCitys[first].latitude) / 180 * pi;
		const double theta1 = allCitys[first].longitude / 180 * pi;
		const double phi2 = (90 - allCitys[second].latitude) / 180 * pi;
		const double theta2 = allCitys[second].longitude / 180 * pi;
		const double c = sin(phi1) * sin(phi2) * cos(theta1 - theta2) + cos(phi1) * cos(phi2);
		const double d = 6371 * acos(c);

		cout << "The distance between " << allCitys[first].CityName << " and  ";
		cout << fixed << setprecision(0) << allCitys[second].CityName << " is " << d << " km"<<endl;

	}


	f1.close();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
