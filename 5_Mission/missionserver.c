modded class MissionServer
{
	const private string cfgPath = "$profile:";
	const private string cfgMainDir = "ClassSelection\\";
	const private string cfgClasses = "ClassSelection\\Classes\\";
	const private string cfgPlayerSaves = "ClassSelection\\PlayerSaves\\";
	
	ref array<ref JsonClassData> m_AvailableClasses;
	ref map<string, ref array<ref JsonClassSelection>> m_PlayerClasses;
	
	
	
	override void OnInit() {
		super.OnInit();
		
		if(GetGame().IsServer()){
			m_PlayerClasses = new map<string, ref array<ref JsonClassSelection>>;
			
			GetRPCManager().AddRPC("ClassSelection", "RequestSyncAvailableClasses", this);
			GetRPCManager().AddRPC("ClassSelection", "SetPlayerClass", this);
			
			if (!FileExist(cfgPath + cfgMainDir)) MakeDirectory(cfgPath + cfgMainDir);
			if (!FileExist(cfgPath + cfgClasses)) MakeDirectory(cfgPath + cfgClasses);
			if (!FileExist(cfgPath + cfgPlayerSaves)) MakeDirectory(cfgPath + cfgPlayerSaves);
			if (!FileExist(cfgPath + "ClassSelection\\ClassDataExample.json")) SaveExampleJSON();
			
			//Load Existing Classes
			 m_AvailableClasses = new array<ref JsonClassData>;
			
			string CurrentClassFileName;
			FileAttr CurrentClassFileAttr;
			
			FindFileHandle ClassFileHandle = FindFile(cfgPath + cfgClasses + "*.json", CurrentClassFileName, CurrentClassFileAttr, FindFileFlags.DIRECTORIES);
			if(CurrentClassFileName) {
				LoadClassJSON(CurrentClassFileName);
				
				while(FindNextFile(ClassFileHandle, CurrentClassFileName, CurrentClassFileAttr)) {
					LoadClassJSON(CurrentClassFileName);
				}
			}
		}
	}
	
	void LoadClassJSON(string ClassName) {
		ref JsonClassData loadedClass = new JsonClassData();
	    JsonFileLoader<JsonClassData>.JsonLoadFile(cfgPath + cfgClasses + ClassName, loadedClass);
		
		if(loadedClass.className) {
			m_AvailableClasses.Insert(loadedClass);
		}
	}
	
	void SaveExampleJSON() {
		JsonClassData example = new JsonClassData();
			
		example.className = "Assault";
		example.primaryWeapons = {
			 new JsonClassWeapon("M4A1", {"M4_RISHndgrd_Black", "M4_MPBttstck_Black", "ACOGOptic"}, {new JsonClassMagazine("Mag_STANAG_30Rnd", 5)}),
			 new JsonClassWeapon("Mosin9130", {}, {new JsonClassMagazine("Ammo_762x54", 5)}),
			 new JsonClassWeapon("Izh43Shotgun", {}, {new JsonClassMagazine("Ammo_12gaPellets", 5)})
		};
		example.secondaryWeapons = {
			 new JsonClassWeapon("MakarovIJ70", {"MakarovPBSuppressor"}, {new JsonClassMagazine("MAG_IJ70_8RND", 5)})
		};
		example.utilities = {
			 new JsonClassWeapon("LandMineTrap")
		};
		example.clothes = {
			 new JsonClassClothing("M65Jacket_Black", "GorkaPants_PautRev", "MilitaryBoots_Redpunk", "TortillaBag", "PlateCarrierComplete")
		};
		
		JsonFileLoader<JsonClassData>.JsonSaveFile(cfgPath + "ClassSelection\\ClassDataExample.json", example);
	}
	
	void RequestSyncAvailableClasses(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if( type == CallType.Server )
	    {
			SendSyncAvailableClasses(sender);
		}
	}
	
	void SendSyncAvailableClasses(PlayerIdentity player) {
		ref array<ref JsonClassData> CustomClasses =  new array<ref JsonClassData>;
		
		if(m_PlayerClasses.Contains(player.GetId())) {
			ref array<ref JsonClassSelection> playerClasses  = m_PlayerClasses.Get(player.GetId());
			
			foreach(JsonClassData copyClass: m_AvailableClasses) {
				ref JsonClassData newClass = new JsonClassData();
				newClass.className = copyClass.className;
				newClass.primaryWeapons = copyClass.primaryWeapons;
				newClass.secondaryWeapons = copyClass.secondaryWeapons;
				newClass.utilities = copyClass.utilities;
				newClass.clothes = copyClass.clothes;

				CustomClasses.Insert(newClass);
			}
			
			foreach(JsonClassData customClass: CustomClasses) {
				foreach(JsonClassSelection playerClass: playerClasses) {
					if(playerClass.className == customClass.className) {
						customClass.selected = playerClass.selected;
						
						//Check Selected Primaries
						foreach(JsonClassWeapon customClassPrimaryWeapon: customClass.primaryWeapons) {
							customClassPrimaryWeapon.selected = false;
							
							if(customClassPrimaryWeapon.name == playerClass.primary.name) {
								customClassPrimaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Secondaries
						foreach(JsonClassWeapon customClassSecondaryWeapon: customClass.secondaryWeapons) {
							customClassSecondaryWeapon.selected = false;
							
							if(customClassSecondaryWeapon.name == playerClass.secondary.name) {
								customClassSecondaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Utilities
						foreach(JsonClassWeapon customClassUtility: customClass.utilities) {
							customClassUtility.selected = false;
							
							if(customClassUtility.name == playerClass.utility.name) {
								customClassUtility.selected = true;
							}
						}
					}
				}
			}
		}
		
		
		GetRPCManager().SendRPC("ClassSelection", "SyncAvailableClasses", new Param1<ref array<ref JsonClassData>>(CustomClasses), true, player);
	}
	
	void SetPlayerClass(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<JsonClassSelection> params;
	    if ( !ctx.Read( params ) ) return;
		
	    if( type == CallType.Server )
	    {
			PlayerBase player = GetPlayerById(sender.GetPlayerId());
			
			//Update Players Custom Classes
			ref JsonClassSelection selectedClass = params.param1;
			selectedClass.selected = true;
			
			if(!m_PlayerClasses.Contains(sender.GetId())) {
				m_PlayerClasses.Set(sender.GetId(), new ref array<ref JsonClassSelection>);
			}
			
			if(m_PlayerClasses.Contains(sender.GetId())) {
				ref array<ref JsonClassSelection> playerClasses = m_PlayerClasses.Get(sender.GetId());
				
				
				bool exists = false;
				foreach(int classIndex, JsonClassSelection playerClass: playerClasses) {
					if(playerClass) {
						playerClass.selected = false;
						playerClasses.Remove(classIndex);
						
						if(selectedClass.className == playerClass.className) {
							exists  = true;
							playerClasses.InsertAt(selectedClass, classIndex);
						}
						else {
							playerClasses.InsertAt(playerClass, classIndex);
						}
					}
				}

				if(!exists) playerClasses.Insert(selectedClass);
				
				m_PlayerClasses.Set(sender.GetId(), playerClasses);
			}
			
			SendSyncAvailableClasses(sender);
			
			//Save Classes
			JsonFileLoader<array<ref JsonClassSelection>>.JsonSaveFile(cfgPath + cfgPlayerSaves + sender.GetId() + ".json", m_PlayerClasses.Get(sender.GetId()));
			
			//Force Respawn
			player.SetHealth(0);
	    }
	}
	
	PlayerBase GetPlayerById (int plyId) {
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
	
		
	void SetClothes(JsonClassClothing classData, PlayerBase player)
	{
		GetGame().ObjectDelete(player.GetHumanInventory().GetEntityInHands());
		player.RemoveAllItems();
		
		player.GetInventory().CreateInInventory(classData.top);
		player.GetInventory().CreateInInventory(classData.pants);
		player.GetInventory().CreateInInventory(classData.shoes);
		player.GetInventory().CreateInInventory(classData.backpack);
		player.GetInventory().CreateInInventory(classData.vest);
	}
	
	Weapon_Base SpawnFullWepaon(ClassWeapon weapon, PlayerBase player, bool InHands = false) {
		TStringArray attachments;
		TStringArray mags;
		Weapon_Base weaponBase;
		
		Print(weapon.GetWeapon().GetType());
		if(InHands) {
			 weaponBase = Weapon_Base.Cast(player.GetHumanInventory().CreateInHands(weapon.GetWeapon().GetType()));
		} 
		else {
			 weaponBase = Weapon_Base.Cast(player.GetInventory().CreateInInventory(weapon.GetWeapon().GetType()));
		}
		
		if(weaponBase) {
			mags =  weapon.GetMagazines();
			Magazine newMag;
			foreach(string mag: mags) {
				newMag = Magazine.Cast(player.GetInventory().CreateInInventory(mag));
			}
			
			if(GetGame().IsMultiplayer())
	        {
	            GetGame().RemoteObjectDelete(weaponBase);
	            GetGame().RemoteObjectDelete(newMag);
	        }			
	
			int mi = weaponBase.GetCurrentMuzzle();
			bool has_mag = false;
			bool has_bullet = false;
			int animationIndex = 0;
	
			//Attach Mag if possible
			if(newMag && weaponBase.CanAttachMagazine(mi, newMag)) {
				weaponBase.AttachMagazine(mi, newMag);
				pushToChamberFromAttachedMagazine(weaponBase, mi);
				has_bullet = true;
				has_mag = true;
			}
			else {
				float ammo_damage;
				string ammo_type;
					
				if(newMag && newMag.LocalAcquireCartridge(ammo_damage, ammo_type)){
					if(weaponBase.GetInternalMagazineMaxCartridgeCount(mi)) {
						while(!weaponBase.IsInternalMagazineFull(mi)) {
							weaponBase.PushCartridgeToInternalMagazine(mi, ammo_damage, ammo_type);
						}
					}
					
					for(int i = 0; i < weaponBase.GetMuzzleCount(); i++) {
						weaponBase.PushCartridgeToChamber(i, ammo_damage, ammo_type);
						has_bullet = true;
					}
					
					if(weaponBase.IsInherited(BoltActionRifle_InnerMagazine_Base)) {
						animationIndex = 1;
					}
					
					if (weaponBase.IsInherited(BoltActionRifle_InnerMagazine_Base) || weaponBase.IsInherited(DoubleBarrel_Base)) {
						has_bullet = false;
					}
				}
			}
			
			weaponBase.UpdateAnimationState(has_bullet, has_mag, animationIndex);
	
			if(GetGame().IsMultiplayer())
	        {
	            GetGame().RemoteObjectCreate(weaponBase);
	            GetGame().RemoteObjectCreate(newMag);
	        }
				
			attachments =  weapon.GetAttachments();
			foreach(string toAddAttachment: attachments) {
				weaponBase.GetInventory().CreateAttachment(toAddAttachment);
			}	
			
			return weaponBase;
		}
		
		return null;
	}

	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		super.OnEvent(eventTypeId, params);
		
		PlayerIdentity identity;
		PlayerBase player;

		switch(eventTypeId)
		{
			case ClientPrepareEventTypeID:
				ClientPrepareEventParams clientPrepareParams;
				Class.CastTo(clientPrepareParams, params);
			
				identity = clientPrepareParams.param1;
				if(identity) {
					LoadPlayerData(identity);
				}
			break;
			case ClientReadyEventTypeID:
				ClientReadyEventParams readyParams;
				Class.CastTo(readyParams, params);
				
				identity = readyParams.param1;
				Class.CastTo(player, readyParams.param2);
				if (!player)
				{
					Debug.Log("ClientReadyEvent: Player is empty");
					return;
				}
			
				SendSyncAvailableClasses(identity);
			break;
		}
	}
	
	void LoadPlayerData(PlayerIdentity identity) {
		ref array<ref JsonClassSelection> playerClasses = new array<ref JsonClassSelection>;
	    JsonFileLoader<array<ref JsonClassSelection>>.JsonLoadFile(cfgPath + cfgPlayerSaves + identity.GetId() + ".json", playerClasses);
		m_PlayerClasses.Set(identity.GetId(), playerClasses);
	}

	override void EquipCharacter()
	{	
		PlayerBase player = PlayerBase.Cast(m_player);
		ref array<ref JsonClassSelection> playerClasses = null;
		
		if(m_PlayerClasses.Contains(player.GetIdentity().GetId())) {
			playerClasses  = m_PlayerClasses.Get(player.GetIdentity().GetId());
		}
		
		if (playerClasses)
		{
			ref JsonClassSelection selectedClass = null;
			
			foreach(ref JsonClassSelection playerClass: playerClasses) {
				if(playerClass.selected) selectedClass = playerClass;
			}
			
			if(!selectedClass) selectedClass = playerClasses.Get(0);
			
			//Check if Class has Weapons and attachments available
			ref JsonClassData foundClass;
			ref ClassWeapon foundPrimary; 
			ref ClassWeapon foundSecondary; 
			ref ClassWeapon foundUtility; 
			
			foreach(JsonClassData baseClass: m_AvailableClasses) {
				if(baseClass.className == selectedClass.className) {
					foundClass = baseClass;
					
					bool validAttachments;
					
					//Check Primary
					foreach(JsonClassWeapon basePrimaryWeapon: baseClass.primaryWeapons){
						if(basePrimaryWeapon.name == selectedClass.primary.name) {
							foundPrimary = ClassWeapon.LoadFromJSON(basePrimaryWeapon);
							
							if(selectedClass.primary.attachments) {
								validAttachments = true;
								foreach(string primary_attachment: selectedClass.primary.attachments) {
									if(basePrimaryWeapon.attachments.Find(primary_attachment)) {
										validAttachments = false;
									}
								}
								
								if(validAttachments) {
									foundPrimary.SetAttachments(selectedClass.primary.attachments);
								}
							}
						}
					}
					
					//Check Secondary
					foreach(JsonClassWeapon baseSecondaryWeapon: baseClass.secondaryWeapons){
						if(baseSecondaryWeapon.name == selectedClass.secondary.name) {
							foundSecondary = ClassWeapon.LoadFromJSON(baseSecondaryWeapon);
							
							if(selectedClass.secondary.attachments) {
								validAttachments = true;
								foreach(string secondary_attachment: selectedClass.secondary.attachments) {
									if(baseSecondaryWeapon.attachments.Find(secondary_attachment)) {
										validAttachments = false;
									}
								}
								
								if(validAttachments) {
									foundSecondary.SetAttachments(selectedClass.secondary.attachments);
								}
							}
						}
					}
					
					//Check Utility
					foreach(JsonClassWeapon baseUtility: baseClass.utilities){
						if(baseUtility.name == selectedClass.utility.name) {
							foundUtility = ClassWeapon.LoadFromJSON(baseUtility);
						}
					}
				}
			}
			
			if(foundClass && foundPrimary) {
				SetClothes(foundClass.clothes.GetRandomElement(), player);
				player.SetQuickBarEntityShortcut(SpawnFullWepaon(foundPrimary, player, true), 0);
				player.SetQuickBarEntityShortcut(SpawnFullWepaon(foundSecondary, player), 1);
				player.SetQuickBarEntityShortcut(SpawnFullWepaon(foundUtility, player), 2);
			}
		}
	}
};