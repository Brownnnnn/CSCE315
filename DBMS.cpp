#include "./libs/DBMS.h"
#include <string>
#include <cctype>


DBMS::DBMS() { }

//-------------------------------------------------------------------------------

DBMS::~DBMS() { }

//-------------------------------------------------------------------------------

vector<json> DBMS::createJson(const string &path)
{
    vector<json> jsons;
    string line;
    ifstream fin(path);
    if(!fin.is_open()) throw runtime_error("file " + path + " not found.");
   
    //parsing json data
    while(getline(fin,line))
        jsons.push_back(json::parse(line)); 

    fin.close();
    return jsons;
}

//-------------------------------------------------------------------------------

table DBMS::createTable(vector<json> &jsons)
{
    if (jsons.size() == 0) throw runtime_error("Error : source data size is zero");

    vector<string> attrs;
    //get and store attribute names
    for (json::iterator it = jsons.at(0).begin(); it != jsons.at(0).end(); ++it) {
        attrs.push_back(it.key());
    }
    //create table use above attrributes
    table t(attrs);

    //insert records
    for(auto js = jsons.begin(); js != jsons.end(); ++js){
        vector<string> data;
        for(auto it = (*js).begin(); it != (*js).end(); ++it){
            //convert json type to string
            ostringstream oss;
            oss << it.value();
            string temp = oss.str();

            //ingore quotation marks
            if(temp.at(0) == '\"') data.push_back(temp.substr(1,temp.size()-2));
            else data.push_back(temp);
        }
        t.insert(record(attrs,data));
    }
    return t;
}

//-------------------------------------------------------------------------------

void DBMS::init()
{
    clear();
    cout << "***********************************************\n";
    cout << "\t\t initializing...\n";
    cout << "***********************************************\n\n";
    cout << "Make sure maximize your terminal! Have Fun!" << endl;

    vector<json> j1 = createJson("./datas/user_20k.json");
    vector<json> j2 = createJson("./datas/business_20k.json");
    vector<json> j3 = createJson("./datas/tip_20k.json");
    vector<json> j4 = createJson("./datas/review_20k.json");
    vector<json> j5 = createJson("./datas/checkin_20k.json");
    table t1 = createTable(j1);
    table t2 = createTable(j2);
    table t3 = createTable(j3);
    table t4 = createTable(j4);
    table t5 = createTable(j5);
    
    //store tables in DBMS private member
    tables = vector<table>{t1,t2,t3,t4,t5};

    db.addTable(t1, "USER");
    db.addTable(t2, "BUSINESS");
    db.addTable(t3, "TIP");
    db.addTable(t4, "REVIEW");
    db.addTable(t5, "CHECKIN");
}

//-------------------------------------------------------------------------------

void DBMS::clear()
{
    cout << string(100,'\n');
}

//-------------------------------------------------------------------------------

