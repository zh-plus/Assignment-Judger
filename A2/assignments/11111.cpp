#include <iostream>
#include <cctype>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <algorithm>

#define city_name 35
#define country_name 35
#define array_size 1000
#define file_name "world_cities.csv"
#define quit "bye"

using namespace std;

struct City
{
    char name[city_name];
    char country[country_name];
    double latitude;
    double longitude;
};

City cities[array_size];

bool insensitiveCompare(string a, string b){
    transform(a.begin(),a.end(),a.begin(),::tolower);
    transform(b.begin(),b.end(),b.begin(),::tolower);
    return (a.compare(b) == 0);
}

bool cityCompare(string user_input, string city){
    transform(user_input.begin(),user_input.end(),user_input.begin(),::tolower);
    transform(city.begin(),city.end(),city.begin(),::tolower);
    for(int i = 0; i<user_input.length(); i++){
        if(user_input[i]!=city[i]){
            if (i+1==user_input.length() && user_input[i]==' '){
                return true;
            }
            return false;
        }
    }
    return true;
}

void DisplayCityTable(){
    for(int i=0; i<array_size; i++){
        cout << cities[i].name << " "
        << cities[i].country << " "
        << cities[i].latitude << " "
        << cities[i].longitude << endl;
    }
}

int getFlagOfCity(string name, int beginnging){
    for(int i=beginnging; i<array_size; i++){
        if(cityCompare(name, cities[i].name)){
            return i;
        }
    }
    return -1;
}

void SplitByCommon(string line, int flag){
    stringstream ss(line);
    string item;
    int p = 0;
    char tmp[20];
    while(getline(ss, item,',')){
        switch (p)
        {
        case 0:
            item.copy(cities[flag].name,city_name-1,0);
            *(cities[flag].name+city_name-1) = '\0';
            break;
        case 2:
            item.copy(cities[flag].country,country_name-1,0);
            *(cities[flag].country+country_name-1) = '\0';
            break;
        case 3:
            item.copy(tmp,20,0);
            cities[flag].latitude= atof(tmp);
            break;
        case 4:
            item.copy(tmp,20,0);
            cities[flag].longitude= atof(tmp);
            break;
        }
        p++;
    }
}

void LoadData(){
    string line;
    fstream f1(file_name, ios::in | ios::out);
    int flag = 0;
    if(f1.good()){
        while(!f1.eof()){
            if(flag >= array_size){
                break;
            }
            getline(f1,line);
            SplitByCommon(line, flag);
            flag++;
        }
        f1.close();
    } else{
        cout << "File Missed";
    }
}

struct City getCity(int flag){
    City res_city;
    int word_num;
    while(true){
        cout << "The "<< (flag==0? "first": "second")<< " city: ";
        gets(res_city.name);
        word_num = strlen(res_city.name);
        if (word_num < 3){
            cout << "Too short\n";
            continue;
        }
        int beginning = 0;
        int name_length = 0;
        int prev_beginning = 0;
        while (beginning != -1){
            beginning = getFlagOfCity(res_city.name,beginning);
            if(beginning!=-1){
                cout << cities[beginning].name << endl;
                prev_beginning = beginning;
                beginning++;
                name_length++;
            }
        }
        if(name_length==0){
            cout << "No this city\n";
        } else if(name_length == 1){
            memcpy(res_city.name, cities[prev_beginning].name, city_name);
            res_city.latitude = cities[prev_beginning].latitude;
            res_city.longitude = cities[prev_beginning].longitude;
            break;
        }
    }
    return res_city;
}

int main()
{
    LoadData();
    // Print for test
    // DisplayCityTable();

    string instruction;
    do{
        City first_city = getCity(0);
        City second_city = getCity(1);

        double distance = 0;
        double phi1 = (90 - first_city.latitude)*M_PI / 180;
        double theta1 = first_city.longitude*M_PI / 180;
        double phi2 = (90 - second_city.latitude)*M_PI / 180;
        double theta2 = second_city.longitude*M_PI / 180;
        double c = sin(phi1) * sin(phi2) * cos(theta1 - theta2) + cos(phi1) * cos(phi2);
        distance = 6371 * acos(c);
        cout << "The distance between "<< first_city.name << " and " << second_city.name << " is " << distance << " km";

        cout << "\nBye? ";
        getline(cin, instruction);
    }while(!insensitiveCompare(instruction, quit));

    return 0;
}