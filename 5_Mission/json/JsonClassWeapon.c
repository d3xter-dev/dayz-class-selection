typedef array<ref JsonClassMagazine> JsonClassMagazineArray;

class JsonClassWeapon {
	string name;
	bool selected;
	ref TStringArray attachments;
	ref JsonClassMagazineArray magazines;
	
	void JsonClassWeapon(string w_name, ref TStringArray w_attachments = null, ref JsonClassMagazineArray w_mags = null) {
		name = w_name;
		attachments = w_attachments;
		magazines = w_mags;
	}
};