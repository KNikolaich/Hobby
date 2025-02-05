// See https://aka.ms/new-console-template for more information
//DayCalculator()

using System.Linq;

DayCalculator.AddDays(
    [
        new Person{ PersonID = 1, Finish = new DateTime(2001, 01, 01)},
        new Person{ PersonID = 2, Finish = new DateTime(2002, 02, 02)}
    ],
    [ 
        new Prolong{ Days = 1, PersonID = 1},  
        new Prolong{ Days = 1, PersonID = 2},
        new Prolong{ Days = 3, PersonID = 1},
    ]
)
.ForEach(x=> Console.WriteLine(x));

record Person
{
    public int PersonID;        
    public DateTime Finish;
}

record Prolong
{
    public int PersonID;
    public int Days;
}

class DayCalculator
{
    public static List<Person> AddDays(Person[] persons, Prolong[] days) 
    {
        var prolongDict = days
        .GroupBy(x => x.PersonID)       
        .ToDictionary(x => x.Key, static y => y.Sum(x=>x.Days));
        
        var personList = persons.ToList();
        personList.ForEach(x=> {
            if(prolongDict.TryGetValue(x.PersonID, out var daysProlong))
            {
                x.Finish = x.Finish.AddDays(daysProlong);
            }
        });
        return personList;
    }
 }