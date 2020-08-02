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
					GetClassSelection().Utils.RefreshConfig(); //Refresh WhiteList
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
			
				GetClassSelection().Utils.RefreshConfig();
				GetClassSelection().SendSyncAvailableClasses(identity);
			break;
		}
	}
	
	override void EquipCharacter()
	{	
		GetClassSelection().GiveClassEquipment(m_player);
	}
};