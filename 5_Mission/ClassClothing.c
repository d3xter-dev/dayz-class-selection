class ClassClothing {
		
	//ToDo for Fututre Clothing Selection
	protected EntityAI m_top;
	protected EntityAI m_pants;
	protected EntityAI m_shoes;
	protected EntityAI m_backpack;
	protected EntityAI m_vest;
	
	protected EntityAI m_gloves;
	protected EntityAI m_belt;
	protected EntityAI m_hat;
	protected EntityAI m_glasses;
	protected EntityAI m_mask;
	protected EntityAI m_armband;
	
	void ClassClothing(JsonClassClothing clothing) {
		TStringArray parts = {"top", "pants", "shoes", "backpack", "vest", "gloves", "belt", "hat", "glasses", "mask", "armband"};

		Object obj = GetGame().CreateObject(clothing.top, Vector(0, 0, 0), true);
		this.m_top = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.pants, Vector(0, 0, 0), true);
		this.m_pants = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.shoes, Vector(0, 0, 0), true);
		this.m_shoes = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.backpack, Vector(0, 0, 0), true);
		this.m_backpack = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.vest, Vector(0, 0, 0), true);
		this.m_vest = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.gloves, Vector(0, 0, 0), true);
		this.m_gloves = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.belt, Vector(0, 0, 0), true);
		this.m_belt = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.hat, Vector(0, 0, 0), true);
		this.m_hat = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.glasses, Vector(0, 0, 0), true);
		this.m_glasses = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(clothing.mask, Vector(0, 0, 0), true);
		this.m_mask = EntityAI.Cast(Entity.Cast(obj));	
			
		obj = GetGame().CreateObject(clothing.armband, Vector(0, 0, 0), true);
		this.m_armband = EntityAI.Cast(Entity.Cast(obj));
	}
	
	EntityAI GetTop() {
		return m_top;
	}
	
	EntityAI GetPants() {
		return m_pants;
	}	
	
	EntityAI GetShoes() {
		return m_shoes;
	}
	
	EntityAI GetBackpack() {
		return m_backpack;
	}
	
	EntityAI GetVest() {
		return m_vest;
	}
	
	EntityAI GetGloves() {
		return m_gloves;
	}
	
	EntityAI GetBelt() {
		return m_belt;
	}
	
	EntityAI GetHat() {
		return m_hat;
	}
	
	EntityAI GetGlasses() {
		return m_glasses;
	}
	
	EntityAI GetMask() {
		return m_mask;
	}
	
	EntityAI GetArmband() {
		return m_armband;
	}
}