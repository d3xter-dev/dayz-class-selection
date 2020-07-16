class ClassData extends UIScriptedMenu {
	
	protected string ClassName = "CLASSNAME";
	protected ref array<ref ClassWeapon> PrimaryWeapons;
	protected ref array<ref ClassWeapon> SecondaryWeapons;
	protected ref array<ref ClassWeapon> Utilities;
	protected ref array<ref ClassClothing> Clothes;

	protected ClassWeapon CurrentPrimary;
	protected ClassWeapon CurrentSecondary;
	protected ClassWeapon CurrentUtility;
	
	protected int CurrentPrimaryIndex = 0;
	protected int CurrentSecondaryIndex = 0;
	protected int CurrentUtilityIndex = 0;
	
	bool selected = false;
	
	void ClassData() {
		PrimaryWeapons = new array<ref ClassWeapon>;
		SecondaryWeapons = new array<ref ClassWeapon>;
		Utilities = new array<ref ClassWeapon>;
		Clothes = new array<ref ClassClothing>;
	}
	
	void SetClassName(string className) {
		ClassName = className;
		
		if(layoutRoot) {
			TextWidget classNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("ClassName"));
			classNameWidget.SetText(className);
		}
	}
	
	void SetPrimaryWeapons(array<ref ClassWeapon> weapons) {
		PrimaryWeapons.Clear();
		foreach(ClassWeapon weapon: weapons) {
			PrimaryWeapons.Insert(weapon);
		}
	}
	
	void SetSecondaryWeapons(array<ref ClassWeapon> weapons) {
		SecondaryWeapons.Clear();
		foreach(ClassWeapon weapon: weapons) {
			SecondaryWeapons.Insert(weapon);
		}
	}
	
	void SetUtilities(array<ref ClassWeapon> weapons) {
		Utilities.Clear();
		foreach(ClassWeapon weapon: weapons) {
			Utilities.Insert(weapon);
		}
	}
		
	void SetClothes(array<ref ClassClothing> clothes) {
		Clothes.Clear();
		foreach(ClassClothing clothing: clothes) {
			Clothes.Insert(clothing);
		}
	}
	
	void Next(string category) {
		int maxIndex = 0;
		switch(category) {
			case "primary":
				maxIndex = PrimaryWeapons.Count() - 1;
				if(CurrentPrimaryIndex != maxIndex) {
					CurrentPrimaryIndex++;
				}
				else {
					CurrentPrimaryIndex = 0;
				}
			break;
			case "secondary":
				maxIndex = SecondaryWeapons.Count() - 1;
				if(CurrentSecondaryIndex != maxIndex) {
					CurrentSecondaryIndex++;
				}
				else {
					CurrentSecondaryIndex = 0;
				}
			break;
			case "utiltiy":
				maxIndex = Utilities.Count() - 1;
				if(CurrentUtilityIndex != maxIndex) {
					CurrentUtilityIndex++;
				}
				else {
					CurrentUtilityIndex = 0;
				}
			break;
		}
	}
	
		
	void Prev(string category) {
		int maxIndex = 0;
		switch(category) {
			case "primary":
				maxIndex = PrimaryWeapons.Count() - 1;
				if(CurrentPrimaryIndex != 0) {
					CurrentPrimaryIndex--;
				}
				else {
					CurrentPrimaryIndex = maxIndex;
				}
			break;
			case "secondary":
				maxIndex = SecondaryWeapons.Count() - 1;
				if(CurrentSecondaryIndex != 0) {
					CurrentSecondaryIndex--;
				}
				else {
					CurrentSecondaryIndex = maxIndex;
				}
			break;
			case "utiltiy":
				maxIndex = Utilities.Count() - 1;
				if(CurrentUtilityIndex != 0) {
					CurrentUtilityIndex--;
				}
				else {
					CurrentUtilityIndex = maxIndex;
				}
			break;
		}
	}
	
	override void Refresh() {
		super.Refresh();
		
		ItemPreviewWidget primaryPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassPrimaryPreview"));
		ItemPreviewWidget secondaryPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassSecondaryPreview"));
		ItemPreviewWidget utilityPreview =  ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ClassUtilityPreview"));
		
		if(PrimaryWeapons.Count()) {
			if(CurrentPrimary) CurrentPrimary.selected = !CurrentPrimary.selected;
			CurrentPrimary = PrimaryWeapons.Get(CurrentPrimaryIndex);
			CurrentPrimary.selected = !CurrentPrimary.selected;
		}
		
		if(SecondaryWeapons.Count()) {
			if(CurrentSecondary) CurrentSecondary.selected = !CurrentSecondary.selected;
			CurrentSecondary = SecondaryWeapons.Get(CurrentSecondaryIndex);
			CurrentSecondary.selected = !CurrentSecondary.selected;
		}
		
		if(Utilities.Count()) {
			if(CurrentUtility) CurrentUtility.selected = !CurrentUtility.selected;
			CurrentUtility = Utilities.Get(CurrentUtilityIndex);
			CurrentUtility.selected = !CurrentUtility.selected;
		}
		
		if(CurrentPrimary) primaryPreview.SetItem(CurrentPrimary.GetWeapon());
		if(CurrentUtility) secondaryPreview.SetItem(CurrentSecondary.GetWeapon());
		if(CurrentSecondary) utilityPreview.SetItem(CurrentUtility.GetWeapon());
	}
	
	void LoadFromJSON(JsonClassData data) {
		SetClassName(data.className);
		selected = data.selected;
		
		ClassWeapon newWeapon;
		array<ref ClassWeapon> primaryWeapons = new array<ref ClassWeapon>();
		foreach(JsonClassWeapon jsonWp: data.primaryWeapons) {
			newWeapon = ClassWeapon.LoadFromJSON(jsonWp);
			primaryWeapons.Insert(newWeapon);
			
			if(newWeapon.selected) {
				CurrentPrimaryIndex = primaryWeapons.Count() - 1;
			}
		}
		
		array<ref ClassWeapon> secondaryWeapons = new array<ref ClassWeapon>;
		foreach(JsonClassWeapon jsonWs: data.secondaryWeapons) {
			newWeapon = ClassWeapon.LoadFromJSON(jsonWs);
			secondaryWeapons.Insert(newWeapon);
			
			if(newWeapon.selected) {
				CurrentSecondaryIndex = secondaryWeapons.Count() - 1;
			}
		}
		
		array<ref ClassWeapon> utilities = new array<ref ClassWeapon>;
		foreach(JsonClassWeapon jsonU: data.utilities) {
			newWeapon = ClassWeapon.LoadFromJSON(jsonU);
			utilities.Insert(newWeapon);
			
			if(newWeapon.selected) {
				CurrentUtilityIndex = utilities.Count() - 1;
			}
		}
		
		array<ref ClassClothing> clothings = new array<ref ClassClothing>;
		foreach(JsonClassClothing jsonC: data.clothes) {
			clothings.Insert(new ClassClothing(jsonC.top, jsonC.pants, jsonC.shoes, jsonC.backpack, jsonC.vest));
		}
		
		SetPrimaryWeapons(primaryWeapons);
		SetSecondaryWeapons(secondaryWeapons);
		SetUtilities(utilities);
		SetClothes(clothings);
		
		Refresh();
	}
	
	JsonClassSelection GetSelection() {
		return new JsonClassSelection(ClassName, CurrentPrimary, CurrentSecondary, CurrentUtility);
	}
}