void DBMS::analysisUser()
{
    cin.get();
    string input;
    cout << "Because I only take first 20k line of orginal data, please use a common name)\n";
    cout << "Such as : bob, peter, john, etc... , otherwise it may not able to find relate information." << endl;
    cout << "Please enter a name : ";
    getline(cin,input);

    //convert input to upper case.
    for(auto &s : input) s = toupper(s);


    clear();
    vector<string> IDs = getUserIDsByName(input);
    if(IDs.size() == 0) cerr << "Error : " << input << " is not in our database." << endl;
    vector<string> tips = getTipByUserIDs(IDs);
    vector<string> reviews = getReviewByUserIDs(IDs);
    cout << "\nWe found " << IDs.size() << " user(s) matched same name.\n\n";
    cout << "WARNING : By default, it only print first record for grading purpose. \n  \
    Do you want to print all ? (Y/N) \n  \
    I suggest you choose NO, ohterwise it will print many lines." << endl;
    
    //initialize preview mode
    bool preview = false;
    char c;
    cin >> c;
    if(c == 'Y' || c == 'y')
        preview = true;

    if(tips.size() == 0 && reviews.size() == 0){
        cerr << "Error : No match data found, Please try other business name. \n(Note: We only handle first 20k line of orginal data. The ID maybe not in our datas.) \n"; 
    }
    else{

        if(tips.size() == 0) cout << "No tips matched this user, [we only take 20k lines from orginal datas]" << endl;
        else{   //get all tips relate this user and pertty print them
            cout << solid_line << "\n" << "Tips we found relate this business : \n" << solid_line << endl;
            for(int i = 0; i < tips.size(); i+=4){
                cout << "BUSINESS_ID :\t" << tips.at(i) << "\n";
                cout << "DATE :\t\t" << tips.at(i+1) << "\n";
                cout << "LIKES :\t\t" << tips.at(i+2) << "\n";
                cout << "TIP :\t\t" << tips.at(i+3) << "\n" << endl;
                //preview mode, it will only print first matching information.
                if(preview == false) break;
            }
        }

        if(reviews.size() == 0) cout << "No reviews matched this business, [we only take 20k lines from orginal datas]" << endl;
        else{   //get all reviews relate this user and pertty print them
            cout << solid_line << "\n" << "Reviews we found relate this business : \n" << solid_line << endl;
            for(int i = 0; i < reviews.size(); i+=7){
                cout << "BUSINESS_ID :\t" << reviews.at(i) << "\n";
                cout << "DATE :\t\t" << reviews.at(i+1) << "\n";
                cout << "STARS :\t\t" << reviews.at(i+2) << "\n";
                cout << "USEFUL :\t" << reviews.at(i+3) << "\n";
                cout << "FUNNY :\t\t" << reviews.at(i+4) << "\n";
                cout << "COOL :\t\t" << reviews.at(i+5) << "\n";
                cout << "REVIEW :\t" << reviews.at(i+6) << "\n" << endl;
                if(preview == false) break;
            }
        }
            
    }
}

//-------------------------------------------------------------------------------

void DBMS::analysisCpny()
{
    cin.get();
    string input;
    cout << "Because I only take first 20k line of orginal data, please use common names )\n";
    cout << "Such as : pizza pizza,  etc... , otherwise it may not able to find relate information." << endl;
    cout << "Please enter a name : ";
    getline(cin,input);

    //convert input to upper case.
    for(auto &s : input) s = toupper(s);

    clear();
    vector<string> IDs = getCpnyIDsByName(input);
    cout << IDs.size();
    if(IDs.size() == 0) cerr << "Error : " << input << " is not in our database." << endl;
    vector<string> tips = getTipByCpnyIDs(IDs);
    vector<string> reviews = getReviewByCpnyIDs(IDs);
    vector<string> times = getCheckInByCpnyIDs(IDs);

    cout << "\nWe found " << IDs.size() << " business matched same name.\n\n";
    cout << "WARNING : By default, it only print first record for grading purpose. \n  \
    Do you want to print all ? (Y/N) \n  \
    I suggest you choose NO, ohterwise it will print many lines." << endl;
    
    //initialize preview mode
    bool preview = false;
    char c;
    cin >> c;
    if(c == 'Y' || c == 'y') {
        preview = true;
    }

    if(tips.size() == 0 && reviews.size() == 0 && times.size() == 0){
         cerr << "Error : No match data found, Please try other user name. \n(Note: We only handle first 20k line of orginal data. The ID maybe not in ourdatas.) \n"; 
    }

    else{
        if(tips.size() == 0) cout << "No tips matched this user, [we only take 20k lines from orginal datas]" << endl;
        else{   //get all tips relate this business and pertty print them
            cout << solid_line << "\n" << "Tips we found relate this user : \n" << solid_line << endl;
            for(int i = 0; i < tips.size(); i+=4){
                cout << "USER_ID :\t" << tips.at(i) << "\n";
                cout << "DATE :\t\t" << tips.at(i+1) << "\n";
                cout << "TIP :\t\t" << tips.at(i+3) << "\n" << endl;
                //preview mode, it will only print first matching information.                
                if(preview == false) break;
            }
        }

        if(reviews.size() == 0) cout << "No reviews matched this user, [we only take 20k lines from orginal datas]" << endl;
        else{   //get all review relate this business and pertty print them
            cout << solid_line << "\n" << "Reviews we found relate this user : \n" << solid_line << endl;
            for(int i = 0; i < reviews.size(); i+=7){
                cout << "USER_ID :\t" << reviews.at(i) << "\n";
                cout << "DATE :\t\t" << reviews.at(i+1) << "\n";
                cout << "STARS :\t\t" << reviews.at(i+2) << "\n";
                cout << "USEFUL :\t" << reviews.at(i+3) << "\n";
                cout << "FUNNY :\t\t" << reviews.at(i+4) << "\n";
                cout << "COOL :\t\t" << reviews.at(i+5) << "\n";
                cout << "REVIEW :\t" << reviews.at(i+6) << "\n" << endl;
                if(preview == false) break;
            }
        }
        if(times.size() == 0) cout << "No checkin time matched this business, [we only take 20k lines from orginal datas]" << endl;
        else{   //get all checkin relate this business and pertty print them
            cout << solid_line << "\n" << "Checkin we found relate this business : \n" << solid_line << endl;
            for(int i = 0; i < times.size(); i+=2){
                cout << "BUSINESS_ID :\t" << times.at(i) << "\n";
                cout << "Check-In :\n";
                vector<string> days = split(times.at(i+1),"[,]\"");
                for(int j = 0; j < days.size(); j++){
                    cout << "\t\t" << days.at(j) << endl; 
                }
                if(preview == false) break;
            }
        }
    }
}

