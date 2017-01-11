
#pragma once
#include "ID.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include "Logger.h"
#include "Assert.h"
#include "Resource.h"
#include <XML/rapidxml_utils.hpp>

using namespace Logger;
using namespace std;
using namespace rapidxml;

 std::vector <int> xmlValues;//variables para xml
 std::vector <int> xmlVal;
string valor;//para comprobar cada uno
static void XML(int level) {

	rapidxml::xml_document<> doc;
	std::ifstream file("niveles.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot;
	pRoot = doc.first_node();
	rapidxml::xml_node<> *pNode;


	if (level == 1) {
		pNode = pRoot->first_node("easy");
		int i = 0;
		for (rapidxml::xml_node<> *pAttr = pNode->first_node(); pAttr; pAttr = pAttr->next_sibling()) {
			xmlValues.push_back(stoi(pAttr->value()));
			i++;
		}
	}

	else if (level == 2) {

		pNode = pRoot->first_node("normal");

		for (rapidxml::xml_node<> *pAttr = pNode->first_node(); pAttr; pAttr = pAttr->next_sibling()) {
			xmlValues.push_back(stoi(pAttr->value()));
		}

	}
	else if (level == 3) {


		pNode = pRoot->first_node("hard");

		for (rapidxml::xml_node<> *pAttr = pNode->first_node(); pAttr; pAttr = pAttr->next_sibling()) {
			xmlValues.push_back(stoi(pAttr->value()));
		}
	}
}
