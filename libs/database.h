
#ifndef database_h
#define database_h

#include "table.h"
#include <string>
#include <vector>
#include <unordered_map>

class database {
private:
    // maps table names to the table objects associated with them
    std::unordered_map<std::string, table*> tableMap;
public:
    // Creates empty database
    database();
    // destructor
    ~database();
    // copy constructor
    database(const database& db);
    // assignment operator
    database& operator=(const database& db);
    // Adds given table object to database under provided name
    void addTable(const table& newTable, std::string tableName);
    // Removes table under given name from database
    void dropTable(std::string tableName);
    // Returns list of all table names in the database
    std::vector<std::string> listTables() const;
    // Returns all tables in database
    std::vector<table> getTables() const;
    // Returns table containing queried information
    table query(std::string SELECT, std::string FROM, std::string WHERE) const;
};

#endif
