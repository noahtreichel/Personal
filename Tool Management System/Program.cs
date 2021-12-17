using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class Program
    {
        // Instantiation of the ToolLibrarySystem
        private static ToolLibrarySystem toolLibrarySystem = new ToolLibrarySystem();
        private static ToolCollection tools;
        private static List<Member> members = new List<Member>();
        private static Member currentMember;
        
        private static void MainMenu()
        {
            Console.Clear();
            Console.WriteLine("Welcome to the Tool Library");
            Console.WriteLine("===========Main Menu===========");
            Console.WriteLine("1. Staff Login");
            Console.WriteLine("2. Member Login");
            Console.WriteLine("0. Exit");
            Console.WriteLine("================================");
            Console.Write("\r\nPlease make a selection (1-2, or 0 to exit): ");
            
            // Variable used to identify which key is pressed, in order to perform specified tasks
            var command = Console.ReadKey();
            if (command.Key == ConsoleKey.D1)
            {
                // Variables used to store the username and password of a member
                string username;
                string password;

                Console.Clear();
                Console.WriteLine("Welcome to the Tool Library");
                Console.WriteLine("===========Login Menu===========\n");      

                Console.Write("Username: ");
                username = Console.ReadLine();
                Console.Write("Password: ");
                password = Console.ReadLine();
                Console.WriteLine();

                // If the username or password is incorrect, deny login, otherwise display the Staff Menu
                if (username != "staff" || password != "today123")
                {
                    Console.WriteLine("Incorrect login details.");
                    Console.ReadLine();
                    MainMenu();
                }
                else
                {
                    Console.WriteLine("Logged in successfully!");
                    Console.ReadLine();
                    StaffMenu();
                }
            }

            if (command.Key == ConsoleKey.D2)
            {
                // Variables used to store the first name, last name, and PIN of a member
                string firstName;
                string lastName;
                string pin;

                Console.Clear();
                Console.WriteLine("Welcome to the Tool Library");
                Console.WriteLine("===========Login Menu===========\n");

                Console.Write("First name: ");
                firstName = Console.ReadLine();                   
                Console.Write("Last name: ");
                lastName = Console.ReadLine();                   
                Console.Write("PIN: ");
                pin = Console.ReadLine();
                Console.WriteLine();

                // Compare the current members specifications with the data input by the user
                for (int i = 0; i < members.Count; i++)
                {
                    if (members[i].FirstName == firstName && members[i].LastName == lastName && members[i].PIN == pin)
                    {
                        currentMember = members[i];
                    }
                }
                        
                // If the specifications do not match, deny login, otherwise display the Member Menu
                if (currentMember == null)
                {
                    Console.WriteLine("Incorrect login details.");
                    Console.ReadLine();
                    MainMenu();
                }
                else
                {
                    Console.WriteLine("Logged in successfully!");
                    Console.ReadLine();
                    MemberMenu();
                }
            }

            if (command.Key == ConsoleKey.D0)
            {
                // Exit the application
                System.Environment.Exit(0);
            }
        }


        /** A function that generates menus
            Used to reduce the number of hard coded menus needed to be created */
        public static void MenuGenerator(string menuType, string[] menuElements, string statement)
        {
            Console.Clear();
            Console.WriteLine("Welcome to the Tool Library");
            Console.WriteLine($"==============={menuType}===============");

            for (int i = 0; i < menuElements.Length; i++)
            {
                Console.WriteLine($"{i + 1}: {menuElements[i]}");
            }

            Console.WriteLine($"0: {statement}");
            Console.WriteLine("========================================");
            Console.Write($"\r\nPlease make a slection (1-{menuElements.Length}, or 0 to {statement.ToLower()}): ");
        }


        // Code related to the Staff Menu --------------------------------------------------
        // Adds a new tool to the ToolCollection class
        public static void add(Tool aTool)
        {
            tools.add(aTool);
        }

        // Adds pieces of an existing tool to the selected tool type
        public static void add(Tool aTool, int quantity)
        {
            aTool.Quantity += quantity;
        }

        // Deletes a tool from the ToolCollection class
        public static void delete(Tool aTool)
        {
            tools.delete(aTool);
        }

        // Deletes the specified quantity of tools from the selected tool type
        public static void delete(Tool aTool, int quantity)
        {
            if ((aTool.Quantity - quantity) <= 0)
            {
                delete(aTool);
            }               
            else
            {
                aTool.Quantity -= quantity;
            }
        }

        // Adds a member to the MemberCollection class
        public static void add(Member aMember)
        {
            toolLibrarySystem.add(aMember);
            members.Add(aMember);
        }

        // Deletes a member from the MemberCollection class
        public static void delete(Member aMember)
        {
            toolLibrarySystem.delete(aMember);
            members.Remove(aMember);
        }


        public static void StaffMenu()
        {
            Console.Clear();
            // Elements to display in the staff menu, implemented through the menu generator
            string[] staffMenuElements =
            {
                "Add a new tool", "Add new pieces of an existing tool", "Remove some pieces of a tool",
                "Register a new member", "Remove a member", "Find the contact number of a member"
            };

            MenuGenerator("Staff Menu", staffMenuElements, "return to main menu");
            
            var command = Console.ReadKey();
            switch (command.Key)
            {
                // Add a new tool
                case ConsoleKey.D1:
                    displayToolMenu();
                    Console.Clear();
                    Console.WriteLine("Welcome to the Tool Library");
                    Console.WriteLine("===========Add a New Tool===========\n");

                    Tool newTool = new Tool();
                    Console.Write("Tool name: ");
                    newTool.Name = Console.ReadLine().Trim();
                    try
                    {
                        Console.Write("Quantity (number): ");
                        newTool.Quantity = Convert.ToInt32(Console.ReadLine());
                        add(newTool);
                        Console.WriteLine($"\nTool '{newTool.Name}' added successfully.");
                    }
                    catch (Exception)
                    {
                        Console.WriteLine("\nInvalid number.");
                    }
                    Console.ReadLine();
                    StaffMenu();
                    break;

                // Add new pieces of an existng tool
                case ConsoleKey.D2:
                    displayToolMenu();
                    Tool[] existingTools = tools.toArray();

                    if (displayTools(existingTools))
                    {
                        try
                        {
                            Console.Write("\nSelect the number corresponding to the tool you wish to increment: ");
                            int toolNumber = Convert.ToInt16(Console.ReadLine());
                            Tool selectedTool = existingTools[toolNumber - 1];
                            Console.Write("Input the quantity you want to add: ");
                            int quantity = Convert.ToInt32(Console.ReadLine());
                            add(selectedTool, quantity);
                            Console.WriteLine($"\nAdded '{quantity}' {selectedTool.Name}(s).\n '{selectedTool.Quantity}' in total.");
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("\nInvalid number.");
                        }
                    }
                    Console.ReadLine();
                    StaffMenu();
                    break;
    
                // Remove some pieces of a tool
                case ConsoleKey.D3:
                    displayToolMenu();
                    Tool[] existingtools = tools.toArray();

                    if (displayTools(existingtools))
                    {
                        try
                        {
                            Console.Write("\nSelect the number corresponding to the tool you wish to remove: ");
                            int toolNumber = Convert.ToInt16(Console.ReadLine());
                            Console.Write("Input the quantity you want to remove: ");
                            int quantity = Convert.ToInt32(Console.ReadLine());
                            Tool selectedTool = existingtools[toolNumber - 1];
                            int remainder = selectedTool.Quantity - quantity;
                            bool fewerThanZero = (remainder <= 0);
                            Console.WriteLine($"\nRemoved {(fewerThanZero ? "all" : Convert.ToString(quantity))} {selectedTool.Name}(s).\n" +
                                $"{(fewerThanZero ? "" : + remainder + " remaining.")}");
                            delete(selectedTool, quantity);
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("\nInvalid number.");
                        }
                    }
                    Console.ReadLine();
                    StaffMenu();
                    break;

                // Register a new member
                case ConsoleKey.D4:
                    Console.Clear();
                    Console.WriteLine("Welcome to the Tool Library");
                    Console.WriteLine("===========Register a New Member===========\n");

                    // Instantiates a new member
                    Member newMember = new Member();
                    Console.Write("First name: ");
                    newMember.FirstName = Console.ReadLine();
                    Console.Write("Last name: ");
                    newMember.LastName = Console.ReadLine();
                    Console.Write("Contact number: ");
                    newMember.ContactNumber = Console.ReadLine();
                    Console.Write("PIN: ");
                    newMember.PIN = Console.ReadLine();

                    // Adds the new member to the system
                    add(newMember);
                    Console.WriteLine($"\nMember '{newMember.FirstName}' added successfully.");
                    Console.ReadLine();
                    StaffMenu();
                    break;

                // Remove a member
                case ConsoleKey.D5:
                    Console.Clear();
                    Console.WriteLine("Welcome to the Tool Library");
                    Console.WriteLine("===========Remove a Member===========\n");

                    if (displayMembers(members))
                    {
                        Console.Write("\nEnter the number corresponding to the member you wish to remove: ");                       
                        try
                        {
                            int memberNumber = Convert.ToInt16(Console.ReadLine());
                            Member selectedMember = members[memberNumber - 1];
                            delete(selectedMember);
                            Console.WriteLine($"\nMember '{selectedMember.FirstName}' removed successfully.");
                        }
                        catch (Exception)
                        {
                            Console.WriteLine("\nInvalid number.");
                        }
                    }
                    Console.ReadLine();
                    StaffMenu();
                    break;

                // Find the contact number of a member
                case ConsoleKey.D6:
                    Console.Clear();
                    Console.WriteLine("Welcome to the Tool Library");
                    Console.WriteLine("===========Find the Contact Number of a Member===========\n");

                    if (members.Count == 0)
                    {
                        Console.WriteLine("No members to display.");
                    }                       
                    else
                    {
                        string returnLine = "Name of member provided does not exist";
                        Console.Write("First name: ");
                        string firstName = Console.ReadLine();
                        Console.Write("Last name: ");
                        string lastName = Console.ReadLine();

                        for (int i = 0; i < members.Count; i++)
                        {
                            if (members[i].FirstName == firstName && members[i].LastName == lastName)
                            {
                                returnLine = ($"{firstName}'s contact number is: {members[i].ContactNumber}");
                            }
                        }
                        Console.WriteLine($"\n{returnLine}");
                    }
                    Console.ReadLine();
                    StaffMenu();
                    break;

                case ConsoleKey.D0:
                    MainMenu();
                    break;
            }                
        }
   

        // Code related to the Member Menu --------------------------------------------------
        public static void MemberMenu()
        {
            Console.Clear();
            // Elements to display in the staff menu, implemented through the menu generator
            string[] memberMenuElements =
            {
                "Display all tools of a tool type", "Borrow a tool", "Return a tool",
                "List all the tools that I am renting", "Display top three (3) most frequently rented tools"
            };

            MenuGenerator("Member Menu", memberMenuElements, "return to main menu");

            var command = Console.ReadKey();
            switch (command.Key)
            {
                case ConsoleKey.D1:
                    try
                    {
                        displayToolMenu();
                        displayTools(tools.toArray());
                    }
                    catch (Exception)
                    {
                        Console.WriteLine("\nInvalid option.");
                    }
                    Console.ReadLine();
                    MemberMenu();
                    break;

                case ConsoleKey.D2:
                    break;
                case ConsoleKey.D3:
                    break;
                case ConsoleKey.D4:
                    break;
                case ConsoleKey.D5:
                    break;
                case ConsoleKey.D0:
                    MainMenu();
                    break;
            }
        }


        // Code related to any Helper Functions --------------------------------------------------
        // Helper function to display all the members currently in the system
        private static bool displayMembers(List<Member> members)
        {
            Console.WriteLine("Registered members: ");
            if (members.Count == 0)
            {
                Console.WriteLine("\tNo members to display.");
                return false;
            }
            else
            {
                for (int i = 0; i < members.Count; i++)
                {
                    Console.WriteLine($"\t{(i + 1)}. {members[i].FirstName} {members[i].LastName}");
                }
                return true;
            }
        }

        // Helper function to display all the tools currently in the selected tool type
        private static bool displayTools(Tool[] tools)
        {
            Console.Clear();
            Console.WriteLine("Welcome to the Tool Library\n");

            Console.WriteLine("Current inventory of selected category: ");
            if (tools.Length == 0)
            {
                Console.WriteLine("\tNo tools to display.");
                return false;
            }
            else
            {
                for (int i = 0; i < tools.Length; i++)
                {
                    Console.Write($"\t{(i + 1)}. ");
                    toolLibrarySystem.displayTools(tools[i].ToString());
                }

                return true;
            }
        } 

        // Helper function to display the tool menu, in addition to accessing the corresponding tool types 
        private static void displayToolMenu()
        {
            // Categories to display in the tool menu
            string[] toolCategories =
            {
                "Gardening Tools", "Flooring Tools", "Fencing Tools", "Measuring Tools", "Cleaning Tools",
                "Painting Tools", "Electronic Tools", "Electricity Tools", "Automotive Tools"
            };
            string[][] toolTypes =
            {
                new string[] {"Line Trimmers", "Lawn Mowers", "Hand Tools", "Wheelbarrows", "Garden Power Tools"},
                new string[] {"Scrapers", "Floor Levelling Tools", "Floor Levelling Materials", "Floor Hand Tools", "Tiling Tools"},
                new string[] {"Hand Tools", "Electric Fencing", "Steel Fencing Tools", "Power Tools", "Fencing Accessories"},
                new string[] {"Distance Tools", "Laser Measure", "Measuring Jugs", "Temperature and Humidity Tools", "Levelling Tools", "Markers"},
                new string[] {"Draining", "Car Cleaning", "Vacuum", "Pressure Cleaners", "Pool Cleaning", "Floor Cleaning"},
                new string[] {"Sanding Tools", "Brushes", "Rollers", "Paint Removal Tools", "Paint Scrapers", "Sprayers"},
                new string[] {"Voltage Tester", "Oscilloscopes", "Thermal Imaging", "Data Test Tool", "Insulation Testers"},
                new string[] {"Test Equipment", "Safety Equipment", "Basic Hand Tools", "Circuit Protection", "Cable Tools"},
                new string[] {"Jacks", "Air Compressors", "Battery Charges", "Socket Tools", "Braking", "Drivetrain"}
            };

            ToolCollection category = null;
            MenuGenerator("Tool Menu", toolCategories, "return to staff menu");
            
            var command = Console.ReadKey();
            int selection;
            switch (command.Key)
            {
                case ConsoleKey.D1:
                    MenuGenerator("Gardening Tools Menu", toolTypes[0], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.gardeningTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D2:
                    MenuGenerator("Flooring Tools Menu", toolTypes[1], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.flooringTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D3:
                    MenuGenerator("Fencing Tools Menu", toolTypes[2], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.fencingTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D4:
                    MenuGenerator("Measuring Tools Menu", toolTypes[3], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.measuringTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D5:
                    MenuGenerator("Cleaning Tools Menu", toolTypes[4], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.cleaningTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D6:
                    MenuGenerator("Painting Tools Menu", toolTypes[5], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.paintingTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D7:
                    MenuGenerator("Electronic Tools Menu", toolTypes[6], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.electronicTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D8:
                    MenuGenerator("Electricity Tools Menu", toolTypes[7], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.electricityTools.TryGetValue(selection, out category);
                    break;

                case ConsoleKey.D9:
                    MenuGenerator("Automotive Tools Menu", toolTypes[8], "return to tool menu");
                    selection = int.Parse(Console.ReadKey().KeyChar.ToString());
                    toolLibrarySystem.automotiveTools.TryGetValue(selection, out category);
                    break;
            }
            tools = category;         
        }


        static void Main(string[] args)
        {
            MainMenu();
            Console.ReadLine();
        }
    }
}
