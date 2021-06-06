using System;

namespace BattleSimulator
{
	static class Battle
	{
		static Fighter[] fighters;

		public static void begin(byte numOpp)
		{
			fighters = new Fighter[numOpp];
			fighters[0] = new Player("McDaMastR", 1);
			
		}
	}
}
