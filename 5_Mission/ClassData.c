class ClassData extends UIScriptedMenu {
	
	protected string ClassName = "CLASSNAME";
	protected ref array<ClassWeapon> PrimaryWeapons;
	protected ref array<ClassWeapon> SecondaryWeapons;
	protected ref array<ClassWeapon> Utilities;
	protected ref array<ClassClothing> Clothes;

	protected ClassWeapon CurrentPrimary;
	protected ClassWeapon CurrentSecondary;
	protected ClassWeapon CurrentUtility;
	
	void ClassData() {
		PrimaryWeapons = new array<ClassWeapon>;
		SecondaryWeapons = new array<ClassWeapon>;
		Utilities = new array<ClassWeapon>;
		Clothes = new array<ClassClothing>;
	}
	
	void SetClassName(string className) {
		ClassName = className;
		
		if(layoutRoot) {
			TextWidget classNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("ClassName"));
			classNameWidget.SetText(className);
		}
	}
	
	void SetPrimaryWeapons(array<ClassWeapon> weapons) {
		PrimaryWeapons.Clear();
		foreach(ClassWeapon weapon: weapons) {
			PrimaryWeapons.Insert(weapon);
		}
	}
	
	void SetSecondaryWeapons(array<ClassWeapon> weapons) {
		SecondaryWeapons.Clear();
		foreach(ClassWeapon weapon: weapons) {
			SecondaryWeapons.Insert(weapon);
		}
	}
	
	void SetUtilities(array<ClassWeapon> weapons) {
		Utilities.Clear();
		foreach(ClassWeapon weapon: weapons) {
			Utilities.Insert(weapon);
		}
	}
		
	void SetClothes(array<ClassClothing> clothes) {
		Clothes.Clear();
		foreach(ClassClothing clothing: clothes) {
			Clothes.Insert(clothing);
		}
	}
	
	void EquipPlayer(){
		//ToDo
	}
	
	override void Refresh() {
		super.Refresh();
		
		ItemPreviewWidget primaryPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassPrimaryPreview"));
		ItemPreviewWidget secondaryPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassSecondaryPreview"));
		ItemPreviewWidget utilityPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassUtilityPreview"));
		
		if(!CurrentPrimary && PrimaryWeapons.Count()) {
			CurrentPrimary = PrimaryWeapons.Get(0);
		}
		
		if(!CurrentSecondary && SecondaryWeapons.Count()) {
			CurrentSecondary = SecondaryWeapons.Get(0);
		}
		
		if(!CurrentUtility && Utilities.Count()) {
			CurrentUtility = Utilities.Get(0);
		}
		
		if(CurrentPrimary) primaryPreview.SetItem(CurrentPrimary.GetWeapon());
		if(CurrentUtility) secondaryPreview.SetItem(CurrentSecondary.GetWeapon());
		if(CurrentSecondary) utilityPreview.SetItem(CurrentUtility.GetWeapon());
	}
}