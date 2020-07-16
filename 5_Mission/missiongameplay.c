modded class MissionGameplay {
	
	ref ClassMenu m_ClassMenu;
	ref array<ref JsonClassData> m_AvailableClasses;
	ref array<ref Widget> m_Classes;
	
	void MissionGameplay()
	{
		m_Classes = new ref array<ref Widget>;
		GetRPCManager().AddRPC("ClassSelection", "SyncAvailableClasses", this, SingeplayerExecutionType.Client);
	}
	
	override void OnKeyPress( int key ) { 
		super.OnKeyPress( key );
		switch ( key ) {
			case KeyCode.KC_COMMA:
				m_Classes = GetClassMenu().Toggle(m_Classes);
			break;
			default:
		}
	}
	
	
	void SyncAvailableClasses(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
	{
	    Param1<array<ref JsonClassData>> classes;
	    if ( !ctx.Read( classes ) ) return;
		
	    if( type == CallType.Client )
	    {
	        Print( "Client function called!" );
	        m_AvailableClasses = classes.param1;
	    }
	}
	
	private ref ClassMenu GetClassMenu() {
		if ( !m_ClassMenu ) {
			
			if(!m_AvailableClasses.Count()) {
				GetRPCManager().SendRPC("ClassSelection", "RequestSyncAvailableClasses", null, true);
			}
			
			m_ClassMenu = new ref ClassMenu(m_AvailableClasses);
			m_ClassMenu.Init();
		}

		return m_ClassMenu;
	}
}