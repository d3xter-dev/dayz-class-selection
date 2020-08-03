class ClassData extends UIScriptedMenu {
	
	protected string ClassName = "CLASSNAME";
	protected ref array<ref ClassItem> PrimaryItems;
	protected ref array<ref ClassItem> SecondaryItems;
	protected ref array<ref ClassItem> Utilities;
	protected ref array<ref ClassItem> GeneralItems;
	protected ref array<ref ClassClothing> Clothes;

	protected ClassItem CurrentPrimary;
	protected ClassItem CurrentSecondary;
	protected ClassItem CurrentUtility;
	
	protected int CurrentPrimaryIndex = 0;
	protected int CurrentSecondaryIndex = 0;
	protected int CurrentUtilityIndex = 0;
	
	bool selected = false;
	
	void ClassData() {
		PrimaryItems = new array<ref ClassItem>;
		SecondaryItems = new array<ref ClassItem>;
		Utilities = new array<ref ClassItem>;
		GeneralItems = new array<ref ClassItem>;
		Clothes = new array<ref ClassClothing>;
	}
	
	void SetClassName(string className) {
		ClassName = className;
		
		if(layoutRoot) {
			TextWidget classNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("ClassName"));
			classNameWidget.SetText(className);
		}
	}
	
	void SetPrimaryItems(array<ref ClassItem> items) {
		PrimaryItems.Clear();
		foreach(ClassItem item: items) {
			PrimaryItems.Insert(item);
		}
	}
	
	void SetSecondaryItems(array<ref ClassItem> items) {
		SecondaryItems.Clear();
		foreach(ClassItem item: items) {
			SecondaryItems.Insert(item);
		}
	}
	
	void SetUtilities(array<ref ClassItem> items) {
		Utilities.Clear();
		foreach(ClassItem item: items) {
			Utilities.Insert(item);
		}
	}
		
	void SetGeneralItems(array<ref ClassItem> items) {
		GeneralItems.Clear();
		foreach(ClassItem item: items) {
			GeneralItems.Insert(item);
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
				maxIndex = PrimaryItems.Count() - 1;
				if(CurrentPrimaryIndex != maxIndex) {
					CurrentPrimaryIndex++;
				}
				else {
					CurrentPrimaryIndex = 0;
				}
			break;
			case "secondary":
				maxIndex = SecondaryItems.Count() - 1;
				if(CurrentSecondaryIndex != maxIndex) {
					CurrentSecondaryIndex++;
				}
				else {
					CurrentSecondaryIndex = 0;
				}
			break;
			case "utility":
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
				maxIndex = PrimaryItems.Count() - 1;
				if(CurrentPrimaryIndex != 0) {
					CurrentPrimaryIndex--;
				}
				else {
					CurrentPrimaryIndex = maxIndex;
				}
			break;
			case "secondary":
				maxIndex = SecondaryItems.Count() - 1;
				if(CurrentSecondaryIndex != 0) {
					CurrentSecondaryIndex--;
				}
				else {
					CurrentSecondaryIndex = maxIndex;
				}
			break;
			case "utility":
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
		
		if(PrimaryItems.Count()) {
			if(CurrentPrimary) CurrentPrimary.selected = !CurrentPrimary.selected;
			CurrentPrimary = PrimaryItems.Get(CurrentPrimaryIndex);
			CurrentPrimary.selected = !CurrentPrimary.selected;
		}
		
		if(SecondaryItems.Count()) {
			if(CurrentSecondary) CurrentSecondary.selected = !CurrentSecondary.selected;
			CurrentSecondary = SecondaryItems.Get(CurrentSecondaryIndex);
			CurrentSecondary.selected = !CurrentSecondary.selected;
		}
		
		if(Utilities.Count()) {
			if(CurrentUtility) CurrentUtility.selected = !CurrentUtility.selected;
			CurrentUtility = Utilities.Get(CurrentUtilityIndex);
			CurrentUtility.selected = !CurrentUtility.selected;
		}
		
		if(CurrentPrimary) primaryPreview.SetItem(CurrentPrimary.GetItem());
		if(CurrentSecondary) secondaryPreview.SetItem(CurrentSecondary.GetItem());
		if(CurrentUtility) utilityPreview.SetItem(CurrentUtility.GetItem());
	}
	
	void LoadFromJSON(JsonClassData data) {
		SetClassName(data.className);
		selected = data.selected;
		
		ClassItem newItem;
		array<ref ClassItem> primaryItems = new array<ref ClassItem>();
		foreach(JsonClassItem jsonWp: data.primaryItems) {
			newItem = ClassItem.LoadFromJSON(jsonWp);
			primaryItems.Insert(newItem);
			
			if(newItem.selected) {
				CurrentPrimaryIndex = primaryItems.Count() - 1;
			}
		}
		
		array<ref ClassItem> secondaryItems = new array<ref ClassItem>;
		foreach(JsonClassItem jsonWs: data.secondaryItems) {
			newItem = ClassItem.LoadFromJSON(jsonWs);
			secondaryItems.Insert(newItem);
			
			if(newItem.selected) {
				CurrentSecondaryIndex = secondaryItems.Count() - 1;
			}
		}
		
		array<ref ClassItem> utilities = new array<ref ClassItem>;
		foreach(JsonClassItem jsonU: data.utilities) {
			newItem = ClassItem.LoadFromJSON(jsonU);
			utilities.Insert(newItem);
			
			if(newItem.selected) {
				CurrentUtilityIndex = utilities.Count() - 1;
			}
		}
		
		array<ref ClassItem> generalItems = new array<ref ClassItem>;
		foreach(JsonClassItem jsonG: data.utilities) {
			newItem = ClassItem.LoadFromJSON(jsonG);
			generalItems.Insert(newItem);
		}
		
		array<ref ClassClothing> clothings = new array<ref ClassClothing>;
		foreach(JsonClassClothing jsonC: data.clothes) {
			clothings.Insert(new ClassClothing(jsonC));
		}
		
		SetPrimaryItems(primaryItems);
		SetSecondaryItems(secondaryItems);
		SetUtilities(utilities);
		SetGeneralItems(generalItems);
		SetClothes(clothings);
	}
	
	JsonClassSelection GetSelection() {
		return new JsonClassSelection(ClassName, CurrentPrimary, CurrentSecondary, CurrentUtility);
	}
}