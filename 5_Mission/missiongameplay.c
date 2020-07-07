modded class MissionGameplay {
	
	ref ClassMenu m_ClassMenu;
	ref array<ref JsonClassData> m_AvailableClasses;
	
	void MissionGameplay()
	{
		GetRPCManager().AddRPC("ClassSelection", "SyncAvailableClasses", this, SingeplayerExecutionType.Client);
	}
	
	override void OnKeyPress( int key ) { 
		super.OnKeyPress( key );
		
		UIManager UIMgr = GetGame().GetUIManager();
		
		switch ( key ) {
			case KeyCode.KC_COMMA:
				if (!GetClassMenu().GetLayoutRoot().IsVisible()){
				 	UIMgr.HideDialog(); 
					UIMgr.CloseAll();
					UIMgr.ShowScriptedMenu( GetClassMenu() , NULL );
				}
				else {
					UIMgr.HideDialog(); 
					UIMgr.CloseAll();
					UIMgr.HideScriptedMenu(GetClassMenu());
				}
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
			m_ClassMenu = new ref ClassMenu(m_AvailableClasses);
			m_ClassMenu.Init();
		}

		return m_ClassMenu;
	}
}