#include "Tokenizer.h"

#include <algorithm>
#include <cctype>



//------------------------------------------------------------------------------

Token_stream::Token_stream(const string &src) :
    full(false), buffer(' '), result(""), 
    opposed(false),source(src.crbegin(),src.crend()) { }

//------------------------------------------------------------------------------

Token_stream::~Token_stream() { }

//------------------------------------------------------------------------------

void Token_stream::putback(Token t)
{
    if (full) throw runtime_error("putback() into a full buffer");
    buffer = t;
    full = true;
}

//------------------------------------------------------------------------------

void Token_stream::popback()
{
    source.pop_back();
}

//------------------------------------------------------------------------------

char Token_stream::getChar()
{
    return source.at(source.size()-1);
}

//------------------------------------------------------------------------------

void Token_stream::composeName(string *word)
{
    char ch;

    while(source.size()){
        //get first char
        ch = getChar();

        // letters digits and underscores
        if(isalpha(ch) || isdigit(ch) || ch == '.' || ch == '_' || ch == '\''){
            *word += ch;
            popback();
        }
        else
            break;
    }
}

//------------------------------------------------------------------------------

// read characters from source and compose a Token
Token Token_stream::getToken()
{
    //"If we already have a Token ready
    if (full) {
        full=false;
        return buffer;
    }
    size_t sz = source.size();

    if(sz <= 0) return Token(eof);

    char ch = getChar();

    //ingore white space.
    while(ch == ' '){
        if(sz == 1) break;   //handle ending spaces
        popback();
        ch = getChar();
    }

    //compose Token.
    switch (ch) {
        case eof: case ';': case ' ':
        case '>': case '<': case '=': case ',':
        case '(': case ')': case '+':
        case '-': case '*': case '/': case '%':
            popback();
            return Token(ch); // let each character represent itself
        default:
            if (isalpha(ch) || isdigit(ch) || ch == '\'') {	// start with a letter

                string word{ch};
                popback();
                composeName(&word);

                if      (word == "AND") return Token(op_and,word);
                else if (word == "OR")  return Token(op_or,word);
                else if (word == "NOT") return Token(op_not,word);
                else return Token(column_name,word);
            }
            throw runtime_error("Bad token");
    }
}

//------------------------------------------------------------------------------

const string& Token_stream::getResult()
{
    return result;
}

//------------------------------------------------------------------------------

void Token_stream::setResult(const string &str)
{
    result = str;
}

//------------------------------------------------------------------------------

void Token_stream::appendResult(const string &str, const string &delim)
{
    result += (str + delim);
}

//------------------------------------------------------------------------------

void Token_stream::insertResult(size_t pos, const string word)
{
    result.insert(pos,word);
}

//------------------------------------------------------------------------------

void Token_stream::setOpposed(bool statu)
{
    opposed = statu;
}

//------------------------------------------------------------------------------

bool Token_stream::getOpposed()
{
    return opposed;
}

//------------------------------------------------------------------------------






/* Grammer */

//------------------------------------------------------------------------------


// deal with string and parentheses
string Primary(Token_stream &ts,Table &tbl)
{
    Token t = ts.getToken();
    switch (t.type) {
        case '(':
        {
            string r = Subterm(ts,tbl);
            t = ts.getToken();
            if (t.type != ')') throw runtime_error("')' expected");
            return r;
        }


        case column_name:
            return t.value;

        case op_not:
            ts.setOpposed(true);
            return Term(ts,tbl);

        //unary operatior will handle if I have time..
        // case '+':
        //     return - Primary(ts,tbl);

        // case '-':
        //     return Primary(ts,tbl);

        default:
            throw runtime_error("Primary expected");
    }
}

//------------------------------------------------------------------------------