//-------------------------------------------------------------------------------

void DBMS::options()
{
    clear();
    cout << main_menu << endl;
    string select, where;
    char c;
    cin >> c;
    while(true){
        switch(c){
            case '1' : //user
            {
                //table[0] is user table
                table t = tables.at(0);
                clear();
                cout << user_menu << endl;
                cin >> c;
                switch(c){
                    case '1' :  //basic info
                        select = "user_id,name,review_count,yelping_since,funny,cool,fans,average_stars";
                        break;

                    case '2' :  //compliments
                        select = "user_id, name, compliment_hot, compliment_more, compliment_profile, compliment_cute,compliment_list, compliment_note, compliment_plain, compliment_cool, compliment_funny, compliment_writer, compliment_photos";
                        break;

                    case '3' :  //frineds
                        select = "name,friends";
                        break;

                    case '4' :  //summary
                    {
                        clear();

                        cout << solid_line << "\n";
                        cout << "\t\tUSER DATA ANALYSIS\n";
                        cout << solid_line << "\n";
                        cout << "Total : \n";
                        cout << "\t User : " << t.count("USER_ID") << "\n";
                        cout << "\t Reviews : " << getTotalInteger(t,"REVIEW_COUNT") << "\n\n";
                        cout << "\t Compliment Hot : " << getTotalInteger(t,"COMPLIMENT_HOT") << "\n";
                        cout << "\t Compliment More : " << getTotalInteger(t,"COMPLIMENT_MORE") << "\n";
                        cout << "\t Compliment Profile : " << getTotalInteger(t,"COMPLIMENT_PROFILE") << "\n";
                        cout << "\t Compliment Cute : " << getTotalInteger(t,"COMPLIMENT_CUTE") << "\n";
                        cout << "\t Compliment List : " << getTotalInteger(t,"COMPLIMENT_LIST") << "\n";
                        cout << "\t Compliment Note : " << getTotalInteger(t,"COMPLIMENT_NOTE") << "\n";
                        cout << "\t Compliment Plain : " << getTotalInteger(t,"COMPLIMENT_PLAIN") << "\n";
                        cout << "\t Compliment Cool : " << getTotalInteger(t,"COMPLIMENT_COOL") << "\n";
                        cout << "\t Compliment Funny :" << getTotalInteger(t,"COMPLIMENT_FUNNY") << "\n\n";
                        cout << "Average : \n";
                        cout << "\t Stars : " << getAvgDouble(t,"AVERAGE_STARS") << "\n";
                        cout << "\t Fans : " << getAvgDouble(t,"FANS") << "\n\n";
                        cout << "\t Compliment Hot : " << getAvgDouble(t,"COMPLIMENT_HOT") << "\n";
                        cout << "\t Compliment More : " << getAvgDouble(t,"COMPLIMENT_MORE") << "\n";
                        cout << "\t Compliment Profile : " << getAvgDouble(t,"COMPLIMENT_PROFILE") << "\n";
                        cout << "\t Compliment Cute : " << getAvgDouble(t,"COMPLIMENT_CUTE") << "\n";
                        cout << "\t Compliment List : " << getAvgDouble(t,"COMPLIMENT_LIST") << "\n";
                        cout << "\t Compliment Note : " << getAvgDouble(t,"COMPLIMENT_NOTE") << "\n";
                        cout << "\t Compliment Plain : " << getAvgDouble(t,"COMPLIMENT_PLAIN") << "\n";
                        cout << "\t Compliment Cool : " << getAvgDouble(t,"COMPLIMENT_COOL") << "\n";
                        cout << "\t Compliment Funny :" << getAvgDouble(t,"COMPLIMENT_FUNNY") << "\n\n";
                        cout << "Oldest member since " << getMin(t,"YELPING_SINCE") << "\n";
                        cout << "Newest member since " << getMax(t,"YELPING_SINCE") << "\n";
                        break;
                    }
                    case '5' :  //analysis
                        clear();
                        analysisUser();
                        break;
                    case '6' :
                    {
                        clear();
                        cout << "Available keys : \n" << solid_line << "\n\n";
                        int cnt = 0;
                        for(auto x : t.getAttributes()){
                            cout << setw(20) << x;
                            //print five attribute each line
                            if(++cnt % 5 == 0) cout << "\n";
                        }
                        cout << endl << endl;
                        //ingore space
                        cin.get();
                        customFilter(select,where);
                        break;
                    }

                    default :
                        throw runtime_error("Error : The command is malformed.[DBMS::options]\n");
                }
                //skip print if analyzation
                if(c == '4' || c =='5') break;
                else{
                    table out = db.query(select, "USER", where);
                    clear();
                    printUser(out);
                    break;
                }
            }
            case '2' : //business
            {
                //Similar pattern, please see case 1 User part for comments
                table t = tables.at(1);
                clear();
                cout << cpny_menu << endl;
                cin >> c;
                switch(c){
                    case '1' :
                        select = "business_id,name,address,city,state,postal_code,latitude,longitude";
                        break;

                    case '2' :
                        clear();
                        select = "name,hours";
                        break;
                    case '3' :  //summary
                    {
                        clear();

                        cout << solid_line << "\n";
                        cout << "\t\tBUSINESS DATA ANALYSIS\n";
                        cout << solid_line << "\n";
                        cout << "Total : \n";
                        cout << "\t BUSINESS :\t" << t.count("BUSINESS_ID") << "\n";
                        cout << "\t Reviews :\t" << getTotalInteger(t,"REVIEW_COUNT") << "\n\n";
                        cout << "Average : \n";
                        cout << "\t Reviews :\t" << getAvgDouble(t,"REVIEW_COUNT") << "\n";
                        break;
                    }
                    case '4' :  //analysis single business
                        clear();
                        analysisCpny();
                        break;
                    case '5' :  //custom filter
                    {
                        clear();
                        cout << "Available keys : \n" << solid_line << "\n\n";
                        int cnt = 0;
                        for(auto x : t.getAttributes()){
                            cout << setw(20) << x;
                            if(++cnt % 5 == 0) cout << "\n";
                        }
                        cout << endl << endl;
                        cin.get();
                        customFilter(select,where);
                        break;
                    }

                    default :
                        throw runtime_error("Error : The command is malformed.[DBMS::options]\n");
                }
                if(c == '3' || c == '4') break;
                table out = db.query(select, "BUSINESS", where);
                clear();
                printCpny(out);
                break;
            }
            case '3' : //tips
            {
                //Similar pattern, please see case 1 User part for comments
                table t = tables.at(2);
                clear();
                cout << tip_menu << endl;
                cin >> c;
                switch(c){
                    case '1' :  //tips basic info
                    {
                        select = "business_id,user_id,date,likes,text";
                        break;
                    }
                    case '2' :  //summary
                    {
                        clear();
                        for(auto x : t.getAttributes()){
                            cout << x << endl;
                        }
                        cout << solid_line << "\n";
                        cout << "\t\tUSER DATA ANALYSIS\n";
                        cout << solid_line << "\n";
                        cout << "Total : \n";
                        cout << "\t Tips : " << t.count("TEXT") << "\n\n";
                        cout << "\t Likes : " << getTotalInteger(t,"LIKES") << "\n\n";

                        cout << "Average : \n";
                        cout << "\t Likes : " << getAvgDouble(t,"LIKES") << "\n\n";
                        cout << "Oldest tip post at " << getMin(t,"DATE") << "\n";
                        cout << "Newest tip post at " << getMax(t,"DATE") << "\n";
                        break;
                    }
                    case '3' :  //custom filter
                    {
                        clear();
                        cout << "Available keys : \n" << solid_line << "\n\n";
                        int cnt = 0;
                        for(auto x : t.getAttributes()){
                            cout << setw(20) << x;
                            if(++cnt % 5 == 0) cout << "\n";
                        }
                        cout << endl << endl;
                        cin.get();
                        customFilter(select,where);
                        break;
                    }

                    default :
                        throw runtime_error("Error : The command is malformed.[DBMS::options]\n");
                }
                if(c == '2') break;
                table out = db.query(select, "TIP", where);
                clear();
                printTip(out);
                break;
            }
            case '4' : //reviews
            {
                //Similar pattern, please see case 1 User part for comments
                table t = tables.at(3);
                clear();
                cout << review_menu << endl;
                cin >> c;
                switch(c){
                    case '1' :  //all review info
                        select = "review_id,business_id,user_id,date,stars,useful,funny,cool,text";
                        break;
                    
                    case '2' :  //summary
                    {
                        clear();
                        for(auto x : t.getAttributes()){
                            cout << x << endl;
                        }
                        cout << solid_line << "\n";
                        cout << "\t\tUSER DATA ANALYSIS\n";
                        cout << solid_line << "\n";
                        cout << "Total : \n";
                        cout << "\t Reviews : " << t.count("TEXT") << "\n\n";
                        cout << "\t Useful : " << getTotalInteger(t,"USEFUL") << "\n";
                        cout << "\t Funny : " << getTotalInteger(t,"FUNNY") << "\n";
                        cout << "\t Cool : " << getTotalInteger(t,"COOL") << "\n";
                        cout << "\t Stars : " << getTotalInteger(t,"STARS") << "\n\n";

                        cout << "Average : \n";
                        cout << "\t Useful : " << getAvgDouble(t,"USEFUL") << "\n";
                        cout << "\t Funny : " << getAvgDouble(t,"FUNNY") << "\n";
                        cout << "\t Cool : " << getAvgDouble(t,"COOL") << "\n";
                        cout << "\t Stars : " << getAvgDouble(t,"STARS") << "\n\n";

                        cout << "Oldest review post at " << getMin(t,"DATE") << "\n";
                        cout << "Newest review post at " << getMax(t,"DATE") << "\n";
                        break;
                    }
                    case '3' :  //custom filter
                    {
                        clear();
                        cout << "Available keys : \n" << solid_line << "\n\n";
                        int cnt = 0;
                        for(auto x : t.getAttributes()){
                            cout << setw(20) << x;
                            if(++cnt % 5 == 0) cout << "\n";
                        }
                        cout << endl << endl;
                        cin.get();
                        customFilter(select,where);
                        break;
                    }
                    default :
                        throw runtime_error("Error : The command is malformed.[DBMS::options]\n");
                }
                if(c == '2') break;
                table out = db.query(select, "REVIEW", where);
                clear();
                printReview(out);
                c = '|';                
                break;
            }
            case '5' : //CheckIn
            {
                //Similar pattern, please see case 1 User part for comments
                table t = tables.at(4);
                clear();
                cout << checkin_menu << endl;
                cin >> c;
                switch(c){
                    case '1' :  //checkin info
                        select = "business_id,time";
                        break;

                    case '2' :  //custom filter
                    {
                        clear();
                        cout << "Available keys : \n" << solid_line << "\n\n";
                        int cnt = 0;
                        for(auto x : t.getAttributes()){
                            cout << setw(20) << x;
                            if(++cnt % 5 == 0) cout << "\n";
                        }
                        cout << endl << endl;
                        cin.get();
                        customFilter(select,where);
                        break;
                    }

                    default :
                        throw runtime_error("Error : The command is malformed.[DBMS::options]\n");
                }
                table out = db.query(select, "CHECKIN", where);
                clear();
                printCheckIn(out);
                break;
            }            
            default :
            {
                cout << "Error : The command is malformed.[DBMS::options]\n";
                break;
            }
        }
        cout << solid_line;
        cout << "\nDo you want to continue ? Y/N \n";
        if(cin >> c && (c == 'Y' || c == 'y')){
            clear();
            cout << main_menu << endl;
            cin >> c;
        }
        else    break;       
    }
}

