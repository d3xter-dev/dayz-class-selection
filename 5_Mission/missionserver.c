modded class MissionServer
{
	private string cfgPath = "$saves:";
	const private string cfgPathServer = "$profile:";

	override void OnInit() {
		super.OnInit();
		
		if(GetGame().IsServer()){
			cfgPath = cfgPathServer;
			
			if (!FileExist(cfgPath + "ClassSelection\\")) MakeDirectory(cfgPath + "\\ClassSelection\\");
			
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
	
			JsonFileLoader<JsonClassData>.JsonSaveFile(cfgPath + "ClassSelection\\ClassDataExample.json", newClassData);
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
		Magazine mag = player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
		//bool attached = gun.AttachMagazine(gun.GetCurrentMuzzle(),magazine);
		//gun.CopyWeaponStateFrom(gun);
		//gun.SyncSelectionState(true, true);
		//gun.SyncEventToRemote(new WeaponEventAttachMagazine(player, magazine));
		//player.GetWeaponManager().Update(0);
		//EntityAI magazine = player.GetInventory().CreateInInventory("Mag_STANAG_30Rnd");
		//player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventAttachMagazine( player, Magazine.Cast(magazine) ) );
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