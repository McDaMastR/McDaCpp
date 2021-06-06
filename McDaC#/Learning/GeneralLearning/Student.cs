using System;

namespace GeneralLearning
{
    enum Grade
    {
        A, B, C, D, E
    }

    class Student
    {
        public string fullName {get;} // Read-only
        public int age {get; set;}
        uint ID;
        Grade[] m_grades = new Grade[6] {Grade.A, Grade.B, Grade.C, Grade.B, Grade.E, Grade.D};
        public Grade[] grades
        {
            get {return m_grades;}
            set {m_grades = value;}
        }

        public Student(string full_name, int age)
        {
            this.fullName = full_name;
            this.age = age;

            var rnd = new Random();
            this.ID = (uint) rnd.Next(int.MaxValue);

            Console.WriteLine("Created Student Class");
        }
    }
}
