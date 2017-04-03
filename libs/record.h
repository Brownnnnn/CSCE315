
#ifndef record_h
#define record_h

#include <string>
#include <unordered_map>
#include <vector>

class record {
	
private:
	// Map attribute name to it's value.
	std::unordered_map<std::string, std::string> data;

public:
	// Default constructor.
	record();
	// Constructor that initializes attributes in map to empty
	record(std::vector<std::string> attributes);
	// Extra constructor to initialize attributes and their values.
	record(std::vector<std::string> attributes, std::vector<std::string> values);
	// Returns the number of entries in record.
	int getSize() const;
	// get attributes
	std::vector<std::string> getAttributes() const; // FIXME may need to remove and use friend class
	// Sets the entry of the data map for a certain attribute.
	void setData(std::string attr, std::string newVal);
	// Adds entry to record
	void addData(std::string newAttr, std::string newVal);
	// Returns the string stored in the data map for given attribute.
	std::string getData(std::string attr) const;
	// removes field for given attribute
	void removeField(std::string attr);
};

#endif
