class ClassWeapon {
	
	protected EntityAI m_EntityWeapon = null;
	protected ref TStringArray m_EntityAttachments;
	protected ref TStringArray m_Magazines = {};
	
	void ClassWeapon(string ObjectName) {
		Object weaponObj = GetGame().CreateObject(ObjectName, Vector(0, 0, 0), true);
		m_EntityWeapon = EntityAI.Cast(EntityAI.Cast(Entity.Cast(weaponObj)));
	}
	
	EntityAI SetAttachments(TStringArray attachments) {
		m_EntityAttachments = attachments;
		
		foreach(string attachment: m_EntityAttachments) {
			m_EntityWeapon.GetInventory().CreateAttachment(attachment);
		}
		
		return  m_EntityWeapon;
	}
	
	void AddMagazines(string type, int quantity) {
		
		//CREATE FAKE MAG
		Weapon_Base gun = Weapon_Base.Cast(m_EntityWeapon);
		Magazine mag = Magazine.Cast(GetGame().CreateObject(type, Vector(0, 0, 0), true));
		gun.AttachMagazine(0, mag);
		gun.SelectionMagazineShow();
		
		for(int i = 0; i < quantity; i++) {
			m_Magazines.Insert(type);
		}
	}
	
	EntityAI GetWeapon() {
		return m_EntityWeapon;
	}
	
	TStringArray GetAttachments() {
		return m_EntityAttachments;
	}	
	
	TStringArray GetMagazines() {
		return m_Magazines;
	}
	
	static ClassWeapon LoadFromJSON(JsonClassWeapon data) {
		ClassWeapon weapon = new ClassWeapon(data.name);
		weapon.SetAttachments(data.attachments);
		
		foreach(JsonClassMagazine mag: data.magazines) {
			weapon.AddMagazines(mag.name, mag.quantity);
		}
		
		return weapon;
	}
}