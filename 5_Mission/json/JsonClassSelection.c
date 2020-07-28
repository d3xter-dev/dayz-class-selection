class JsonClassSelection {
	string className;
	bool selected = false;
	ref JsonClassItem primary;
	ref JsonClassItem secondary;
	ref JsonClassItem utility;
	
	void JsonClassSelection(string s_classname, ClassItem s_primary, ClassItem s_secondary, ClassItem s_utility) {
		className = s_classname;
		primary = s_primary.ToJSON();
		secondary = s_secondary.ToJSON();
		utility = s_utility.ToJSON();
	}
}