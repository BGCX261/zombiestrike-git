using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SGP;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Linq;

namespace TileEditor
{

    public partial class Form1 : Form
    {
   

        
      
      

       // int tileID = 0;
        int tilesDraw = 0;
        



        Rectangle selectedRect;
       

        // SGP C# wrappers
        CSGP_Direct3D m_D3D = CSGP_Direct3D.GetInstance();
        CSGP_TextureManager m_TM = CSGP_TextureManager.GetInstance();

        bool isPressed = false;

        // Application running flag property
        public bool Running
        {
            get;
            set;
        }
        int CurrTool = 0;
        Grid worldGrid = new Grid();

        //List of Nodes for pathfinding


        List<Node> navGraph =  new  List<Node>();


        //Stuff for tiles
        List<Object> ListofObjects = new List<Object>();

        Size tSetSize = new Size(4, 7);
        Size tileSize = new Size(64, 64);
        Grid tileGrid = new Grid();
        int D3DTileId = -1;
        Tile selectedTile = new Tile();
        string filePath = "";
        string objFilePath;


        //Stuff for Objects

        Size objectSetSize = new Size(8, 8);
        Tile selectedObject = new Tile();
        Grid objectGrid = new Grid();
        int D3DObjID = -1;
        int objectID = 0;
        List<string> names = new List<string>();
        


        //Map stuff

        Tile[,] TileMap = new Tile[8, 8];

        Size mapSize = new Size(8, 8);
        Grid mapGrid = new Grid();
        Point mapOrigin = new Point(0, 0);

        List<Collidable> collisionObjects = new List<Collidable>();
        int collisionID = 0;


        ////////////////////////////////////////////////////////////////////////////////
        // CONSTRUCTOR
        //	- initializes the form controls & moves the working directory
        ////////////////////////////////////////////////////////////////////////////////
        public Form1()
        {
            // Initialize form components
            InitializeComponent();
            this.Focus();
            CSGP_Direct3D m_D3D = CSGP_Direct3D.GetInstance();
            CSGP_TextureManager m_TM = CSGP_TextureManager.GetInstance();
            m_D3D.Initialize(tilePanel, true);
            m_D3D.AddRenderTarget(worldPanel);
            m_D3D.AddRenderTarget(objectPanel);
            
            m_TM.Initialize(m_D3D.Device, m_D3D.Sprite);

            //filePath = "tset.png";
            //D3DTileId = m_TM.LoadTexture(filePath);
            //D3DObjID = m_TM.LoadTexture(filePath);

            // Change the Current Working Directory out of the 'bin\Debug\' or 'bin\Release\' folders
            string dir = System.IO.Directory.GetCurrentDirectory();
            int index = dir.LastIndexOf("bin\\Debug");
            if (index != -1)
                dir = dir.Remove(index);
            index = dir.LastIndexOf("bin\\Release");
            if (index != -1)
                dir = dir.Remove(index);

            System.IO.Directory.SetCurrentDirectory(dir);




            // Store the Current Working Directory in the text box
            //txtWorkingDirectory.Text = System.IO.Directory.GetCurrentDirectory();
        }

        ////////////////////////////////////////////////////////////////////////////////
        // Form1_FormClosing
        //	- triggered when the form is about to be closed (after the close button is clicked)
        //	- sets flag to tell main loop to quit
        ////////////////////////////////////////////////////////////////////////////////
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Running = false;
        }


        ////////////////////////////////////////////////////////////////////////////////
        // Initialize
        //	- initialize the SGP wrappers
        //	- load assets
        ////////////////////////////////////////////////////////////////////////////////
        public void Initialize()
        {
            // Access the SGP Wrapper singletons
            m_D3D.Initialize(splitContainer1.Panel1, true);
           // m_D3D.AddRenderTarget(splitContainer1.Panel2);
            m_TM.Initialize(m_D3D.Device, m_D3D.Sprite);
            comboBoxAIStates.DataSource = Enum.GetValues(typeof(AIStates));
           
            // Load assets
           // m_nTileSheetID = m_TM.LoadTexture("resource/graphics/SGD_Tank.png", Color.FromArgb(255, 0, 0, 0).ToArgb()); // w/o color key
          //  tilesDraw = m_TM.LoadTexture("../../resource/graphics/tset.png");
            //filePath = "resource\\graphics;
            //string fullPath = System.IO.Directory.GetCurrentDirectory();
            //string[] split = fullPath.Split(new Char[] { '\\', '\n' });

            //string tempString = "";
            //for (int i = 0; i < split.Length - 2; i++)
            //{
            //    tempString += split[i] + "\\";
            //}
            //tempString += filePath;


            //D3DTileId = m_TM.LoadTexture(tempString);


            objFilePath = "Resources\\ObjectImage.png";


            D3DObjID = m_TM.LoadTexture(objFilePath);

            //m_TileSet = new TileSet[mapSize.Width, mapSize.Height];
            //r_TileSet = new TileSet[renderTiles.Width, renderTiles.Height];

            names.Add("PSpwn");
            names.Add("ESpwn");
            names.Add("Goal");
            names.Add("ChkPnt");
            names.Add("Door");
            names.Add("Door\nOpener");
            for (int i = 6; i < objectSetSize.Width * objectSetSize.Height; i++)
            {
            names.Add("Empty");
                
            }


            numericMapRows.Value = mapSize.Height;
            numericMapCol.Value = mapSize.Width;
            for (int i = 0; i < mapSize.Width; i++)
            {
                for (int j = 0; j < mapSize.Height; j++)
                {
                    TileMap[i, j] = new Tile();
                    TileMap[i, j].X = tileSize.Width * i;
                    TileMap[i, j].Y = tileSize.Height * j;
                    TileMap[i, j].sRect = new Rectangle(0, 0, tileSize.Width, tileSize.Height);
                 


                   
                }
            }

         

            // Anything else
            tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);
            objectPanel.AutoScrollMinSize = new Size(objectSetSize.Width * tileSize.Width, objectSetSize.Height * tileSize.Height);

            splitContainer1.Panel2.AutoScrollMinSize = new Size(mapSize.Width * tileSize.Width, mapSize.Height * tileSize.Height);

            
            

