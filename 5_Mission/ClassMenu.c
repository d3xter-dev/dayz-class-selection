class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	private ref array<ref JsonClassData> m_AvailableClasses;
	
	void ClassMenu(ref array<ref JsonClassData> classes){
		m_AvailableClasses = classes;
	}
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\5_Mission\\layouts\\menu.layout" );
		return layoutRoot;
	}
	
	override void OnShow() {
		super.OnShow();
		GetGame().GetInput().ChangeGameFocus( 1 );
		
		float startX = 80;
		float startY = 70;
		
		foreach(JsonClassData jsonClassData: m_AvailableClasses) {
			Widget classFrame  = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\5_Mission\\layouts\\class.layout", layoutRoot);
			
			ClassData classData = null;
			classFrame.GetScript(classData);
			classFrame.SetPos(startX, startY);
			layoutRoot.AddChild(classFrame);
			
			classData.layoutRoot = classFrame;
			classData.LoadFromJSON(jsonClassData);
			
			startX += 380;
		}
	}
	
	override void OnHide() {
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
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
		Widget background = newClass.FindAnyWidget("ClassFrameBackground");
		
		if(m_currentClass) {
			m_currentClass.FindAnyWidget("ClassFrameBackground").SetColor(background.GetColor());
		}
		
		m_currentClass = newClass;
		background.SetColor(COLOR_RED_A);
	}
}