class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\3_Game\\menu.layout" );
		return layoutRoot;
	}
	
	override void OnShow() {
		super.OnShow();
		GetGame().GetInput().ChangeGameFocus( 1 );
		
		float startX = 80;
		float startY = 70;
		
		array<string> classes = new array<string>;
		classes.Insert("Assault");
		classes.Insert("Support");
		classes.Insert("Sniper");
		
		foreach(string className: classes) {
			Widget classFrame  = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\3_Game\\class.layout", layoutRoot);
			
			ClassData classData = null;
			classFrame.GetScript(classData);
			classData.layoutRoot = classFrame;
			classData.SetClassName(className);
			
			switch(className) {
				case "Assault":
					TStringArray weapons = {"M4A1"};
					classData.SetPrimaryWeapons(weapons);
					TStringArray utilities = {"LandMineTrap"};
					classData.SetUtilities(utilities);
					break;
				case "Support":

					break;
				case "Sniper":

					break;
			}
			
			classData.Refresh();
			
			classFrame.SetPos(startX, startY);
			layoutRoot.AddChild(classFrame);
			startX += 380;
		}
	}
	
	override void OnHide() {
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
	}
	
	override bool OnClick( Widget w, int x, int y, int button ) {
		//Print(w);
		
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		//Print(target);
		//Print(eventType);
		
		return super.OnEvent(eventType, target, parameter0, parameter1);
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
		background.SetColor(COLOR_BLUE);
	}
}