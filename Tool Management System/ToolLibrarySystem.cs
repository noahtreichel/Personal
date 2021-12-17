using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class ToolLibrarySystem : iToolLibrarySystem
    {
        // Dictionaries of all the tool categories, and their corresponding tool types to be stored
        public Dictionary<int, ToolCollection> gardeningTools = new Dictionary<int, ToolCollection>();
        private ToolCollection lineTrimmers = new ToolCollection();
        private ToolCollection lawnMowers = new ToolCollection();
        private ToolCollection gardeningHandTools = new ToolCollection();
        private ToolCollection wheelbarrows = new ToolCollection();
        private ToolCollection gardenPowerTools = new ToolCollection();

        public Dictionary<int, ToolCollection> flooringTools = new Dictionary<int, ToolCollection>();
        private ToolCollection scrapers = new ToolCollection();
        private ToolCollection floorLasers = new ToolCollection();
        private ToolCollection floorLevellingTools = new ToolCollection();
        private ToolCollection floorLevellingMaterials = new ToolCollection();
        private ToolCollection floorHandTools = new ToolCollection();
        private ToolCollection tilingTools = new ToolCollection();

        public Dictionary<int, ToolCollection> fencingTools = new Dictionary<int, ToolCollection>();
        private ToolCollection fencingHandTools = new ToolCollection();
        private ToolCollection electricFencing = new ToolCollection();
        private ToolCollection steelFencingTools = new ToolCollection();
        private ToolCollection powerTools = new ToolCollection();
        private ToolCollection fencingAccessories = new ToolCollection();

        public Dictionary<int, ToolCollection> measuringTools = new Dictionary<int, ToolCollection>();
        private ToolCollection distanceTools = new ToolCollection();
        private ToolCollection laserMeasurer = new ToolCollection();
        private ToolCollection measuringJugs = new ToolCollection();
        private ToolCollection tempuratureHumidityTools = new ToolCollection();
        private ToolCollection levellingTools = new ToolCollection();
        private ToolCollection markers = new ToolCollection();

        public Dictionary<int, ToolCollection> cleaningTools = new Dictionary<int, ToolCollection>();
        private ToolCollection draining = new ToolCollection();
        private ToolCollection carCleaning = new ToolCollection();
        private ToolCollection vacuum = new ToolCollection();
        private ToolCollection pressureCleaners = new ToolCollection();
        private ToolCollection poolCleaning = new ToolCollection();
        private ToolCollection floorCleaning = new ToolCollection();

        public Dictionary<int, ToolCollection> paintingTools = new Dictionary<int, ToolCollection>();
        private ToolCollection sandingTools = new ToolCollection();
        private ToolCollection brushes = new ToolCollection();
        private ToolCollection rollers = new ToolCollection();
        private ToolCollection paintRemovalTools = new ToolCollection();
        private ToolCollection paintScrapers = new ToolCollection();
        private ToolCollection sprayers = new ToolCollection();

        public Dictionary<int, ToolCollection> electronicTools = new Dictionary<int, ToolCollection>();
        private ToolCollection voltageTester = new ToolCollection();
        private ToolCollection oscilloscopes = new ToolCollection();
        private ToolCollection thermalImaging = new ToolCollection();
        private ToolCollection dataTestTool = new ToolCollection();
        private ToolCollection insulationTesters = new ToolCollection();

        public Dictionary<int, ToolCollection> electricityTools = new Dictionary<int, ToolCollection>();
        ToolCollection testEquipment = new ToolCollection();
        ToolCollection safetyEquipment = new ToolCollection();
        ToolCollection basicHandTools = new ToolCollection();
        ToolCollection circuitProtection = new ToolCollection();
        ToolCollection cableTools = new ToolCollection();

        public Dictionary<int, ToolCollection> automotiveTools = new Dictionary<int, ToolCollection>();
        private ToolCollection jacks = new ToolCollection();
        private ToolCollection airCompressors = new ToolCollection();
        private ToolCollection batteryChargers = new ToolCollection();
        private ToolCollection socketTools = new ToolCollection();
        private ToolCollection braking = new ToolCollection();
        private ToolCollection drivetrain = new ToolCollection();

        public ToolLibrarySystem()
        {
            // Constructors of the tool types for each tool category
            gardeningTools.Add(1, lineTrimmers);
            gardeningTools.Add(2, lawnMowers);
            gardeningTools.Add(3, gardeningHandTools);
            gardeningTools.Add(4, wheelbarrows);
            gardeningTools.Add(5, gardenPowerTools);

            flooringTools.Add(1, scrapers);
            flooringTools.Add(2, floorLasers);
            flooringTools.Add(3, floorLevellingTools);
            flooringTools.Add(4, floorLevellingMaterials);
            flooringTools.Add(5, floorHandTools);
            flooringTools.Add(6, tilingTools);

            fencingTools.Add(1, fencingHandTools);
            fencingTools.Add(2, electricFencing);
            fencingTools.Add(3, steelFencingTools);
            fencingTools.Add(4, powerTools);
            fencingTools.Add(5, fencingAccessories);

            measuringTools.Add(1, distanceTools);
            measuringTools.Add(2, laserMeasurer);
            measuringTools.Add(3, measuringJugs);
            measuringTools.Add(4, tempuratureHumidityTools);
            measuringTools.Add(5, levellingTools);
            measuringTools.Add(6, markers);

            cleaningTools.Add(1, draining);
            cleaningTools.Add(2, carCleaning);
            cleaningTools.Add(3, vacuum);
            cleaningTools.Add(4, pressureCleaners);
            cleaningTools.Add(5, poolCleaning);
            cleaningTools.Add(6, floorCleaning);

            paintingTools.Add(1, sandingTools);
            paintingTools.Add(2, brushes);
            paintingTools.Add(3, rollers);
            paintingTools.Add(4, paintRemovalTools);
            paintingTools.Add(5, paintScrapers);
            paintingTools.Add(6, sprayers);

            electronicTools.Add(1, voltageTester);
            electronicTools.Add(2, oscilloscopes);
            electronicTools.Add(3, thermalImaging);
            electronicTools.Add(4, dataTestTool);
            electronicTools.Add(5, insulationTesters);

            electricityTools.Add(1, testEquipment);
            electricityTools.Add(2, safetyEquipment);
            electricityTools.Add(3, basicHandTools);
            electricityTools.Add(4, circuitProtection);
            electricityTools.Add(5, cableTools);

            automotiveTools.Add(1, jacks);
            automotiveTools.Add(2, airCompressors);
            automotiveTools.Add(3, batteryChargers);
            automotiveTools.Add(4, socketTools);
            automotiveTools.Add(5, braking);
            automotiveTools.Add(6, drivetrain);
        }

        // Instantiation of the MemberCollection class to store the members
        private MemberCollection members = new MemberCollection();
        public ToolCollection tools;

        public void add(Tool aTool) // add a new tool to the system
        {
            tools.add(aTool);
        }

        public void add(Tool aTool, int quantity) //add new pieces of an existing tool to the system
        {
            aTool.Quantity += quantity;
        }

        public void delete(Tool aTool) //delte a given tool from the system
        {
            tools.delete(aTool);
        }

        public void delete(Tool aTool, int quantity) //remove some pieces of a tool from the system
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

        public void add(Member aMember) //add a new memeber to the system
        {
            members.add(aMember);
        }

        public void delete(Member aMember) //delete a member from the system
        {
            members.delete(aMember);
        }

        public void displayBorrowingTools(iMember aMember) //given a member, display all the tools that the member are currently renting
        {

        }

        public void displayTools(string aToolType) // display all the tools of a tool type selected by a member
        {
            Console.WriteLine($"{aToolType}");
        }

        public void borrowTool(iMember aMember, iTool aTool) //a member borrows a tool from the tool library
        {

        }

        public void returnTool(iMember aMember, iTool aTool) //a member return a tool to the tool library
        {

        }

        public string[] listTools(iMember aMember) //get a list of tools that are currently held by a given member
        {
            List<string> toolsList = new List<string>();

            string[] tools = toolsList.ToArray();
            return tools;
        }

        public void displayTopThree() //Display top three most frequently borrowed tools by the members in the descending order by the number of times each tool has been borrowed.
        {

        }
    }
}
