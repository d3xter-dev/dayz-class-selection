modded class WeaponFSM {
	void UpdateAnimationState (bool has_bullet, bool has_mag, int specificIndex = 0) {
		array<WeaponStableState> candidates = new array<WeaponStableState>;
		
		int tc = m_Transitions.Count();
		for (int i = 0; i < tc; ++i)
		{
			WeaponTransition trans = m_Transitions.Get(i);
			WeaponStableState state = WeaponStableState.Cast(trans.m_srcState);
			if (state && state.HasBullet() == has_bullet && state.HasMagazine() == has_mag && state.IsJammed() == false)
				candidates.Insert(state);
		}
		
		int cc = candidates.Count();
		if (cc)
		{
			WeaponStableState selected = candidates.Get(specificIndex);
			Terminate();
			m_State = selected;
			Start(null, true);
			selected.SyncAnimState();
		}
		else
		{
			wpnDebugPrint("[wpnfsm] RandomizeFSMState - warning - cannot randomize, no states available");
		}
	}
}