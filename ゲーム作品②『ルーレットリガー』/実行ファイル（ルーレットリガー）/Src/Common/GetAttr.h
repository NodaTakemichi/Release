#pragma once
#include<string>
#include"./rapidxml/rapidxml.hpp"
#include"./rapidxml/rapidxml_utils.hpp"
#include"../_debug/_DebugConOut.h"

class GetAttr
{
public:
	bool operator() (rapidxml::xml_node<>* node, std::string atrName, int& value)
	{
		if (node == nullptr)
		{
			return false;
		}

		auto atrStr = node->first_attribute(atrName.c_str());
		if (atrStr == nullptr)
		{
			return false;
		}
		value = atoi(atrStr->value());
		return  true;
	};

	bool operator() (rapidxml::xml_node<>* node, std::string atrName, float& value)
	{
		if (node == nullptr)
		{
			return false;
		}

		auto atrStr = node->first_attribute(atrName.c_str());
		if (atrStr == nullptr)
		{
			return false;
		}
		value = atoi(atrStr->value());
		return  true;
	};

	bool operator() (rapidxml::xml_node<>* node, std::string atrName, double& value)
	{
		if (node == nullptr)
		{
			return false;
		}

		auto atrStr = node->first_attribute(atrName.c_str());
		if (atrStr == nullptr)
		{
			//TRACE("アトリビュート取得エラー\n");
			return false;
		}
		value = atof(atrStr->value());
		return  true;
	};

	bool operator() (rapidxml::xml_node<>* node, std::string atrName, unsigned int& value)
	{
		if (node == nullptr)
		{
			return false;
		}

		auto atrStr = node->first_attribute(atrName.c_str());
		if (atrStr == nullptr)
		{
			//TRACE("アトリビュート取得エラー\n");
			return false;
		}
		value = atoi(atrStr->value());
		return  true;
	};

	bool operator() (rapidxml::xml_node<>* node, std::string atrName, std::string& str)
	{
		if (node == nullptr)
		{
			return false;
		}

		auto atrStr = node->first_attribute(atrName.c_str());
		if (atrStr == nullptr)
		{
			//TRACE("アトリビュート取得エラー\n");
			return false;
		}
		str = atrStr->value();
		return  true;
	};

};