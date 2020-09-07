class ClassSelectionUtils {
	static const string cfgPath = "$profile:";
	static const string cfgMainDir = "ClassSelection\\";
	static const string cfgClasses = "ClassSelection\\Classes\\";
	static const string cfgPlayerSaves = "ClassSelection\\PlayerSaves\\";
	static const string version = "v0.3";
	ref JsonConfig config;
	
	ref map<string, ref TStringArray> GetWhiteList() {
		RefreshConfig();
		return config.whiteList;
	}
	
	void RefreshConfig() {
		JsonFileLoader<JsonConfig>.JsonLoadFile(cfgPath + "ClassSelection\\Config.json", config);
	}
	
	void CheckVersion() {
		RefreshConfig();
		
		if(config) {
			string error = "";
			if(config.version != version) {
				error = "The Class-Selection mod had some important changes, check the workshop, fix your JSON files and set the version in the Config.json to " + version;
				Print(error); 
				Debug.LogError(error);
				Error(error);
				GetGame().RequestExit(IDC_MAIN_QUIT);
			}
			
			/*
			if(config.showClassSelectOnRespawn &&  config.giveWeaponsAfterDeath) {
				error = "The Class-Selection mod doesn't support showClassSelectOnRespawn and giveWeaponsAfterDeath on the same time, decide what to use and change the config!";
				Print(error); 
				Debug.LogError(error);
				Error(error);
				GetGame().RequestExit(IDC_MAIN_QUIT);
			}*/
		}
		else {
			config = SaveConfigExample();
		}
	}
	
	static void CreateDefaultFiles() {
		if (!FileExist(cfgPath + cfgMainDir)) MakeDirectory(cfgPath + cfgMainDir);
		if (!FileExist(cfgPath + cfgClasses)) {
			MakeDirectory(cfgPath + cfgClasses);
			SaveClassExampleJSON(cfgPath + cfgClasses);
		}
		if (!FileExist(cfgPath + cfgPlayerSaves)) MakeDirectory(cfgPath + cfgPlayerSaves);
		if (!FileExist(cfgPath + "ClassSelection\\ClassDataExample_"+ version +".json")) SaveClassExampleJSON(cfgPath + "ClassSelection\\");
		if (!FileExist(cfgPath + "ClassSelection\\GeneralItems.json")) SaveItemsExampleJSON(cfgPath + "ClassSelection\\");
	}
	
	static ref array<ref JsonClassItem> LoadGeneralItems() {
		ref array<ref JsonClassItem> items = new array<ref JsonClassItem>;
		JsonFileLoader<array<ref JsonClassItem>>.JsonLoadFile(cfgPath + "ClassSelection\\GeneralItems.json", items);
		return items;
	}
	
	static private ref JsonClassData LoadClassJSON(string ClassName) {
		ref JsonClassData loadedClass = new JsonClassData();
	    JsonFileLoader<JsonClassData>.JsonLoadFile(cfgPath + cfgClasses + ClassName, loadedClass);
		
		if(loadedClass.className) {
			return loadedClass;
		}
		
		return null;
	}
	
	static ref array<ref JsonClassData> LoadClasses() {
		ref array<ref JsonClassData> classes = new array<ref JsonClassData>;
		
		string CurrentClassFileName;
		FileAttr CurrentClassFileAttr;
		
		FindFileHandle ClassFileHandle = FindFile(cfgPath + cfgClasses + "*.json", CurrentClassFileName, CurrentClassFileAttr, FindFileFlags.DIRECTORIES);
		if(CurrentClassFileName) {
			classes.Insert(LoadClassJSON(CurrentClassFileName));
			
			while(FindNextFile(ClassFileHandle, CurrentClassFileName, CurrentClassFileAttr)) {
				classes.Insert(LoadClassJSON(CurrentClassFileName));
			}
		}
		
		if(classes.Count() == 0) {
			string error = "No valid Classes for Class Selection Loaded, maybe invalid JSON? Try to check with online tools.";
			Print(error); 
			Debug.LogError(error);
			Error(error);
			GetGame().RequestExit(IDC_MAIN_QUIT);
		}
		
		return classes;
	}
	
	static ref JsonConfig SaveConfigExample() {
		ref JsonConfig example = new JsonConfig();
		example.version = version;
		example.whiteList = new map<string, ref TStringArray>;
		example.whiteList["Admin"] = {"76561198160761279"};
		example.keyToOpen = "KC_COMMA";

		JsonFileLoader<ref JsonConfig>.JsonSaveFile(cfgPath + cfgMainDir + "Config.json", example);
		
		return example;
	}
	
	static void SaveItemsExampleJSON(string path) {
		array<ref JsonClassItem> example = new array<ref JsonClassItem>;
		
		example.Insert(new JsonClassItem("Rag", 6, {}, null, {}));
		example.Insert(new JsonClassItem("TacticalBaconCan", 5, {}, null, {}));
		example.Insert(new JsonClassItem("WaterBottle", 0, {}, null, {}));
		example.Insert(new JsonClassItem("HuntingKnife", 0, {}, null, {}));
		
		JsonFileLoader<array<ref JsonClassItem>>.JsonSaveFile(path + "GeneralItems.json", example);
	}
	
	static void SaveClassExampleJSON(string path) {
		JsonClassData example = new JsonClassData();
			
		example.className = "Assault";
		example.primaryItems = {
			 new JsonClassItem("M4A1", 0, {"M4_RISHndgrd_Black", "M4_MPBttstck_Black", "ACOGOptic"}, null, {new JsonClassMagazine("Mag_STANAG_30Rnd", 5)}),
			 new JsonClassItem("Mosin9130", 0, {}, null, {new JsonClassMagazine("Ammo_762x54", 5)}),
			 new JsonClassItem("Izh43Shotgun", 0, {}, null, {new JsonClassMagazine("Ammo_12gaPellets", 5)})
		};
		example.secondaryItems = {
			 new JsonClassItem("MakarovIJ70", 0, {"MakarovPBSuppressor"}, null, {new JsonClassMagazine("MAG_IJ70_8RND", 5)})
		};
		example.utilities = {
			 new JsonClassItem("LandMineTrap")
		};
		example.generalItems = {
			 new JsonClassItem("FirstAidKit", 0, null, {new JsonClassItem("SalineBagIV"), new JsonClassItem("Epinephrine")})
		};
		
		JsonClassClothing clothing  = new JsonClassClothing();
		clothing.top = "M65Jacket_Black";
		clothing.pants = "GorkaPants_PautRev";
		clothing.shoes = "MilitaryBoots_Redpunk";
		clothing.backpack = "TortillaBag";
		clothing.vest = "PlateCarrierVest";
		clothing.gloves = "TacticalGloves_Black";
		clothing.belt = "Belt";
		clothing.hat = "BallisticHelmet_UN";
		clothing.glasses = "AviatorGlasses";
		clothing.mask = "GasMask";
		clothing.armband = "Armband_Pink";
		clothing.vestAttachments = {
			"PlateCarrierHolster",
			"PlateCarrierPouches",
			"M67Grenade",
			"M67Grenade",
			"M67Grenade",
		};
		clothing.backpackAttachments = {
			"Chemlight_Blue"
		};
		
		example.clothes = {
			clothing
		};
		
		JsonFileLoader<JsonClassData>.JsonSaveFile(path + "ClassDataExample_"+ version +".json", example);
	}
	
	static ref array<ref JsonClassSelection> LoadPlayerData(PlayerIdentity identity) {
		ref array<ref JsonClassSelection> playerClasses = new array<ref JsonClassSelection>;
	    JsonFileLoader<array<ref JsonClassSelection>>.JsonLoadFile(cfgPath + cfgPlayerSaves + identity.GetId() + ".json", playerClasses);
		return playerClasses;
	}
	
	static void SavePlayerClasses(array<ref JsonClassSelection> classes, string id) {
		JsonFileLoader<array<ref JsonClassSelection>>.JsonSaveFile(cfgPath + cfgPlayerSaves + id + ".json", classes);
	}
	
	static PlayerBase GetPlayerById (int plyId) {
		array<Man> players = new array<Man>;
		PlayerBase result = NULL;
	
		if (GetGame().IsMultiplayer()) {
			GetGame().GetPlayers(players);
	
			for (int i = 0; i < players.Count(); i++) {
				if (players.Get(i).GetIdentity().GetPlayerId() == plyId) {
					result = PlayerBase.Cast(players.Get(i));
				}
			}
		} else {
			result = PlayerBase.Cast(GetGame().GetPlayer());
		}
	
		return result;
	}
	
	static int StringToKeyCode(string code) {
		switch(code) {
			case "KC_ESCAPE":
				return KeyCode.KC_ESCAPE;
			break;
			case "KC_1":
				return KeyCode.KC_1;
			break;
			case "KC_2":
				return KeyCode.KC_2;
			break;
			case "KC_3":
				return KeyCode.KC_3;
			break;
			case "KC_4":
				return KeyCode.KC_4;
			break;
			case "KC_5":
				return KeyCode.KC_5;
			break;
			case "KC_6":
				return KeyCode.KC_6;
			break;
			case "KC_7":
				return KeyCode.KC_7;
			break;
			case "KC_8":
				return KeyCode.KC_8;
			break;
			case "KC_9":
				return KeyCode.KC_9;
			break;
			case "KC_0":
				return KeyCode.KC_0;
			break;
			case "KC_MINUS":
				return KeyCode.KC_MINUS;
			break;
			case "KC_EQUALS":
				return KeyCode.KC_EQUALS;
			break;
			case "KC_BACK":
				return KeyCode.KC_BACK;
			break;
			case "KC_TAB":
				return KeyCode.KC_TAB;
			break;
			case "KC_Q":
				return KeyCode.KC_Q;
			break;
			case "KC_W":
				return KeyCode.KC_W;
			break;
			case "KC_E":
				return KeyCode.KC_E;
			break;
			case "KC_R":
				return KeyCode.KC_R;
			break;
			case "KC_T":
				return KeyCode.KC_T;
			break;
			case "KC_Y":
				return KeyCode.KC_Y;
			break;
			case "KC_U":
				return KeyCode.KC_U;
			break;
			case "KC_I":
				return KeyCode.KC_I;
			break;
			case "KC_O":
				return KeyCode.KC_O;
			break;
			case "KC_P":
				return KeyCode.KC_P;
			break;
			case "KC_LBRACKET":
				return KeyCode.KC_LBRACKET;
			break;
			case "KC_RBRACKET":
				return KeyCode.KC_RBRACKET;
			break;
			case "KC_RETURN":
				return KeyCode.KC_RETURN;
			break;
			case "KC_LCONTROL":
				return KeyCode.KC_LCONTROL;
			break;
			case "KC_A":
				return KeyCode.KC_A;
			break;
			case "KC_S":
				return KeyCode.KC_S;
			break;
			case "KC_D":
				return KeyCode.KC_D;
			break;
			case "KC_F":
				return KeyCode.KC_F;
			break;
			case "KC_G":
				return KeyCode.KC_G;
			break;
			case "KC_H":
				return KeyCode.KC_H;
			break;
			case "KC_J":
				return KeyCode.KC_J;
			break;
			case "KC_K":
				return KeyCode.KC_K;
			break;
			case "KC_L":
				return KeyCode.KC_L;
			break;
			case "KC_SEMICOLON":
				return KeyCode.KC_SEMICOLON;
			break;
			case "KC_APOSTROPHE":
				return KeyCode.KC_APOSTROPHE;
			break;
			case "KC_GRAVE":
				return KeyCode.KC_GRAVE;
			break;
			case "KC_LSHIFT":
				return KeyCode.KC_LSHIFT;
			break;
			case "KC_BACKSLASH":
				return KeyCode.KC_BACKSLASH;
			break;
			case "KC_Z":
				return KeyCode.KC_Z;
			break;
			case "KC_X":
				return KeyCode.KC_X;
			break;
			case "KC_C":
				return KeyCode.KC_C;
			break;
			case "KC_V":
				return KeyCode.KC_V;
			break;
			case "KC_B":
				return KeyCode.KC_B;
			break;
			case "KC_N":
				return KeyCode.KC_N;
			break;
			case "KC_M":
				return KeyCode.KC_M;
			break;
			case "KC_COMMA":
				return KeyCode.KC_COMMA;
			break;
			case "KC_PERIOD":
				return KeyCode.KC_PERIOD;
			break;
			case "KC_SLASH":
				return KeyCode.KC_SLASH;
			break;
			case "KC_RSHIFT":
				return KeyCode.KC_RSHIFT;
			break;
			case "KC_MULTIPLY":
				return KeyCode.KC_MULTIPLY;
			break;
			case "KC_LMENU":
				return KeyCode.KC_LMENU;
			break;
			case "KC_SPACE":
				return KeyCode.KC_SPACE;
			break;
			case "KC_CAPITAL":
				return KeyCode.KC_CAPITAL;
			break;
			case "KC_F1":
				return KeyCode.KC_F1;
			break;
			case "KC_F2":
				return KeyCode.KC_F2;
			break;
			case "KC_F3":
				return KeyCode.KC_F3;
			break;
			case "KC_F4":
				return KeyCode.KC_F4;
			break;
			case "KC_F5":
				return KeyCode.KC_F5;
			break;
			case "KC_F6":
				return KeyCode.KC_F6;
			break;
			case "KC_F7":
				return KeyCode.KC_F7;
			break;
			case "KC_F8":
				return KeyCode.KC_F8;
			break;
			case "KC_F9":
				return KeyCode.KC_F9;
			break;
			case "KC_F10":
				return KeyCode.KC_F10;
			break;
			case "KC_NUMLOCK":
				return KeyCode.KC_NUMLOCK;
			break;
			case "KC_SCROLL":
				return KeyCode.KC_SCROLL;
			break;
			case "KC_NUMPAD7":
				return KeyCode.KC_NUMPAD7;
			break;
			case "KC_NUMPAD8":
				return KeyCode.KC_NUMPAD8;
			break;
			case "KC_NUMPAD9":
				return KeyCode.KC_NUMPAD9;
			break;
			case "KC_SUBTRACT":
				return KeyCode.KC_SUBTRACT;
			break;
			case "KC_NUMPAD4":
				return KeyCode.KC_NUMPAD4;
			break;
			case "KC_NUMPAD5":
				return KeyCode.KC_NUMPAD5;
			break;
			case "KC_NUMPAD6":
				return KeyCode.KC_NUMPAD6;
			break;
			case "KC_ADD":
				return KeyCode.KC_ADD;
			break;
			case "KC_NUMPAD1":
				return KeyCode.KC_NUMPAD1;
			break;
			case "KC_NUMPAD2":
				return KeyCode.KC_NUMPAD2;
			break;
			case "KC_NUMPAD3":
				return KeyCode.KC_NUMPAD3;
			break;
			case "KC_NUMPAD0":
				return KeyCode.KC_NUMPAD0;
			break;
			case "KC_DECIMAL":
				return KeyCode.KC_DECIMAL;
			break;
			case "KC_OEM_102":
				return KeyCode.KC_OEM_102;
			break;
			case "KC_F11":
				return KeyCode.KC_F11;
			break;
			case "KC_F12":
				return KeyCode.KC_F12;
			break;
			case "KC_NUMPADEQUALS":
				return KeyCode.KC_NUMPADEQUALS;
			break;
			case "KC_PREVTRACK":
				return KeyCode.KC_PREVTRACK;
			break;
			case "KC_AT":
				return KeyCode.KC_AT;
			break;
			case "KC_COLON":
				return KeyCode.KC_COLON;
			break;
			case "KC_UNDERLINE":
				return KeyCode.KC_UNDERLINE;
			break;
			case "KC_STOP":
				return KeyCode.KC_STOP;
			break;
			case "KC_AX":
				return KeyCode.KC_AX;
			break;
			case "KC_UNLABELED":
				return KeyCode.KC_UNLABELED;
			break;
			case "KC_NEXTTRACK":
				return KeyCode.KC_NEXTTRACK;
			break;
			case "KC_NUMPADENTER":
				return KeyCode.KC_NUMPADENTER;
			break;
			case "KC_RCONTROL":
				return KeyCode.KC_RCONTROL;
			break;
			case "KC_MUTE":
				return KeyCode.KC_MUTE;
			break;
			case "KC_CALCULATOR":
				return KeyCode.KC_CALCULATOR;
			break;
			case "KC_PLAYPAUSE":
				return KeyCode.KC_PLAYPAUSE;
			break;
			case "KC_MEDIASTOP":
				return KeyCode.KC_MEDIASTOP;
			break;
			case "KC_VOLUMEDOWN":
				return KeyCode.KC_VOLUMEDOWN;
			break;
			case "KC_VOLUMEUP":
				return KeyCode.KC_VOLUMEUP;
			break;
			case "KC_WEBHOME":
				return KeyCode.KC_WEBHOME;
			break;
			case "KC_NUMPADCOMMA":
				return KeyCode.KC_NUMPADCOMMA;
			break;
			case "KC_DIVIDE":
				return KeyCode.KC_DIVIDE;
			break;
			case "KC_SYSRQ":
				return KeyCode.KC_SYSRQ;
			break;
			case "KC_RMENU":
				return KeyCode.KC_RMENU;
			break;
			case "KC_PAUSE":
				return KeyCode.KC_PAUSE;
			break;
			case "KC_HOME":
				return KeyCode.KC_HOME;
			break;
			case "KC_UP":
				return KeyCode.KC_UP;
			break;
			case "KC_PRIOR":
				return KeyCode.KC_PRIOR;
			break;
			case "KC_LEFT":
				return KeyCode.KC_LEFT;
			break;
			case "KC_RIGHT":
				return KeyCode.KC_RIGHT;
			break;
			case "KC_END":
				return KeyCode.KC_END;
			break;
			case "KC_DOWN":
				return KeyCode.KC_DOWN;
			break;
			case "KC_NEXT":
				return KeyCode.KC_NEXT;
			break;
			case "KC_INSERT":
				return KeyCode.KC_INSERT;
			break;
			case "KC_DELETE":
				return KeyCode.KC_DELETE;
			break;
			case "KC_LWIN":
				return KeyCode.KC_LWIN;
			break;
			case "KC_RWIN":
				return KeyCode.KC_RWIN;
			break;
			case "KC_APPS":
				return KeyCode.KC_APPS;
			break;
			case "KC_POWER":
				return KeyCode.KC_POWER;
			break;
			case "KC_SLEEP":
				return KeyCode.KC_SLEEP;
			break;
			case "KC_WAKE":
				return KeyCode.KC_WAKE;
			break;
			case "KC_MEDIASELECT":
				return KeyCode.KC_MEDIASELECT;
			break;
		}
		
		return -1;
	}
}