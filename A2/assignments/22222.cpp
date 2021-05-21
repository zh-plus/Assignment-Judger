#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;

int load_info();
int query(char[],int);
int cal_dist(char[], char[], float, float, float, float);
char uppercase(char);
int check_in(char[]);
#define  max_len_name  35
#define  arr_size  1000
#define  minimun_in 3
struct city_info{
    char city_name[max_len_name+1];
    // char province_or_state[max_len_name+1];
    char country[max_len_name+1];
    float latitude;
    float longitude;
};

city_info *ct_ifos;
city_info *possible_cities_1;
city_info *possible_cities_2;

int main(){
    ct_ifos = new city_info[arr_size];
    possible_cities_1 = new city_info[arr_size];
    possible_cities_2 = new city_info[arr_size];
    int flag = load_info();
    if(flag!=1) return -1;
    //datas
    int cnt = 0;
    city_info query_result_1; city_info query_result_2;
    while(true && cnt <10000){
        cnt++;
        int possible_num_1 =0; int possible_num_2 = 0;
        char first_in[max_len_name+1];
        char second_in[max_len_name+1];

        cout << "Enter two Names"<< endl;
        cout << "please enter the name of first city(whenever enter bye to exit):";
        int flag = check_in(first_in) ;
        if(flag == -1){
            cout << "The name must be more than 3 chars" << endl; continue;
        }else if(flag == 0){
            cout << "goodbye! :)";
            return 0;
        }else if(flag == 2){
            cout << "The name must be less than " << max_len_name << "chars" << endl;continue;
        }

        cout << "please enter the name of second city(whenever enter bye to exit):";
        flag = check_in(second_in);
        if(flag == -1){
            cout << "The name must be more than 3 chars" << endl; continue;
        }else if(flag == 0){
            cout << "goodbye! :)";
            return 0;
        }else if(flag == 2){
            cout << "The name must be less than " << max_len_name << "chars" << endl;continue;
        }

        possible_num_1 = query(first_in,1);

        possible_num_2 = query(second_in,2);

        if(possible_num_1 == 0){
            cout << "The first city " << first_in << " doesn't exist."<<endl;
            continue;
        }else if(possible_num_1 > 1){
            int res = 0;
            cout << "The possible first city you want to find:"<<endl;
            for(int i = 0;i < possible_num_1; i++){
                cout << i << "." << possible_cities_1[i].city_name << ',' << possible_cities_1[i].country << endl;
            }
            cout << "please enter a integer of what you want:";
            cin >> res; cin.get();
            query_result_1 = possible_cities_1[res];
        }else{
            query_result_1 = possible_cities_1[0];
        }


        if(possible_num_2 == 0){
            cout << "The second city " << second_in << " doesn't exist."<<endl;
            continue;
        }else if(possible_num_2 > 1){
            int res = 0;
            cout << "The possible second city you want to find:"<<endl;
            for(int i = 0;i < possible_num_2; i++){
                cout << i << "." << possible_cities_2[i].city_name << ","<< possible_cities_2[i].country << endl;
            }
            cout << "please enter a integer of what you want:";
            cin >> res; cin.get();
            query_result_2 = possible_cities_2[res];
        }else{
            query_result_2 = possible_cities_2[0];
        }
        break;
    }

    if(!strcmp(query_result_1.city_name,query_result_2.city_name) && !strcmp(query_result_1.country,query_result_2.country)){
        cout << "You are searching for the same location, the result is: 0" << endl;
        return 0;
    }
    cal_dist(query_result_1.city_name,query_result_2.city_name,
    query_result_1.latitude,query_result_1.longitude,
    query_result_2.latitude,query_result_2.longitude);
    delete [] ct_ifos;
    delete [] possible_cities_1;
    delete [] possible_cities_2;
}