string Term(Token_stream &ts,Table &tbl)
{
    string left = Primary(ts,tbl);
    Token t = ts.getToken();

    while(true) {
        switch (t.type) {
            case ';': return ts.getResult();

            case '*' : case '/' :
            {

	            string right = Primary(ts,tbl);

	            if(isdigit(left[0])){
					switch(t.type){
						case '*' : left = to_string(stod(left) * stod(right)); break;
						case '/' : left = to_string(stod(left) / stod(right)); break;
					}
	            }
	            else{
		            size_t index = tbl.getIndexByName(left);


	                double rhs = stod(right);
                    if(rhs == 0 && t.type != '*') throw runtime_error("Error : divided by zero. [Term]");

	                if(ts.visited){ //handle multiple operation in expression
			            string str;
			            for(auto &r : ts.temp_result){
                            if(t.type == '*')       r *= rhs;
                            else if(t.type == '/')  r /= rhs;

				            str += (to_string(r) + " ");
			            }
			            ts.setResult(str);
	                }
	                else{           //only one operation
			            for(auto it = tbl.begin(); it != tbl.end(); ++it){
				            double lhs = stod(it->getEntry(index));
                            if(t.type == '*')       lhs *= rhs;
                            else if(t.type == '/')  lhs /= rhs;

				            ts.appendResult(to_string(lhs)," ");
				            ts.temp_result.push_back(lhs);
			            }
			            ts.visited = true;
		            }
	                ts.insertResult(0,left+",");
		        }
	            t = ts.getToken();
	            break;
            }

            case '>': case '<' : case '=':
            {
                size_t index = tbl.getIndexByName(left);
				VAR_TYPE type = tbl.getColumnType(index);

	            string right = Primary(ts,tbl);
	            for(auto it = tbl.begin(); it != tbl.end(); ++it){
		            switch(type){
			            case INT : case INTEGER :
			            {
				            auto lhs = stoi(it->getEntry(index));
				            auto rhs = stoi(right);
				            ts.Compare(lhs,rhs,tbl,t.type,it,ts);
				            break;
			            }

			            case FLOAT : case REAL :
			            {
				            auto lhs = stod(it->getEntry(index));
				            auto rhs = stod(right);
				            ts.Compare(lhs,rhs,tbl,t.type,it,ts);
				            break;
			            }
			            case CHAR : case VARCHAR :
			            {
				            auto lhs = it->getEntry(index);
				            auto rhs = right;
				            ts.Compare(lhs,rhs,tbl,t.type,it,ts);
				            break;
			            }
			            case DATE :
			            {
				            auto lhs = parseDate(it->getEntry(index));
				            auto rhs = stoi(right);
				            ts.Compare(lhs,rhs,tbl,t.type,it,ts);
				            break;
			            }

			            case TIME :
			            {
				            auto lhs = parseTime(it->getEntry(index));
				            auto rhs = stoi(right);
				            ts.Compare(lhs,rhs,tbl,t.type,it,ts);
				            break;
			            }
			            case NONE :
				            throw runtime_error("Error : You should set Attribute type before comparison");
		            }
                }
                ts.setOpposed(false);
                t = ts.getToken();
                break;
            }

            default:
                ts.putback(t);
		        return left;
        }
    }
}

//------------------------------------------------------------------------------

string Subterm(Token_stream &ts, Table &tbl)
{
    string left = Term(ts,tbl);
    Token t = ts.getToken();

    while(true) {
        switch (t.type) {
            //TODO : string '+' operation
            //TODO : TIME and Date '+ -' operation and validation.

            case '+': case '-' :    //Not it only work for numeric value
            {
                size_t index = tbl.getIndexByName(left);
                string right = Term(ts,tbl);
                double rhs = stod(right);


                if(ts.visited){     //handle multiple operation in expression
                    string str;
                    for(auto &r : ts.temp_result){
	                    if(t.type == '+')   r += rhs;
		                else                r -= rhs;
	                    str += (to_string(r) + " ");
                    }
                    ts.setResult(str);
                }
                else{               //only one operation
                    for(auto it = tbl.begin(); it != tbl.end(); ++it){
                        double lhs = stod(it->getEntry(index));
                        if(t.type == '+') lhs += rhs;
                        else              lhs -= rhs;

                        ts.appendResult(to_string(lhs)," ");
                        //TODO : This could add a check condition to simplfied.
                        ts.temp_result.push_back(lhs);
                    }
	                ts.visited = true;
                }

                ts.insertResult(0,left+",");

                t = ts.getToken();
                break;
            }
            default:
                ts.putback(t);
                if(ts.getResult() == "") return left;
                else return ts.getResult();
        }
    }
}

