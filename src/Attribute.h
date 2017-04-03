#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <cctype>
#include <string>


using namespace std;

enum VAR_TYPE {
    NONE,
    INT,
    INTEGER,
    FLOAT,
    REAL,
    CHAR,
    VARCHAR,
    DATE,
    TIME
};

struct Attribute
{
    string name;
    VAR_TYPE type;

    Attribute(VAR_TYPE t, const string &n);
    Attribute(const string &n);
};
#endif