//-------------------------------------------------------------------------------
/*  pertty prints   */

void DBMS::printUser(const table& t) 
{
    //print attribute name
    string compliment = "COMPLIMENT";
    for(auto x: t.getAttributes()) {
        size_t pos = x.find(compliment);
        if( x == "USER_ID"){
            cout << setw(25) << x;
        }
        else if(pos != string::npos)
        {
            string temp = x.substr(compliment.size() + pos + 1, x.size());
            cout << setw(10) << temp;
        }
        else{
            cout << setw(20) << x;
        }
    }
    cout << "\n\n";
    for(auto it = t.begin(); it != t.end(); it++) {
        cout << " ";
        for(auto x: t.getAttributes()) {
            if(x == "USER_ID"){
                cout << setw(25) << it->getData(x);
            }
            else if(x.find(compliment) != string::npos) {
                cout << setw(10) << it->getData(x);
            }
            else if(x == "FRIENDS"){
                cout << "\n\n";
                vector<string> friends = split(it->getData(x),"[,]\"");
                int cnt = 0;
                for(auto f : friends){
                    cout << f << "\t";
                    if(++cnt % 5 == 0){
                        cout << "\n";
                    }
                }
                cout << "\n" << solid_line << endl;
            }
            else
                cout << setw(20) << it->getData(x);
        }
        cout << endl << endl;
    }
    //in case print a lot of data,
    //so print attribute name again in the bottom
    for(auto x: t.getAttributes()) {
        size_t pos = x.find(compliment);
        if( x == "USER_ID"){
            cout << setw(25) << x;
        }
        else if(pos != string::npos)
        {
            string temp = x.substr(compliment.size() + pos + 1, x.size());
            cout << setw(10) << temp;
        }
        else{
            cout << setw(20) << x;
        }
    }
    cout << endl;
}

