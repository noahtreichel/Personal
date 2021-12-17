using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class Tool : iTool
    {
        private string name;
        private int quantity;
        private int availableQuantity;
        private int noBorrowings;

        public string Name // get and set the name of this tool
        {
            get { return name; }
            set { name = value; }
        }

        public int Quantity //get and set the quantity of this tool
        {
            get { return quantity; }
            set { int borrowedNum = quantity - availableQuantity; quantity = value; availableQuantity = quantity - borrowedNum; }
        }

        public int AvailableQuantity //get and set the quantity of this tool currently available to lend
        {
            get { return availableQuantity; }
            set { availableQuantity = value; }
        }

        public int NoBorrowings //get and set the number of times that this tool has been borrowed
        {
            get { return noBorrowings; }
            set { noBorrowings = value; }
        }

        public iMemberCollection GetBorrowers  //get all the members who are currently holding this tool
        {
            get;
        }

        public void addBorrower(iMember aMember) //add a member to the borrower list
        {

        }

        public void deleteBorrower(iMember aMember) //delte a member from the borrower list
        {

        }

        public override string ToString() //return a string containning the name and the available quantity quantity this tool 
        {
            return ($"Name: {Name}, Quantity: {AvailableQuantity}");
        }
    }
}
