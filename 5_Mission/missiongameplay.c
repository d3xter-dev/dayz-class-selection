modded class MissionGameplay {
	
	ref ClassMenu m_ClassMenu;
	
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
	
	
	private ref ClassMenu GetClassMenu() {
		if ( !m_ClassMenu ) {
			m_ClassMenu = new ref ClassMenu;
			m_ClassMenu.Init();
		}

		return m_ClassMenu;
	}
}