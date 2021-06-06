using System;

namespace GeneralLearning
{
    class Program
    {
        static void Main(/* string[] args */)
        {
            var student = new Student("John KFC", 17);
            Console.WriteLine("Student: " + student.fullName);
            Console.WriteLine("\tAge: {0}", student.age);
            Console.WriteLine("\tGrades:");
            foreach (var grade in student.grades)
                Console.WriteLine("\t\t" + grade);
            Console.WriteLine();

            string str = "4200";
            int i = Convert.ToInt32(str);
            ushort j = ushort.Parse(str);
            Console.WriteLine("Conversions:\n\tString: {0}\n\tConvert: {1}\n\tParse: {2}", str, i, j);
        }
    }
}