//-------------------------------------------------------------------------------
/*  pertty prints   */

void DBMS::printCpny(const table& t) 
{
    //print attribute name
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(22) << x;
        }
        else if(x == "ADDRESS" || x == "NAME"){
            cout << setw(45) << x;
        }
        else if(x == "STATE"){
            cout << setw(7) << x;
        }
        else{
            cout << setw(15) << x;
        }
    }
    cout << "\n\n";
    for(auto it = t.begin(); it != t.end(); it++) {

        for(auto x: t.getAttributes()) {
            if(x.find("ID") != string::npos){
                cout << setw(22) << it->getData(x);
            }
            else if( x == "ADDRESS" || x == "NAME"){
                cout << setw(45) << it->getData(x).substr(0,43);
            }
            else if(x == "STATE"){
                cout << setw(7) << it->getData(x);
            }
            else if(x == "HOURS"){
                if(it->getData(x) == "NULL")
                {
                    cout << "\n" << "\tNOT AVAILABLE" << endl;
                    cout << solid_line << endl;
                }
                else{
                    cout << "\n";
                    vector<string> days = split(it->getData(x),"[,]\" ");
                    for(int i = 0; i < days.size(); i+=2){
                        cout << setw(20) << days.at(i) << setw(10) << days.at(i+1) << endl; 
                    }
                    cout << solid_line << endl;
                }
            }
            else
                cout << setw(15) << it->getData(x).substr(0,13);
        }
        cout << endl << endl;
    }
    //in case print a lot of data,
    //so print attribute name again in the bottom
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(22) << x;
        }
        else if(x == "ADDRESS" || x == "NAME"){
            cout << setw(45) << x;
        }
        else if(x == "STATE"){
            cout << setw(7) << x;
        }
        else{
            cout << setw(15) << x;
        }
    }
    cout << endl;
}