int load_info(){

    ifstream inf;
    inf.open("world_cities.csv",ios::in);

    if(!inf.good()){
        cout << "warning: The file doesn't exist.";
        return -1;
    }

    int tmp=0;
    while(!inf.eof()){
        if(tmp == arr_size){cout << "warning: There are some information unloaded."<<"(most records: "<< arr_size << ")" <<endl; return -2;}
        if(!inf.good()){cout << "error: read file mistakes." << endl;return-2;}

        float num = 0;char word[max_len_name+1];
        for(int state = 0; state <= 4;state++){
        for(int i = 0;i<max_len_name+1;i++){word[i]=0;}

            if(state <= 2){

                for(int i = 0; i < max_len_name +1; i++){
                    char w = inf.get();
                    if(i >= max_len_name && w!=','){
                            if(state!=1){
                                cout << "warning: there are at least 1 name truncated"<<"(max length: "<< max_len_name << ")" <<endl;
                                return -1;
                            }
                            do{}while(inf.get()!=',');
                            w = ',';
                        }
                    if(w == ','){
                        switch (state)
                        {
                        case 0:
                            strcpy(ct_ifos[tmp].city_name, word);
                            break;
                        case 1:
                            // strcpy(ct_ifos[tmp].province_or_state, word);
                            break;
                        case 2:
                            strcpy(ct_ifos[tmp].country, word);
                            break;
                        default:
                            break;
                        }
                        break;
                    }else if(w == 10){
                        break;
                    }else{
                        word[i] = w;
                    }
                }
            }else{
                if(inf.peek() == ','|| inf.peek() == 10){inf.get();}
                inf >> num;
                switch (state)
                {
                case 3:
                    ct_ifos[tmp].latitude = num;
                    break;
                case 4:
                    ct_ifos[tmp].longitude = num;
                    break;
                default:
                    break;
                }
                if(inf.peek() == 10){inf.get();}
            }

        }
        tmp++;
    }

    inf.close();
    // cout << "The input info from file are:"<<endl;
    // for(int i = 0; i < arr_size; i++){
    //     cout << "i: " << i<< " ";
    //     cout << ct_ifos[i].city_name << ",";
    //     // cout << ct_ifos[i].province_or_state<<",";
    //     cout << ct_ifos[i].country<<",";
    //     cout << ct_ifos[i].latitude<<",";
    //     cout << ct_ifos[i].longitude<<endl;
    // }
    return 1;

}

int query(char s1[],int num){
    int same_num = 0;
    int len = strlen(s1);
    if(num == 1){

        for(int i = 0; i < arr_size; i++){
            int cnt = 0;
            for(int j = 0; j < len; j++){
                if(/*isalpha(s1[j]) && */uppercase(ct_ifos[i].city_name[j]) == uppercase(s1[j])){
                    cnt++;
                }else{
                    break;
                }
            }
            if(cnt==len){
                possible_cities_1[same_num] = ct_ifos[i];
                same_num++;
            }
        }
        return same_num;
    }else{
        for(int i = 0; i < arr_size; i++){
            int cnt = 0;
            for(int j = 0; j < len; j++){
                if(/*isalpha(s1[j]) && */uppercase(ct_ifos[i].city_name[j]) == uppercase(s1[j])){
                    cnt++;
                }else{
                    break;
                }
            }
            if(cnt==len){
                possible_cities_2[same_num] = ct_ifos[i];
                same_num++;
            }
        }
        return same_num;
    }

}



int cal_dist(char ct1[],char ct2[], float lat1, float lgt1, float lat2, float lgt2){

    float phi1= (90 - lat1)/ 180 * M_PI;
    float theta1 = lgt1 / 180 * M_PI;
    float phi2 = (90 - lat2)/ 180 * M_PI;
    float theta2 = lgt2 / 180 * M_PI;
    double c = sin(phi1) * sin(phi2) * cos(theta1 - theta2) + cos(phi1) * cos(phi2);
    double d = 6371 * acos(c);

    //result
    printf("The distance between %s and %s is %f km\n",ct1,ct2,d);
    return 0;
}


char uppercase(char a){

    if(a>='a'&&a<='z'){
        return (char)(a-32);
    }
    return a;
}

int check_in(char flag[]){
    for(int i = 0; i <= max_len_name; i++){flag[i]=0;}
    bool begin_or_end = true;
    char tmp;
    int p = 0;int cnt = 0;
    while(true){
        tmp = cin.get();

        if(tmp=='\n'){
            if(cnt){flag[p-1] = 0;}
            break;
        }

        if(begin_or_end){
            if(tmp == ' '){continue;}
            else{
                flag[p] = tmp;
                p++;cnt=0;
                begin_or_end = false;
            }
        }else{
            if(tmp == ' '){
                flag[p]=tmp;
                p++;cnt=1;
                begin_or_end = true;
            }else{
                flag[p]=tmp;
                p++;
            }
        }
    }

    if(strlen(flag)<3){//too short
        return -1;
    }
    if(strlen(flag)==3 && (uppercase(flag[0])=='B')
        && (uppercase(flag[1])=='Y') && (uppercase(flag[2])=='E')){//bye
            return 0;
        }

    if(strlen(flag)>max_len_name){
        return 2;
    }
    return 1;
}

