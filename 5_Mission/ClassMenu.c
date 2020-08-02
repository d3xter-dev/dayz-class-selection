class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	private ref array<ref JsonClassData> m_AvailableClasses;
	private ref array<ref Widget> m_DrawnWigets;
	private int m_currentClassIndex = 1;
	private int m_classesToShow = 3;
	bool selectedClass = false;
	
	void ClassMenu(){
		m_AvailableClasses = new ref array<ref JsonClassData>;
		m_DrawnWigets = new ref array<ref Widget>;
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
		
			int index = 0;
			foreach(JsonClassData jsonClassData: m_AvailableClasses) {
				ClassData classData = new ClassData();
				classData.LoadFromJSON(jsonClassData);
				
				if(classData.selected) {
					m_currentClassIndex = index;
				}			
				index++;
			}
			
			DrawClasses();
		}
	}
	
	private void DrawClass(int index, int position = 0) {
		float containerW, containerH, frameW, frameH;
		GetLayoutRoot().GetSize(containerW, containerH);
		
		float startX = 80;
		float startY = containerH * 0.16;
		
		ClassData classData = null;
		ref Widget classFrame  = GetGame().GetWorkspace().CreateWidgets( "d3xters-class-selection\\Scripts\\5_Mission\\layouts\\class.layout", layoutRoot);
		classFrame.GetScript(classData);
		classFrame.GetSize(frameW, frameH);
	
		if(position > 0) {
			startX += ((containerW - 160 - (frameW * 3)) / 2 + frameW) * position;
		}
		classFrame.SetPos(startX, startY);
			
		layoutRoot.AddChild(classFrame);
		
		classData.layoutRoot = classFrame;
		classData.LoadFromJSON(m_AvailableClasses.Get(index));
		classData.Refresh();
		
		if(classData.selected) {
			m_currentClass = classFrame;
		}
		
		m_DrawnWigets.Insert(classFrame);
	}
	
	private void DrawClasses() {
		foreach(Widget drawnWidget: m_DrawnWigets) {
			drawnWidget.Unlink();
		}
		m_DrawnWigets.Clear();
		
		int firstItem = m_currentClassIndex - 1; 
		int lastItem = m_currentClassIndex + 1;
		
		if(m_currentClassIndex == 0) {
			 firstItem = m_AvailableClasses.Count() - 1;
		}
		if(m_currentClassIndex == (m_AvailableClasses.Count() - 1)) {
			lastItem = 0;
		}
		
		DrawClass(firstItem, 0);
		DrawClass(m_currentClassIndex, 1);
		DrawClass(lastItem, 2);

		ChangeCurrentClass(m_currentClass);	
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
			case "ClassesPrev":
				m_currentClassIndex--;
				if(m_currentClassIndex < 0) m_currentClassIndex = m_AvailableClasses.Count() - 1;
				DrawClasses();
			break;
			case "ClassesNext":
				m_currentClassIndex++;
				if(m_currentClassIndex > (m_AvailableClasses.Count() - 1)) m_currentClassIndex = 0;
				DrawClasses();
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
			Hide();
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
		if (!GetLayoutRoot().IsVisible()){
			Show();
		}
		else {
			Hide();
		}
	}
	
	void Show() {
		if(!m_AvailableClasses || !m_AvailableClasses.Count()) {
			GetRPCManager().SendRPC("ClassSelection", "RequestSyncAvailableClasses", null, true);
		}
		
		UIManager UIMgr = GetGame().GetUIManager();
		if(UIMgr && !GetLayoutRoot().IsVisible() && m_AvailableClasses && m_AvailableClasses.Count()) {
			UIMgr.HideDialog(); 
			UIMgr.CloseAll();
			UIMgr.ShowScriptedMenu(this , NULL );
		}
	}
	
	void Hide() {
		UIManager UIMgr = GetGame().GetUIManager();
		if(UIMgr && GetLayoutRoot().IsVisible()) {
			UIMgr.HideDialog(); 
			UIMgr.CloseAll();
			UIMgr.HideScriptedMenu(this);
		}
	}
}