//-------------------------------------------------------------------------------
/*  pertty prints   */

void DBMS::printTip(const table& t) 
{
    //print attribute name
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(25) << x;
        }
        else if(x == "LIKES"){
            cout << setw(8) << x;
        }
        else if(x == "DATE"){
            cout << setw(15) << x;
        }
        else{
            cout << "  " << x;   
        }
    }
    cout << "\n\n";
    for(auto it = t.begin(); it != t.end(); it++) {
        for(auto x: t.getAttributes()) {
            if(x.find("ID") != string::npos){
                cout << setw(25) << it->getData(x);
            }
            else if( x == "LIKES"){
                cout << setw(8) << it->getData(x);
            }
            else if( x == "TEXT"){
                cout << "  ";
                if(it->getData(x).size() > 100){
                    cout << it->getData(x).substr(0,100) << "...";
                }
                else{
                    cout << it->getData(x);
                }
            }
            else
                cout << setw(15) << it->getData(x).substr(0,13);
        }
        cout << endl << endl;
    }
    //in case print a lot of data,
    //so print attribute again in the bottom
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(25) << x;
        }
        else if(x == "LIKES"){
            cout << setw(8) << x;
        }
        else if(x == "DATE"){
            cout << setw(15) << x;
        }
        else{
            cout << "  " << x;   
        }
    }    
    cout << endl;
}

