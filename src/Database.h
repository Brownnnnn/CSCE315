#ifndef DATABASE_H
#define DATABASE_H
/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <map>
#include <vector>
#include <string>
#include <iostream>
/*************************************************************************
	User Defined Includes
**************************************************************************/
#include "Table.h"


class Database
{
private:
    //Tables stored in map and sorted by name
    map<string, Table> tables;
    bool tableExists(const string name);

    Table parseFrom(const string &FROM);
    map<string,vector<double>> parseSelect(const string &SELECT, Table &tbl);
    vector<size_t> parseWhere(const string &WHERE, Table &tbl);

public:
    Database();
    ~Database();
    
    //Take a single table object and a name and add that table to the database
    void addTable(string name, Table table);
    
    //Takes a table name and deletes it from the database
    void dropTable(string name);

    //List of all table names in the database
    const std::vector<string> listTableNames();

    //Return all the tables in the database
    std::map<string, Table> getTables();

    //Queries database and returns table
    const Table Query(string SELECT, string FROM, string WHERE);

};


#endif