class ClassItem {
	
	protected EntityAI m_Entity = null;
	protected ref TStringArray m_EntityAttachments;
	protected ref TStringArray m_Magazines = {};
	protected ref array<ref ClassItem> m_Cargo = {};
	protected int m_Quantity = 0;
	
	bool selected = false;
	
	void ClassItem(string ObjectName) {
		Object itemObj = GetGame().CreateObject(ObjectName, Vector(0, 0, 0), true);
		m_Entity = EntityAI.Cast(EntityAI.Cast(Entity.Cast(itemObj)));
	}
	
	EntityAI SetAttachments(TStringArray attachments) {
		m_EntityAttachments = attachments;
		
		foreach(string attachment: m_EntityAttachments) {
			m_Entity.GetInventory().CreateAttachment(attachment);
		}
		
		return  m_Entity;
	}
	
	void SetQuantity(int q) {
		m_Quantity = q;
	}
	
	void AddCargo(ClassItem item) {
		m_Cargo.Insert(item);
	}
	
	void AddMagazines(string type, int quantity) {
		//CREATE FAKE MAG
		Weapon_Base gun = Weapon_Base.Cast(m_Entity);
		Magazine mag = Magazine.Cast(GetGame().CreateObject(type, Vector(0, 0, 0), true));
		gun.AttachMagazine(0, mag);
		gun.SelectionMagazineShow();
		
		for(int i = 0; i < quantity; i++) {
			m_Magazines.Insert(type);
		}
	}
	
	EntityAI GetItem() {
		return m_Entity;
	}
	
	array<ref ClassItem> GetCargo() {
		return m_Cargo;
	}
	
	int GetQuantity() {
		return m_Quantity;
	}
	
	TStringArray GetAttachments() {
		return m_EntityAttachments;
	}	
	
	TStringArray GetMagazines() {
		return m_Magazines;
	}
	
	static ClassItem LoadFromJSON(JsonClassItem data) {
		ClassItem item = new ClassItem(data.name);
		item.selected = data.selected;
		
		if(data.quantity) {
			item.SetQuantity(data.quantity);
		}
		
		if(data.attachments) {
			item.SetAttachments(data.attachments);
		}
		
		if(data.cargo) {
			foreach(JsonClassItem _item: data.cargo) {
				item.AddCargo(LoadFromJSON(_item));
			}
		}
		
		if(data.magazines) {
			foreach(JsonClassMagazine mag: data.magazines) {
				item.AddMagazines(mag.name, mag.quantity);
			}
		}
		
		return item;
	}
	
	array<ref JsonClassItem> GetCargoJSON() {
		array<ref JsonClassItem> cargo = new array<ref JsonClassItem>;
		foreach(ClassItem cargoItem:  m_Cargo) {
			cargo.Insert(cargoItem.ToJSON());
		}
		
		return cargo;
	}
	
	JsonClassItem ToJSON() {
		return new JsonClassItem(GetItem().GetType(), GetQuantity(), GetAttachments(), GetCargoJSON());
	}
}