class ClassData extends UIScriptedMenu {
	
	protected string ClassName = "CLASSNAME";
	protected array<EntityAI> PrimaryWeapons;
	protected array<EntityAI> SecondaryWeapons;
	protected array<EntityAI> Utilities;
	protected array<EntityAI> Clothes;

	protected EntityAI CurrentPrimary;
	protected EntityAI CurrentSecondary;
	protected EntityAI CurrentUtility;
	
	void SetClassName(string className) {
		ClassName = className;
		
		if(layoutRoot) {
			TextWidget classNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("ClassName"));
			classNameWidget.SetText(className);
		}
	}
	
	void SetPrimaryWeapons(TStringArray weapons) {
		PrimaryWeapons.Clear();
		foreach(string weapon: weapons) {
			//Object apple = GetGame().CreateObject( "FruitApple", String2Vector("0 10 0"), false );
			//Print( apple.GetID() );
			EntityAI entity = EntityAI.Cast(null);
			entity.GetLODByName(weapon);
			PrimaryWeapons.Insert(entity);
		}
	}
	
	void SetSecondaryWeapons(TStringArray weapons) {
		SecondaryWeapons.Clear();
		foreach(string weapon: weapons) {
			EntityAI entity = EntityAI.Cast(null);
			entity.GetLODByName(weapon);
			SecondaryWeapons.Insert(entity);
		}
	}
	
	void SetUtilities(TStringArray utilities) {
		Utilities.Clear();
		foreach(string utility: utilities) {
			EntityAI entity = EntityAI.Cast(null);
			entity.GetLODByName(utility);
			Utilities.Insert(entity);
		}
	}
		
	void SetClothes(TStringArray clothes) {
		Clothes.Clear();
		foreach(string clothing: clothes) {
			EntityAI entity = EntityAI.Cast(null);
			entity.GetLODByName(clothing);
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