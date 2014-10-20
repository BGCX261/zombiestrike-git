using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;


namespace TileEditor
{
    public enum AIStates { PATROL, ALERT, WAIT, TO_ALARM, SEARCHING, GO_HOME, ATTACK };

    class Object
    {

       
        public List<string> events = new List<string>();
        public List<int> waypoints = new List<int>();


        private int numEnemies = 0;

        public int NumEnemies
        {
            get { return numEnemies; }
            set { numEnemies = value; }
        }

        private int m_nType = -1;

        public int Type
        {
            get { return m_nType; }
            set { m_nType = value; }
        }

        public Object(int type)
        {
            m_nType = type;
        }

        public Rectangle sRect = new Rectangle();

        Point position;

        public Point Position
        {
            get { return position; }
            set { position = value; }
        }

        string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        private int state;

        public int State
        {
            get { return state; }
            set { state = value; }
        }

    }


    class Node
    {

        Point position;

        public Point Position
        {
            get { return position; }
            set { position = value; }
        }

        private int index;

        public int Index
        {
            get { return index; }
            set { index = value; }
        }

        private string tag;

        public string Tag
        {
            get { return tag; }
            set { tag = value; }
        }


       public List<int> edges = new List<int>();



    }
}
