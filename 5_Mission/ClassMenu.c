class ClassMenu extends UIScriptedMenu {

	private ref Widget m_currentClass;
	
	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\5_Mission\\layouts\\menu.layout" );
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
		
		//General Clothing
		array<ClassClothing> general_clothings =  new array<ClassClothing>;
		ClassClothing clothing = new ClassClothing("M65Jacket_Black", "GorkaPants_PautRev", "MilitaryBoots_Redpunk", "TortillaBag", "PlateCarrierComplete");
		general_clothings.Insert(clothing);
		
		//Weapons
		ClassWeapon m4_full  = new ClassWeapon("M4A1");
		m4_full.SetAttachments({"M4_RISHndgrd_Black", "M4_MPBttstck_Black", "ACOGOptic"});
		m4_full.AddMagazines("Mag_STANAG_30Rnd", 5);
		
		ClassWeapon mp5  = new ClassWeapon("MP5K");
		mp5.SetAttachments({"MP5_PRailHndgrd", "MP5k_StockBttstck", "M68Optic", "PistolSuppressor"});
		mp5.AddMagazines("MAG_MP5_30Rnd", 5);
		
		ClassWeapon svd  = new ClassWeapon("SVD");
		svd.SetAttachments({"PSO11Optic"});
		svd.AddMagazines("Mag_SVD_10Rnd", 5);
		
		ClassWeapon makarov  = new ClassWeapon("MakarovIJ70");
		makarov.SetAttachments({"MakarovPBSuppressor"});
		makarov.AddMagazines("MAG_IJ70_8RND", 5);
		
		ClassWeapon mine  = new ClassWeapon("LandMineTrap");
		
		//Assault Class
		array<ClassWeapon> assaultPrimaries =  new array<ClassWeapon>;
		array<ClassWeapon> assaultSecondaries =  new array<ClassWeapon>;
		array<ClassWeapon> assaultUtilities =  new array<ClassWeapon>;

		assaultPrimaries.Insert(m4_full);
		assaultSecondaries.Insert(makarov);
		assaultUtilities.Insert(mine);
		
		//Support Class
		array<ClassWeapon> supportPrimaries =  new array<ClassWeapon>;
		array<ClassWeapon> supportSecondaries =  new array<ClassWeapon>;
		array<ClassWeapon> supportUtilities =  new array<ClassWeapon>;

		supportPrimaries.Insert(mp5);
		supportSecondaries.Insert(makarov);
		supportUtilities.Insert(mine);
		
		//Sniper Class
		array<ClassWeapon> sniperPrimaries =  new array<ClassWeapon>;
		array<ClassWeapon> sniperSecondaries =  new array<ClassWeapon>;
		array<ClassWeapon> snipertUtilities =  new array<ClassWeapon>;

		sniperPrimaries.Insert(svd);
		sniperSecondaries.Insert(makarov);
		snipertUtilities.Insert(mine);
		
		
		foreach(string className: classes) {
			Widget classFrame  = GetGame().GetWorkspace().CreateWidgets( "Test Mod\\Scripts\\5_Mission\\layouts\\class.layout", layoutRoot);
			
			ClassData classData = null;
			classFrame.GetScript(classData);
			classData.layoutRoot = classFrame;
			classData.SetClassName(className);
			
			switch(className) {
				case "Assault":
					classData.SetPrimaryWeapons(assaultPrimaries);
					classData.SetUtilities(assaultUtilities);
					classData.SetClothes(general_clothings);
					classData.SetSecondaryWeapons(assaultSecondaries);
					break;
				case "Support":
					classData.SetPrimaryWeapons(supportPrimaries);
					classData.SetUtilities(supportUtilities);
					classData.SetClothes(general_clothings);
					classData.SetSecondaryWeapons(supportSecondaries);
					break;
				case "Sniper":
					classData.SetPrimaryWeapons(sniperPrimaries);
					classData.SetUtilities(snipertUtilities);
					classData.SetClothes(general_clothings);
					classData.SetSecondaryWeapons(sniperSecondaries);
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