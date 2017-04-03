#ifndef TABLE_H
#define TABLE_H

/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <iostream>
#include <map>

/*************************************************************************
	User Defined Includes
**************************************************************************/
#include "Attribute.h"
#include "Record.h"

using namespace std;

class Table
{
private:
    //Attributes
    vector<Attribute> columns;
    //Records
    vector<Record> rows;
    //Table Keys
    vector<bool> keys;
    //Check table has particular column.
    bool columnExist(const string &name) const;
    string myMinMax(const string &str, char op) const;
public:
    Table();
    Table(vector<string> names);
    ~Table();
    //Copy constructor
    Table(const Table &table);

    //Copy assignment
    Table& operator = (const Table& table);


    //Adds a column(Attribute) to end of the table.
    void addColumn(const string &name);
    void addColumn(Attribute attr);

    //delete a column from table.
    void deleteColumn(const string &name);
    void deleteColumn(Attribute attr);

    //Adds a row(Record) to the table.
    void insertRow(Record &r);


    //Return an iterator points to first record.
    vector<Record>::const_iterator begin()const;
    
    //Return an iterator to the point after the last record.
    vector<Record>::const_iterator end()const;

    //Return the first element of rows.
    const Record& first() const;

    //Return the last element of rows.
    const Record& last() const;

    //Takes another table as input and produces combined table as output.
    Table crossJoin(Table other);
    
    //The join create one entry for each row of the first table,
    //with the additional columns from the matching key in the second table.
    Table naturalJoin(Table other);

    //Return the index of particular colum name.
    size_t getIndexByName(const string& name) const;

    //Return the number of rows in the table.
    size_t getRowSize() const;

    //Return the number of columns in the table.
    size_t getColumnSize() const;

	VAR_TYPE getColumnType(int index) const;

    //added 2017/2/25
    //Return a list of values in given column.
    vector<string> getColumnValues(int index)const;
    
    //Return a list of columns for that table.
    vector<Attribute> getColumns() const;

    //Return a list of keys for that table.
    vector<Attribute> getKeys() const;

    //Return a list of rows for that table.
    vector<Record> getRows() const ;
    
    //Assign new attributes to table.
    void setColumns(vector<Attribute> new_columns);

    //Take a set of column names that will form the key.
    void setKeys(vector<string> names);

    //Return the number of non-null value of give column.
    int Count(const string &name) const;

    //Return smallest value of give column.
    string Min(const string &name) const;

    //Return largeest value of give column.
    string Max(const string &name) const;

    //Outputs for test purpose.
    ostream& Print(ostream& os);
    
};
int parseDate(const string &value);
int parseTime(const string &value);

#endif