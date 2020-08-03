typedef array<ref JsonClassItem> JsonClassItemArray;
typedef array<ref JsonClassClothing> JsonClassClothingArray;

class JsonClassData {
	string className;
	bool selected;
	
	ref JsonClassItemArray primaryItems;
	ref JsonClassItemArray secondaryItems;
	ref JsonClassItemArray utilities;
	ref JsonClassItemArray generalItems;
	ref JsonClassClothingArray clothes;
};