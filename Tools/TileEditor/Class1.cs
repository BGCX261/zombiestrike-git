using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using SGP;

namespace TileEditor
{
    class TileSet
    {
        private Rectangle tileRect;

        public Rectangle TileRect
        {
            get { return tileRect; }

            set { tileRect = value; }
        }

        private int id;

        public int Id
        {
            get { return id; }

            set { id = value; }
        }

        public int X
        {
            get { return tileRect.X; }

            set { tileRect.X = value; }
        }

        public int Y
        {
            get { return tileRect.Y; }

            set { tileRect.Y = value; }
        }


        public int TileWidth
        {
            get { return tileRect.Width; }

            set { tileRect.Width = value;}
        }

        public int TileHeight
        {
            get { return tileRect.Height; }

            set { tileRect.Height = value; }
        }

        private int objectID;

        public int ObjectID
        {
            get { return objectID; }

            set { objectID = value; }
        }

        public enum MyEnum
        {
            BLANK,UITILE,ITILE,PLAYER,SPAWNER,ALARM, KEYCARD, KEYPAD, TERMINAL, ENERGY, STAMINA, SHADOWS
        };

    }

    class Layer
    {



       public List<TileSet> tiles;

       private bool isvisisble;

       public bool IsVisible
       {
           get { return isvisisble; }
           set { isvisisble = value; }
       }

    }

    class Collidable
    {

        Point m_ptPostion;

        public Point PtPostion
        {
            get { return m_ptPostion; }
            set { m_ptPostion = value; }
        }

        int id;

        public int Id
        {
            get { return id; }
            set { id = value; }
        }

        int index;

        public int Index
        {
            get { return index; }
            set { index = value; }
        }
       public List<string> eventTrigger = new List<string>();

       
    }


    public class Tile
    {
        public Tile()
        {


        }

        public Rectangle sRect = new Rectangle();

        private Point m_ptPosition;

        public int X
        {
            get { return m_ptPosition.X; }
            set { m_ptPosition.X = value; }
        }
        public int Y
        {
            get { return m_ptPosition.Y; }
            set { m_ptPosition.Y = value; }
        }
    }

    public class Grid
    {

        public Grid()
        {



        }

        private CSGP_Direct3D grid = CSGP_Direct3D.GetInstance();

        public virtual void DrawGrid(Point pos, Size tileSize, int numRows, int numCols)
        {
            Point start = new Point();
            Point finish = new Point();

            start.X = pos.X;
            finish.X = pos.X + numCols * tileSize.Width;

            for (int currCell = 0; currCell < numRows; currCell++)
            {
                start.Y = pos.Y + currCell * tileSize.Height;
                finish.Y = start.Y;
                grid.DrawLine(start.X, start.Y, finish.X, finish.Y, Color.Black, 3);
            }

            start.Y = pos.Y;
            finish.Y = pos.Y + numRows * tileSize.Width;

            for (int currCell = 0; currCell < numCols; currCell++)
            {
                start.X = pos.X + currCell * tileSize.Width;
                finish.X = start.X;
                grid.DrawLine(start.X, start.Y, finish.X, finish.Y, Color.Black, 3);

            }

        }



    }



}


