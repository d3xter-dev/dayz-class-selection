modded class MissionServer
{
	private string cfgPath = "$saves:";
	const private string cfgPathServer = "$profile:";
	
	ref array<ref JsonClassData> m_AvailableClasses;

	override void OnInit() {
		super.OnInit();
		
		if(GetGame().IsServer()){
			GetRPCManager().AddRPC("ClassSelection", "SetPlayerClass", this);
			
			cfgPath = cfgPathServer;
			
			if (!FileExist(cfgPath + "ClassSelection\\")) MakeDirectory(cfgPath + "\\ClassSelection\\");
			
			
		    m_AvailableClasses = new array<ref JsonClassData>;
			ref JsonClassData newClassData = new JsonClassData();
			
			newClassData.className = "Assault";
			newClassData.primaryWeapons = {
				 new JsonClassWeapon("M4A1", {"M4_RISHndgrd_Black", "M4_MPBttstck_Black", "ACOGOptic"}, {new JsonClassMagazine("Mag_STANAG_30Rnd", 5)})
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
			Print(newClassData.className);
			Print(m_AvailableClasses.Get(0).className);
			
			JsonFileLoader<JsonClassData>.JsonSaveFile(cfgPath + "ClassSelection\\ClassDataExample.json", newClassData);
		}
	}
	
	void SetPlayerClass(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<JsonClassSelection> params;
	    if ( !ctx.Read( params ) ) return;
		
	    if( type == CallType.Server )
	    {
			JsonClassSelection selectedClass = params.param1;
			
	        Print( "Server function called!" );

			PlayerBase player = GetPlayerById(sender.GetPlayerId());
			//player.RemoveAllItems();
		
			//Check if Class has Weapons and attachments available
			ref JsonClassData foundClass;
			ref ClassWeapon foundPrimary; 
			
			foreach(JsonClassData fuck_json: m_AvailableClasses) {
				if(fuck_json.className == selectedClass.className) {
					foundClass = fuck_json;
					foreach(JsonClassWeapon fuck_weapon: fuck_json.primaryWeapons){
						if(fuck_weapon.name == selectedClass.primary.name) {
							foundPrimary = ClassWeapon.LoadFromJSON(fuck_weapon);
							
							bool validAttachments = true;
							foreach(string attachment: selectedClass.primary.attachments) {
								if(fuck_weapon.attachments.Find(attachment)) {
									validAttachments = false;
								}
							}
							
							if(validAttachments) {
								foundPrimary.SetAttachments(selectedClass.primary.attachments);
							}
						}
					}
				}
			}
			
			TStringArray attachments;
			TStringArray mags;
			if(foundClass && foundPrimary) {
				Weapon_Base primary = Weapon_Base.Cast(player.GetInventory().CreateInInventory(selectedClass.primary.name));
				attachments =  foundPrimary.GetAttachments();
				foreach(string toAddAttachment: attachments) {
					primary.GetInventory().CreateAttachment(toAddAttachment);
				}
				
				mags =  foundPrimary.GetMagazines();
				foreach(string mag: mags) {
					player.GetInventory().CreateInInventory(mag);
				}
			}
			
			// check if selectedClass.param1.className in m_AvailableClasses 
			// loop through primary weapons of that class
			// if found continue to loop through all attachments if they exists for that WeaponStateBase
			// if not cancel and ban / kill / explode player.
			// Then create in inventory	
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
			
				GetRPCManager().SendRPC("ClassSelection", "SyncAvailableClasses", new Param1<ref array<ref JsonClassData>>(m_AvailableClasses), true, identity);
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

	EntityAI getItemEntWithAmount(PlayerBase player, string itemName,int amount)
	{
		EntityAI item = player.GetInventory().CreateInInventory(itemName);
		ItemBase.Cast(item).SetQuantity(amount);
		return item;
	}
	
	
	
	void getClothes(PlayerBase player)
	{
		TStringArray topsArray = {"M65Jacket_Black","M65Jacket_Khaki","M65Jacket_Tan","M65Jacket_Olive","TTsKOJacket_Camo","GorkaEJacket_Summer","GorkaEJacket_Flat","GorkaEJacket_Autumn","GorkaEJacket_PautRev","RidersJacket_Black"};
		TStringArray pantsArray = {"GorkaPants_PautRev","GorkaPants_Flat","GorkaPants_Autumn","GorkaPants_Summer","CargoPants_Blue","CargoPants_Beige","CargoPants_Black","CargoPants_Green","CargoPants_Grey","TTSKOPants","HunterPants_Autumn","HunterPants_Brown","HunterPants_Spring","HunterPants_Summer","HunterPants_Winter"};
		TStringArray shoesArray = {"MilitaryBoots_Redpunk","MilitaryBoots_Black"};
		TStringArray backpackArray  = {"TortillaBag","HuntingBag","SmershBag","AssaultBag_Ttsko","AssaultBag_Black","AssaultBag_Green","CoyoteBag_Brown","CoyoteBag_Green","AliceBag_Green","AliceBag_Black","AliceBag_Camo"};
		TStringArray vestsArray = {"PlateCarrierComplete","HighCapacityVest_Olive","HighCapacityVest_Black"};
		
		player.RemoveAllItems();

		EntityAI item = player.GetInventory().CreateInInventory(topsArray.GetRandomElement());
		EntityAI item2 = player.GetInventory().CreateInInventory(pantsArray.GetRandomElement());
		EntityAI item3 = player.GetInventory().CreateInInventory(shoesArray.GetRandomElement());
		EntityAI item4 = player.GetInventory().CreateInInventory(backpackArray.GetRandomElement());
		EntityAI item5 = player.GetInventory().CreateInInventory(vestsArray.GetRandomElement());
		
	}
	
	void getWeaponWithAttachments(PlayerBase player, string weapon) {		
		
		Weapon_Base gun = player.GetHumanInventory().CreateInHands(weapon);		
		gun.GetInventory().CreateAttachment("M4_RISHndgrd_Black");
		gun.GetInventory().CreateAttachment("M4_MPBttstck_Black");
		
		player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
		player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
		player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
		player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
	}

	override void EquipCharacter()
	{	
		
		EntityAI item = m_player.GetInventory().CreateInInventory(topsArray.GetRandomElement());
		EntityAI item2 = m_player.GetInventory().CreateInInventory(pantsArray.GetRandomElement());
		EntityAI item3 = m_player.GetInventory().CreateInInventory(shoesArray.GetRandomElement());
			
		
		EntityAI itemTop;
		EntityAI itemEnt;
		float rand;

		PlayerBase player = PlayerBase.Cast(m_player);
		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			getClothes(player);
			getWeaponWithAttachments(player,"M4A1");
			
			EntityAI rpg  =  player.GetInventory().CreateInInventory("ExpansionRPG7");
			for (int i = 0; i < 5; i++) {
				player.GetInventory().CreateInInventory("ExpansionAmmoRPG");
			}
			player.SetQuickBarEntityShortcut(rpg, 2, true);
			
			EntityAI rags  =  getItemEntWithAmount(player,"Rag", 6);
			player.SetQuickBarEntityShortcut(rags, 3, true);

			EntityAI landmine = player.GetInventory().CreateInInventory("LandMineTrap");
			player.SetQuickBarEntityShortcut(landmine, 4, true);
			landmine = player.GetInventory().CreateInInventory("LandMineTrap");
			player.SetQuickBarEntityShortcut(landmine, 5, true);
		}
	}
};