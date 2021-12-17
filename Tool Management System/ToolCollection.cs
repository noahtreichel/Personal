using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class ToolCollection : iToolCollection
    {
        private int number;
        // Instantiation of a Tool array to store the tools
        private Tool[] toolCollection = new Tool[50];

        public int Number // get the number of the types of tools in the community library
        {
            get { return number; }
        }

        public void add(Tool aTool) //add a given tool to this tool collection
        {
            for (int i = 0; i < toolCollection.Length; i++)
            {
                if (toolCollection[i] == null)
                {
                    toolCollection[i] = aTool;
                    break;
                }
            }
            number++;
        }

        public void delete(iTool aTool) //delete a given tool from this tool collection
        {

        }

        /**
        public Boolean search(iTool aTool) //search a given tool in this tool collection. Return true if this tool is in the tool collection; return false otherwise
        {

        }
        */

        public Tool[] toArray() // output the tools in this tool collection to an array of iTool
        {
            Tool[] array = new Tool[Number];
            int toolIndex = 0;
            for (int i = 0; i < toolCollection.Length; i++)
            {
                if (toolCollection[i] != null)
                {
                    array[toolIndex] = toolCollection[i];
                    toolIndex++;
                }
            }
            return array;
        }
    }
}
