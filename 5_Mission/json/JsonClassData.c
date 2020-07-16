typedef array<ref JsonClassWeapon> JsonClassWeaponArray;
typedef array<ref JsonClassClothing> JsonClassClothingArray;

class JsonClassData {
	string className;
	bool selected;
	
	ref JsonClassWeaponArray primaryWeapons;
	ref JsonClassWeaponArray secondaryWeapons;
	ref JsonClassWeaponArray utilities;
	ref JsonClassClothingArray clothes;
};