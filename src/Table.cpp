#include "Table.h"
using namespace std;

Table::Table(){}
Table::~Table(){}
Table::Table(vector<string> names)
{
    for(int i=0; i<names.size(); ++i){
        //Avoid duplicate columns
        if(columnExist(names[i]))   throw runtime_error("ERROR : Duplicate attribute name detected. [Table::constructor]");
        columns.push_back(Attribute(names[i]));
        keys.push_back(false);
    }
}

//------------------------------------------------------------------------------

Table::Table(const Table &table):
        columns(table.columns), rows(table.rows), keys(table.keys) { }

//------------------------------------------------------------------------------
//Copy Assignment

Table& Table::operator=(const Table& table)
{
    columns = table.columns;
    rows = table.rows;
    keys = table.keys;
    return *this;
}

//------------------------------------------------------------------------------

void Table::addColumn(const string &name)
{
    //Avoid duplicate columns
    if(columnExist(name)) return;

    columns.push_back(Attribute(name));

    //Exist records will pushback NULL value
    for(auto it = rows.begin(); it!= rows.end(); ++it){
        it->setEntry(getColumnSize()-1,"NULL");
        cout << "debug" << endl;
        cout << it->getEntry(getColumnSize()-1) << endl;
    }
    keys.push_back(false);

}

//------------------------------------------------------------------------------

void Table::addColumn(Attribute attr)
{
    //Avoid duplicate
    if(columnExist(attr.name)) return;

    columns.push_back(attr);

    //Exist records will pushback NULL value
    for(auto it = rows.begin(); it!= rows.end(); ++it){
        it->setEntry(getColumnSize()-1,"NULL");
        cout << "debug" << endl;
        cout << it->getEntry(getColumnSize()-1) << endl;
    }
    keys.push_back(false);
}

//------------------------------------------------------------------------------

void Table::deleteColumn(const string &name)
{
    if(!columnExist(name))  throw runtime_error("ERROR : Column does not exist in the table. [Table::getIndexByName]");

    size_t index = getIndexByName(name);

    columns.erase(columns.begin() + index);
    keys.erase(keys.begin() + index);
    
    //delete each row's value
    for(auto it = rows.begin(); it != rows.end(); ++it){
        it->deleteEntry(index);
    }
}

//------------------------------------------------------------------------------

void Table::deleteColumn(Attribute attr)
{
    if(!columnExist(attr.name)) throw runtime_error("ERROR : Column does not exist in the table. [Table::getIndexByName]");

    size_t index = getIndexByName(attr.name);

    columns.erase(columns.begin() + index);
    keys.erase(keys.begin() + index);
    
    //delete each row's value
    for(auto it = rows.begin(); it != rows.end(); ++it){
        it->deleteEntry(index);
    }
}

//------------------------------------------------------------------------------

void Table::insertRow(Record &new_record)
{
    //check upper bound
    size_t columns_size = columns.size();
    size_t entries_size = new_record.getSize();

    if(columns_size < entries_size){
        throw runtime_error("ERROR : Column size is smaller than row size. [Table::insertRow]");
    }
    else if( columns_size == entries_size){
        rows.push_back(new_record);
    }
    else{
        auto length = columns_size - entries_size;
        //temp will contain length many "NULL" string
        Record temp(length);
        new_record.Join(temp);
        rows.push_back(new_record);
       
    }
}

//------------------------------------------------------------------------------

vector<Record>::const_iterator Table::begin()const
{
    return rows.cbegin();
}

//------------------------------------------------------------------------------
    
vector<Record>::const_iterator Table::end()const
{
    return rows.cend();
}

//------------------------------------------------------------------------------

const Record& Table::first() const
{
    return rows.front();
}

//------------------------------------------------------------------------------

const Record& Table::last() const
{
    return rows.back();
}

//------------------------------------------------------------------------------

Table Table::crossJoin(Table other)
{
    if(getColumns().empty()) return other;
    if(other.getColumns().empty()) return *this;

    for(auto a : getColumns()){
        if(other.columnExist(a.name))
            throw runtime_error("ERROR : Duplicate key name detected, perhaps you want use NATRUAL JOIN? [Table::crossJoin]");
    }    

    Table t;

    for(auto it : getColumns()){
        t.addColumn(it);
    }
    for(auto it : other.getColumns()){
        t.addColumn(it);
    }

    //Combine records and insert to temp table.
    for(const Record &r1 : rows){
        for(const Record &r2 : other.rows){
            Record temp(r1);
            temp.Join(r2);
            t.insertRow(temp);
        }
    }

    return t;
}

