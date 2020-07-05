class ClassData extends UIScriptedMenu {
	
	protected string ClassName = "CLASSNAME";
	protected ref array<EntityAI> PrimaryWeapons;
	protected ref array<EntityAI> SecondaryWeapons;
	protected ref array<EntityAI> Utilities;
	protected ref array<EntityAI> Clothes;

	protected EntityAI CurrentPrimary;
	protected EntityAI CurrentSecondary;
	protected EntityAI CurrentUtility;
	
	void ClassData() {
		PrimaryWeapons = new array<EntityAI>;
		SecondaryWeapons = new array<EntityAI>;
		Utilities = new array<EntityAI>;
		Clothes = new array<EntityAI>;
	}
	
	void SetClassName(string className) {
		ClassName = className;
		
		if(layoutRoot) {
			TextWidget classNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("ClassName"));
			classNameWidget.SetText(className);
		}
	}
	
	void SetPrimaryWeapons(TStringArray weapons) {
		
		foreach(string weapon: weapons) {
			Object weaponObj = GetGame().CreateObject(weapon, Vector(0, 0, 0), true);
			EntityAI entity = EntityAI.Cast(Entity.Cast(weaponObj));
			PrimaryWeapons.Insert(entity);
		}
	}
	
	void SetSecondaryWeapons(TStringArray weapons) {
		SecondaryWeapons.Clear();
		foreach(string weapon: weapons) {
			Object weaponObj = GetGame().CreateObject(weapon, Vector(0, 0, 0), true);
			EntityAI entity = EntityAI.Cast(Entity.Cast(weaponObj));
			SecondaryWeapons.Insert(entity);
		}
	}
	
	void SetUtilities(TStringArray utilities) {
		Utilities.Clear();
		foreach(string utility: utilities) {
			Object utilityObj = GetGame().CreateObject(utility, Vector(0, 0, 0), true);
			EntityAI entity = EntityAI.Cast(Entity.Cast(utilityObj));
			Utilities.Insert(entity);
		}
	}
		
	void SetClothes(TStringArray clothes) {
		Clothes.Clear();
		foreach(string clothing: clothes) {
			Object clothingObj = GetGame().CreateObject(clothing, Vector(0, 0, 0), true);
			EntityAI entity = EntityAI.Cast(Entity.Cast(clothingObj));
			Clothes.Insert(entity);
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
		
		primaryPreview.SetItem(CurrentPrimary);
		secondaryPreview.SetItem(CurrentSecondary);
		utilityPreview.SetItem(CurrentUtility);
	}
}