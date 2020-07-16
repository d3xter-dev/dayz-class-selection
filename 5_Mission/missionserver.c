modded class MissionServer
{
	private string cfgPath = "$saves:";
	const private string cfgPathServer = "$profile:";
	
	ref array<ref JsonClassData> m_AvailableClasses;
	ref map<string, ref array<ref JsonClassSelection>> m_PlayerClasses;
	
	override void OnInit() {
		super.OnInit();
		
		if(GetGame().IsServer()){
			m_PlayerClasses = new map<string, ref array<ref JsonClassSelection>>;
			
			GetRPCManager().AddRPC("ClassSelection", "RequestSyncAvailableClasses", this);
			GetRPCManager().AddRPC("ClassSelection", "SetPlayerClass", this);
			
			cfgPath = cfgPathServer;
			
			if (!FileExist(cfgPath + "ClassSelection\\")) MakeDirectory(cfgPath + "\\ClassSelection\\");
			
			
		    m_AvailableClasses = new array<ref JsonClassData>;
			ref JsonClassData newClassData = new JsonClassData();
			
			newClassData.className = "Assault";
			newClassData.primaryWeapons = {
				 new JsonClassWeapon("M4A1", {"M4_RISHndgrd_Black", "M4_MPBttstck_Black", "ACOGOptic"}, {new JsonClassMagazine("Mag_STANAG_30Rnd", 5)}),
				 new JsonClassWeapon("Mosin9130", {}, {new JsonClassMagazine("Ammo_762x54", 5)}),
				 new JsonClassWeapon("Izh43Shotgun", {}, {new JsonClassMagazine("Ammo_12gaPellets", 5)})
			};
			newClassData.secondaryWeapons = {
				 new JsonClassWeapon("MakarovIJ70", {"MakarovPBSuppressor"}, {new JsonClassMagazine("MAG_IJ70_8RND", 5)})
			};
			newClassData.utilities = {
				 new JsonClassWeapon("LandMineTrap")
			};
			newClassData.clothes = {
				 new JsonClassClothing("M65Jacket_Black", "GorkaPants_PautRev", "MilitaryBoots_Redpunk", "TortillaBag", "PlateCarrierComplete")
			};
			
			m_AvailableClasses.Insert(newClassData);
			
			JsonFileLoader<JsonClassData>.JsonSaveFile(cfgPath + "ClassSelection\\ClassDataExample.json", newClassData);
		}
	}
	
	void RequestSyncAvailableClasses(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if( type == CallType.Server )
	    {
			SendSyncAvailableClasses(sender);
		}
	}
	
	void SendSyncAvailableClasses(PlayerIdentity player) {
		ref array<ref JsonClassData> CustomClasses = m_AvailableClasses;
		
		if(m_PlayerClasses.Contains(player.GetId())) {
			ref array<ref JsonClassSelection> playerClasses  = m_PlayerClasses.Get(player.GetId());
			
			foreach(JsonClassData customClass: CustomClasses) {
				foreach(JsonClassSelection playerClass: playerClasses) {
					if(playerClass.className == customClass.className) {
						customClass.selected = playerClass.selected;
						
						//Check Selected Primaries
						foreach(JsonClassWeapon customClassPrimaryWeapon: customClass.primaryWeapons) {
							if(customClassPrimaryWeapon.name == playerClass.primary.name) {
								customClassPrimaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Secondaries
						foreach(JsonClassWeapon customClassSecondaryWeapon: customClass.secondaryWeapons) {
							if(customClassSecondaryWeapon.name == playerClass.secondary.name) {
								customClassSecondaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Utilities
						foreach(JsonClassWeapon customClassUtility: customClass.utilities) {
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
				ref array<ref JsonClassSelection> playerClasses  = m_PlayerClasses.Get(sender.GetId());
				
				bool exsits = false;
				
				foreach(int classIndex, JsonClassSelection playerClass: playerClasses) {
					playerClass.selected = false;
					
					if(selectedClass.className == playerClass.className) {
						exsits = true;
						playerClasses.Remove(classIndex);
					}
				}
				playerClasses.Insert(selectedClass);
				
				m_PlayerClasses.Set(sender.GetId(), playerClasses);
			}
		
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
				SpawnFullWepaon(foundPrimary, player, true);
				SpawnFullWepaon(foundSecondary, player);
				SpawnFullWepaon(foundUtility, player);
			}
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
	
	void SpawnFullWepaon(ClassWeapon weapon, PlayerBase player, bool InHands = false) {
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
		}
	}

	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		super.OnEvent(eventTypeId, params);
		
		PlayerIdentity identity;
		PlayerBase player;
		
		switch(eventTypeId)
		{
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
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}
	
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override void EquipCharacter()
	{	
		EntityAI itemTop;
		EntityAI itemEnt;
		float rand;

		PlayerBase player = PlayerBase.Cast(m_player);
		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			//TODO
		}
	}
};