/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <typeinfo>

/*************************************************************************
	User Defined Includes
**************************************************************************/
#include "database.h"
#include "record.h"
#include "table.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


class DBMS{

private:
    database db;
    vector<table> tables;

public:
    DBMS();
    ~DBMS();
    //Parsing json file and store in vector
    vector<json> createJson(const string &path);

    //Use json to create table
    table createTable(vector<json> &jsons);
    
    //Initialize database, table
    void init();

    //Clear screen (Terminal)
    void clear();
    
    //main UI 
    void options();

    //Analize single user
    void analysisUser();

    //Analize single business
    void analysisCpny();

    //Allow user to write their own query
    void customFilter(string &SELECT, string &WHERE);

    //Pertty print
    void printUser(const table &t);
    void printCpny(const table &t);
    void printTip(const table &t);
    void printReview(const table &t);
    void printCheckIn(const table &t);

    //Return list of matched user ID b/c users might have same first name.
    vector<string> getUserIDsByName(const string &name);
    
    //Return list of matched business ID b/c it might have muliple location.
    vector<string> getCpnyIDsByName(const string &name);
    
    //Return all tip wrote by this user.
    vector<string> getTipByUserIDs(vector<string> &IDs);

    //Return all tip relate this business.
    vector<string> getTipByCpnyIDs(vector<string> &IDs);

    //Return all check-in relate this business.
    vector<string> getCheckInByCpnyIDs(vector<string> &IDs);

    //Return all review wrote by this user.
    vector<string> getReviewByUserIDs(vector<string> &IDs);

    //Retrun all review relate this business.
    vector<string> getReviewByCpnyIDs(vector<string> &IDs);
    

};


/*************************************************************************
	Helper function
**************************************************************************/
//Take a string and single(or multi) delimeter, return list of sepreated word.
vector<string> split(const string &s, string delim);

//Take a table and attribute name as input, return sum of all INTEGER
int getTotalInteger(table &t,const string &attr);

//Take a table and attribute name as input, return average of all INTEGER
int getAvgInteger(table &t,const string &attr);

//Take a table and attribute name as input, return sum of all DOUBLE
double getTotalDouble(table &t, const string &attr);

//Take a table and attribute name as input, return average of all DOUBLE
double getAvgDouble(table &t,const string &attr);

//Take a table and attribute name as input, return max value of give attribute
string getMax(table &t,const string &attr);

//Take a table and attribute name as input, return min value of give attribute
string getMin(table &t,const string &attr);


/*************************************************************************
	Messages
**************************************************************************/

//menu's header
string header(const string &name);


const string bars = "\n|                                                       |\n";
const string solid_line = "=========================================================";

const string welcome_prompt = "|\t\t Welcome To Database System \t\t|";
const string user_prompt = "|\t\t\tUser Menu\t\t\t|";
const string cpny_prompt = "|\t\t\tBusiness Menu\t\t\t|";
const string checkin_prompt = "|\t\t\tCheck-in Menu\t\t\t|";
const string tip_prompt = "|\t\t\tTip Menu\t\t\t|";
const string review_prompt = "|\t\t\tReview Menu\t\t\t|";

const string main_menu = header(welcome_prompt) + 
"|\t\t    1.  Users\t\t\t\t|" + bars +
"|\t\t    2.  Business\t\t\t|" + bars +
"|\t\t    3.  Tips\t\t\t\t|" + bars +
"|\t\t    4.  Review\t\t\t\t|" + bars +
"|\t\t    5.  CheckIn\t\t\t\t|" + bars + solid_line;

const string user_menu = header(user_prompt) + 
"|\t\t    1.  Basic Info\t\t\t|" + bars +
"|\t\t    2.  Compliments\t\t\t|" + bars +
"|\t\t    3.  Friends\t\t\t\t|" + bars +
"|\t\t    4.  Data Analysis [Bonus]\t\t|" + bars +
"|\t\t    5.  Single User Analysis [Bonus]\t|" + bars +
"|\t\t    6.  Custom Filter\t\t\t|" + bars + solid_line;

const string cpny_menu = header(cpny_prompt) +
"|\t\t    1.  All Business Info\t\t|" + bars +
"|\t\t    2.  Business Hours\t\t\t|" + bars +
"|\t\t    3.  Data Analysis [Bonus]\t\t|" + bars +
"|\t\t    4.  Single Business Analysis [Bonus]|" + bars +
"|\t\t    5.  Custom Filter\t\t\t|" + bars + solid_line;

const string tip_menu = header(tip_prompt) +
"|\t\t    1.  All Tips Info\t\t\t|" + bars +
"|\t\t    2.  Data Analysis [Bonus]\t\t|" + bars +
"|\t\t    3.  Custom Filter\t\t\t|" + bars + solid_line;

const string review_menu = header(review_prompt) +
"|\t\t    1.  All Review Info\t\t\t|" + bars +
"|\t\t    2.  Data Analysis [Bonus]\t\t|" + bars +
"|\t\t    3.  Custom Filter\t\t\t|" + bars + solid_line;

const string checkin_menu = header(checkin_prompt) +
"|\t\t    1.  All Checkin Info\t\t|" + bars +
"|\t\t    2.  Custom Filter\t\t\t|" + bars + solid_line;
