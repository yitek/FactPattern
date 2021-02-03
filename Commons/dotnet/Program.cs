using System;
using System.Linq.Expressions;

namespace FactPattern
{
    
    class Program
    {
        enum Genders { 
            Female,
            Male
        }
        
        static void Main(string[] args)
        {
            Genders? gender = Genders.Male;
            object genderObj = gender;
            int? x = (int?)genderObj;
            Console.WriteLine(x);
            
            Console.ReadKey();
        }

    }
}