//-------------------------------------------------------------------------------
/*  pertty prints   */

void DBMS::printReview(const table& t) 
{
    //print attribute name
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(25) << x;
        }
        else if(x == "STARS" || x == "FUNNY" || x == "COOL" || x == "USEFUL"){
            cout << setw(8) << x;
        }
        else if(x == "DATE"){
            cout << setw(15) << x;
        }
        else{
            cout << "  " << x;   
        }
    }
    cout << "\n\n";
    for(auto it = t.begin(); it != t.end(); it++) {

        for(auto x: t.getAttributes()) {
            if(x.find("ID") != string::npos){
                cout << setw(25) << it->getData(x);
            }
            else if( x == "STARS" || x == "FUNNY" || x == "COOL" || x == "USEFUL"){
                cout << setw(8) << it->getData(x);
            }
            else if( x == "TEXT"){
                cout << "  ";
                if(it->getData(x).size() > 50){
                    cout << it->getData(x).substr(0,50) << "...";
                }
                else{
                    cout << it->getData(x);
                }
            }
            else
                cout << setw(15) << it->getData(x).substr(0,13);
        }
        cout << endl << endl;
    }
    //in case print a lot of data,
    //so print attribute again in the bottom
    for(auto x: t.getAttributes()) {
        if( x.find("ID") != string::npos){
            cout << setw(25) << x;
        }
        else if(x == "STARS" || x == "FUNNY" || x == "COOL" || x == "USEFUL"){
            cout << setw(8) << x;
        }
        else if(x == "DATE"){
            cout << setw(15) << x;
        }
        else{
            cout << "  " << x;   
        }
    }
    cout << endl;
}

//-------------------------------------------------------------------------------
/*  pertty prints   */