            // success
            Running = true;
        }



        ////////////////////////////////////////////////////////////////////////////////
        // Terminate
        //	- unload assets
        //	- terminate the SGP wrappers
        ////////////////////////////////////////////////////////////////////////////////
        public void Terminate()
        {
            // Shut down the wrappers
            m_TM.Terminate();
            m_D3D.Terminate();
        }




        ////////////////////////////////////////////////////////////////////////////////
        // Update
        //	- update timers & game entities
        ////////////////////////////////////////////////////////////////////////////////
        public void Refresh()
        {
            numericTileWidth.Value = tileSize.Width;
            numericTileHeight.Value = tileSize.Height;
            numericSheetWidth.Value = tSetSize.Width;
            numericUpDownSheetHeight.Value = tSetSize.Height;
           
            numericObjID.Value = selectedObject.Y * objectSetSize.Width + selectedObject.X;
            if (Objects.SelectedIndex != -1)
                numericNumEnemies.Value = ListofObjects[Objects.SelectedIndex].NumEnemies;
            else
                numericNumEnemies.Value = (decimal)0.0;

            textBoxNumEvents.Text = EventTable.Items.Count.ToString();

            if (Objects.SelectedIndex != -1)
                textBoxStartState.Text = ListofObjects[Objects.SelectedIndex].State.ToString();
            else
                textBoxStartState.Text = "";
            

        }


        ////////////////////////////////////////////////////////////////////////////////
        // Render
        //	- render game entities
        ////////////////////////////////////////////////////////////////////////////////
        public void RenderAll()
        {
            if (D3DTileId != -1)
                DrawTileSheet();
            
            DrawObjectSheet();
            DrawTileMap();

           
           
        }

        private void Form1_FormClosing_1(object sender, FormClosingEventArgs e)
        {
            Running = false;
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            if (m_D3D.Device != null)
            {
                m_D3D.Resize(worldPanel, worldPanel.ClientSize.Width, worldPanel.ClientSize.Height, true);
                m_D3D.Resize(tilePanel, tilePanel.ClientSize.Width, tilePanel.ClientSize.Height, true);
                m_D3D.Resize(objectPanel, objectPanel.ClientSize.Width, objectPanel.ClientSize.Height, true);

            }
            RenderAll();
        }

        private void Form1_Scroll(object sender, ScrollEventArgs e)
        {
            if (m_D3D.Device != null)
            {
                m_D3D.Resize(worldPanel, worldPanel.ClientSize.Width, worldPanel.ClientSize.Height, true);
                m_D3D.Resize(tilePanel, tilePanel.ClientSize.Width, tilePanel.ClientSize.Height, true);
                m_D3D.Resize(objectPanel, objectPanel.ClientSize.Width, objectPanel.ClientSize.Height, true);
            }

            RenderAll();
        }



       

     



        private void tilePanel_MouseClick(object sender, MouseEventArgs e)
        {
            Point offset = tilePanel.AutoScrollPosition;

            if (e.Button == MouseButtons.Left)
            {
                // Calculate the selected tile.
                if ((e.X / tileSize.Width) < tSetSize.Width && (e.Y / tileSize.Height) < tSetSize.Height)
                {
                    selectedTile.X = (e.X - offset.X) / tileSize.Width;
                    selectedTile.Y = (e.Y - offset.Y) / tileSize.Height;
                }
            }
        }

        private void objectPanel_MouseClick(object sender, MouseEventArgs e)
        {
            Point offset = objectPanel.AutoScrollPosition;

            if (e.Button == MouseButtons.Left)
            {
                if ((e.X / tileSize.Width) < objectSetSize.Width && (e.Y / tileSize.Height) < objectSetSize.Height)
                {
                    selectedObject.X = (e.X - offset.X) / tileSize.Width;
                    selectedObject.Y = (e.Y - offset.Y) / tileSize.Height;

                    objectID = selectedObject.Y * objectSetSize.Width + selectedObject.X;
                    textBoxObjName.Text = names[objectID];
                }
            }
        }


   

        


        //private void loadTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        //{
        //    OpenFileDialog fDialogue = new OpenFileDialog();

        //    fDialogue.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + fDialogue.FileName);

        //    fDialogue.Filter = "BMP Files|*.bmp|JPEG |*.jpg";

        //    if (DialogResult.OK == fDialogue.ShowDialog())
        //    {
        //        if (tilesDraw != null)
        //        {
        //            m_TM.UnloadTexture(tilesDraw);
        //        }

                

        //        m_TM.LoadTexture(fDialogue.FileName);

        //       //Point xy = new Point(tileSize.Width * renderTiles.Width, tileSize.Height * renderTiles.Height);

        //        tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);
        //       worldPanel.AutoScrollMinSize = new Size(mapSize.Width * tileSize.Width, mapSize.Height * tileSize.Height);
        //       objectPanel.AutoScrollMinSize = new Size(objectSetSize.Width * tileSize.Width, objectSetSize.Height * tileSize.Height);


        //       // splitContainer1.Panel1.HorizontalScroll.Minimum = renderTiles.Width * tileSize.Width;
        //       // splitContainer1.Panel1.VerticalScroll.Minimum = renderTiles.Height * tileSize.Height;


                
        //    }


        //}


      

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to exit? Any unsaved data will be lost.", "ALERT", MessageBoxButtons.OKCancel, MessageBoxIcon.Exclamation) == DialogResult.OK)
            {
                Running = false;
                this.Close();
            }
        }


        private void Layers_SelectedIndexChanged(object sender, EventArgs e)
        {
            CurrTool = (int)Layers.SelectedIndex;
        }

        private void deleteSelected_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                int selected = Objects.SelectedIndex;
                ListofObjects.RemoveAt(Objects.SelectedIndex);
                Objects.Items.RemoveAt(Objects.SelectedIndex);
                EventTable.Items.Clear();
                WayPoints.Items.Clear();
                if (selected != 0)
                    Objects.SelectedIndex = selected - 1;
            }
        }

        private void deleteAll_Click(object sender, EventArgs e)
        {
            ListofObjects.Clear();
            Objects.Items.Clear();
        }

        private void addEvent_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1 && eventString.Text != "")
            {
               
                    EventTable.Items.Add(eventString.Text.ToString());
                    ListofObjects[Objects.SelectedIndex].events.Add(eventString.Text);
                    numericNumEnemies.Value = ListofObjects[Objects.SelectedIndex].NumEnemies;
            }
            
        }

        private void Objects_SelectedIndexChanged(object sender, EventArgs e)
        {
            EventTable.Items.Clear();
            WayPoints.Items.Clear();
            if(Objects.SelectedIndex != -1)
            {
                for (int i = 0; i < ListofObjects[Objects.SelectedIndex].events.Count(); i++)
                      EventTable.Items.Add(ListofObjects[Objects.SelectedIndex].events[i].ToString());

                for (int i = 0; i < ListofObjects[Objects.SelectedIndex].waypoints.Count(); i++)
                    WayPoints.Items.Add(ListofObjects[Objects.SelectedIndex].waypoints[i].ToString());

               
            }
        }

       

        private void CollisionCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (CollisionCheckBox.Checked == true)
            {
                collisionID = 0;
                TriggerCheckBox.Checked = false;
                checkBox1.Checked = false;

            }
        }

        private void TriggerCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (TriggerCheckBox.Checked == true)
            {
                collisionID = 1;
                CollisionCheckBox.Checked = false;
                checkBox1.Checked = false;
            }
        }

       private void DrawTileSheet()
        {
            Point tileOffset = tilePanel.AutoScrollPosition;

            m_D3D.Clear(tilePanel, Color.White);
            m_D3D.DeviceBegin();
            m_D3D.SpriteBegin();

            m_TM.Draw(D3DTileId, tileOffset.X, tileOffset.Y);

            tileGrid.DrawGrid(tileOffset, tileSize, tSetSize.Height, tSetSize.Width);

            Rectangle temp = Rectangle.Empty;
            temp.X = selectedTile.X * tileSize.Width + tileOffset.X;
            temp.Y = selectedTile.Y * tileSize.Height + tileOffset.Y;
            temp.Width = tileSize.Width;
            temp.Height = tileSize.Height;

            m_D3D.DrawHollowRect(temp, Color.Red, 3);

            // End left panel rendering
            m_D3D.SpriteEnd();
            m_D3D.DeviceEnd();
            m_D3D.Present();
        }
       
        private void DrawObjectSheet()
       {
           Point objectOffset = objectPanel.AutoScrollPosition;

           m_D3D.Clear(objectPanel, Color.White);
           m_D3D.DeviceBegin();
           m_D3D.SpriteBegin();

           //draw tilesheet
           m_TM.Draw(D3DObjID, objectOffset.X, objectOffset.Y);

           //draw grid
           objectGrid.DrawGrid(objectOffset, tileSize, objectSetSize.Height, objectSetSize.Width);
           
           for (int row = 0; row < objectSetSize.Height; row++)
           {
               for (int col = 0; col < objectSetSize.Width; col++)
               {
                   int ID = row * objectSetSize.Width + col;


                   if (ID < names.Count())
                       m_D3D.DrawText(names[ID], col * tileSize.Width + objectOffset.X, row * tileSize.Height + objectOffset.Y, Color.Black);
                       
                   
                           
                   

                   m_D3D.DrawText(ID.ToString(), col * tileSize.Width + objectOffset.X , row * tileSize.Height + objectOffset.Y + tileSize.Height-20, Color.Black);

               }
           }
           //red selection box
           Rectangle obj_select = Rectangle.Empty;
           obj_select.X = selectedObject.X * tileSize.Width + objectOffset.X;
           obj_select.Y = selectedObject.Y * tileSize.Height + objectOffset.Y;
           obj_select.Width = tileSize.Width;
           obj_select.Height = tileSize.Height;

           m_D3D.DrawHollowRect(obj_select, Color.Red, 3);

           m_D3D.SpriteEnd();
           m_D3D.DeviceEnd();
           m_D3D.Present();


       }

        private void DrawTileMap()
        {

            m_D3D.Clear(worldPanel, Color.White);
            m_D3D.DeviceBegin();
            m_D3D.SpriteBegin();

            Point worldOffset = worldPanel.AutoScrollPosition;

            
            //Start drawing tiles
            for (int x = 0; x < mapSize.Width; x++)
            {
                for (int y = 0; y < mapSize.Height; y++)
                {
                    // Calculate where the tile is going to be drawn in the map.
                    Rectangle destRect = Rectangle.Empty;
                    destRect.X = x * tileSize.Width + worldOffset.X;
                    destRect.Y = y * tileSize.Height + worldOffset.Y;
                    destRect.Size = tileSize;
                    
                    // Draw the tile. 
                    if (destRect.X <= worldPanel.Width && destRect.X >= worldPanel.Left -tileSize.Width && destRect.Y <= worldPanel.Height && destRect.Y >= worldPanel.Top - tileSize.Height)
                    {
                        try
                        {
                            m_TM.Draw(D3DTileId, destRect.X, destRect.Y, 1.0f, 1.0f, TileMap[x, y].sRect);
                        }
                        catch
                        { }
                    }
                }
            }

           

            for (int i = 0; i < ListofObjects.Count; i++)
            {
                // Calculate where the tile is going to be drawn in the map.
                Rectangle destRect = Rectangle.Empty;
                destRect.X = ListofObjects[i].Position.X * tileSize.Width + worldOffset.X;
                destRect.Y = ListofObjects[i].Position.Y * tileSize.Height + worldOffset.Y;
                destRect.Size = tileSize;

                m_TM.Draw(D3DObjID, destRect.X, destRect.Y, 1.0f, 1.0f, ListofObjects[i].sRect);
                m_D3D.DrawText(ListofObjects[i].Type.ToString(), ((int)destRect.X), ((int)destRect.Y + (tileSize.Height - 20)), Color.Black);
                m_D3D.DrawText(ListofObjects[i].Name, (int)destRect.X , (int)destRect.Y, Color.Black);

            }

            mapGrid.DrawGrid(worldOffset, tileSize, mapSize.Height, mapSize.Width);


            //Start drawing collision tiles
            for (int i = 0; i < collisionObjects.Count; i++)
            {
                Rectangle destRect = Rectangle.Empty;
                destRect.X = collisionObjects[i].PtPostion.X * tileSize.Width + worldOffset.X;
                destRect.Y = collisionObjects[i].PtPostion.Y * tileSize.Height + worldOffset.Y;
                destRect.Width = tileSize.Width;
                destRect.Height = tileSize.Height;

                if (collisionObjects[i].Id == 0)
                    m_D3D.DrawRect(destRect, Color.FromArgb(125, 255, 0, 0));


                else
                    m_D3D.DrawRect(destRect, Color.FromArgb(125, 255, 255, 0));

            }

            //Start drawing nav graph tiles
            for (int i = 0; i < navGraph.Count; i++)
            {
                Rectangle destRect = Rectangle.Empty;
                destRect.X = navGraph[i].Position.X * tileSize.Width + worldOffset.X;
                destRect.Y = navGraph[i].Position.Y * tileSize.Height + worldOffset.Y;
                destRect.Width = tileSize.Width;
                destRect.Height = tileSize.Height;

               
                    m_D3D.DrawRect(destRect, Color.FromArgb(125, 0, 0, 255));
                    m_D3D.DrawText(navGraph[i].Index.ToString(), navGraph[i].Position.X * tileSize.Width + worldOffset.X, navGraph[i].Position.Y * tileSize.Height + worldOffset.Y, Color.White);


              

            }



            m_D3D.SpriteEnd();
            m_D3D.DeviceEnd();
            m_D3D.Present();

        }


        private void worldPanel_MouseDown(object sender, MouseEventArgs e)
        {
            isPressed = true;


            Point offset = worldPanel.AutoScrollPosition;


            int x = (e.X - offset.X) / tileSize.Width;
            int y = (e.Y - offset.Y) / tileSize.Height;

            if (x < mapSize.Width && y < mapSize.Height)
            {
                switch (CurrTool)
                {
                    case 0: // Tile
                        {
                            if (D3DTileId != -1)
                            {
                                TileMap[x, y].sRect.X = selectedTile.X * tileSize.Width;
                                TileMap[x, y].sRect.Y = selectedTile.Y * tileSize.Height;
                                TileMap[x, y].sRect.Width = tileSize.Width;
                                TileMap[x, y].sRect.Height = tileSize.Height;

                                TileMap[x, y].X = x * tileSize.Width;
                                TileMap[x, y].Y = y * tileSize.Height;
                            }
                                
                           


                        }
                        break;
                    case 1: // Objects
                        {
                            Point test = new Point(x, y);
                            bool taken = false;
                            for (int i = 0; i < ListofObjects.Count; i++)
                            {
                                if (ListofObjects[i].Position == test)
                                    taken = true; // or break??? maybe return?? then we wouldnt need the "taken" bool or the following if check
                            }
                            if (!taken)
                            {
                                Object temp = new Object(objectID);
                                temp.Position = new Point(x, y);
                                temp.sRect.X = selectedObject.X * tileSize.Width;
                                temp.sRect.Y = selectedObject.Y * tileSize.Height;
                                temp.sRect.Width = tileSize.Width;
                                temp.sRect.Height = tileSize.Height;
                                temp.Name = names[objectID];

                                for (int currObj = 0; currObj < ListofObjects.Count; currObj++)
                                {
                                    if (ListofObjects[currObj].Type == objectID)
                                        temp.Name = ListofObjects[currObj].Name;
                                }
                                ListofObjects.Add(temp);
                                Objects.Items.Add(names[objectID] + objectID);
                                Objects.SelectedIndex = ListofObjects.Count - 1;
                            }
                        }
                        break;
                    //case 2: // Line
                    //    {
                    //        //Line temp = new Line();
                    //        //temp.Point1 = new Point(x, y);
                    //        //pathfinderList.Add(temp);
                    //        //listBoxLines.Items.Add("Line " + listBoxLines.Items.Count);
                    //        //listBoxLines.SelectedIndex = listBoxLines.Items.Count - 1;
                    //    }
                    //    break;
                    //case 3: // Entry
                    //    {
                    //        //Entry temp = new Entry();
                    //        //temp.PositionInRoom = new Point(x, y);
                    //        //pathfinderList[listBoxLines.SelectedIndex].Entries.Add(temp);
                    //        //listBoxEntries.Items.Add("Entry " + listBoxEntries.Items.Count);
                    //        //listBoxEntries.SelectedIndex = listBoxEntries.Items.Count - 1;
                    //    }
                    //    break;
                    case 2: // collision
                        {
                            if (collisionID != 2)
                            {
                                bool taken = false;
                                for (int i = 0; i < collisionObjects.Count; i++)
                                {
                                    if (collisionObjects[i].PtPostion == new Point(x, y))
                                        taken = true;
                                }
                                if (!taken)
                                {
                                    Collidable temp = new Collidable();
                                    temp.PtPostion = new Point(x, y);
                                    temp.Id = collisionID;
                                    collisionObjects.Add(temp);
                                    temp.Index = collisionObjects.Count;
                                    if (temp.Id == 0)
                                        listBoxCollisionObjects.Items.Add("Collider" + temp.Index);
                                    else
                                        listBoxCollisionObjects.Items.Add("Trigger" + temp.Index);
    	                                
                                }
                            }
                            else
                            {
                                for (int i = 0; i < collisionObjects.Count; i++)
                                {
                                    if (collisionObjects[i].PtPostion == new Point(x, y))
                                    {
                                        collisionObjects.RemoveAt(i);
                                        listBoxCollisionObjects.Items.RemoveAt(i);

                                    }
                                }
                            }
                        }
                        break;
                    case 3:
                        {
                            Point test = new Point(x, y);
                            bool taken = false;
                            for (int i = 0; i < navGraph.Count; i++)
                            {
                                if (navGraph[i].Position == test)
                                    taken = true; // or break??? maybe return?? then we wouldnt need the "taken" bool or the following if check
                            }
                            if (!taken)
                            {
                                Node temp = new Node();
                                temp.Position = new Point(x, y);
                                temp.Index = navGraph.Count();
                                navGraph.Add(temp);
                                PathNodes.Items.Add("Node " + temp.Index);
                                PathNodes.SelectedIndex = navGraph.Count - 1;
                            }

                        }
                        break;

                }

            }
        }

        private void worldPanel_MouseUp(object sender, MouseEventArgs e)
        {
            isPressed = false;
        }

        private void worldPanel_MouseMove(object sender, MouseEventArgs e)
        {
            Point offset = worldPanel.AutoScrollPosition;


            int x = (e.X - offset.X) / tileSize.Width;
            int y = (e.Y - offset.Y) / tileSize.Height;

            if (isPressed == true)
            {
                switch (CurrTool)
                {
                    case 0: // Tile
                        {
                            if (x < mapSize.Width && y < mapSize.Height)
                            {
                                try
                                {
                                    TileMap[x, y].sRect.X = selectedTile.X * tileSize.Width;
                                    TileMap[x, y].sRect.Y = selectedTile.Y * tileSize.Height;
                                    TileMap[x, y].sRect.Width = tileSize.Width;
                                    TileMap[x, y].sRect.Height = tileSize.Height;
                                    TileMap[x, y].X = x * tileSize.Width;
                                    TileMap[x, y].Y = y * tileSize.Height;

                                }
                                catch { }
                            }
                        }
                        break;
                    case 1: // Objects
                        if (x < mapSize.Width && y < mapSize.Height)
                        {
                            Point test = new Point(x, y);
                            bool taken = false;
                            for (int i = 0; i < ListofObjects.Count; i++)
                            {
                                if (ListofObjects[i].Position == test)
                                    taken = true; // or break??? maybe return?? then we wouldnt need the "taken" bool or the following if check
                            }
                            if (!taken)
                            {
                                Object obj = new Object(objectID);
                                obj.Position = new Point(x, y);
                                obj.sRect.X = selectedObject.X * tileSize.Width;
                                obj.sRect.Y = selectedObject.Y * tileSize.Height;
                                obj.sRect.Width = tileSize.Width;
                                obj.sRect.Height = tileSize.Height;
                                for (int currObj = 0; currObj < ListofObjects.Count; currObj++)
                                {
                                    if (ListofObjects[currObj].Type == objectID)
                                        obj.Name = ListofObjects[currObj].Name;
                                }
                                ListofObjects.Add(obj);
                                Objects.Items.Add(names[objectID] + objectID);
                                Objects.SelectedIndex = Objects.Items.Count - 1;
                            }
                        }
                        break;
                    case 2: // collision
                        {
                            if (collisionID != 2)
                            {
                                bool taken = false;
                                for (int i = 0; i < collisionObjects.Count; i++)
                                {
                                    if (collisionObjects[i].PtPostion == new Point(x, y))
                                        taken = true;
                                }
                                if (!taken)
                                {
                                    Collidable temp = new Collidable();
                                    temp.PtPostion = new Point(x, y);
                                    temp.Id = collisionID;
                                    collisionObjects.Add(temp);
                                    temp.Index = collisionObjects.Count;
                                    if (temp.Id == 0)
                                        listBoxCollisionObjects.Items.Add("Collider" + temp.Index);
                                    else
                                        listBoxCollisionObjects.Items.Add("Trigger" + temp.Index);
                                }
                            }
                            else
                            {
                                for (int i = 0; i < collisionObjects.Count; i++)
                                {
                                    if (collisionObjects[i].PtPostion == new Point(x, y))
                                    {
                                        collisionObjects.RemoveAt(i);
                                        listBoxCollisionObjects.Items.RemoveAt(i);

                                    }
                                }
                            }
                        }
                        break;
                    case 3:
                        {
                            Point test = new Point(x, y);
                            bool taken = false;
                            for (int i = 0; i < navGraph.Count; i++)
                            {
                                if (navGraph[i].Position == test)
                                    taken = true; // or break??? maybe return?? then we wouldnt need the "taken" bool or the following if check
                            }
                            if (!taken)
                            {
                                Node temp = new Node();
                                temp.Position = new Point(x, y);
                                temp.Index = navGraph.Count();
                                navGraph.Add(temp);
                                PathNodes.Items.Add("Node " + temp.Index);
                                PathNodes.SelectedIndex = navGraph.Count - 1;
                            }
                        }
                        break;
                }
            }
        }
        

        private void numericTileWidth_ValueChanged(object sender, EventArgs e)
        {
            tileSize.Width = (int)numericTileWidth.Value;
            tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);
        }

        private void numericTileHeight_ValueChanged(object sender, EventArgs e)
        {
            tileSize.Height = (int)numericTileHeight.Value;
            tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);

        }

        private void numericSheetWidth_ValueChanged(object sender, EventArgs e)
        {
            tSetSize.Width = (int)numericSheetWidth.Value;
            tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);

        }

        private void numericUpDownSheetHeight_ValueChanged(object sender, EventArgs e)
        {
            tSetSize.Height = (int)numericUpDownSheetHeight.Value;
            tilePanel.AutoScrollMinSize = new Size(tSetSize.Width * tileSize.Width, tSetSize.Height * tileSize.Height);

        }


        private void numericMapRows_ValueChanged_1(object sender, EventArgs e)
        {

         
	        

        }

        private void numericMapCol_ValueChanged_1(object sender, EventArgs e)
        {
           

        }


        private void buttonDeleteEvent_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                int selected = EventTable.SelectedIndex;
                ListofObjects[Objects.SelectedIndex].events.RemoveAt(EventTable.SelectedIndex);
                EventTable.Items.RemoveAt(EventTable.SelectedIndex);
                if (selected != 0)
                    EventTable.SelectedIndex = selected - 1;
            }
        }

        private void buttonDeleteAllWaypoints_Click(object sender, EventArgs e)
        {
            WayPoints.Items.Clear();
            if (Objects.SelectedIndex != -1)
                ListofObjects[Objects.SelectedIndex].waypoints.Clear();
                
            
        }

        private void buttonDeleteAllEvents_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                EventTable.Items.Clear();
                ListofObjects[Objects.SelectedIndex].events.Clear();
            }
            
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            collisionID = 2;
            TriggerCheckBox.Checked = false;
            CollisionCheckBox.Checked = false;
        }

        private void listBoxCollisionObjects_SelectedIndexChanged(object sender, EventArgs e)
        {
            listBoxTileEvents.Items.Clear();

            if (listBoxCollisionObjects.SelectedIndex != -1)
            {
                for (int i = 0; i < collisionObjects[listBoxCollisionObjects.SelectedIndex].eventTrigger.Count(); i++)
                    listBoxTileEvents.Items.Add(collisionObjects[listBoxCollisionObjects.SelectedIndex].eventTrigger[i].ToString());

                
            }
        }

        private void buttonDeleteWaypoint_Click(object sender, EventArgs e)
        {
            if (WayPoints.SelectedIndex != -1)
            {
                int selected = WayPoints.SelectedIndex;
                ListofObjects[Objects.SelectedIndex].waypoints.RemoveAt(WayPoints.SelectedIndex);
                WayPoints.Items.RemoveAt(WayPoints.SelectedIndex);
                if (selected != 0)
                    WayPoints.SelectedIndex = selected - 1;
            }
        }

        private void listBoxPathNodes_SelectedIndexChanged(object sender, EventArgs e)
        {
            listBoxEdges.Items.Clear();
           
            if (PathNodes.SelectedIndex != -1)
            {
                for (int i = 0; i < navGraph[PathNodes.SelectedIndex].edges.Count(); i++)
                    listBoxEdges.Items.Add(navGraph[PathNodes.SelectedIndex].edges[i].ToString());

                textBoxCurrNodeTag.Text = navGraph[PathNodes.SelectedIndex].Tag;

               
            }
        }

        private void buttonDeletePathNode_Click(object sender, EventArgs e)
        {
            if (PathNodes.SelectedIndex != -1)
            {
                int selected = PathNodes.SelectedIndex;
                navGraph.RemoveAt(PathNodes.SelectedIndex);
                PathNodes.Items.RemoveAt(PathNodes.SelectedIndex);
                for (int i = selected; i < navGraph.Count; i++)
                {
                    PathNodes.Items.RemoveAt(i);
                    PathNodes.Items.Insert(i, "Node" + (navGraph[i].Index - 1).ToString());
                    navGraph[i].Index--;
                }

                if (selected != 0)
                    PathNodes.SelectedIndex = selected - 1;
            }
        }

        private void buttonDeleteAllNodes_Click(object sender, EventArgs e)
        {
            navGraph.Clear();
            PathNodes.Items.Clear();
            listBoxEdges.Items.Clear();
        }

        private void buttonAddEdge_Click(object sender, EventArgs e)
        {
            if (PathNodes.SelectedIndex != -1)
            {
                navGraph[PathNodes.SelectedIndex].edges.Add((int)numericEdgeIndex.Value);
                listBoxEdges.Items.Add((int)numericEdgeIndex.Value);
                int selected = listBoxEdges.Items.Count;

                if (selected != 0)
                    listBoxEdges.SelectedIndex = selected -1;
            }
        }

        private void buttonDeleteEdge_Click(object sender, EventArgs e)
        {
            if (listBoxEdges.SelectedIndex != -1)
            {
                int selected = listBoxEdges.SelectedIndex;
                navGraph[PathNodes.SelectedIndex].edges.RemoveAt(listBoxEdges.SelectedIndex);
                listBoxEdges.Items.RemoveAt(listBoxEdges.SelectedIndex);
                if (selected != 0)
                    listBoxEdges.SelectedIndex = selected - 1;
            }
        }

        private void buttonDeleteAllEdges_Click(object sender, EventArgs e)
        {
            if (PathNodes.SelectedIndex != -1)
            {
                int selected = PathNodes.SelectedIndex;
                navGraph[PathNodes.SelectedIndex].edges.Clear();
                listBoxEdges.Items.Clear();
                if (selected != 0)
                    PathNodes.SelectedIndex = selected - 1;
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\Resource\\Levels");

            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.DefaultExt = "xml";


            if (DialogResult.OK == dlg.ShowDialog())
            {
                System.Xml.XmlTextWriter writer = new System.Xml.XmlTextWriter(dlg.FileName, System.Text.Encoding.UTF8);
                writer.WriteStartDocument();
                writer.WriteRaw("\n");

                writer.WriteStartElement("Level");
                // Save out the fileName, without the path
                string fileName = new string('\0', 1);
                fileName = fileName.Remove(0);
                for (int i = 0; i < filePath.Length; i++)
                {
                    fileName = fileName.Insert(fileName.Length, filePath[i].ToString());
                    if (filePath[i] == '/' || filePath[i] == '\\')
                        fileName = fileName.Remove(0);
                }
                writer.WriteString(filePath);
               // writer.WriteEndAttribute();
             //   writer.WriteEndElement();
               // writer.WriteAttributeString("filename", fileName);
              
                writer.WriteRaw("\n\t");
                writer.WriteStartElement("world_info");
                writer.WriteAttributeString("width", tileSize.Width.ToString());
                writer.WriteAttributeString("height", tileSize.Height.ToString());
                writer.WriteAttributeString("worldWidth", mapSize.Width.ToString());
                writer.WriteAttributeString("worldHeight", mapSize.Height.ToString());
                writer.WriteAttributeString("tileSheetWidth", tSetSize.Width.ToString());
                writer.WriteAttributeString("tileSheetHeight", tSetSize.Height.ToString());


                writer.WriteString(filePath);
               // writer.WriteAttributeString("tileSet", filePath);
                writer.WriteEndElement();
                writer.WriteRaw("\n");

               
                Object tOBject = new Object(0);
                for (int i = 0; i < ListofObjects.Count; i++)
			    {
                    if (ListofObjects[i].Type == 0)
                    {

                        tOBject = ListofObjects[i];
                       
                        //ListofObjects.RemoveAt(i);

                    }
			    }
                writer.WriteStartElement("player_info");
              
                writer.WriteAttributeString("id", tOBject.Type.ToString());
                writer.WriteAttributeString("posX", tOBject.Position.X.ToString());
                writer.WriteAttributeString("posY", tOBject.Position.Y.ToString());
                writer.WriteAttributeString("numEvents", tOBject.events.Count.ToString());
                writer.WriteAttributeString("numWaypoints", tOBject.waypoints.Count.ToString());
                writer.WriteAttributeString("numEnemies", tOBject.NumEnemies.ToString());

                writer.WriteAttributeString("sRectleft", tOBject.sRect.Left.ToString());
                writer.WriteAttributeString("sRecttop", tOBject.sRect.Top.ToString());
                writer.WriteAttributeString("sRectwidth", tOBject.sRect.Width.ToString());
                writer.WriteAttributeString("sRectheight", tOBject.sRect.Height.ToString());

                for (int currWaypoint = 0; currWaypoint < tOBject.waypoints.Count(); currWaypoint++)
                {
                    string waypoint = "waypoint";
                    waypoint += currWaypoint.ToString();

                    writer.WriteAttributeString(waypoint, tOBject.waypoints[currWaypoint].ToString());

                }
                writer.WriteString(tOBject.Name);
                for (int currEvent = 0; currEvent < tOBject.events.Count(); currEvent++)
                {
                    writer.WriteRaw("/");
                    writer.WriteString(tOBject.events[currEvent].ToString());

                } 


                writer.WriteEndElement();
                writer.WriteRaw("\n");
               

                // Save the Tile Data
                writer.WriteStartElement("tile_list");
                writer.WriteRaw("\n");

                for (int y = 0; y < mapSize.Height; y++)
                {
                    for (int x = 0; x < mapSize.Width; x++)
                    {
                        writer.WriteRaw("\t\t");
                        writer.WriteStartElement("tile_info");
                        writer.WriteAttributeString("rposx", TileMap[x, y].sRect.Left.ToString());
                        writer.WriteAttributeString("rposy", TileMap[x, y].sRect.Top.ToString());
                        writer.WriteAttributeString("posX", TileMap[x, y].X.ToString());
                        writer.WriteAttributeString("posY", TileMap[x, y].Y.ToString());
                        writer.WriteEndElement();
                        writer.WriteRaw("\n");
                    }
                }
                writer.WriteRaw("\t");
                writer.WriteEndElement();
                writer.WriteRaw("\n\t");

                // Save the collision data
                writer.WriteStartElement("collsion_list");
                writer.WriteRaw("\n");
                for (int currCollidable = 0; currCollidable < collisionObjects.Count; currCollidable++)
                {
                    writer.WriteRaw("\t\t");
                    writer.WriteStartElement("collision_tile");
                    writer.WriteAttributeString("posX", collisionObjects[currCollidable].PtPostion.X.ToString());
                    writer.WriteAttributeString("posY", collisionObjects[currCollidable].PtPostion.Y.ToString());
                    writer.WriteAttributeString("type", collisionObjects[currCollidable].Id.ToString());
                    writer.WriteAttributeString("index", collisionObjects[currCollidable].Index.ToString());

                    for (int currEvent = 0; currEvent < collisionObjects[currCollidable].eventTrigger.Count(); currEvent++)
		        	{
                        writer.WriteString(collisionObjects[currCollidable].eventTrigger[currEvent].ToString());
                        writer.WriteRaw("/");
			          //writer.WriteAttributeString("event" + currEvent, collisionObjects[currCollidable].eventTrigger[currEvent].ToString());
			        }
                    

                    writer.WriteEndElement();
                    writer.WriteRaw("\n");
                }
                writer.WriteRaw("\t");
                writer.WriteEndElement();
                writer.WriteRaw("\n\t");

                // Save the pathfinding data
                writer.WriteStartElement("graph_list");
                writer.WriteRaw("\n");
                for (int i = 0; i < navGraph.Count; i++)
                {
                    writer.WriteRaw("\t\t");
                    writer.WriteStartElement("node_info");
                    writer.WriteAttributeString("posX", navGraph[i].Position.X.ToString());
                    writer.WriteAttributeString("posY", navGraph[i].Position.Y.ToString());
                    writer.WriteAttributeString("index", navGraph[i].Index.ToString());
                    writer.WriteAttributeString("numEdges", navGraph[i].edges.Count.ToString());
                    for (int j = 0; j < navGraph[i].edges.Count; j++)
                    {
                        writer.WriteAttributeString("edge" + j, navGraph[i].edges[j].ToString());
                    }
                    writer.WriteString(navGraph[i].Tag);
                   // writer.WriteRaw("\t\t");
                    writer.WriteEndElement();
                    writer.WriteRaw("\n");
                }
                writer.WriteRaw("\t");
                writer.WriteEndElement();
                writer.WriteRaw("\n\t");

                // Save the object data
                writer.WriteStartElement("objects_list");
                writer.WriteRaw("\n");
                for (int i = 0; i < ListofObjects.Count; i++)
                {
                    if (ListofObjects[i].Type == 0)
                        continue;
                    writer.WriteRaw("\t\t");
                    writer.WriteStartElement("object_info");
                    writer.WriteAttributeString("id", ListofObjects[i].Type.ToString());
                    writer.WriteAttributeString("posX", ListofObjects[i].Position.X.ToString());
                    writer.WriteAttributeString("posY", ListofObjects[i].Position.Y.ToString());
                    writer.WriteAttributeString("numEvents", ListofObjects[i].events.Count.ToString());
                    writer.WriteAttributeString("numWaypoints", ListofObjects[i].waypoints.Count.ToString());
                    writer.WriteAttributeString("numEnemies", ListofObjects[i].NumEnemies.ToString());
                    writer.WriteAttributeString("startState", ListofObjects[i].State.ToString());
                    writer.WriteAttributeString("sRectleft", ListofObjects[i].sRect.Left.ToString());
                    writer.WriteAttributeString("sRecttop", ListofObjects[i].sRect.Top.ToString());
                    writer.WriteAttributeString("sRectwidth", ListofObjects[i].sRect.Width.ToString());
                    writer.WriteAttributeString("sRectheight", ListofObjects[i].sRect.Height.ToString());

                    
                   
                    for (int currWaypoint = 0; currWaypoint < ListofObjects[i].waypoints.Count(); currWaypoint++)
			        {
                        string waypoint = "waypoint";
                        waypoint += currWaypoint.ToString();
                  
                        writer.WriteAttributeString(waypoint , ListofObjects[i].waypoints[currWaypoint].ToString());
			            
			        }
                    writer.WriteString(ListofObjects[i].Name);
                    for (int currEvent = 0; currEvent < ListofObjects[i].events.Count(); currEvent++)
                    {

                        //writer.WriteAttributeString("event" + currEvent, ListofObjects[i].events[currEvent].ToString());
                        writer.WriteRaw("/");
                        writer.WriteString(ListofObjects[i].events[currEvent].ToString());



                        //writer.WriteAttributeString("event", ListofObjects[i].events[currEvent].ToString());

                    } 
                    writer.WriteEndElement();
                    writer.WriteRaw("\n");
                }
                writer.WriteRaw("\t");
                writer.WriteEndElement();
                writer.WriteRaw("\n");

                writer.WriteEndElement();

                // You forgot these two lines:
                writer.WriteEndDocument();
                writer.Close();
            }

        }

      
        private void buttonNameObjTile_Click_1(object sender, EventArgs e)
        {
           
                for (int currObject = 0; currObject < ListofObjects.Count; currObject++)
                {
                    if (ListofObjects[currObject].Type == (int)numericObjID.Value)
                    {
                        ListofObjects[currObject].Name = textBoxObjName.Text;
                    }
                }
                names[(int)numericObjID.Value] = textBoxObjName.Text; 
            
        }

     

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                ListofObjects[Objects.SelectedIndex].NumEnemies = (int)numericNumEnemies.Value;
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {

           
              OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

            if (DialogResult.OK == dlg.ShowDialog())
            {

                ListofObjects = new List<Object>();
                navGraph = new List<Node>();
                collisionObjects = new List<Collidable>();

                PathNodes.Items.Clear();
                Objects.Items.Clear();
                listBoxCollisionObjects.Items.Clear();

                numericMapCol.Value = 0;
                numericMapCol.Value = 0;
                numericMapRows.Value = 0;
                numericSheetWidth.Value = 0;

               

                XElement xRoot = XElement.Load(dlg.FileName);
                //XElement xLevel = xRoot.Element("Level");
                
                
                

                XElement xWorldInfo = xRoot.Element("world_info");
                XAttribute xTWidth = xWorldInfo.Attribute("width");
                tileSize.Width = int.Parse(xTWidth.Value);
                XAttribute xTHeight = xWorldInfo.Attribute("height");
                tileSize.Height = int.Parse(xTHeight.Value);
                XAttribute xWWidth = xWorldInfo.Attribute("worldWidth");
                mapSize.Width = int.Parse(xWWidth.Value);
                XAttribute xWHeight = xWorldInfo.Attribute("worldHeight");
                mapSize.Height = int.Parse(xWHeight.Value);
                XAttribute xTSWidth = xWorldInfo.Attribute("tileSheetWidth");
                tSetSize.Width = int.Parse(xTSWidth.Value);
                XAttribute xTSHeight = xWorldInfo.Attribute("tileSheetHeight");
                tSetSize.Height = int.Parse(xTSHeight.Value);
                filePath = xWorldInfo.Value;

                numericMapCol.Value = mapSize.Width;
                numericMapRows.Value = mapSize.Height;

                TileMap = new Tile[mapSize.Width, mapSize.Height];
                for (int i = 0; i < mapSize.Width; ++i)
                {
                    for (int j = 0; j < mapSize.Height; ++j)
                    {
                        TileMap[i, j] = new Tile();
                        TileMap[i, j].X = 0;
                        TileMap[i, j].Y = 0;
                        TileMap[i, j].sRect = new Rectangle(0, 0, tileSize.Width, tileSize.Height);
                    }
                }

                if (D3DTileId != -1)
                {
                    m_TM.UnloadTexture(D3DTileId);
                    D3DTileId = -1;

                }
                D3DTileId = m_TM.LoadTexture(filePath);

                XElement xPlayer = xRoot.Element("player_info");

                if (xPlayer != null)
                {
                    XAttribute xId = xPlayer.Attribute("id");
                    XAttribute xPosX = xPlayer.Attribute("posX");
                    XAttribute xPosY = xPlayer.Attribute("posY");
                    XAttribute xNumEvents = xPlayer.Attribute("numEvents");
                    XAttribute xNumWaypoints = xPlayer.Attribute("numWaypoints");
                    XAttribute xNumEnemies = xPlayer.Attribute("numEnemies");
                    XAttribute xState = xPlayer.Attribute("startState");

                    XAttribute xSRectLeft = xPlayer.Attribute("sRectleft");
                    XAttribute xSRectTop = xPlayer.Attribute("sRecttop");
                    XAttribute xSRectWidth = xPlayer.Attribute("sRectwidth");
                    XAttribute xSRectHeight = xPlayer.Attribute("sRectheight");




                    Object tObject = new Object(int.Parse(xId.Value));
                    tObject.Position = new Point(int.Parse(xPosX.Value), int.Parse(xPosY.Value));
                    tObject.NumEnemies = int.Parse(xNumEnemies.Value);
                    tObject.sRect = new Rectangle(int.Parse(xSRectLeft.Value), int.Parse(xSRectTop.Value), int.Parse(xSRectWidth.Value), int.Parse(xSRectHeight.Value));
                    if (xState != null)
                    {
                        tObject.State = int.Parse(xState.Value);
                        
                    }


                    string NameandEvents = xPlayer.Value;
                    string[] info = NameandEvents.Split('/');
                    tObject.Name = info[0];
                    for (int i = 1; i < info.Length; i++)
                    {

                        tObject.events.Add(info[i]);
                    }
                    int numWaypoints = int.Parse(xNumWaypoints.Value);

                    for (int currWP = 0; currWP < numWaypoints; currWP++)
                    {
                        string waypoint = "waypoint";
                        waypoint += currWP;
                        XAttribute xWaypoint = xPlayer.Attribute(waypoint);
                        tObject.waypoints.Add(int.Parse(xWaypoint.Value));
                    }

                    ListofObjects.Add(tObject);
                    Objects.Items.Add(tObject.Name + " " + tObject.Type);
                    names[tObject.Type] = tObject.Name;
                }
               
              // D3DTileId = m_TM.LoadTexture(filePath);
                XElement xTileList = xRoot.Element("tile_list");
                IEnumerable<XElement> xTiles = xTileList.Elements();

                int count = 0;
                //int rowcount = 0;

                foreach (XElement xTile in xTiles)
                {
                    Tile tTile = new Tile();
                    Rectangle sourceRect = new Rectangle();

                    XAttribute xSourceRectLeft = xTile.Attribute("rposx");
                    sourceRect.X = int.Parse(xSourceRectLeft.Value);

                    XAttribute xSourceRectTop = xTile.Attribute("rposy");
                    sourceRect.Y = int.Parse(xSourceRectTop.Value);

                    XAttribute xPositionX = xTile.Attribute("posX");
                    tTile.X = int.Parse(xPositionX.Value);
                    XAttribute xPositionY = xTile.Attribute("posY");
                    tTile.Y = int.Parse(xPositionY.Value);

                    sourceRect.Width = tileSize.Width;
                    sourceRect.Height = tileSize.Height;

                    tTile.sRect = sourceRect;
                    //tTile.Y = (count / mapSize.Width) * tileSize.Width;
                    //tTile.X = count % mapSize.Height * tileSize.Height;

                    TileMap[tTile.X / tileSize.Width, tTile.Y / tileSize.Width] = tTile;
                    //TileMap[(count / mapSize.Width) * tileSize.Width, (count % mapSize.Width) * tileSize.Height] = tTile;
                    count++;
                }

                XElement xCollsionList = xRoot.Element("collsion_list");
                if (xCollsionList != null)
                {
                    IEnumerable<XElement> xColliders = xCollsionList.Elements();

                    foreach (XElement xCollider in xColliders)
                    {
                        Collidable tCollider = new Collidable();

                        XAttribute xPosX = xCollider.Attribute("posX");
                        XAttribute xPosY = xCollider.Attribute("posY");
                        XAttribute xType = xCollider.Attribute("type");
                        XAttribute xIndex = xCollider.Attribute("index");

                        string eventTrigger = xCollider.Value;

                        string[] info = eventTrigger.Split('/');
                       
                        for (int i = 1; i < info.Length; i++)
                        {

                            tCollider.eventTrigger.Add(info[i]);
                        }
                        if (eventTrigger.Length > 0)
                            tCollider.eventTrigger.Add(eventTrigger.Remove(eventTrigger.Length - 1));


                        tCollider.Id = int.Parse(xType.Value);
                        tCollider.PtPostion = new Point(int.Parse(xPosX.Value), int.Parse(xPosY.Value));
                        tCollider.Index = int.Parse(xIndex.Value);

                        collisionObjects.Add(tCollider);
                        if (tCollider.Id == 0)
                            listBoxCollisionObjects.Items.Add("Collider" + tCollider.Index);
                        else
                            listBoxCollisionObjects.Items.Add("Trigger" + tCollider.Index);

                    }

                }
               
                XElement xNavLayer = xRoot.Element("graph_list");
                if (xNavLayer != null)
                {
                    IEnumerable<XElement> xNodes = xNavLayer.Elements();

                    foreach (XElement xNode in xNodes)
                    {
                        Node tNode = new Node();

                        XAttribute xPosX = xNode.Attribute("posX");
                        XAttribute xPosY = xNode.Attribute("posY");
                        XAttribute xIndex = xNode.Attribute("index");
                        XAttribute xNumEdges = xNode.Attribute("numEdges");

                        tNode.Position = new Point(int.Parse(xPosX.Value), int.Parse(xPosY.Value));

                        tNode.Index = int.Parse(xIndex.Value);

                        int numEdges = int.Parse(xNumEdges.Value);
                        tNode.Tag = xNode.Value;
                        for (int currEdge = 0; currEdge < numEdges; currEdge++)
                        {
                            string name = "edge";
                            name += currEdge;
                            XName xname = name;

                            XAttribute xEdge = xNode.Attribute(xname);
                            tNode.edges.Add(int.Parse(xEdge.Value));

                        }
                        navGraph.Add(tNode);
                        PathNodes.Items.Add("Node" + tNode.Index.ToString());
                }
              


                    XElement xObjectLayer = xRoot.Element("objects_list");

                    if (xObjectLayer != null)
                    {
                         IEnumerable<XElement> xObjects = xObjectLayer.Elements();

                        foreach (XElement xObject in xObjects)
                        {
                           
                            XAttribute xId = xObject.Attribute("id");
                            XAttribute xPosX = xObject.Attribute("posX");
                            XAttribute xPosY = xObject.Attribute("posY");
                            XAttribute xNumEvents = xObject.Attribute("numEvents");
                            XAttribute xNumWaypoints = xObject.Attribute("numWaypoints");
                            XAttribute xNumEnemies = xObject.Attribute("numEnemies");

                            XAttribute xSRectLeft = xObject.Attribute("sRectleft");
                            XAttribute xSRectTop = xObject.Attribute("sRecttop");
                            XAttribute xSRectWidth = xObject.Attribute("sRectwidth");
                            XAttribute xSRectHeight = xObject.Attribute("sRectheight");



                            
                            Object tObject = new Object(int.Parse(xId.Value));
                            tObject.Position = new Point(int.Parse(xPosX.Value), int.Parse(xPosY.Value));
                            tObject.NumEnemies = int.Parse(xNumEnemies.Value);
                            tObject.sRect = new Rectangle(int.Parse(xSRectLeft.Value), int.Parse(xSRectTop.Value), int.Parse(xSRectWidth.Value), int.Parse(xSRectHeight.Value));
                            
                            

                            string NameandEvents = xObject.Value;
                           string[] info = NameandEvents.Split('/');
                           tObject.Name = info[0];
                           for (int i = 1; i < info.Length; i++)
                           {
                               
                               tObject.events.Add(info[i]);
                           }
                            int numWaypoints = int.Parse(xNumWaypoints.Value);

                            for (int currWP = 0; currWP < numWaypoints; currWP++)
                            {
                                string waypoint = "waypoint";
                                waypoint += currWP;
                                XAttribute xWaypoint = xObject.Attribute(waypoint);
                                tObject.waypoints.Add(int.Parse(xWaypoint.Value));
                            }

                            ListofObjects.Add(tObject);
                            Objects.Items.Add(tObject.Name +" "+ tObject.Type);
                            names[tObject.Type] = tObject.Name;
                        }
                    }
                   

                }

                worldPanel.AutoScrollMinSize = new Size(tileSize.Width * mapSize.Width, tileSize.Height * mapSize.Height);

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                ListofObjects[Objects.SelectedIndex].waypoints.Add((int)numericWaypoint.Value);
                WayPoints.Items.Add((int)numericWaypoint.Value);
            }
        }

        private void buttonAddTileEvent_Click(object sender, EventArgs e)
        {
            if (listBoxCollisionObjects.SelectedIndex != -1)
            {
                collisionObjects[listBoxCollisionObjects.SelectedIndex].eventTrigger.Add(textBoxEventTrigger.Text);
                listBoxTileEvents.Items.Add(textBoxEventTrigger.Text);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
     
            if (listBoxCollisionObjects.SelectedIndex != -1 )
            {
                int selected = listBoxCollisionObjects.SelectedIndex;
                collisionObjects.RemoveAt(listBoxCollisionObjects.SelectedIndex);
                listBoxCollisionObjects.Items.RemoveAt(listBoxCollisionObjects.SelectedIndex);
                for (int i = selected; i < collisionObjects.Count; i++)
                {

                    listBoxCollisionObjects.Items.RemoveAt(i);
                    if (collisionObjects[i].Id == 0)
                    {
                        listBoxCollisionObjects.Items.Insert(i, "Collider" + (collisionObjects[i].Index - 1).ToString());
                        collisionObjects[i].Index--;
                    }
                    else
                    {
                        listBoxCollisionObjects.Items.Insert(i, "Trigger" + (collisionObjects[i].Index - 1).ToString());
                        collisionObjects[i].Index--;
                    }
                    
                }
                if (selected != 0)
                {
                    listBoxCollisionObjects.SelectedIndex = selected - 1;
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            collisionObjects.Clear();
            listBoxCollisionObjects.Items.Clear();
        }

        private void buttonSetMapSize_Click(object sender, EventArgs e)
        {
            Tile[,] currMap = TileMap;
            Size currSize = mapSize;

            mapSize.Width = (int)numericMapCol.Value;
            mapSize.Height = (int)numericMapRows.Value;
            TileMap = new Tile[(int)numericMapCol.Value, (int)numericMapRows.Value];

            for (int x = 0; x < (int)numericMapCol.Value; x++)
            {
                for (int y = 0; y < (int)numericMapRows.Value; y++)
                {
                    TileMap[x, y] = new Tile();
                    TileMap[x, y].sRect.Width = tileSize.Width;
                    TileMap[x, y].sRect.Height = tileSize.Height;
                    TileMap[x, y].X = x * tileSize.Width;
                    TileMap[x, y].Y = y * tileSize.Height;


                }
            }
           

            // Copy old map into new map
            int minX = (mapSize.Width <= currSize.Width ? mapSize.Width : currSize.Width);
            int minY = (mapSize.Height <= currSize.Height ? mapSize.Height : currSize.Height);
            for (int x = 0; x < minX; x++)
            {
                for (int y = 0; y < minY; y++)
                {
                    TileMap[x, y] = currMap[x, y];
                }
            }

            worldPanel.AutoScrollMinSize = new Size(tileSize.Width * mapSize.Width, tileSize.Height * mapSize.Height);
        }

        private void loadTilesetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "Import";
            dlg.Filter = "All Files|*.*|PNG Files|*.png|BMP Files|*.bmp|JPEG Files|*.jpg";
            dlg.FilterIndex = 1;

            objFilePath = "resource\\graphics";
             string fullPath = System.IO.Directory.GetCurrentDirectory();
              string[] split2 = fullPath.Split(new Char[] { '\\', '\n' });

                string tempString2 = "";
                for (int i = 0; i < split2.Length - 3; i++)
                {
                    tempString2 += split2[i] + "\\";
                }
                tempString2 += objFilePath;

                dlg.InitialDirectory = tempString2;


                string entire;
                string final;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                entire = dlg.FileName;
                string[] split3 = entire.Split(new Char[] { '\\', '\n' });


                final = split3[split3.Length - 1];

                string[] split4 = final.Split(new Char[] { '.', '\n' });

                string fileType = split4[split4.Length - 1];

                if (fileType != "png")
                {
                    if (fileType != "bmp")
                    {
                        if (fileType != "jpg")
                        {
                            string message = "Unable to import: " + final + ".\n\nIncorrect filetype was selected.";
                            MessageBox.Show(message);
                            return;
                        }
                    }
                }
                // Unload prev sheet
                if (D3DTileId != -1 && D3DTileId != D3DObjID)
                {
                    m_TM.UnloadTexture(D3DTileId);
                    D3DTileId = -1;
                }

                // Load assets
                filePath = entire;
                D3DTileId = m_TM.LoadTexture(entire);	// w/o color key
                
                
               
            }
            
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (textBoxNodeTag.Text != "" && PathNodes.SelectedIndex != -1)
            {
                textBoxCurrNodeTag.Text = textBoxNodeTag.Text;
                navGraph[PathNodes.SelectedIndex].Tag = textBoxNodeTag.Text;
                textBoxNodeTag.Text = "";
            }
        }

        private void comboBoxAIStates_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (Objects.SelectedIndex != -1)
            {
                ListofObjects[Objects.SelectedIndex].State = comboBoxAIStates.SelectedIndex;
                
            }
        }

     

     

       

      

    




     

      

    


                    
     

     

      


     

       

    }
}
