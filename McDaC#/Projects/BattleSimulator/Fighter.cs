using System;
using System.Collections.Generic;

namespace BattleSimulator
{
	abstract class Fighter
	{
		public enum Input
		{
			none = 0,
			attack
		}

		public enum Action
		{
			none = 0,
			attack_1
		}

		protected uint currentHealth, maxHealth;
		protected Input mainInput;
		protected Action action;

		public Fighter(uint LV)
		{
			currentHealth = 40 * LV;
			maxHealth = 40 * LV;

			mainInput = Input.none;
			action = Action.none;

			incFighters();
		}

		~Fighter()
		{
			decFighters();
		}

		public void printHealthBar()
		{
			Console.WriteLine($"{currentHealth} / {maxHealth}");
		}

		public abstract void mainAction();


		public static List<string> text 
		{ 
			get
			{
				var temp = text;
				text.Clear();
				return temp;
			} 
			private set 
			{
				text = value;
			}
		}
		public static byte num_fighters
		{
			get; 
			private set;
		}

		static void incFighters()
		{
			num_fighters++;
		}

		static void decFighters()
		{
			num_fighters--;
		}
	}

	class Player : Fighter
	{
		string username;

		public Player(string username, uint LV)
			: base(LV)
		{
			this.username = username;
		}

		public override void mainAction()
		{
			Console.WriteLine("| Attack | ");
			mainInput = (Input) Convert.ToInt32(Console.ReadLine());

			switch (mainInput)
			{
			case Input.attack: 
				attackAction(); 
				break;
			}
		}

		private void attackAction()
		{
			do {
				Console.WriteLine("\r| Opp1 | Back | ");
				byte attackInput = Convert.ToByte(Console.ReadLine());

				switch (attackInput)
				{
				case 1:
					action = Action.attack_1;
					return;
				case 2:
					return;
				}
			} while (true);
		}
	}

	class Opponent : Fighter
	{
		public Opponent(uint LV)
			: base(LV)
		{

		}

		public override void mainAction()
		{
			
		}
	}
}
