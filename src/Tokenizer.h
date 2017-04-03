/*
    SQL Grammer

    Expression:
    
        SubTerm
        Comma
        Or
        And
        Eof

        Expression AND Term
        Expression OR Term
        Expression , SubTerm

    SubTerm:

        Term
        +  -

        subTerm + Term
        subTerm - Term

    Term:

        Primary
        * / > < =

        Term (* / %) Primary
        Term (> < =) Primary

    Primary:

        Number
        Attribute Name
        Not Operator
        ()
        
        Not Term
        ( Expression )
    Name :
        [a-zA-z][a-zA_z_0-9]    //A letter followed by zero or more letters, underscores, Point, and digits
*/





#ifndef TOKENIZER_H
#define TOKENIZER_H

/*************************************************************************
	Standard Library Includes
**************************************************************************/
#include <string>
#include <iostream>
#include <sstream>
/*************************************************************************
	User Defined Includes
**************************************************************************/
#include "Table.h"
#include "Record.h"

using namespace std;


//denote type of token, will change to Enum if time enough
const char column_name = '8';
const char eof = '$';
const char op_and = '&';
const char op_or = 'o';
const char op_not = 'n';
const char op_comma = ',';

class Token {
public:
    char type;              //type of token
    string value;
    Token(char ch)             : type(ch), value("")   {}
    Token(char ch, string val) : type(ch), value(val) {}
};

class Token_stream {
public:
    
    // Make a Token_stream that reads from source
    Token_stream(const string &src);
    ~Token_stream();


    
    // Put token back into the token stream
    void putback(Token t);

    // Read characters from source and compose a Token
    Token getToken();
    bool getOpposed();
    const string& getResult();
    
    // Take a word and delim put in the begining of result
    // I use it to insert Attribute name and a delimiter
    void appendResult(const string &str, const string &delim);
    
    // Take a word and insert at pos
    void insertResult(size_t pos, const string word);

    void setOpposed(bool statu);
    void setResult(const string &str);


    template <typename T>
    void Compare(const T &lhs, const T &rhs, Table &tbl, const char &type, vector<Record>::const_iterator it, Token_stream &ts);
	bool visited = false;
	vector<double> temp_result;

private:
    // Is there a Token in the buffer?
    bool full;

    // Keep a Token put back using putback()
    Token buffer;

    string source;
    string result;

    //NOT operator
    bool opposed;
    
    //Remove the last char in the source string
    void popback();

    //Get the last char in the source string
    char getChar();
    
    //Construct Attribute Name or Conditional Operator(AND NOT OR)
    void composeName(string *word);


};

/* Grammer */

string Primary(Token_stream &ts, Table &tbl);
string Term(Token_stream &ts, Table &tbl);
string Subterm(Token_stream &ts, Table &tbl);
string Expression(Token_stream &ts, Table &tbl);


//Reture list of words splited by delimiter.
vector<string> split(const string &s, string delim);

//Moves duplicates to front, returning end of duplicates range
//Take a SORTED RANGE as input
template<class TIter>
TIter Duplicates(TIter begin, TIter end);

#endif