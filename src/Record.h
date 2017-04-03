#ifndef RECORD_H
#define RECORD_H

/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <string>
#include <vector>

using namespace std;

class Record{
private:
    vector<string> values;
public:
    Record(vector<string> v);

    //Allows creation of a record of arbitrary size, and initialize entries to NULL
    Record(int size);
    ~Record();
    //Return the size of the record
    size_t getSize()const;

    //Return an entry by index
    string getEntry(int index)const;

    //Return record's value
    vector<string> getValues();

    //Return an iterator points to first record.
    vector<string>::const_iterator begin()const;
    
    //Return an iterator to the point after the last record.
    vector<string>::const_iterator end()const;

    //write an entry by index
    void setEntry(int index, const string &entry);

    //Takes another record as input and produces combined record as output.
    void Join(const Record &other);

    void deleteEntry(int index);

};
#endif