//------------------------------------------------------------------------------

//Take two tables as input and produces one table as output
//The first table should have an attribute name(s) matching the key from the second table
Table Table::naturalJoin(Table other)
{
    vector<Attribute> matching_columns;
    
    //get matched columns
    for(auto a : getColumns()){
        if(other.columnExist(a.name))
            matching_columns.push_back(a);
    }  

    //handle errors
    if(!matching_columns.size())
        throw runtime_error("ERROR : The first table does not have mathcing attribute name.[Table::naturalJoin]");
    
    Table t;

    //addColumn will handle duplicate attribute.
    for(auto it : getColumns()){
        t.addColumn(it);
    }
    for(auto it : other.getColumns()){
        t.addColumn(it);
    }
    //Traversal rows in table 1
    for(auto i = 0; i < rows.size(); ++i){
        //Traversal rows in table 2
        for(auto j = 0; j < other.rows.size(); ++j){
            for(auto key : matching_columns){
                //get table 1 key column index
                auto t1_index = getIndexByName(key.name);
                //get table 2 key column index
                auto t2_index = other.getIndexByName(key.name);
                //insert row if both have same column name.
                if(rows[i].getEntry(t1_index) == other.rows[j].getEntry(t2_index)){
                    vector<Record> temp_row = other.rows;
                    temp_row.at(j).deleteEntry(t2_index);
                    Record temp(rows.at(i));
                    temp.Join(temp_row.at(j));
                    t.insertRow(temp);
                }
            }
        }
    }
    return t;
}

//------------------------------------------------------------------------------

size_t Table::getColumnSize()const
{
    return columns.size();
}

//------------------------------------------------------------------------------

size_t Table::getRowSize() const
{
    return rows.size();
}

//------------------------------------------------------------------------------

vector<Attribute> Table::getColumns() const
{
    return columns;
}

//------------------------------------------------------------------------------

vector<Attribute> Table::getKeys() const
{
    vector<Attribute> temp;

    for(int i = 0; i < columns.size(); ++i){
        if(keys.at(i))
            temp.push_back(columns[i]);
    }
    return temp;
}

//------------------------------------------------------------------------------

vector<Record> Table::getRows() const
{
    return rows;
}

//------------------------------------------------------------------------------

size_t Table::getIndexByName(const string& name) const
{
    //Handle invalid name
    if(!columnExist(name)) throw runtime_error("ERROR : Column does not exist in the table. [Table::getIndexByName]");

    for(auto it = columns.cbegin(); it != columns.cend(); ++it){
        if(it->name == name) return it - columns.cbegin();
    }
}

//------------------------------------------------------------------------------

vector<string> Table::getColumnValues(int index) const
{
    vector<string> result;
    for(const auto &r : rows){
        if(r.getEntry(index) != "NULL" && r.getEntry(index) != "")
            result.push_back(r.getEntry(index));
    }
    return result;
}

VAR_TYPE Table::getColumnType(int index) const
{
	return columns[index].type;
}

//------------------------------------------------------------------------------

void Table::setColumns(vector<Attribute> new_columns)
{
    columns = new_columns;
}

//------------------------------------------------------------------------------

void Table::setKeys(vector<string> names)
{
    vector<bool> temp(columns.size(),false);    
    for( auto n : names){
        temp.at(getIndexByName(n)) = true;
    }
    keys = temp;
}

//------------------------------------------------------------------------------

int Table::Count(const string &name) const
{
    size_t index = getIndexByName(name);
    return (getColumnValues(index).size());
}

//------------------------------------------------------------------------------

