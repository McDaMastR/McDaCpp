using System;

namespace BattleSimulator
{
	enum MenuInput
	{
		beginBattle = 0, tutorial, credits, login, logout, quit
	}

	static class General
	{
		public static MenuInput menuMessage()
		{
			const string msg = 
@"Enter 0 to Begin Battle
Enter 1 for the tutorial
Enter 2 for credits
Enter 3 to login
Enter 4 to logout
Enter 5 to exit the game";

			Console.WriteLine(msg);
			var input_string = Console.ReadLine();
			var input = (MenuInput) Convert.ToInt32(input_string);
			Console.Clear();

			return input;
		}

		public static bool confirmBattle()
		{
			do {
				Console.WriteLine("{0}: LV {1} EXP {2}", "You", 1, 0);
				Console.WriteLine("\tHealth: {0}", 100);

				Console.WriteLine("Number of Opponents: {0}", 1);
				Console.WriteLine("Opponent {0}:", 1);
				Console.WriteLine("\tHealth: {0}", 100);

				Console.WriteLine("Continue? [y/n]: ");
				var input = Console.ReadLine();

				if (input == "y" || input == "n")
					return input == "y";
			} while (true);
		}
	}
}
