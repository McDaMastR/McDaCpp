using System;

namespace BattleSimulator
{
	class Program
	{
		static void Main(/* string[] args */)
		{
			Console.WriteLine("Welcome to C# Battle Simulator!\nEnter to Start");
			while (Console.ReadKey().Key != ConsoleKey.Enter);
			Console.Clear();

			gameLoop();
			Console.WriteLine("Qutting Application...");
		}

		static void gameLoop()
		{
			do {
				var menu_input = General.menuMessage();

				switch (menu_input) 
				{
				case MenuInput.beginBattle	: battle(); break;
				case MenuInput.tutorial	  	: Console.WriteLine("Git Good"); break;
				case MenuInput.credits	  	: Console.WriteLine("By McDaMastR"); break;
				case MenuInput.login	  	: Console.WriteLine("No Login yet"); break;
				case MenuInput.logout	  	: Console.WriteLine("What do you think?"); break;
				case MenuInput.quit		  	: return;
				}

				Console.Clear();
			} while (true);
		}

		static void battle()
		{
			var result = General.confirmBattle();
			if (result)
				Battle.begin();
		}
	}
}