string Table::myMinMax(const string &name, char op) const
{

    size_t index = getIndexByName(name);
    VAR_TYPE type = columns[index].type;
    if(columns.at(index).type == NONE) throw runtime_error("Error : Attribute type not defined. [Table:myMinMax]");

    vector<string> values = getColumnValues(index);
    if(values.size() == 0) throw runtime_error("Error : No valid data detected. [Table:myMinMax]");

    string min,max;
    vector<int> i_num;
    vector<double> d_num;

    switch(type){
        case INT : 
        case INTEGER :
            for(auto v : values)    i_num.push_back(stoi(v));   break;

        case FLOAT : 
        case REAL :
            for(auto v : values)    d_num.push_back(stod(v));   break;

        case CHAR:
        case VARCHAR:
            break;

        case DATE:
            for(auto v : values)    i_num.push_back(parseDate(v));  break;

        case TIME:
            for(auto v : values)    i_num.push_back(parseTime(v));  break;
        default:
            throw runtime_error("Error : The command is malformed.[Table::max]");
    }

    if(i_num.size() != 0){
        sort(i_num.begin(),i_num.end(),less<int>());
        min = to_string(i_num.at(0));
        max = to_string(i_num.at(i_num.size()-1));
    }
    else if( d_num.size() != 0){
        sort(d_num.begin(),d_num.end(),less<double>());
        min = to_string(d_num.at(0));
        max = to_string(d_num.at(d_num.size()-1));        
    }
    else{
        sort(values.begin(),values.end(),less<string>());
        min = values.at(0);
        max = values.at(values.size()-1);
    }
    
    
    switch(op){
        case '>':
            switch(type){
                case DATE:
                    max.insert(4,"-");
                    max.insert(7,"-");
                    return max;

                case TIME:
                    max.insert(2,":");
                    max.insert(5,":");
                    return max;

                default:
                    return max;         
            }
        case '<':
            switch(type){
                case DATE:
                    min.insert(4,"-");
                    min.insert(7,"-");
                    return min;

                case TIME:
                    min.insert(2,":");
                    min.insert(5,":");
                    return min;

                default:
                    return min;         
            }
        default:
            throw runtime_error("Error : Operation is not support[Table::myMinMax]");
    }
}

//------------------------------------------------------------------------------

string Table::Min(const string &name) const
{
    return myMinMax(name,'<');
}

//------------------------------------------------------------------------------

string Table::Max(const string &name) const
{
    return myMinMax(name,'>');
}

//------------------------------------------------------------------------------

ostream& Table::Print(ostream& os)
{

    if(columns.size()== 0 && rows.size() == 0){
        cerr << "WARNING : PRINTING EMPTY TABLE.[Table::Print]" << endl;
    }
    else{
        os << "************* TABLE INFORMATION *************" << "\n\n";


	    if(columns.size()!= 0 && rows.size() == 0){ //true if where clause didnt return any index.
		    os << "Warning : No qualified data found, Please check your Where clause.\n\n";
	    }
	    else{
		    //os << "Column size : " << getColumnSize() << "\n\n";
		    os << "--------------------------------------------\n";
		    for(auto it = columns.begin(); it != columns.end(); ++it){
			    os << it->name << "\t|";
		    }

		    os << "\n--------------------------------------------\n";
		    for(auto it = rows.begin(); it !=rows.end(); ++it){
			    for(auto entry = (*it).getValues().begin(); entry != (*it).getValues().end(); ++entry){
				    os << *entry << "\t|";
			    }
			    os << "\n";
		    }
		    os << "--------------------------------------------\n\n";
	    }

    }
    return os;
}

//------------------------------------------------------------------------------

bool Table::columnExist(const string &name) const
{
    for(const auto c : columns){
        if(c.name == name)
            return true;
    }
    return false;
}

//------------------------------------------------------------------------------

int parseDate(const string &value)
{
    int year, month, day;
    bool leapyear = false, isValidDate = false;

    if(sscanf(value.c_str(), "%d-%d-%d", &year, &month, &day) != 3){
        throw runtime_error("Error : Date format is malformed.[Table::parseDate]");
    }

                
    //check leapyear
    if ((year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0))
		leapyear = true;

    //check date range
    if(year > 1000 && year <= 2200){ //only works for reasonable year.
        if(month>0 && month <= 12){
            if(month == 2 && leapyear){
                if(day>0 && day<=29){
                    isValidDate = true;
                }
            }
            else if(month ==2 && !leapyear){
                if(day>0 && day<=28){
                    isValidDate = true;
                }
            }
            else if(month == 4 || month == 6 || month == 9 || month == 11){
                if(day>0 && day<= 30){
                    isValidDate = true;
                }
            }
            else if(day > 0 && day <= 31){
                isValidDate = true;
            }
        }
    }

    if(isValidDate) return year*10000 + month*100 + day;
    else throw runtime_error("Error : Date out of range [Table::parseDate]");
}

//------------------------------------------------------------------------------

int parseTime(const string &value)
{
    int hour, minute, second;
    if (sscanf(value.c_str(), "%d:%d:%d", &hour, &minute, &second) != 3){
        throw runtime_error("Error : Time format is malformed.[Table::parseTime]");
    }
    if((hour <= 0 || hour > 23) ||
       (minute <= 0 || minute > 59) || 
       (second <= 0 || second > 59)){
        throw runtime_error("Error : Time out of range[Table::parseTime]");
    }             

    return hour*10000 + minute*100 + second;
}

