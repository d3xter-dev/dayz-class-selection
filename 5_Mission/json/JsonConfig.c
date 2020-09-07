class JsonConfig {
	string version;
	string keyToOpen;
	bool giveWeaponsAfterDeath = false;
	bool showClassSelectOnRespawnOnly = false;
	ref map<string, ref TStringArray> whiteList;
}