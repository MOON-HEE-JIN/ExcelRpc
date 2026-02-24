#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <list>

#define EnumToString(x) #x

enum BuildType
{
	BuildNone,
	MSVC,
	Cshap,
};

enum Node_Type
{
	Name,
	Variable,
	Array,
	Struct,
	StructArray,
	EMPTY
};

struct ENode
{
	std::string EName;
	std::string EValue;
	std::string EComment;
	bool tab;
};

struct Node
{
	Node_Type Type = Node_Type::Name;
	std::string VariableName = "";
	std::string VariableType = "";
	std::string Name = "";
	int ArraySize = 0;

	Node()
	{

	}

	Node(Node_Type _type)
	{
		Type = _type;
	}

	Node(Node_Type _type, std::string _name)
	{
		Type = _type;
		Name = _name;
	}

	Node(Node_Type _type, std::string _name, std::string _variableName)
	{
		Type = _type;
		Name = _name;
		VariableName = _variableName;
	}

	Node(Node_Type _type, std::string _name, std::string _variableName, int _size)
	{
		Type = _type;
		Name = _name;
		VariableName = _variableName;
		ArraySize = _size;
	}

};

typedef std::map<std::string, std::list<Node>>::iterator STRUCTMAP_ITER;
typedef std::map<std::string, std::list<ENode>>::iterator ENUMMAP_ITER;
typedef std::map<std::string, std::string>::iterator STRSTR_ITER;
typedef std::unordered_map<std::string, std::string>::iterator UNSTRSTR_ITER;

typedef std::list<Node>::iterator STRUCTLIST_ITER;
typedef std::list<ENode>::iterator ENUMLIST_ITER;