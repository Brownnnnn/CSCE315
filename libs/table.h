    
#ifndef table_h
#define table_h

#include "record.h"
#include <string>
#include <vector>
#include <list>

class table{
private:
    // list of all records in the table
    std::list<record> recs;
    //vector of attributes the table contains
    std::list<std::string> attributes;
    // key stuff
    std::string keyAttribute = "";

public:
    //cretes an empty table with no records or attributes
    table();
    //creates a table with given attributes
    table(std::vector<std::string> attr);
    //adds an attribute (column) to the table
    void addAttribute(std::string newAttr);
    //deletes that attribute
    void deleteAttribute(std::string attribute);
    //inserts a record to the table
    void insert(record newRecord);
    //returns attributes of table
    std::vector<std::string> getAttributes() const;
    //returns size of table (# of records)
    int getSize() const;
    //creates key for table
    void makeKey(std::string key);
    // retrieve key if set, otherwise throw error
    std::string getKey() const;
    // tells if key is set
    bool keyIsSet() const;
    //combines two tables and outputs one
    table crossJoin(const table& table1, const table& table2) const;
    //combines two tables, as long as an attribute of first matches key of second
    table naturalJoin(const table& table1, const table& table2) const;
    int count(std::string attribute) const;
    record max(std::string attribute) const;
    record min(std::string attribute) const;
    std::list<record>::iterator begin();
    std::list<record>::const_iterator begin() const;
    std::list<record>::iterator end();
    std::list<record>::const_iterator end() const;
    
};

#endif /* table_h */
