modded class MissionGameplay {
	
	ref ClassMenu m_ClassMenu;
	ref array<ref JsonClassData> m_AvailableClasses;

	void MissionGameplay()
	{
		GetRPCManager().AddRPC("ClassSelection", "SyncAvailableClasses", this, SingeplayerExecutionType.Client);
	}
	
	override void OnKeyPress( int key ) { 
		super.OnKeyPress( key );
		switch ( key ) {
			case KeyCode.KC_COMMA:
				GetClassMenu().Toggle();
			break;
			default:
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if(m_Initialized && !IsPaused() && !HasSelectedClass() && !GetClassMenu().GetLayoutRoot().IsVisible()) {
			GetClassMenu().Toggle();
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
	
	bool HasSelectedClass() {
		if(m_AvailableClasses) {
			foreach(ref JsonClassData classData: m_AvailableClasses) {
				if(classData.selected) {
					return true;
				}
			}
		}
		
		return false;
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