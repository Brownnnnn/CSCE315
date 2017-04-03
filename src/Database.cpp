/*************************************************************************
	User defined libraries
**************************************************************************/
#include "Database.h"
#include "Table.h"
#include "Tokenizer.h"

Database::Database(){}
Database::~Database(){}

void Database::addTable(std::string name, Table table) {
    tables.insert(std::pair<std::string, Table>(name, table));
}

void Database::dropTable(std::string name) {
    tables.erase(name);
}

const std::vector<std::string> Database::listTableNames() {
    std::vector<std::string> names;
    for (std::map<std::string, Table>::iterator i = tables.begin(); i != tables.end(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::map<std::string, Table> Database::getTables(){
    return tables;
}

bool Database::tableExists(const std::string name) {
    for (std::map<std::string, Table>::iterator i = tables.begin(); i != tables.end(); ++i) {
        if (i->first == name) return true;
    }
    return false;
}

const Table Database::Query(std::string SELECT, std::string FROM, std::string WHERE) {

    Table result_tbl;
    vector<Attribute> matched_columns;
    
    map<string,vector<double>> select_result;

    /* FROM CLAUSE */
    Table mathched_table = parseFrom(FROM);
    vector<Attribute> all_columns = mathched_table.getColumns();

    /* SELECT CLAUSE */
    if(SELECT == "*") {
        matched_columns = all_columns;
    }
    else{
        select_result = parseSelect(SELECT,mathched_table);

	    for(const auto &tbl : all_columns){
            auto found = select_result.find(tbl.name);
			if( found != select_result.cend()){
				matched_columns.push_back(Attribute(tbl.name));
			}
	    }
    }

    /* WHERE CLAUSE */
    
    //parseWhere return matched_rowsed row index.
    vector<size_t> matched_rows = parseWhere(WHERE, mathched_table);
	if(matched_rows.size() == 0){
		cout << "Warning : No qualified data found, Please check your Where clause." << endl;
	}

    /* Parsing Data */
    result_tbl.setColumns(matched_columns);
    Record r(matched_columns.size());

    for(int i = 0; i < matched_rows.size(); ++i){
        int index = matched_rows.at(i);
        for(int j = 0; j < matched_columns.size(); ++ j){
            string name = matched_columns.at(j).name;
            //get column index of matched table.
	        size_t k = mathched_table.getIndexByName(name);
	        
            auto iter = select_result.find(name);
            //if attribute name is in Expression    (Name (+ - * /) Number)
	        if(iter->second.size() > 0 && SELECT != "*"){
		        r.setEntry(j,to_string(iter->second.at(index)));
	        }
	        else{   //if only attribute name, get data from mathched table
		        string value = mathched_table.getRows().at(index).getEntry(k);
		        r.setEntry(j,value);
	        }
        }
        result_tbl.insertRow(r);
    }
    return result_tbl;
}


Table Database::parseFrom(const string &FROM)
{
    bool exist = false;
    vector<string> names = listTableNames();
    for(const auto c : names)
        if(c == FROM) exist = true;
    if(!exist) throw runtime_error("Error : Table is not exist. [Database::parseFrom]");
    return getTables()[FROM];
}

map<string,vector<double>> Database::parseSelect(const string &SELECT,Table &tbl)
{
    map<string,vector<double>> results;
    string name;
    vector<double> values;

    Token_stream ts(SELECT);
    string data = Expression(ts,tbl);

    //get size of data
    int semi_cnt = count(data.cbegin(),data.cend(),';');
	int comma_cnt = count(data.cbegin(), data.cend(), ',');

    if(semi_cnt > 0) {//Multiple Attribute names or Expression.
	    vector<string> ret_vec = split(data, ";");

	    for (auto str : ret_vec) {
		    comma_cnt = count(str.cbegin(), str.cend(), ',');
		    if (comma_cnt > 0) {//if Attribute name in Expression
			    vector<string> word = split(str,",");   //word contain [NAME,VALUES]
			    name = word.at(0);  //
			    string value =  word[1];
                
			    int space_cnt = count(value.cbegin(), value.cend(), ' ');
			    if (space_cnt > 0) {
                    //get individual value.
				    vector<string> temp = split(value, " ");
				    for (auto v : temp) {
					    values.push_back(stod(v));
				    }
				    results.insert({name, values});
			    }
		    } else {    //Only Attribute name in the select cluase
			    name = str;
			    values.resize(0);
			    results.insert({name, values});
		    }
	    }
    }
	else if (comma_cnt > 0){    //Single condition and attribute name in Expression
	    vector<string> word = split(data, ",");
	    name = word.at(0);
	    string value =  word[1];

	    int space_cnt = count(word.at(1).cbegin(), word.at(1).cend(), ' ');
	    if (space_cnt > 0) {
		    vector<string> temp = split(value, " ");
		    for (auto v : temp) {
			    values.push_back(stod(v));
		    }
		    results.insert({name, values});
	    }
    }
    else{   //Only Single attribute name
        values.resize(0);
        results.insert({data, values});
    }

    return results;

}

vector<size_t> Database::parseWhere(const string &WHERE,Table &tbl)
{
    //matched row will store matching index.
    vector<size_t> matched_rows;
    Token_stream ts(WHERE);
    string results = Expression(ts,tbl);

    //get individual index
    vector<string>temp_ret = split(results," ");

    for(auto t : temp_ret){
        matched_rows.push_back(stoi(t));
    }
    return matched_rows;

}