void DBMS::printCheckIn(const table& t) 
{
    for(auto it = t.begin(); it != t.end(); it++) {
        for(auto x: t.getAttributes()) {
            if( x == "BUSINESS_ID"){
                cout << x << " : " << it->getData(x);
            }
            else{
                cout << "\n\n";
                vector<string> time = split(it->getData(x),",\"[]");
                sort(time.begin(),time.end());
                int cnt = 0;
                for(auto t : time){
                    if(t.size() < 8)    cout << t << "\t\t";
                    else                cout << t << '\t';
                    if(++cnt %10 == 0)  cout << "\n";
                }
                cout << "\n" << solid_line << endl;
            }
        }
        cout << endl;
    }
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getUserIDsByName(const string &name)
{
    table t = tables.at(0);
    vector<string> v;
    for(auto it = t.begin(); it != t.end(); ++it){
        if(it->getData("NAME") == name){
            v.push_back(it->getData("USER_ID"));
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getCpnyIDsByName(const string &name)
{
    // cout << name << endl;
    table t = tables[1];
    vector<string> v;
    for(auto it = t.begin(); it != t.end(); ++it){
        if(it->getData("NAME") == name){
            v.push_back(it->getData("BUSINESS_ID"));
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getTipByUserIDs(vector<string> &IDs)
{

    table t = tables.at(3);    //3 is index of tip table
    vector<string> v;

    for(auto it = t.begin(); it != t.end(); ++it){
        for(auto x : IDs){
            if(it->getData("USER_ID") == x){
                v.push_back(it->getData("BUSINESS_ID"));
                v.push_back(it->getData("DATE"));
                v.push_back(it->getData("LIKES"));
                v.push_back(it->getData("TEXT"));
            }
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getTipByCpnyIDs(vector<string> &IDs)
{
    table t = tables.at(3);    //3 is index of tip table
    vector<string> v;

    for(auto it = t.begin(); it != t.end(); ++it){
        for(auto x : IDs){
            if(it->getData("BUSINESS_ID") == x){
                v.push_back(it->getData("USER_ID"));
                v.push_back(it->getData("DATE"));
                v.push_back(it->getData("TEXT"));
            }
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getCheckInByCpnyIDs(vector<string> &IDs)
{
    table t = tables.at(4);    //4 is index of review table
    vector<string> v;

    for(auto it = t.begin(); it != t.end(); ++it){
        for(auto x : IDs){
            if(it->getData("BUSINESS_ID") == x){
                v.push_back(it->getData("BUSINESS_ID"));
                v.push_back(it->getData("TIME"));
            }
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getReviewByUserIDs(vector<string> &IDs)
{
    table t = tables.at(2);    //2 is index of tip table
    vector<string> v;

    for(auto it = t.begin(); it != t.end(); ++it){
        for(auto x : IDs){
            if(it->getData("USER_ID") == x){
                v.push_back(it->getData("BUSINESS_ID"));
                v.push_back(it->getData("DATE"));
                v.push_back(it->getData("STARS"));
                v.push_back(it->getData("USEFUL"));
                v.push_back(it->getData("FUNNY"));
                v.push_back(it->getData("COOL"));
                v.push_back(it->getData("TEXT"));
            }
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

vector<string> DBMS::getReviewByCpnyIDs(vector<string> &IDs)
{
    table t = tables.at(2);    //2 is index of tip table
    vector<string> v;

    for(auto it = t.begin(); it != t.end(); ++it){
        for(auto x : IDs){
            if(it->getData("BUSINESS_ID") == x){
                v.push_back(it->getData("USER_ID"));
                v.push_back(it->getData("DATE"));
                v.push_back(it->getData("STARS"));
                v.push_back(it->getData("USEFUL"));
                v.push_back(it->getData("FUNNY"));
                v.push_back(it->getData("COOL"));
                v.push_back(it->getData("TEXT"));
            }
        }
    }
    return v;
}

//-------------------------------------------------------------------------------

int getTotalInteger(table &t,const string &attr)
{
    int ret = 0;
    for(auto it = t.begin(); it != t.end(); it++) {
        for(auto x: t.getAttributes()) {
            if(x == attr) ret += stoi(it->getData(x));
        }
    }
    return ret;
}

//-------------------------------------------------------------------------------

int getAvgInteger(table &t,const string &attr)
{
    int sz = t.count(attr);
    if(sz == 0) throw runtime_error("Error : Size is zero.");
    int ret = getTotalInteger(t,attr);    
    return (int)ret/sz;
}

//-------------------------------------------------------------------------------

double getTotalDouble(table &t, const string &attr)
{
    double ret = 0;
    for(auto it = t.begin(); it != t.end(); it++) {
        for(auto x: t.getAttributes()) {
            if(x == attr) ret += stod(it->getData(x));
        }
    }
    return ret;
}

//-------------------------------------------------------------------------------

double getAvgDouble(table &t,const string &attr)
{
    double sz = t.count(attr);
    if(sz == 0) throw runtime_error("Error : Size is zero.");
    double ret = getTotalDouble(t,attr);    
    return ret/sz;
}
//-------------------------------------------------------------------------------

string getMin(table &t,const string &attr)
{
    record r = t.min(attr);
    string min = r.getData(attr);
    return min;
}

//-------------------------------------------------------------------------------

string getMax(table &t,const string &attr)
{
    record r = t.max(attr);
    string max = r.getData(attr);
    return max;
}

//-------------------------------------------------------------------------------
string header(const string &name)
{
    return solid_line + bars + name + bars + solid_line + bars;
}

vector<string> split(const string &s, string delim)
{
    stringstream scan (s);
    string line;
    vector<string> wordVector;

    while(getline(scan,line))
    {
        std:size_t prev = 0, pos;
        while ((pos = line.find_first_of(delim, prev)) != string::npos)
        {
            if (pos > prev)
                wordVector.push_back(line.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < line.length())
            wordVector.push_back(line.substr(prev, string::npos));
    }
    return wordVector;
}

void DBMS::customFilter(string &SELECT, string &WHERE)
{
    cout << "SELECT : ";
    getline(cin,SELECT);
    cout << "WHERE (CAN BE EMPTY) : ";
    getline(cin,WHERE);    
}