class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	private ref array<ref JsonClassData> m_AvailableClasses;
	private ref array<ref Widget> m_DrawnWigets;
	private int m_currentClassIndex = 1;
	private int m_classesToShow = 3;
	private int m_currentPage = 1;
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
			int count = 0;
			int page = 1;
			
			foreach(JsonClassData jsonClassData: m_AvailableClasses) {
				count++;
				
				ClassData classData = new ClassData();
				classData.LoadFromJSON(jsonClassData);
				
				if(classData.selected) {
					m_currentClassIndex = index;
					m_currentPage = page;
				}	
				
				if(count == 3) {
					page++;
					count = 0;
				}
						
				index++;
			}
			
			DrawClasses();
		}
	}
	
	private int GetPages() {
		if(m_AvailableClasses.Count() <= m_classesToShow) return 1;
		return (Math.Ceil(m_AvailableClasses.Count() / m_classesToShow);
	}
	
	private int GetCurrentPage() {
		return m_currentPage;
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
		
		int firstItem = (GetCurrentPage() * m_classesToShow) - 3; 
		int middleItem = (GetCurrentPage() * m_classesToShow) - 2;
		int lastItem = GetCurrentPage() * m_classesToShow - 1;

		if(m_AvailableClasses.Get(firstItem)) DrawClass(firstItem, 0);
		if(m_AvailableClasses.Get(middleItem)) DrawClass(middleItem, 1);
		if(m_AvailableClasses.Get(lastItem)) DrawClass(lastItem, 2);

		ChangeCurrentClass(m_currentClass);
		TextWidget PageNumber = TextWidget.Cast(GetLayoutRoot().FindAnyWidget("PageNumber"));
		PageNumber.SetText("Page: " +  GetCurrentPage() + "/" + GetPages());	
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
				m_currentPage--;
				if(m_currentPage <= 0) m_currentPage = GetPages();
				DrawClasses();
			break;
			case "ClassesNext":
				m_currentPage++;
				if(GetCurrentPage() > GetPages()){
					m_currentPage = 1;
				} 
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