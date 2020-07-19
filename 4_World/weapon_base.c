modded class Weapon_Base
{
	void UpdateAnimationState (bool has_bullet, bool has_mag, int specificIndex = 0)
	{
		if (m_fsm)
		{
			m_fsm.UpdateAnimationState(has_bullet, has_mag, specificIndex);
			SyncSelectionState(has_bullet, has_mag);
		}
	}

};

