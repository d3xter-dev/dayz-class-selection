modded class MissionGameplay {
	
	ref ClassMenu m_ClassMenu;
	ref array<ref JsonClassData> m_AvailableClasses;
	int m_KeyToOpen = KeyCode.KC_COMMA;

	void MissionGameplay()
	{
		GetRPCManager().SendRPC("ClassSelection", "RequestKeyToOpen", null, true);
		GetRPCManager().AddRPC("ClassSelection", "SyncAvailableClasses", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ClassSelection", "SyncKeyToOpen", this, SingeplayerExecutionType.Client);
	}
	
	override void OnKeyPress( int key ) { 
		switch ( key ) {
			case m_KeyToOpen:
				GetClassMenu().Toggle();
			break;
			default:
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if(m_Initialized  && !IsPaused() && player && player.IsPlayerLoaded() && !IsPlayerRespawning() && player.IsAlive()) {
			if(!HasSelectedClass()) {
				GetClassMenu().Show();
			}
		}
		else {
			GetClassMenu().Hide();
		}
	}
	
	void SyncAvailableClasses(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
	    Param1<array<ref JsonClassData>> classes;
	    if ( !ctx.Read( classes ) ) return;
		
	    if( type == CallType.Client )
	    {
	        Print( "ClassSelection - Sync Classes!" );
	        m_AvailableClasses = classes.param1;
	    }
	}
	
	void SyncKeyToOpen(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
	    Param1<int> KeyToOpen;
	    if ( !ctx.Read( KeyToOpen ) ) return;
		
	    if( type == CallType.Client )
	    {
	        Print( "ClassSelection - Sync Key!" );
	        m_KeyToOpen = KeyToOpen.param1;
	    }
	}
	
	bool HasSelectedClass() {
		if(m_AvailableClasses) {
			foreach(ref JsonClassData classData: m_AvailableClasses) {
				if(classData.selected) {
					return true;
				}
			}
		}
		
		return GetClassMenu().selectedClass;
	}
	
	private ref ClassMenu GetClassMenu() {
		if ( !m_ClassMenu ) {
			m_ClassMenu = new ref ClassMenu();
			m_ClassMenu.Init();
		}
		
		m_ClassMenu.SetAvailableClasses(m_AvailableClasses);

		return m_ClassMenu;
	}
}