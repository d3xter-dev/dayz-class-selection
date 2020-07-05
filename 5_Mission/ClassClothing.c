class ClassClothing {
		
	protected EntityAI m_top;
	protected EntityAI m_pants;
	protected EntityAI m_shoes;
	protected EntityAI m_backpack;
	protected EntityAI m_vest;

	void ClassClothing(string top, string pants, string shoes, string backpack, string vest) {
		Object obj = GetGame().CreateObject(top, Vector(0, 0, 0), true);
		m_top = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(pants, Vector(0, 0, 0), true);
		m_pants = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(shoes, Vector(0, 0, 0), true);
		m_shoes = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(backpack, Vector(0, 0, 0), true);
		m_backpack = EntityAI.Cast(Entity.Cast(obj));
		
		obj = GetGame().CreateObject(vest, Vector(0, 0, 0), true);
		m_vest = EntityAI.Cast(Entity.Cast(obj));
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
}