
/*************************************************************************
	User defined libraries
**************************************************************************/
#include "Attribute.h"

Attribute::Attribute(VAR_TYPE t, const string &n) : type(t), name(n) { }
Attribute::Attribute(const string &n) : type(NONE),name(n){}
