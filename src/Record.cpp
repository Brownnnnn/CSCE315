/*************************************************************************
	User defined libraries
**************************************************************************/
#include "Record.h"

Record::Record(int size)
{
    if(size < 0) throw runtime_error("ERROR : size cannot be negative.");
    vector<string>temp(size,"NULL");
    values = temp;
}
Record::~Record() { }
Record::Record(vector<string> v){
    values = v;
 };

size_t Record::getSize()const
{
    return values.size();
}

void Record::setEntry(int index, const string &entry)
{
    if (index < 0)  throw runtime_error("ERROR : Index cannot be negative [Record::setEntry]");
    else if (index >= values.size())    values.resize(index * 2);
    values.at(index) = entry;
}

string Record::getEntry(int index)const 
{
    if (index > values.size()) throw runtime_error("Error : out of bound [Record::getEntry]");
    return values[index];
}

vector<string> Record::getValues()
{
    return values;
}
//Return an iterator points to first record.
vector<string>::const_iterator Record::begin()const
{
    return values.cbegin();
}
    
//Return an iterator to the point after the last record.
vector<string>::const_iterator Record::end()const
{
    return values.cend();
}

void Record::Join(const Record &other)
{
    values.insert(values.end(), other.values.begin(), other.values.end());
}

void Record::deleteEntry(int index)
{
    values.erase(values.begin()+index);
}
