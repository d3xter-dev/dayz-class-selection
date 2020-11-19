class JsonConfig {
	string version;
	string keyToOpen;
	bool giveWeaponsAfterDeath = false;
	bool showClassSelectOnRespawnOnly = false;
	bool overrideEquipCharacter = true;
	bool overrideStartingEquipSetup = false;
	ref map<string, ref TStringArray> whiteList;
}