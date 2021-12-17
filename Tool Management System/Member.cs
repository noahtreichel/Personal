using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class Member : iMember, IComparable
    {
        private string firstName;
        private string lastName;
        private string contactNumber;
        private string pin;

        public string FirstName  //get and set the first name of this member
        {
            get { return firstName; }
            set { firstName = value; }
        }
        public string LastName //get and set the last name of this member
        {
            get { return lastName; }
            set { lastName = value; }
        }

        public string ContactNumber //get and set the contact number of this member
        {
            get { return contactNumber; }
            set { contactNumber = value; }
        }

        public string PIN //get and set the password of this member
        {
            get { return pin; }
            set { pin = value; }
        }

        public string[] Tools //get a list of tools that this memebr is currently holding
        {
            get;
        }

        public void addTool(iTool aTool) //add a given tool to the list of tools that this member is currently holding
        {

        }

        public void deleteTool(iTool aTool) //delete a given tool from the list of tools that this member is currently holding
        {

        }

        public override string ToString() //return a string containing the first name, lastname, and contact phone number of this memeber
        {
            return ($"{FirstName} {LastName}, {ContactNumber}");
        }

        // A function that compares members by their first and last names
        public int CompareTo(object obj)
        {
            Member otherMember = obj as Member;
            int position = String.Compare(this.FirstName, otherMember.FirstName);

            if (position == 0)
            {
                position = String.Compare(this.LastName, otherMember.LastName);
                return position;
            }
            else
            {
                return position;
            }
        }
    }
}
