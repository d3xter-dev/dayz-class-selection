modded class MissionServer
{
	ref ClassSelectionClass DayZClassSelectionClass;
	
	override void OnInit() {
		super.OnInit();
		GetClassSelection();
	}
	
	ClassSelectionClass GetClassSelection() {
		if(DayZClassSelectionClass == null) {
			DayZClassSelectionClass = new ClassSelectionClass();
		}
		
		return DayZClassSelectionClass;
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
					GetClassSelection().LoadPlayerData(identity);
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
			
				GetClassSelection().SendSyncAvailableClasses(identity);
			break;
		}
	}
	
	override void EquipCharacter(MenuDefaultCharacterData char_data)
	{	
		super.EquipCharacter(char_data);
		
		if(GetClassSelection().Utils.config.overrideEquipCharacter) {
			GetClassSelection().GiveClassEquipment(m_player);
		}
	}
	
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		super.StartingEquipSetup(player, clothesChosen);
		
		if(GetClassSelection().Utils.config.overrideStartingEquipSetup) {
			 GetClassSelection().GiveClassEquipment(m_player);
		}
	}
};