class JsonClassSelection {
	string className;
	bool selected = false;
	ref JsonClassWeapon primary;
	ref JsonClassWeapon secondary;
	ref JsonClassWeapon utility;
	
	void JsonClassSelection(string s_classname, ClassWeapon s_primary, ClassWeapon s_secondary, ClassWeapon s_utility) {
		className = s_classname;
		primary = s_primary.ToJSON();
		secondary = s_secondary.ToJSON();
		utility = s_utility.ToJSON();
	}
}