//------------------------------------------------------------------------------

string Expression(Token_stream &ts, Table &tbl)
{
    string left = Subterm(ts,tbl); 
    Token t = ts.getToken();

    while(true) {
        switch(t.type) {
	        case op_comma:
            {
                //clear and replace with new result
	            ts.setResult("");
                string right = Subterm(ts,tbl);
                left += (";" + right);
	            ts.setResult(left);

                t = ts.getToken();
                break;
            }

            case op_or:
            {
                string right = Term(ts,tbl);

                //convert a line of index into individual one
                vector<int>index;
                vector<string> temp = split(ts.getResult()," ");
                for(auto i : temp)
                    index.push_back(stoi(i));
                
                //sort and erase duplicate element
                sort(index.begin(),index.end());
                auto end_unique = unique(index.begin(),index.end());
                index.erase(end_unique, index.end());

                //use new indexs to replace old one
                ts.setResult("");
                for(auto c : index)
                    ts.appendResult(to_string(c)," ");

                t=ts.getToken();
                break;
            }

            case op_and:
            {
	            string right = Term(ts,tbl);
	            
                //convert a line of index into individual one
	            vector<int>index;
                vector<string> temp = split(ts.getResult()," ");
	            for(auto i : temp)
		            index.push_back(stoi(i));
	            
                //sort and erase unique element
	            sort(index.begin(),index.end());
	            auto end_duplicate =  Duplicates(index.begin(),index.end());
	            if(end_duplicate == index.begin()){
		            ts.setResult("");
		            left = "";
		            t = ts.getToken();
		            break;
	            }
	            index.erase(end_duplicate,index.end());
	            ts.setResult("");
                //use new indexs to replace old one
	            for(const auto &c : index)
		            ts.appendResult(to_string(c)," ");
	            
	            t=ts.getToken();
	            break;
            }

            case eof:
                if(ts.getResult() == "") return left;
                else return ts.getResult();

            default:
                ts.putback(t);
                if(ts.getResult() == "") return left;
                else return ts.getResult();
        }
    }
}

//------------------------------------------------------------------------------

vector<string> split(const string &s, string delim)
{
    stringstream scan (s);
    string line;
    vector<string> wordVector;

    while(getline(scan,line))
    {
        std:size_t prev = 0, pos;
        while ((pos = line.find_first_of(delim, prev)) != string::npos)
        {
            if (pos > prev)
                wordVector.push_back(line.substr(prev, pos-prev));
            prev = pos+1;
        }
        if (prev < line.length())
            wordVector.push_back(line.substr(prev, string::npos));
    }
    return wordVector;
}

//------------------------------------------------------------------------------

template <typename T>
void Token_stream::Compare(const T &lhs, const T &rhs, Table &tbl, const char &type, vector<Record>::const_iterator it, Token_stream &ts)
{
	if((getOpposed() &&
	    ((type == '>' && lhs <= rhs) ||
	     (type == '=' && lhs != rhs) ||
	     (type == '<' && lhs >= rhs))))
			ts.appendResult(to_string(it - tbl.begin())," ");

	else if ((type == '>' && lhs > rhs) ||
	         (type == '=' && lhs == rhs) ||
		     ((type == '<') && (lhs < rhs)))
				ts.appendResult(to_string(it - tbl.begin())," ");
}

//------------------------------------------------------------------------------

template<class TIter>

TIter Duplicates(TIter begin, TIter end) {
	TIter dup = begin;
	for (TIter it = begin; it != end; ++it) {
		TIter next = it;
		++next;
		TIter const miss = std::mismatch(next, end, it).second;
		if (miss != it) {
			*dup++ = *miss;
			it = miss;
		}
	}
	return dup;
}

//------------------------------------------------------------------------------
