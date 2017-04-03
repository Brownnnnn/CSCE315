#include <iostream>
#include "json.hpp"

using namespace std;
using nlohmann::json;

const string star_line  = "* * * * * * * * * * * * * * * * * * * * * * * * * *";
const string solid_line = "- - - - - - - - - - - - - - - - - - - - - - - - - -";
const string welcome_prompt = "\tWelcome to simple SQL database.";
const string main_menu = "\t1. Display Users\n\t2. Display Business\n\t3. Combine Table\n\t4. Quit\n";
const string user_menu = "\tDisplay Users\n\t\t1. Display Single User\n\t\t2. Display All users\n\t\t3. Back\n";
const string cpny_menu = "\tDisplay Companys\n\t\t1. Display Single Company\n\t\t2. Display All Companys\n\t\t3. Back\n";
void clear(){
    cout << string( 100, '\n' );
}
void back(){
    clear();
    cout << main_menu << endl;  
}

void options(){
    cout << main_menu << endl;
    
    char c;
    cin >> c;
    while(true){
        switch(c){
            case '1' : 
            {
                clear();
                cout << user_menu << endl;
                cin >> c;
                if(c == '3'){
                    back();
                    cin >> c;
                    break;  
                }
                break;
            }
            case '2' :
            {
                clear();
                cout << cpny_menu << endl;
                cin >> c;
                break;
            }
            case '3' :  break;
            case '4' :  break;
        }
    }
}
int main(){
    
    json j;
    json j2 = {
    {"pi", 3.141},
    {"happy", true},
    {"name", "Niels"},
    {"nothing", nullptr},
    {"answer", {
        {"everything", 42}
    }},
    {"list", {1, 0, 2}},
    {"object", {
        {"currency", "USD"},
        {"value", 42.99}
    }}
    };
    cout << j2["pi"] << endl;
    return 0;
}
