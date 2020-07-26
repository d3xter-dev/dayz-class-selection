class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	private ref array<ref JsonClassData> m_AvailableClasses;
	bool selectedClass = false;
	
	void ClassMenu(){
		m_AvailableClasses = new ref array<ref JsonClassData>;
	}
	
	void SetAvailableClasses(ref array<ref JsonClassData> classes){
		m_AvailableClasses = classes;
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "d3xters-class-selection\\Scripts\\5_Mission\\layouts\\menu.layout" );
		return layoutRoot;
	}
	
	override void OnShow() {
		if(m_AvailableClasses && m_AvailableClasses.Count()) {
			super.OnShow();
			GetGame().GetInput().ChangeGameFocus( 1 );
			
			float startX = 80;
			float startY = 100;
			ClassData classData = null;
			
			
			foreach(JsonClassData jsonClassData: m_AvailableClasses) {
				ref Widget classFrame  = GetGame().GetWorkspace().CreateWidgets( "d3xters-class-selection\\Scripts\\5_Mission\\layouts\\class.layout", layoutRoot);
				
				classFrame.GetScript(classData);
				classFrame.SetPos(startX, startY);
				layoutRoot.AddChild(classFrame);
				
				classData.layoutRoot = classFrame;
				classData.LoadFromJSON(jsonClassData);
				
				if(classData.selected) {
					m_currentClass = classFrame;
				}
				
				startX += 380;
			}
			
			ChangeCurrentClass(m_currentClass);
		}
	}
	
	override void OnHide() {
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		ClassData classData = null;
			
		switch(w.GetName()) {
			case "PrimaryNext":
				w.GetParent().GetParent().GetScript(classData);
				classData.Next("primary");
				classData.Refresh();
			break;
			case "PrimaryPrev":
				w.GetParent().GetParent().GetScript(classData);
				classData.Prev("primary");
				classData.Refresh();
			break;			
			case "SecondaryNext":
				w.GetParent().GetParent().GetScript(classData);
				classData.Next("secondary");
				classData.Refresh();
			break;
			case "SecondaryPrev":
				w.GetParent().GetParent().GetScript(classData);
				classData.Prev("secondary");
				classData.Refresh();
			break;			
			case "UtilityNext":
				w.GetParent().GetParent().GetScript(classData);
				classData.Next("utility");
				classData.Refresh();
			break;
			case "UtilityPrev":
				w.GetParent().GetParent().GetScript(classData);
				classData.Prev("utility");
				classData.Refresh();
			break;
			case "SelectButton":
				SelectClass();
			break;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	
	void SelectClass() {
		if(m_currentClass) {
			ClassData data;
			m_currentClass.GetScript(data);
			GetRPCManager().SendRPC("ClassSelection", "SetPlayerClass", new Param1<ref JsonClassSelection>(data.GetSelection()));	
			selectedClass = true;
			Toggle();
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch(w.GetName()) {
			case "ClassFrame":
				ChangeCurrentClass(w);
				break;
		}
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	void ChangeCurrentClass(Widget newClass) {
		if(newClass) {
			ClassData oldClassData = null;
			ClassData newClassData = null;
			
			Widget background = newClass.FindAnyWidget("ClassFrameBackground");
		
			if(m_currentClass) {
				m_currentClass.GetScript(oldClassData);
				oldClassData.selected = !oldClassData.selected;
				m_currentClass.FindAnyWidget("ClassFrameBackground").SetColor(background.GetColor());
			}
			
			newClass.GetScript(newClassData);
			newClassData.selected = !newClassData.selected;
			m_currentClass = newClass;
			
			background.SetColor(COLOR_RED_A);
		}
	}
	
	void Toggle() {
		if(!m_AvailableClasses || !m_AvailableClasses.Count()) {
			GetRPCManager().SendRPC("ClassSelection", "RequestSyncAvailableClasses", null, true);
		}
		
		UIManager UIMgr = GetGame().GetUIManager();
		if(UIMgr) {
			UIMgr.HideDialog(); 
			UIMgr.CloseAll();
		
			if (!GetLayoutRoot().IsVisible() && m_AvailableClasses && m_AvailableClasses.Count()){
				UIMgr.ShowScriptedMenu(this , NULL );
			}
			else {
				UIMgr.HideScriptedMenu(this);
			}
		}
	}
}