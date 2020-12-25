class ClassSelectionClass {
	ref ClassSelectionUtils Utils = new ClassSelectionUtils;
	
	ref array<ref JsonClassData> m_AvailableClasses;
	ref array<ref JsonClassItem> m_GerneralItems;
	ref map<string, ref array<ref JsonClassSelection>> m_PlayerClasses;
	ref array<string> m_PlayersToRespawn;
	
	void ClassSelectionClass() {
		if(GetGame().IsServer()){
			m_PlayerClasses = new map<string, ref array<ref JsonClassSelection>>;
			m_PlayersToRespawn = new array<string>;
			
			GetRPCManager().AddRPC("ClassSelection", "RequestSyncAvailableClasses", this);
			GetRPCManager().AddRPC("ClassSelection", "RequestConfig", this);
			GetRPCManager().AddRPC("ClassSelection", "SetPlayerClass", this);
		
			Utils.CreateDefaultFiles();
			
			//Load General Items
			m_GerneralItems = Utils.LoadGeneralItems();
			
			//Load Existing Classes
			m_AvailableClasses = Utils.LoadClasses();
			
			//Check Config and Version
			Utils.CheckVersion();
		}
	}
	
	void RequestSyncAvailableClasses(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if( type == CallType.Server )
	    {
			SendSyncAvailableClasses(sender);
		}
	}
	
	void RequestConfig(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		if( type == CallType.Server )
	    {
			ref JsonConfig clientConfig = new JsonConfig();
			clientConfig.keyToOpen = Utils.config.keyToOpen;
			clientConfig.giveWeaponsAfterDeath = Utils.config.giveWeaponsAfterDeath;
			clientConfig.showClassSelectOnRespawnOnly = Utils.config.showClassSelectOnRespawnOnly;
			clientConfig.version = Utils.config.version;
			
			GetRPCManager().SendRPC("ClassSelection", "SyncConfig", new Param1<ref JsonConfig>(clientConfig), true, sender);
		}
	}
	
	void SendSyncAvailableClasses(PlayerIdentity player) {
		ref array<ref JsonClassData> CustomClasses =  new array<ref JsonClassData>;
		
		foreach(JsonClassData copyClass: m_AvailableClasses) {
			if(!PlayerCanAccessClass(copyClass.className, player)) continue;
			
			ref JsonClassData newClass = new JsonClassData();
			newClass.className = copyClass.className;
			newClass.primaryItems = copyClass.primaryItems;
			newClass.secondaryItems = copyClass.secondaryItems;
			newClass.utilities = copyClass.utilities;
			newClass.clothes = copyClass.clothes;

			CustomClasses.Insert(newClass);
		}
		
		if(m_PlayerClasses.Contains(player.GetId())) {
			ref array<ref JsonClassSelection> playerClasses  = m_PlayerClasses.Get(player.GetId());
			
			foreach(JsonClassData customClass: CustomClasses) {
				foreach(JsonClassSelection playerClass: playerClasses) {
					if(playerClass.className == customClass.className) {
						customClass.selected = playerClass.selected;
						
						//Check Selected Primaries
						foreach(JsonClassItem customClassPrimaryWeapon: customClass.primaryItems) {
							customClassPrimaryWeapon.selected = false;
							
							if(customClassPrimaryWeapon.name == playerClass.primary.name) {
								customClassPrimaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Secondaries
						foreach(JsonClassItem customClassSecondaryWeapon: customClass.secondaryItems) {
							customClassSecondaryWeapon.selected = false;
							
							if(customClassSecondaryWeapon.name == playerClass.secondary.name) {
								customClassSecondaryWeapon.selected = true;
								
								//ToDo: Check Selected Attachments
							}
						}
						
						//Check Selected Utilities
						foreach(JsonClassItem customClassUtility: customClass.utilities) {
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
	
	void LoadPlayerData(PlayerIdentity identity) {
		m_PlayerClasses.Set(identity.GetId(), Utils.LoadPlayerData(identity));
	}
	
	bool PlayerCanAccessClass(string className, PlayerIdentity identity) {
		ref map<string, ref TStringArray> whiteList = Utils.GetWhiteList();
		foreach(string name, ref TStringArray players: whiteList) {
			if(name == className) {
				players = whiteList.Get(name);
				if(players.Find(identity.GetPlainId()) > -1) {
					return true;
				}
				
				return false;
			}
		}
		
		return true;
	}
	
	void SetPlayerClass(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<JsonClassSelection> params;
	    if ( !ctx.Read( params ) ) return;
		
	    if( type == CallType.Server )
	    {
			PlayerBase player = Utils.GetPlayerById(sender.GetPlayerId());
			bool PlayerHasClasses = false;
			
			//Update Players Custom Classes
			ref JsonClassSelection selectedClass = params.param1;
			selectedClass.selected = true;
			
			if(PlayerCanAccessClass(selectedClass.className, sender)) {
				if(!m_PlayerClasses.Contains(sender.GetId())) {
					m_PlayerClasses.Set(sender.GetId(), new ref array<ref JsonClassSelection>);
				}
				
				if(m_PlayerClasses.Contains(sender.GetId())) {
					ref array<ref JsonClassSelection> playerClasses = m_PlayerClasses.Get(sender.GetId());
					
					bool exists = false;
					foreach(int classIndex, JsonClassSelection playerClass: playerClasses) {
						PlayerHasClasses = true;
						
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
			}
			SendSyncAvailableClasses(sender);
			
			//Save ClassSelectionUtils
			Utils.SavePlayerClasses(m_PlayerClasses.Get(sender.GetId()), sender.GetId());
			
			//Force Respawn
			if(!Utils.config.giveWeaponsAfterDeath || !PlayerHasClasses) {
				player.SetHealth(0);
			}
	    }
	}
	
		
	void SetClothes(JsonClassClothing classData, PlayerBase player)
	{
		GetGame().ObjectDelete(player.GetHumanInventory().GetEntityInHands());
		player.RemoveAllItems();
		
		player.GetInventory().CreateInInventory(classData.top);
		player.GetInventory().CreateInInventory(classData.pants);
		player.GetInventory().CreateInInventory(classData.shoes);
		player.GetInventory().CreateInInventory(classData.gloves);
		player.GetInventory().CreateInInventory(classData.glasses);
		player.GetInventory().CreateInInventory(classData.mask);
		player.GetInventory().CreateInInventory(classData.armband);
		player.GetInventory().CreateInInventory(classData.hat);
		
		ItemBase vest = ItemBase.Cast(player.GetInventory().CreateInInventory(classData.vest));
		ItemBase backpack = ItemBase.Cast(player.GetInventory().CreateInInventory(classData.backpack));
		ItemBase belt = ItemBase.Cast(player.GetInventory().CreateInInventory(classData.belt));
		
		if(vest) {
			foreach(string vestAttachment: classData.vestAttachments) {
				vest.GetInventory().CreateAttachment(vestAttachment);
			}
		}

		if(backpack) {
			foreach(string backpackAttachment: classData.backpackAttachments) {
				backpack.GetInventory().CreateAttachment(backpackAttachment);
			}
		}
		
		if(belt) {
			foreach(string beltAttachment: classData.beltAttachments) {
				belt.GetInventory().CreateAttachment(beltAttachment);
			}
		}
	}
	
	ItemBase SpawnItem(ClassItem item, PlayerBase player, bool InHands = false, bool SkipQuantity = false, ItemBase container = null) {
		if(item) {
			ItemBase ent_Item;
			TStringArray mags = item.GetMagazines();
			EntityAI vest =  player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Vest"));
			
			if(InHands) {
				 GetGame().ObjectDelete(player.GetHumanInventory().GetEntityInHands());
				 ent_Item = ItemBase.Cast(player.GetHumanInventory().CreateInHands(item.GetItem().GetType()));
			} 
			else if(container) {
				ent_Item = ItemBase.Cast(container.GetInventory().CreateInInventory(item.GetItem().GetType()));
			}
			else {
				EntityAI shoes =  player.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Feet"));
				if(shoes && !ent_Item) {
					ent_Item = ItemBase.Cast(shoes.GetInventory().CreateInInventory(item.GetItem().GetType()));
				}
				
				if(vest && !ent_Item) {
					ent_Item = ItemBase.Cast(vest.GetInventory().CreateInInventory(item.GetItem().GetType()));
				}
				
				if(!ent_Item) {
					ent_Item = ItemBase.Cast(player.GetInventory().CreateInInventory(item.GetItem().GetType()));
				}
			}
			
			if(ent_Item) {
				if(mags && mags.Count()) {
					Weapon_Base weaponBase = Weapon_Base.Cast(ent_Item);
					
					if(weaponBase) {
						Magazine newMag;
						foreach(string mag: mags) {
							newMag = null;
							if(vest) {
								newMag = Magazine.Cast(vest.GetInventory().CreateInInventory(mag));
							}
							
							if(!newMag) {
								newMag = Magazine.Cast(player.GetInventory().CreateInInventory(mag));
							}
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
							}
						}
					
						weaponBase.UpdateAnimationState(has_bullet, has_mag, animationIndex);
				
						if(GetGame().IsMultiplayer())
				        {
				            GetGame().RemoteObjectCreate(weaponBase);
				            GetGame().RemoteObjectCreate(newMag);
				        }
					}
				}
				
				//Add attachments
				TStringArray attachments =  item.GetAttachments();
				foreach(string attachment: attachments) {
					EntityAI addedAttachment = ent_Item.GetInventory().CreateAttachment(attachment);
					if(addedAttachment) addedAttachment.GetInventory().CreateAttachment("Battery9V");
				}
				
				//If items have quantity add as much
				if(!SkipQuantity && item.GetQuantity() > 0) {
					if(ent_Item.CanBeSplit()) {
						ent_Item.SetQuantity(item.GetQuantity());
					}
					else {
						for(int q = 0; q < item.GetQuantity() - 1; q++) {
							SpawnItem(item, player, false, true, container);
						}
					}
				}	
				
				//Add cargo
				if(ent_Item.CanDisplayCargo() && item.GetCargo().Count()) {
					ref array<ref ClassItem> cargo = item.GetCargo();
					foreach(ClassItem cargoItem: cargo) {
						SpawnItem(cargoItem, player, false, false, ent_Item);
					} 
				}
				
				if(ent_Item.IsFood()) {
					return Edible_Base.Cast(ent_Item);
				}
				
				if(ent_Item.IsWeapon()) {
					return Weapon_Base.Cast(ent_Item);
				}
				
				return ent_Item;
			}
		}
			
		return null;
	}
	
	void CheckClassItem(array<ref JsonClassItem> baseItems, JsonClassItem selectedItem, out ClassItem foundItem) {
		foreach(JsonClassItem baseItem: baseItems){
			if(baseItem.name == selectedItem.name) {
				foundItem = ClassItem.LoadFromJSON(baseItem);
				
				if(selectedItem.attachments) {
					bool validAttachments = true;
					
					foreach(string attachemnt: selectedItem.attachments) {
						if(baseItem.attachments.Find(attachemnt)) {
							validAttachments = false;
						}
					}
					
					if(validAttachments) {
						foundItem.SetAttachments(selectedItem.attachments);
					}
				}
			}
		}
	}

	void GiveClassEquipment(EntityAI ent_player) {
		PlayerBase player = PlayerBase.Cast(ent_player);
		ref array<ref JsonClassSelection> playerClasses = null;
		
		if(m_PlayerClasses.Contains(player.GetIdentity().GetId())) {
			playerClasses  = m_PlayerClasses.Get(player.GetIdentity().GetId());
		}
		
		bool giveItemsToPlayer =  true;
		
		if(playerClasses && Utils.config.showClassSelectOnRespawnOnly) {
			if(m_PlayersToRespawn.Find(player.GetIdentity().GetId()) == -1) {
				giveItemsToPlayer = false;
				m_PlayerClasses.Remove(player.GetIdentity().GetId());
				m_PlayersToRespawn.Insert(player.GetIdentity().GetId());
			}
		}
		
		if (playerClasses && giveItemsToPlayer)
		{
			ref JsonClassSelection selectedClass = null;
			
			foreach(ref JsonClassSelection playerClass: playerClasses) {
				if(playerClass.selected) selectedClass = playerClass;
			}
			
			if(!selectedClass && !Utils.config.showClassSelectOnRespawnOnly) selectedClass = playerClasses.Get(0);
			
			if(selectedClass && PlayerCanAccessClass(selectedClass.className, player.GetIdentity())) {
				//Check if Class has Weapons and attachments available
				ref JsonClassData foundClass;
				ref ClassItem foundPrimary; 
				ref ClassItem foundSecondary; 
				ref ClassItem foundUtility; 
				
				foreach(JsonClassData baseClass: m_AvailableClasses) {
					if(baseClass.className == selectedClass.className) {
						foundClass = baseClass;
						
						CheckClassItem(baseClass.primaryItems, selectedClass.primary, foundPrimary);
						CheckClassItem(baseClass.secondaryItems, selectedClass.secondary, foundSecondary);
						CheckClassItem(baseClass.utilities, selectedClass.utility, foundUtility);
					}
				}
				
				if(foundClass) {
					SetClothes(foundClass.clothes.GetRandomElement(), player);
					player.SetQuickBarEntityShortcut(SpawnItem(foundPrimary, player, true), 0);
					player.SetQuickBarEntityShortcut(SpawnItem(foundSecondary, player), 1);
					player.SetQuickBarEntityShortcut(SpawnItem(foundUtility, player), 2);
					
					foreach(JsonClassItem classGeneralItem: foundClass.generalItems) {
						SpawnItem(ClassItem.LoadFromJSON(classGeneralItem), player);
					}
				}			
			}
			
			foreach(JsonClassItem generalItem: m_GerneralItems) {
				SpawnItem(ClassItem.LoadFromJSON(generalItem), player);
			}
			
			m_PlayersToRespawn.RemoveItem(player.GetIdentity().GetId());
		}
		
		SendSyncAvailableClasses(player.GetIdentity());
	}
}