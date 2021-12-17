using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assignment
{
    class MemberCollection : iMemberCollection
    {
        private int number;
        // Instantiation of the Binary Search Tree to store the members
        private BSTree memberCollection = new BSTree();

        public int Number // get the number of members in the community library
        {
            get { return number; }
        }

        public void add(Member aMember) //add a new member to this member collection, make sure there are no duplicates in the member collection
        {
            memberCollection.Insert(aMember);
            number++;
        }

        public void delete(Member aMember) //delete a given member from this member collection, a member can be deleted only when the member currently is not holding any tool
        {
            memberCollection.Delete(aMember);
            number--;
        }

        /**
        public Boolean search(iMember aMember) //search a given member in this member collection. Return true if this memeber is in the member collection; return false otherwise.
        {

        }
        */

        public Member[] toArray() //output the members in this collection to an array of iMember
        {
            return memberCollection.ItemArray;
        }
    }
}
