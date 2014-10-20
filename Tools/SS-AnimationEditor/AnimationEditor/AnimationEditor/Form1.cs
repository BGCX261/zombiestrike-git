using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using System.IO;

namespace AnimationEditor
{
    public partial class Form1 : Form
    {
        // SGP C# wrappers
        SGP.CSGP_Direct3D m_D3D = null;
        SGP.CSGP_TextureManager m_TM = null;


        // Animation stuff
		int         sheetID         = -1;
        Size        sheetSize       = new Size(0, 0);
        string      sheetPath       = "";

        Frame       currFrame       = new Frame();
        List<Frame> allFrames       = new List<Frame>();

        bool        looping         = false;
		float       animTimer       = 0;

        bool        playPreview     = false;
        int         frameindex      = 0;
		float       previewScale    = 1.0f;
		float       scaleFlyweight  = 0.05f;



        // Other stuff
        bool    useFont             = false;
        bool    mousehold           = false;
        PointF  mousePosCurr        = new PointF(0, 0);
        PointF  mousePosSave        = new PointF(0, 0);

        Color   backColor           = Color.Black;
        Color   previewColor        = Color.Black;
        Color   anchorColor         = Color.Black;
        Color   rotationColor       = Color.Red;
        Color   sourcerectColor     = Color.Blue;
        Color   collisionrectColor  = Color.Green;





        // Application looping flag property
        public bool Looping
        {
            get;
            set;
        }



        ////////////////////////////////////////////////////////////////////////////////
        // CONSTRUCTOR
        //	    - initializes the form controls & moves the working directory
        ////////////////////////////////////////////////////////////////////////////////
        public Form1()
        {
            InitializeComponent();



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
        // Initialize
        //      - initialize the SGP wrappers
        //      - load assets
        //
        // Terminate
        //	    - unload assets
        //	    - terminate the SGP wrappers
        ////////////////////////////////////////////////////////////////////////////////
        public void Initialize()
        {
            // Access the SGP Wrapper singletons
            m_D3D = SGP.CSGP_Direct3D.GetInstance();
            m_TM = SGP.CSGP_TextureManager.GetInstance();


            // Initialize the Direct3D render targets
            m_D3D.Initialize(panel2, false);
            m_D3D.AddRenderTarget(panel1);
            m_D3D.AddRenderTarget(panel2);


            // Initialize the Texture Manager
            m_TM.Initialize(m_D3D.Device, m_D3D.Sprite);



            // Load assets
            /*
            sheetID = m_TM.LoadTexture("resource/graphics/nxc_bat_heihachi.PNG", Color.FromArgb(255, 0, 0, 0));	// w/o color key
            sheetSize.Width = m_TM.GetTextureWidth(sheetID);
            sheetSize.Height = m_TM.GetTextureHeight(sheetID);
            */


            this.MaximumSize            = this.DefaultMaximumSize; // 1938 x 1050
            this.MaximizedBounds        = new Rectangle(0, 0, this.DefaultMaximumSize.Width, this.DefaultMaximumSize.Height);
            this.WindowState            = FormWindowState.Maximized;
            //this.MaximumSize            = this.DefaultMaximumSize; // 1938 x 1050
            //this.MaximizedBounds        = new Rectangle(0, 0, this.DefaultMaximumSize.Width, this.DefaultMaximumSize.Height);
            //this.WindowState            = FormWindowState.Maximized;


            // Setup the form properly
            Size defSize = new Size(0, 0);
            SetupNewSheet(defSize, "");


            /*
            panel1.AutoScrollMinSize = sheetSize;

            numericUpDownMousePosX.Maximum = (decimal)sheetSize.Width;
            numericUpDownMousePosY.Maximum = (decimal)sheetSize.Height;

            textBoxSheetName.Text = GetName("resource/graphics/nxc_bat_heihachi.PNG");
            //textBoxSheetName.Size = new Size(textBoxSheetName.Text.Length * 5, textBoxSheetName.Size.Height);


            numericUpDownAnchorPointX.Maximum = (decimal)sheetSize.Width;
            numericUpDownAnchorPointY.Maximum = (decimal)sheetSize.Height;

            numericUpDownRotationPointX.Maximum = (decimal)sheetSize.Width;
            numericUpDownRotationPointY.Maximum = (decimal)sheetSize.Height;

            numericUpDownSourcePointX.Maximum = (decimal)sheetSize.Width;
            numericUpDownSourcePointY.Maximum = (decimal)sheetSize.Height;
            numericUpDownSourceEndX.Maximum = (decimal)sheetSize.Width;
            numericUpDownSourceEndY.Maximum = (decimal)sheetSize.Height;

            numericUpDownCollisionPointX.Maximum = (decimal)sheetSize.Width;
            numericUpDownCollisionPointY.Maximum = (decimal)sheetSize.Height;
            numericUpDownCollisionEndX.Maximum = (decimal)sheetSize.Width;
            numericUpDownCollisionEndY.Maximum = (decimal)sheetSize.Height;
            */
            LockAllControls();

            // success.
            Looping = true;
        }
        public void Terminate()
        {
            // Unload assets
            if (sheetID != -1)
            {
                m_TM.UnloadTexture(sheetID);
                sheetID = -1;
            }


            // Shut down the wrappers
            m_TM.Terminate();
            m_D3D.Terminate();
        }



        ////////////////////////////////////////////////////////////////////////////////
        // UpdateForm
        //	    - update timers & game entities
        // RenderAll
        //	    - render game stuff
        ////////////////////////////////////////////////////////////////////////////////
        public void UpdateForm(float dt)
        {
            // Update preview timer
            if (playPreview == true && allFrames.Count > 1)
            {
                animTimer += dt;


                // Update preview frame
                if (animTimer >= allFrames[frameindex].Duration)
                {
                    frameindex++;
                    animTimer = 0;
                    //if (frameindex >= allFrames.Count)
                    //    frameindex = 0;


                    if (looping == true && frameindex >= allFrames.Count)
                    {
                        frameindex = 0;
                    }
                    else if (looping == false && frameindex >= allFrames.Count)
                    {
                        frameindex--;
                    }
                }


            }



            //////////////////////////
            // Update the editor
            //////////////////////////
            toolStripStatusLabel1.Text = "Form size: " + this.Size.ToString();
        }
        public void RenderAll()
        {
            // Render Direct3D panels
            RenderSheetStuff();
            RenderPreviewStuff();

            // Draw the application time (in milliseconds with 2 precision) in the text box


            // Draw the fps in the text box

        }



        ////////////////////////////////////////////////////////////////////////////////
        // Other Renders
        //	    - render individual stuff
        ////////////////////////////////////////////////////////////////////////////////
        public void RenderSheetStuff()
        {
            // Clear the render target
            //m_D3D.Clear(panel1, Color.FromArgb(255, 0, 0, 0));
            m_D3D.Clear(panel1, backColor);
            

            // Begin rendering
            m_D3D.DeviceBegin();
            m_D3D.SpriteBegin();
            {
                ////////////////////
                // Draw the image
                ////////////////////

                if (sheetID != -1)
                {
                    // Calc where drawn if not scrolled
                    Point offset = new Point(0, 0);

                    // Add AutoScrollPosition
                    offset.X += panel1.AutoScrollPosition.X;
                    offset.Y += panel1.AutoScrollPosition.Y;


                    Rectangle rect = new Rectangle();
                    rect.X = 0;
                    rect.Y = 0;
                    rect.Width = sheetSize.Width;
                    rect.Height = sheetSize.Height;

                    float rotCX = sheetSize.Width * 0.5f;
                    float rotCY = sheetSize.Height * 0.5f;

                    //m_TM.Draw(sheetID, offset.X, offset.Y, 1.0f, 1.0f, Rectangle.Empty, 153.0f, 199.0f, 0.0f);
                    m_TM.Draw(sheetID, offset.X, offset.Y, 1.0f, 1.0f, rect, rotCX, rotCY, 0.0f);
                }


                ////////////////////////////////////////
                // Draw the frame data
                ////////////////////////////////////////


                // anchor point
                DrawCrossMark(currFrame.AnchorPoint, panel1, 8, 2, anchorColor);


                // rotation point
                DrawCrossMark(currFrame.RotationPoint, panel1, 8, 2, rotationColor);



                Point asp = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);

                // source rect
                DrawCrossMark(currFrame.SourcePoint, panel1, 6, 2, sourcerectColor);
                DrawCrossMark(currFrame.SourceEnd, panel1, 6, 2, sourcerectColor);
                //DrawRectangle(currFrame.SourcePoint, currFrame.SourceSize, panel1, 8, sourcerectColor);
                DrawLinedRectangle(currFrame.SourcePoint, currFrame.SourceEnd, panel1, 8, sourcerectColor);
                /*
                Rectangle sRect = new Rectangle();
                sRect.X         = (int)currFrame.SourcePoint.X;
                sRect.Y         = (int)currFrame.SourcePoint.Y;
                sRect.Width     = (int)currFrame.SourceSize.Width;
                sRect.Height    = (int)currFrame.SourceSize.Height;
                Point asp = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
                sRect.Offset(asp.X + 8, asp.Y + 8);
                m_D3D.DrawHollowRect(sRect, sourcerectColor, 2);
                */
                /*
                Point sS    = new Point((int)currFrame.SourcePoint.X, (int)currFrame.SourcePoint.Y);
                Point sE    = new Point((int)currFrame.SourceEnd.X, (int)currFrame.SourceEnd.Y);
                sS.Offset(asp.X + 8, asp.Y + 8);
                sE.Offset(asp.X + 8, asp.Y + 8);
                m_D3D.DrawLine(sS.X, sS.Y, sE.X, sS.Y, sourcerectColor, 2);
                m_D3D.DrawLine(sS.X, sS.Y, sS.X, sE.Y, sourcerectColor, 2);
                m_D3D.DrawLine(sE.X, sE.Y, sE.X, sS.Y, sourcerectColor, 2);
                m_D3D.DrawLine(sE.X, sE.Y, sS.X, sE.Y, sourcerectColor, 2);
                */


                // collision rect
                DrawCrossMark(currFrame.CollisionPoint, panel1, 6, 2, collisionrectColor);
                DrawCrossMark(currFrame.CollisionEnd, panel1, 6, 2, collisionrectColor);
                //DrawRectangle(currFrame.CollisionPoint, currFrame.CollisionSize, panel1, 8, collisionrectColor);
                DrawLinedRectangle(currFrame.CollisionPoint, currFrame.CollisionEnd, panel1, 8, collisionrectColor);
                /*
                Rectangle sRect = new Rectangle();
                sRect.X         = (int)currFrame.CollisionPoint.X;
                sRect.Y         = (int)currFrame.CollisionPoint.Y;
                sRect.Width     = (int)currFrame.SourceSize.Width;
                sRect.Height    = (int)currFrame.SourceSize.Height;
                Point asp = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
                sRect.Offset(asp.X + 8, asp.Y + 8);
                m_D3D.DrawHollowRect(sRect, collisionrectColor, 2);
                */
                /*
                Point cS    = new Point((int)currFrame.CollisionPoint.X, (int)currFrame.CollisionPoint.Y);
                Point cE    = new Point((int)currFrame.CollisionEnd.X, (int)currFrame.CollisionEnd.Y);
                Point asp   = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
                cS.Offset(asp.X + 8, asp.Y + 8);
                cE.Offset(asp.X + 8, asp.Y + 8);
                m_D3D.DrawLine(cS.X, cS.Y, cE.X, cS.Y, collisionrectColor, 2);
                m_D3D.DrawLine(cS.X, cS.Y, cS.X, cE.Y, collisionrectColor, 2);
                m_D3D.DrawLine(cE.X, cE.Y, cE.X, cS.Y, collisionrectColor, 2);
                m_D3D.DrawLine(cE.X, cE.Y, cS.X, cE.Y, collisionrectColor, 2);
                */
            }

            // End rendering
            m_D3D.SpriteEnd();
            m_D3D.DeviceEnd();
            m_D3D.Present();
        }
        public void RenderPreviewStuff()
        {
            // Clear the render target
            //m_D3D.Clear(panel2, Color.FromArgb(255, 0, 0, 0));
            m_D3D.Clear(panel2, previewColor);


            // Begin rendering
            m_D3D.DeviceBegin();
            m_D3D.SpriteBegin();
            {
                //////////////////////////////
                // Draw the preview image
                //////////////////////////////

                if (sheetID != -1 && allFrames.Count > 0 && frameindex > -1 && frameindex < allFrames.Count)
                {
                    // source rect
                    Rectangle framerect     = new Rectangle();
                    framerect.X             = (int)allFrames[frameindex].SourceRectangle.X;
                    framerect.Y             = (int)allFrames[frameindex].SourceRectangle.Y;
                    framerect.Width         = (int)allFrames[frameindex].SourceRectangle.Width;
                    framerect.Height        = (int)allFrames[frameindex].SourceRectangle.Height;


                    // position
                    Point position  = new Point();
                    position.X      = (panel2.Size.Width / 2)   - framerect.Width;
                    position.Y      = (panel2.Size.Height / 2)  - framerect.Height;


                    // anchor point
                    Point anchor    = new Point();
                    anchor.X        = (int)allFrames[frameindex].AnchorPoint.X;
                    anchor.Y        = (int)allFrames[frameindex].AnchorPoint.Y;


                    // anchor point size
                    Size anchorsize     = new Size();
                    anchorsize.Width    = anchor.X - framerect.X;
                    anchorsize.Height   = anchor.Y - framerect.Y;


                    //anchor.Offset(anchorsize.Width, anchorsize.Height);


                    // offset position by anchor point
                    //position.X      -= anchor.X;
                    //position.Y      -= anchor.Y;
                    position.Offset(anchorsize.Width, anchorsize.Height);



                    //Rectangle sourcerect    = new Rectangle();
                    //sourcerect              = Rectangle.FromLTRB(position.X, position.Y, position.X + framerect.Width, position.Y + framerect.Height);



                    // rotation point
                    PointF rotationpoint    = new PointF();
                    rotationpoint.X         = allFrames[frameindex].RotationPoint.X;
                    rotationpoint.Y         = allFrames[frameindex].RotationPoint.Y;




                    // draw current frame of preview animation
                    m_TM.Draw(sheetID, position.X, position.Y, previewScale, previewScale, framerect, rotationpoint.X, rotationpoint.Y, 0.0f);
                    //m_TM.Draw(sheetID, position.X, position.Y, 1.0f, 1.0f, sourcerect);
                    //m_TM.Draw(sheetID, position.X, position.Y, 1.0f, 1.0f, sourcerect, rotationpoint.X, rotationpoint.Y, 0.0f);
                }


                ////////////////////////////////////////
                // Draw the frame data
                ////////////////////////////////////////


            }
            // End rendering
            m_D3D.SpriteEnd();
            m_D3D.DeviceEnd();
            m_D3D.Present();
        }



        ////////////////////////////////////////////////////////////////////////////////
        // Form1_FormClosing
        //	    - triggered when the form is about to be closed (after the close button is clicked)
        //	    - sets flag to tell main loop to quit
        ////////////////////////////////////////////////////////////////////////////////
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
        }






/* /////////////////////////////////////////////////////////
// menuStrip1:  File
                Import
                Colors
///////////////////////////////////////////////////////// */


        // File -> New
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            New();
        }
        // File -> Open
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Open();
        }
        // File -> Save As
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Save_As();
        }
        // File -> Exit
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
            //Exit();
        }



        // Import -> Sprite Sheet
        private void spriteSheetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Please choose an image file to import.\n\n\nNOTE:\nThe image will be stretched if it's size is not a power of 2...");


            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "Import";
            dlg.Filter = "All Files|*.*|PNG Files|*.png|BMP Files|*.bmp|JPEG Files|*.jpg";
            dlg.FilterIndex = 1;


            // Go to correct default directory
            string currdir          = System.IO.Directory.GetCurrentDirectory();
            string defaultPath      = SubtractSomeFilePath(currdir, 7);
            defaultPath             += "resource\\graphics";
            dlg.InitialDirectory    = defaultPath;
            //dlg.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\Tacwars\\resource\\Images");


            string entire;
            string final;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                entire = dlg.FileName;
                final = GetName(entire);

                string filetype = GetFileType(final);
                filetype = filetype.ToLower();

                if (filetype != "png")
                {
                    if (filetype != "bmp")
                    {
                        if (filetype != "jpg")
                        {
                            string message = "Unable to import: " + final + ".\n\nIncorrect filetype was selected.";
                            MessageBox.Show(message);
                            return;
                        }
                    }
                }

                // Unload prev sheet
                if (sheetID != -1)
                {
                    m_TM.UnloadTexture(sheetID);
                    sheetID = -1;
                }

                // Load assets
                sheetID = m_TM.LoadTexture(entire);	// w/o color key
                sheetSize.Width = m_TM.GetTextureWidth(sheetID);
                sheetSize.Height = m_TM.GetTextureHeight(sheetID);
                sheetPath = entire;

                // Setup the form properly
                SetupNewSheet(sheetSize, final);

                useFont = false;

                //Reset();
                //UnlockAllControls();
                //UnlockButtons();
                buttonAddFrame.Enabled = true;
            }
        }
        // Import -> Font Sheet



        // Color -> Background
        private void backgroundToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //SummonColorBox(backColor);
            backColor = SummonColorBox(backColor);
        }
        // Color -> Anchor Point
        private void anchorPointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            anchorColor = SummonColorBox(anchorColor);
        }
        // Color -> Rotation Point
        private void rotationPointToolStripMenuItem_Click(object sender, EventArgs e)
        {
            rotationColor = SummonColorBox(rotationColor);
        }
        // Color -> Source Rectangle
        private void sourceRectangleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            sourcerectColor = SummonColorBox(sourcerectColor);
        }
        // Color -> Collision Rectangle
        private void collisionRectangleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            collisionrectColor = SummonColorBox(collisionrectColor);
        }
        // Color -> Preview Window
        private void previewWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            previewColor = SummonColorBox(previewColor);
        }






/* /////////////////////////////////////////////////////////
// Frame Data:  Data Manipulation
                Preview stuff
                Buttons
                Check boxes
                Numeric ^v
///////////////////////////////////////////////////////// */


        // Data Manipulation
        private void listBoxAllFrames_MouseClick(object sender, MouseEventArgs e)
        {
            if (sheetID == -1)
            {
                return;
            }


            if (listBoxAllFrames.IndexFromPoint(e.X, e.Y) < 0)
            {
                listBoxAllFrames.ClearSelected();
                DeselectFrame();

                buttonAddFrame.Enabled = true;
                buttonRemoveFrame.Enabled = false;
                //buttonUpdateFrame.Enabled = false;
            }
            else
            {
                SelectFrame(listBoxAllFrames.SelectedIndex);

                //buttonAddFrame.Enabled = false;
                buttonRemoveFrame.Enabled = true;
                //buttonUpdateFrame.Enabled = true;

                //if (listBoxAllFrames.SelectedIndex >= listBoxAllFrames.Items.Count || listBoxAllFrames.SelectedIndex < 0)
                //    // OUT OF BOUNDS!
                //    return;

                //frameindex = listBoxAllFrames.SelectedIndex;
                //SelectFrame(listBoxAllFrames.SelectedIndex);

                /*
                if (listBoxAllFrames.SelectedIndex < 0)
                {
                    // OUT OF BOUNDS!
                    buttonRemoveFrame.Enabled = false;
                    buttonUpdateFrame.Enabled = false;


                    listBoxAllFrames.SelectedIndex  = -1;
                    frameindex                      = listBoxAllFrames.SelectedIndex;
                    return;
                }
            
                buttonRemoveFrame.Enabled = true;
                buttonUpdateFrame.Enabled = true;

                frameindex = listBoxAllFrames.SelectedIndex;
                currFrame = allFrames[frameindex];

                SelectFrame();
                */
            }
        }
        private void textBoxFrameTrigger_TextChanged(object sender, EventArgs e)
        {
            //currFrame.Trigger = textBoxFrameTrigger.Text;

            if (listBoxAllFrames.SelectedIndex < 0 || listBoxAllFrames.SelectedIndex >= listBoxAllFrames.Items.Count)
                return;

            allFrames[listBoxAllFrames.SelectedIndex].Trigger = textBoxFrameTrigger.Text;
        }
        private void numericUpDownFrameDuration_ValueChanged(object sender, EventArgs e)
        {
            //currFrame.Duration = (float)numericUpDownFrameDuration.Value;

            if (listBoxAllFrames.SelectedIndex < 0 || listBoxAllFrames.SelectedIndex >= listBoxAllFrames.Items.Count)
                return;

            allFrames[listBoxAllFrames.SelectedIndex].Duration = (float)numericUpDownFrameDuration.Value;
        }




        // Preview Stuff
        private void checkBoxFrameLooping_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxFrameLooping.Checked == true)
                looping = true;
            else
                looping = false;
        }
        private void buttonPreviwPlay_Click(object sender, EventArgs e)
        {
            if (allFrames.Count > 0)
            {
                playPreview = true;
            }
        }
        private void buttonPreviewRestart_Click(object sender, EventArgs e)
        {
            if (allFrames.Count > 0)
                frameindex = 0;
        }
        private void buttonPreviewPause_Click(object sender, EventArgs e)
        {
            playPreview = false;
        }
        private void buttonPreviewNext_Click(object sender, EventArgs e)
        {
            frameindex++;

            if (frameindex >= allFrames.Count)
            {
                frameindex = 0;
            }

            listBoxAllFrames.SelectedIndex = frameindex;
            SelectFrame(frameindex);
        }
        private void buttonPreviewPrev_Click(object sender, EventArgs e)
        {
            frameindex--;

            if (frameindex < 0)
            {
                frameindex = allFrames.Count - 1;
            }

            listBoxAllFrames.SelectedIndex = frameindex;
            SelectFrame(frameindex);
        }
        private void buttonPreviewZoomIn_Click(object sender, EventArgs e)
        {
            previewScale += scaleFlyweight;
        }
        private void buttonPreviewZoomOut_Click(object sender, EventArgs e)
        {
            previewScale -= scaleFlyweight;

            //if (previewScale < 1.0f)
            //{
            //    previewScale = 1.0f;
            //}
        }




        // Buttons -> Frame: Add, Insert & Remove
        private void buttonAddFrame_Click(object sender, EventArgs e)
        {
            Frame frame = new Frame();

            /*
            frame.SourcePoint           = currFrame.SourcePoint;
            frame.SourceEnd             = currFrame.SourceEnd;
            frame.SourceSize            = currFrame.SourceSize;
            frame.SourceRectangle       = currFrame.SourceRectangle;
            
            frame.CollisionPoint        = currFrame.CollisionPoint;
            frame.CollisionEnd          = currFrame.CollisionEnd;
            frame.CollisionSize         = currFrame.CollisionSize;
            frame.CollisionRectangle    = currFrame.CollisionRectangle;
            
            frame.AnchorPoint           = currFrame.AnchorPoint;
            frame.RotationPoint         = currFrame.RotationPoint;
            
            frame.Trigger               = currFrame.Trigger;
            
            frame.Duration              = currFrame.Duration;
            */

            allFrames.Add(frame);
            //frameindex++;
            
            string newFrame = "Frame " + allFrames.Count.ToString();
            listBoxAllFrames.Items.Add(newFrame);
            
            Reset();
            currFrame = new Frame();
            buttonAddFrame.Enabled = true;

            listBoxAllFrames.SelectedIndex = allFrames.Count - 1;
            SelectFrame(listBoxAllFrames.SelectedIndex);
        }
        private void buttonRemoveFrame_Click(object sender, EventArgs e)
        {
            int index = listBoxAllFrames.SelectedIndex;

            // invalid index
            if (index < 0)
                return;

            int listboxCount = listBoxAllFrames.Items.Count;

            // only 1 thing exists
            if (index == 0 && listboxCount == 1)
            {
                Reset();
                currFrame = new Frame();

                // Get rid of thing at index
                listBoxAllFrames.Items.RemoveAt(index);
                allFrames.RemoveAt(index);
            }

            // first of more than 1 thing
            else if (index == 0 && listboxCount > 1)
            {
                //SelectFrame(index + 1);
                Reset();
                currFrame = new Frame();


                // Get rid of thing at index
                listBoxAllFrames.Items.RemoveAt(index);
                allFrames.RemoveAt(index);


                // Rename frames due to update
                for (int i = 0; i < listBoxAllFrames.Items.Count; i++)
                {
                    int next = i + 1;
                    listBoxAllFrames.Items[i] = "Frame " + next.ToString();
                }

                DeselectFrame();
                frameindex--;
            }

            // last of more than 1 thing
            else if (index == listboxCount - 1 && listboxCount > 1)
            {
                //SelectFrame(index - 1);
                Reset();
                currFrame = new Frame();


                // Get rid of thing at index
                listBoxAllFrames.Items.RemoveAt(index);
                allFrames.RemoveAt(index);


                // Rename frames due to update
                for (int i = 0; i < listBoxAllFrames.Items.Count; i++)
                {
                    int next = i + 1;
                    listBoxAllFrames.Items[i] = "Frame " + next.ToString();
                }

                DeselectFrame();
                frameindex--;
            }

            // 0 < index < total
            else
            {
                Reset();
                currFrame = new Frame();


                // Get rid of thing at index
                listBoxAllFrames.Items.RemoveAt(index);
                allFrames.RemoveAt(index);


                // Rename frames due to update
                for (int i = 0; i < listBoxAllFrames.Items.Count; i++)
                {
                    int next = i + 1;
                    listBoxAllFrames.Items[i] = "Frame " + next.ToString();
                }

                DeselectFrame();
                frameindex--;
            }
        }



        // Check boxes
        private void checkBoxAnchorPoint_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxAnchorPoint.Checked == true)
            {
                checkBoxRotationPoint.Checked       = false;
                checkBoxSourceRectangle.Checked     = false;
                checkBoxCollisionRectangle.Checked  = false;

                numericUpDownFramePointX.Value      = (decimal)currFrame.AnchorPoint.X;
                numericUpDownFramePointY.Value      = (decimal)currFrame.AnchorPoint.Y;
            }
        }
        private void checkBoxRotationPoint_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxRotationPoint.Checked == true)
            {
                checkBoxAnchorPoint.Checked         = false;
                checkBoxSourceRectangle.Checked     = false;
                checkBoxCollisionRectangle.Checked  = false;

                numericUpDownFramePointX.Value      = (decimal)currFrame.RotationPoint.X;
                numericUpDownFramePointY.Value      = (decimal)currFrame.RotationPoint.Y;
            }
        }
        private void checkBoxSourceRectangle_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxSourceRectangle.Checked == true)
            {
                checkBoxAnchorPoint.Checked         = false;
                checkBoxRotationPoint.Checked       = false;
                checkBoxCollisionRectangle.Checked  = false;

                numericUpDownFrameRectX.Value       = (decimal)currFrame.SourcePoint.X;
                numericUpDownFrameRectY.Value       = (decimal)currFrame.SourcePoint.Y;

                numericUpDownFrameRectW.Value       = (decimal)currFrame.SourceEnd.X;
                numericUpDownFrameRectH.Value       = (decimal)currFrame.SourceEnd.Y;
            }
        }
        private void checkBoxCollisionRectangle_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxCollisionRectangle.Checked == true)
            {
                checkBoxAnchorPoint.Checked         = false;
                checkBoxRotationPoint.Checked       = false;
                checkBoxSourceRectangle.Checked     = false;

                numericUpDownFrameRectX.Value       = (decimal)currFrame.CollisionPoint.X;
                numericUpDownFrameRectY.Value       = (decimal)currFrame.CollisionPoint.Y;

                numericUpDownFrameRectW.Value       = (decimal)currFrame.CollisionEnd.X;
                numericUpDownFrameRectH.Value       = (decimal)currFrame.CollisionEnd.Y;
            }
        }




        // Numeric ^v -> Frame Points
        private void numericUpDownFramePointX_ValueChanged(object sender, EventArgs e)
        {
            // anchor point
            if (checkBoxAnchorPoint.Checked == true)
            {
                currFrame.AnchorPoint       = new PointF((float)numericUpDownFramePointX.Value, currFrame.AnchorPoint.Y);
                textBoxAnchorPoint.Text     = currFrame.AnchorPoint.ToString();
            }

            // rotation point
            if (checkBoxRotationPoint.Checked == true)
            {
                currFrame.RotationPoint     = new PointF((float)numericUpDownFramePointX.Value, currFrame.RotationPoint.Y);
                textBoxRotationPoint.Text   = currFrame.RotationPoint.ToString();
            }
        }
        private void numericUpDownFramePointY_ValueChanged(object sender, EventArgs e)
        {
            // anchor point
            if (checkBoxAnchorPoint.Checked == true)
            {
                currFrame.AnchorPoint       = new PointF(currFrame.AnchorPoint.X, (float)numericUpDownFramePointY.Value);
                textBoxAnchorPoint.Text     = currFrame.AnchorPoint.ToString();
            }

            // rotation point
            if (checkBoxRotationPoint.Checked == true)
            {
                currFrame.RotationPoint     = new PointF(currFrame.RotationPoint.X, (float)numericUpDownFramePointY.Value);
                textBoxRotationPoint.Text   = currFrame.RotationPoint.ToString();
            }
        }


        // Numeric ^v -> Frame Rectangles
        private void numericUpDownFrameRectX_ValueChanged(object sender, EventArgs e)
        {
            // source rectangle
            if (checkBoxSourceRectangle.Checked == true)
            {
                currFrame.SourcePoint       = new PointF((float)numericUpDownFrameRectX.Value, currFrame.SourcePoint.Y);
                textBoxSourcePoint.Text     = currFrame.SourcePoint.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.SourceEnd, currFrame.SourcePoint);
                currFrame.SourceSize        = new SizeF(correctSize);
                textBoxSourceSize.Text      = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            }

            // collision rectangle
            if (checkBoxCollisionRectangle.Checked == true)
            {
                currFrame.CollisionPoint    = new PointF((float)numericUpDownFrameRectX.Value, currFrame.CollisionPoint.Y);
                textBoxCollisionPoint.Text  = currFrame.CollisionPoint.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.CollisionEnd, currFrame.CollisionPoint);
                currFrame.CollisionSize     = new SizeF(correctSize);
                textBoxCollisionSize.Text   = currFrame.CollisionSize.Width.ToString() + " x " + currFrame.CollisionSize.Height.ToString();
            }
        }
        private void numericUpDownFrameRectY_ValueChanged(object sender, EventArgs e)
        {
            // source rectangle
            if (checkBoxSourceRectangle.Checked == true)
            {
                currFrame.SourcePoint       = new PointF(currFrame.SourcePoint.X, (float)numericUpDownFrameRectY.Value);
                textBoxSourcePoint.Text     = currFrame.SourcePoint.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.SourceEnd, currFrame.SourcePoint);
                currFrame.SourceSize        = new SizeF(correctSize);
                textBoxSourceSize.Text      = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            }

            // collision rectangle
            if (checkBoxCollisionRectangle.Checked == true)
            {
                currFrame.CollisionPoint    = new PointF(currFrame.CollisionPoint.X, (float)numericUpDownFrameRectY.Value);
                textBoxCollisionPoint.Text  = currFrame.CollisionPoint.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.CollisionEnd, currFrame.CollisionPoint);
                currFrame.CollisionSize     = new SizeF(correctSize);
                textBoxCollisionSize.Text   = currFrame.CollisionSize.Width.ToString() + " x " + currFrame.CollisionSize.Height.ToString();
            }
        }
        private void numericUpDownFrameRectW_ValueChanged(object sender, EventArgs e)
        {
            // source rectangle
            if (checkBoxSourceRectangle.Checked == true)
            {
                currFrame.SourceEnd         = new PointF((float)numericUpDownFrameRectW.Value, currFrame.SourceEnd.Y);
                textBoxSourceEnd.Text       = currFrame.SourceEnd.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.SourceEnd, currFrame.SourcePoint);
                currFrame.SourceSize        = new SizeF(correctSize);
                textBoxSourceSize.Text      = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            }

            // collision rectangle
            if (checkBoxCollisionRectangle.Checked == true)
            {
                currFrame.CollisionEnd      = new PointF((float)numericUpDownFrameRectW.Value, currFrame.CollisionEnd.Y);
                textBoxCollisionEnd.Text    = currFrame.CollisionEnd.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.CollisionEnd, currFrame.CollisionPoint);
                currFrame.CollisionSize     = new SizeF(correctSize);
                textBoxCollisionSize.Text   = currFrame.CollisionSize.Width.ToString() + " x " + currFrame.CollisionSize.Height.ToString();
            }
        }
        private void numericUpDownFrameRectH_ValueChanged(object sender, EventArgs e)
        {
            // source rectangle
            if (checkBoxSourceRectangle.Checked == true)
            {
                currFrame.SourceEnd         = new PointF(currFrame.SourceEnd.X, (float)numericUpDownFrameRectH.Value);
                textBoxSourceEnd.Text       = currFrame.SourceEnd.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.SourceEnd, currFrame.SourcePoint);
                currFrame.SourceSize        = new SizeF(correctSize);
                textBoxSourceSize.Text      = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            }

            // collision rectangle
            if (checkBoxCollisionRectangle.Checked == true)
            {
                currFrame.CollisionEnd      = new PointF(currFrame.CollisionEnd.X, (float)numericUpDownFrameRectH.Value);
                textBoxCollisionEnd.Text    = currFrame.CollisionEnd.ToString();

                SizeF correctSize           = GetCorrectRectSize(currFrame.CollisionEnd, currFrame.CollisionPoint);
                currFrame.CollisionSize     = new SizeF(correctSize);
                textBoxCollisionSize.Text   = currFrame.CollisionSize.Width.ToString() + " x " + currFrame.CollisionSize.Height.ToString();
            }
        }














/* /////////////////////////////////////////////////////////
// Mouse stuff: Clicks
                Move
///////////////////////////////////////////////////////// */


        // Mouse -> Clicks
        private void panel1_MouseClick(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;
            offset.X -= panel1.AutoScrollPosition.X;
            offset.Y -= panel1.AutoScrollPosition.Y;

            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                mousePosSave.X = (float)offset.X;
                mousePosSave.Y = (float)offset.Y;
            }
            else if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                // Go to max & min points
                if (offset.X > (int)numericUpDownFramePointX.Maximum)
                    offset.X = (int)numericUpDownFramePointX.Maximum;
                else if (offset.X <= (int)numericUpDownFramePointX.Minimum)
                    offset.X = (int)numericUpDownFramePointX.Minimum;

                if (offset.Y > (int)numericUpDownFramePointY.Maximum)
                    offset.Y = (int)numericUpDownFramePointY.Maximum;
                else if (offset.Y <= (int)numericUpDownFramePointY.Minimum)
                    offset.Y = (int)numericUpDownFramePointY.Minimum;


                // anchor point
                if (checkBoxAnchorPoint.Checked == true)
                {
                    textBoxAnchorPoint.Text = offset.ToString();
                    currFrame.AnchorPoint = new PointF((float)offset.X, (float)offset.Y);

                    // Set new points
                    numericUpDownFramePointX.Value = (decimal)offset.X;
                    numericUpDownFramePointY.Value = (decimal)offset.Y;
                }

                // rotation point
                if (checkBoxRotationPoint.Checked == true)
                {
                    textBoxRotationPoint.Text = offset.ToString();
                    currFrame.RotationPoint = new PointF((float)offset.X, (float)offset.Y);

                    // Set new points
                    numericUpDownFramePointX.Value = (decimal)offset.X;
                    numericUpDownFramePointY.Value = (decimal)offset.Y;
                }
            }
        }
        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            mousehold = true;



            Point offset = e.Location;
            offset.X -= panel1.AutoScrollPosition.X;
            offset.Y -= panel1.AutoScrollPosition.Y;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                // Go to max & min points
                if (offset.X > (int)numericUpDownFrameRectX.Maximum)
                    offset.X = (int)numericUpDownFrameRectX.Maximum;
                else if (offset.X < (int)numericUpDownFrameRectX.Minimum)
                    offset.X = (int)numericUpDownFrameRectX.Minimum;

                if (offset.Y > (int)numericUpDownFrameRectY.Maximum)
                    offset.Y = (int)numericUpDownFrameRectY.Maximum;
                else if (offset.Y < (int)numericUpDownFrameRectY.Minimum)
                    offset.Y = (int)numericUpDownFrameRectY.Minimum;


                // source rect -> start point
                if (checkBoxSourceRectangle.Checked == true)
                {
                    textBoxSourcePoint.Text = offset.ToString();
                    currFrame.SourcePoint = new PointF((float)offset.X, (float)offset.Y);
                    currFrame.SourceEnd = new PointF((float)offset.X, (float)offset.Y);

                    // Set new points
                    numericUpDownFrameRectX.Value = (decimal)offset.X;
                    numericUpDownFrameRectY.Value = (decimal)offset.Y;
                }

                // collision rect -> start point
                if (checkBoxCollisionRectangle.Checked == true)
                {
                    textBoxCollisionPoint.Text = offset.ToString();
                    currFrame.CollisionPoint = new PointF((float)offset.X, (float)offset.Y);
                    currFrame.CollisionEnd = new PointF((float)offset.X, (float)offset.Y);

                    // Set new points
                    numericUpDownFrameRectX.Value = (decimal)offset.X;
                    numericUpDownFrameRectY.Value = (decimal)offset.Y;
                }
            }



            /*
            // source rect -> end point
            if (checkBoxSourceRectangle.Checked == true)
            {
                // Go to max & min points
                if (offset.X > (int)numericUpDownFrameRectW.Maximum)
                    offset.X = (int)numericUpDownFrameRectW.Maximum;
                else if (offset.X < (int)numericUpDownFrameRectW.Minimum)
                    offset.X = (int)numericUpDownFrameRectW.Minimum;

                if (offset.Y > (int)numericUpDownFrameRectH.Maximum)
                    offset.Y = (int)numericUpDownFrameRectH.Maximum;
                else if (offset.Y < (int)numericUpDownFrameRectH.Minimum)
                    offset.Y = (int)numericUpDownFrameRectH.Minimum;



                textBoxSourceEnd.Text = offset.ToString();
                currFrame.SourceEnd = new PointF((float)offset.X, (float)offset.Y);
            }


            // collision rect -> end point
            if (checkBoxCollisionRectangle.Checked == true)
            {
                // Go to max & min points
                if (offset.X > (int)numericUpDownCollisionEndX.Maximum)
                    offset.X = (int)numericUpDownCollisionEndX.Maximum;
                else if (offset.X < (int)numericUpDownCollisionEndX.Minimum)
                    offset.X = (int)numericUpDownCollisionEndX.Minimum;

                if (offset.Y > (int)numericUpDownCollisionEndY.Maximum)
                    offset.Y = (int)numericUpDownCollisionEndY.Maximum;
                else if (offset.Y < (int)numericUpDownCollisionEndY.Minimum)
                    offset.Y = (int)numericUpDownCollisionEndY.Minimum;



                textBoxCollisionEnd.Text = offset.ToString();
                currFrame.CollisionEnd = new PointF((float)offset.X, (float)offset.Y);
            }
            */
        }
        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            mousehold = false;



            Point offset = e.Location;
            offset.X -= panel1.AutoScrollPosition.X;
            offset.Y -= panel1.AutoScrollPosition.Y;


            // source rect -> final rect
            if (checkBoxSourceRectangle.Checked == true)
            {
                // Fix rect points
                PointF srStart = currFrame.SourcePoint;
                PointF srEnd = currFrame.SourceEnd;

                PointF tempPoint1 = new PointF();
                PointF tempPoint2 = new PointF();

                // If End.X is behind Start.X
                if (srEnd.X < srStart.X)
                {
                    tempPoint1 = srStart;
                    tempPoint2 = srEnd;

                    srStart.X = srEnd.X;
                    srEnd.X = tempPoint1.X;
                }

                // If End.Y is above Start.Y
                if (srEnd.Y < srStart.Y)
                {
                    tempPoint1 = srStart;
                    tempPoint2 = srEnd;

                    srStart.Y = srEnd.Y;
                    srEnd.Y = tempPoint1.Y;
                }


                // Set fixed rect points
                currFrame.SourcePoint = srStart;
                currFrame.SourceEnd = srEnd;

                textBoxSourcePoint.Text = currFrame.SourcePoint.ToString();
                textBoxSourceEnd.Text = currFrame.SourceEnd.ToString();

                numericUpDownFrameRectX.Value = (decimal)currFrame.SourcePoint.X;
                numericUpDownFrameRectY.Value = (decimal)currFrame.SourcePoint.Y;

                numericUpDownFrameRectW.Value = (decimal)currFrame.SourceEnd.X;
                numericUpDownFrameRectH.Value = (decimal)currFrame.SourceEnd.Y;


                // source size
                SizeF correctSize = GetCorrectRectSize(currFrame.SourceEnd, currFrame.SourcePoint);
                currFrame.SourceSize = new SizeF(correctSize);

                //textBoxSourceSize.Text = currFrame.SourceSize.ToString();
                textBoxSourceSize.Text = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            }

            // collision rect -> final rect
            if (checkBoxCollisionRectangle.Checked == true)
            {
                // Fix rect points
                PointF srStart = currFrame.CollisionPoint;
                PointF srEnd = currFrame.CollisionEnd;

                PointF tempPoint1 = new PointF();
                PointF tempPoint2 = new PointF();


                // If End.X is behind Start.X
                if (srEnd.X < srStart.X)
                {
                    tempPoint1 = srStart;
                    tempPoint2 = srEnd;

                    srStart.X = srEnd.X;
                    srEnd.X = tempPoint1.X;
                }

                // If End.Y is above Start.Y
                if (srEnd.Y < srStart.Y)
                {
                    tempPoint1 = srStart;
                    tempPoint2 = srEnd;

                    srStart.Y = srEnd.Y;
                    srEnd.Y = tempPoint1.Y;
                }


                // Set fixed rect points
                currFrame.CollisionPoint = srStart;
                currFrame.CollisionEnd = srEnd;

                textBoxCollisionPoint.Text = currFrame.CollisionPoint.ToString();
                textBoxCollisionEnd.Text = currFrame.CollisionEnd.ToString();

                numericUpDownFrameRectX.Value = (decimal)currFrame.CollisionPoint.X;
                numericUpDownFrameRectY.Value = (decimal)currFrame.CollisionPoint.Y;

                numericUpDownFrameRectW.Value = (decimal)currFrame.CollisionEnd.X;
                numericUpDownFrameRectH.Value = (decimal)currFrame.CollisionEnd.Y;


                // collision size
                SizeF correctSize = GetCorrectRectSize(currFrame.CollisionEnd, currFrame.CollisionPoint);
                currFrame.CollisionSize = new SizeF(correctSize);

                //textBoxCollisionSize.Text = currFrame.CollisionSize.ToString();
                textBoxCollisionSize.Text = currFrame.CollisionSize.Width.ToString() + " x " + currFrame.CollisionSize.Height.ToString();
            }
        }


        // Mouse -> Move
        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            Point offset = e.Location;
            offset.X -= panel1.AutoScrollPosition.X;
            offset.Y -= panel1.AutoScrollPosition.Y;

            mousePosCurr.X = (float)offset.X;
            mousePosCurr.Y = (float)offset.Y;

            textBoxMousePosX.Text = "X: " + ((float)offset.X).ToString();
            textBoxMousePosY.Text = "Y: " + ((float)offset.Y).ToString();


            if (offset.X > numericUpDownMousePosX.Maximum)
                numericUpDownMousePosX.Value = numericUpDownMousePosX.Maximum;
            else if (offset.X < numericUpDownMousePosX.Minimum)
                numericUpDownMousePosX.Value = numericUpDownMousePosX.Minimum;
            else
                numericUpDownMousePosX.Value = (decimal)offset.X;

            if (offset.Y > numericUpDownMousePosY.Maximum)
                numericUpDownMousePosY.Value = numericUpDownMousePosY.Maximum;
            else if (offset.Y < numericUpDownMousePosY.Minimum)
                numericUpDownMousePosY.Value = numericUpDownMousePosY.Minimum;
            else
                numericUpDownMousePosY.Value = (decimal)offset.Y;



            if (mousehold == true)
            {
                // Go to max & min points
                if (offset.X > (int)numericUpDownFrameRectX.Maximum)
                    offset.X = (int)numericUpDownFrameRectX.Maximum;
                else if (offset.X < (int)numericUpDownFrameRectX.Minimum)
                    offset.X = (int)numericUpDownFrameRectX.Minimum;

                if (offset.Y > (int)numericUpDownFrameRectY.Maximum)
                    offset.Y = (int)numericUpDownFrameRectY.Maximum;
                else if (offset.Y < (int)numericUpDownFrameRectY.Minimum)
                    offset.Y = (int)numericUpDownFrameRectY.Minimum;


                // source rect -> start point
                if (checkBoxSourceRectangle.Checked == true)
                {
                    textBoxSourceEnd.Text = offset.ToString();
                    currFrame.SourceEnd = new PointF((float)offset.X, (float)offset.Y);
                }

                // collision rect -> start point
                if (checkBoxCollisionRectangle.Checked == true)
                {
                    textBoxCollisionEnd.Text = offset.ToString();
                    currFrame.CollisionEnd = new PointF((float)offset.X, (float)offset.Y);
                }
            }
        }



/* /////////////////////////////////////////////////////////
// MY METHODS: Helpers, etc...
///////////////////////////////////////////////////////// */

        // Reset!
        private void Reset()
        {
            // points
            numericUpDownFramePointX.Value  = 0;
            numericUpDownFramePointY.Value  = 0;
            textBoxAnchorPoint.Text         = numericUpDownFramePointX.Value.ToString();
            textBoxRotationPoint.Text       = numericUpDownFramePointY.Value.ToString();



            // rects
            numericUpDownFrameRectX.Value   = 0;
            numericUpDownFrameRectY.Value   = 0;
            numericUpDownFrameRectW.Value   = 0;
            numericUpDownFrameRectH.Value   = 0;

            textBoxSourcePoint.Text         = numericUpDownFrameRectX.Value.ToString();
            textBoxCollisionPoint.Text      = numericUpDownFrameRectY.Value.ToString();

            textBoxSourceEnd.Text           = numericUpDownFrameRectW.Value.ToString();
            textBoxCollisionEnd.Text        = numericUpDownFrameRectH.Value.ToString();

            textBoxSourceSize.Text          = "0 x 0";
            textBoxCollisionSize.Text       = "0 x 0";


            // trigger
            textBoxFrameTrigger.Text = "NONE";


            // duration
            numericUpDownFrameDuration.Value = (decimal)0.25f;


            // other stuff
            previewScale = 1.0f;
            scaleFlyweight = 0.05f;

            // Finish the reset
            LockAllControls();
            UnlockAllControls();
            //ToggleAddFrame();
            LockButtons();
        }

        private void FullReset()
        {
            Reset();

            // Unload prev sheet
            if (sheetID != -1)
            {
                m_TM.UnloadTexture(sheetID);
                sheetID = -1;
            }

            currFrame       = new Frame();
            
            allFrames.Clear();
            listBoxAllFrames.Items.Clear();

            animTimer       = 0;
            
            playPreview     = false;
            frameindex      = 0;


            backColor           = Color.Black;
            previewColor        = Color.Black;
            anchorColor         = Color.Black;
            rotationColor       = Color.Red;
            sourcerectColor     = Color.Blue;
            collisionrectColor  = Color.Green;





            // Setup the form properly
            Size defSize = new Size(0, 0);
            SetupNewSheet(defSize, "");

            // Lock controls
            LockAllControls();
            LockButtons();
        }


        // New
        private void New()
        {
            FullReset();
        }
        // Open
        private void Open()
        {
            // OpenFileDialog Suff
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = "All Files|*.*|XML Files|*.xml";
            open.FilterIndex = 1;

            // Go to correct default directory
            string currdir          = System.IO.Directory.GetCurrentDirectory();
            string defaultPath      = SubtractSomeFilePath(currdir, 7);
            defaultPath             += "resource\\config";
            open.InitialDirectory   = defaultPath;


            try
            {
                if (DialogResult.OK == open.ShowDialog())
                {
                    string filetype = GetFileType(GetName(open.FileName));
                    filetype = filetype.ToLower();

                    if (filetype != "xml")
                    {
                        string message = "Unable to import: " + open.FileName + ".\n\nIncorrect filetype was selected.";
                        MessageBox.Show(message);
                        return;
                    }


                    // Animations
                    XElement xAnimations = XElement.Load(open.FileName);


                    // Animations -> Animation
                    IEnumerable<XElement> xAnimation = xAnimations.Elements();

                    // Animations -> Animation[i]
                    foreach (XElement xAnimStuff in xAnimation)
                    {
                        // Animations -> Animation[i] -> Looping
                        XAttribute xLooping = xAnimStuff.Attribute("looping");


                        // Animations -> Animation[i] -> SheetName
                        string xSheetName = xAnimStuff.Value;


                        int loop = Convert.ToInt32(xLooping.Value);

                        if (loop == 0)
                        {
                            looping = false;
                            checkBoxFrameLooping.Checked = false;
                        }
                        else
                        {
                            looping = true;
                            checkBoxFrameLooping.Checked = true;
                        }


                        string sheetname = xSheetName;


                        // Animations -> Animation[i] -> Frame
                        IEnumerable<XElement> xFrames = xAnimation.Elements("Frame");

                        // Animations -> Animation[i] -> Frame[j]
                        foreach (XElement xFrame in xFrames)
                        {
                            // Animations -> Animation[i] -> Frame[j].SourceRect
                            XAttribute xSX = xFrame.Attribute("sx");
                            XAttribute xSY = xFrame.Attribute("sy");
                            XAttribute xSW = xFrame.Attribute("sw");
                            XAttribute xSH = xFrame.Attribute("sh");

                            // Animations -> Animation[i] -> Frame[j].CollisionRect
                            XAttribute xCX = xFrame.Attribute("cx");
                            XAttribute xCY = xFrame.Attribute("cy");
                            XAttribute xCW = xFrame.Attribute("cw");
                            XAttribute xCH = xFrame.Attribute("ch");

                            // Animations -> Animation[i] -> Frame[j].AnchorPoint
                            XAttribute xPX = xFrame.Attribute("px");
                            XAttribute xPY = xFrame.Attribute("py");

                            // Animations -> Animation[i] -> Frame[j].RotationPoint
                            XAttribute xRX = xFrame.Attribute("rx");
                            XAttribute xRY = xFrame.Attribute("ry");

                            // Animations -> Animation[i] -> Frame[j].Duration
                            XAttribute xDur = xFrame.Attribute("duration");


                            // Animations -> Animation[i] -> Frame[j].Trigger
                            string xTrigger = xFrame.Value;





                            RectangleF sourceRect = RectangleF.FromLTRB(
                                (float)Convert.ToDouble(xSX.Value),
                                (float)Convert.ToDouble(xSY.Value),
                                (float)Convert.ToDouble(xSW.Value),
                                (float)Convert.ToDouble(xSH.Value));
                            //RectangleF sourceRect       = new RectangleF();
                            //sourceRect.X                = (float)Convert.ToDouble(xSX.Value);
                            //sourceRect.Y                = (float)Convert.ToDouble(xSY.Value);
                            //sourceRect.Width            = (float)Convert.ToDouble(xSW.Value);
                            //sourceRect.Height           = (float)Convert.ToDouble(xSH.Value);

                            RectangleF collisionRect = RectangleF.FromLTRB(
                                (float)Convert.ToDouble(xCX.Value),
                                (float)Convert.ToDouble(xCY.Value),
                                (float)Convert.ToDouble(xCW.Value),
                                (float)Convert.ToDouble(xCH.Value));
                            //RectangleF collisionRect    = new RectangleF();
                            //collisionRect.X             = (float)Convert.ToDouble(xCX.Value);
                            //collisionRect.Y             = (float)Convert.ToDouble(xCY.Value);
                            //collisionRect.Width         = (float)Convert.ToDouble(xCW.Value);
                            //collisionRect.Height        = (float)Convert.ToDouble(xCH.Value);
                            
                            PointF anchorPoint          = new PointF();
                            anchorPoint.X               = sourceRect.Left + (float)Convert.ToDouble(xPX.Value);
                            anchorPoint.Y               = sourceRect.Top + (float)Convert.ToDouble(xPY.Value);
                            //PointF anchorPoint          = new PointF();
                            //anchorPoint.X               = (float)Convert.ToDouble(xPX.Value);
                            //anchorPoint.Y               = (float)Convert.ToDouble(xPY.Value);
                            
                            PointF rotationPoint        = new PointF();
                            rotationPoint.X             = sourceRect.Left + (float)Convert.ToDouble(xRX.Value);
                            rotationPoint.Y             = sourceRect.Top + (float)Convert.ToDouble(xRY.Value);
                            //PointF rotationPoint        = new PointF();
                            //rotationPoint.X             = (float)Convert.ToDouble(xRX.Value);
                            //rotationPoint.Y             = (float)Convert.ToDouble(xRY.Value);

                            float duration              = (float)Convert.ToDouble(xDur.Value);

                            string frameTrigger         = xTrigger;



                            Frame frame = new Frame();

                            frame.SourcePoint           = new PointF(sourceRect.X, sourceRect.Y);
                            frame.SourceEnd             = new PointF(sourceRect.X + sourceRect.Width, sourceRect.Y + sourceRect.Height);
                            frame.SourceSize            = new SizeF(sourceRect.Width, sourceRect.Height);
                            frame.SourceRectangle       = sourceRect;
                            //frame.SourceRectangle       = new RectangleF(frame.SourcePoint, frame.SourceSize);

                            frame.CollisionPoint        = new PointF(collisionRect.X, collisionRect.Y);
                            frame.CollisionEnd          = new PointF(collisionRect.X + collisionRect.Width, collisionRect.Y + collisionRect.Height);
                            frame.CollisionSize         = new SizeF(collisionRect.Width, collisionRect.Height);
                            frame.CollisionRectangle    = collisionRect;
                            //frame.CollisionRectangle    = new RectangleF(frame.CollisionPoint, frame.CollisionSize);
                            //sourceRect.Offset(collisionRect.Width, collisionRect.Height);
                            //frame.CollisionRectangle    = sourceRect;


                            frame.AnchorPoint           = new PointF(anchorPoint.X, anchorPoint.Y);

                            frame.RotationPoint         = new PointF(rotationPoint.X, rotationPoint.Y);

                            frame.Duration              = duration;

                            frame.Trigger               = frameTrigger;



                            allFrames.Add(frame);

                            int noneLocation = sheetname.LastIndexOf(frame.Trigger);

                            if (noneLocation >= 0)
                                sheetname = sheetname.Substring(0, noneLocation);

                            textBoxSheetName.Text = sheetname;
                        }


                        // Animations -> Animation[i] -> Sheet
                        IEnumerable<XElement> xSheets = xAnimation.Elements("Sheet");


                        // Animations -> Animation[i] -> Sheet[j]
                        foreach (XElement xSheet in xSheets)
                        {
                            // Animations -> Animation[i] -> Sheet[j].path
                            XAttribute xPath = xSheet.Attribute("path");

                            string filepath = xPath.Value;

                            sheetPath = filepath;
                        }


                    }


                    // Unload prev sheet
                    if (sheetID != -1)
                    {
                        m_TM.UnloadTexture(sheetID);
                        sheetID = -1;
                    }

                    // Load assets
                    sheetID = m_TM.LoadTexture(sheetPath);	// w/o color key
                    sheetSize.Width = m_TM.GetTextureWidth(sheetID);
                    sheetSize.Height = m_TM.GetTextureHeight(sheetID);

                    // Setup the form properly
                    SetupNewSheet(sheetSize, GetName(sheetPath));

                    useFont = false;

                    Reset();


                    if (allFrames.Count > 0)
                    {
                        //PopulateControls(allFrames[0]);

                        //SelectFrame(0);
                        //listBoxAllFrames.SelectedIndex = listBoxAllFrames.Items.;


                        LockAllControls();
                        UnlockAllControls();
                        LockButtons();
                        UnlockButtons();
                    }

                    for (int i = 0; i < allFrames.Count; i++)
                    {
                        int next = i + 1;

                        listBoxAllFrames.Items.Add("Frame " + next.ToString());
                    }

                    //splitContainer1.Panel2.Invalidate();
                }
            }
            catch// (Exception exp)
            {
                //MessageBox.Show(exp.Message);
                string message = "Unable to open: " + GetName(open.FileName) + "\n\nMake sure correct file was selected.";
                MessageBox.Show(message);
            }
        }
        // Save As
        private void Save_As()
        {
            SaveFileDialog save = new SaveFileDialog();

            save.Filter = "All Files|*.*|XML Files|*.xml";
            save.FilterIndex = 1;
            save.DefaultExt = "xml";


            // Go to correct default directory
            string currdir          = System.IO.Directory.GetCurrentDirectory();
            string defaultPath      = SubtractSomeFilePath(currdir, 7);
            defaultPath             += "resource\\config";
            save.InitialDirectory   = defaultPath;


            if (save.ShowDialog() == DialogResult.OK)
            {
                // Animations
                XElement xAnimations = new XElement("Animations");


                // Animations -> Animation
                XElement xAnimation = new XElement("Animation");
                xAnimations.Add(xAnimation);


                // Animations -> Animation.Looping
                XAttribute xLooping;
                if (looping == true)
                    xLooping = new XAttribute("looping", 1);
                else
                    xLooping = new XAttribute("looping", 0);


                // Animations -> Animation.ImageName
                string name = textBoxSheetName.Text;


                xAnimation.Add(xLooping);
                xAnimation.Value = name;



                for (int i = 0; i < allFrames.Count(); i++)
                {
                    // Animations -> Animation -> Frame
                    XElement xFrame = new XElement("Frame");

                    Frame currframe = allFrames[i];
                    
                    SizeF anchorsize    = new SizeF();
                    anchorsize.Width    = currframe.AnchorPoint.X - currframe.SourcePoint.X;
                    anchorsize.Height   = currframe.AnchorPoint.Y - currframe.SourcePoint.Y;
                    SizeF rotationsize  = new SizeF();
                    rotationsize.Width  = currframe.RotationPoint.X - currframe.SourcePoint.X;
                    rotationsize.Height = currframe.RotationPoint.Y - currframe.SourcePoint.Y;

                    SizeF collisionsizeXY   = new SizeF();
                    collisionsizeXY.Width   = currframe.CollisionPoint.X - currframe.SourcePoint.X;
                    collisionsizeXY.Height  = currframe.CollisionPoint.Y - currframe.SourcePoint.Y;



                    // Animations -> Animation -> Frame.SourceRect
                    XAttribute xSX = new XAttribute("sx", allFrames[i].SourcePoint.X);
                    XAttribute xSY = new XAttribute("sy", allFrames[i].SourcePoint.Y);
                    XAttribute xSW = new XAttribute("sw", allFrames[i].SourcePoint.X + allFrames[i].SourceSize.Width);
                    XAttribute xSH = new XAttribute("sh", allFrames[i].SourcePoint.Y + allFrames[i].SourceSize.Height);
                    //XAttribute xSX = new XAttribute("sx", allFrames[i].SourcePoint.X);
                    //XAttribute xSY = new XAttribute("sy", allFrames[i].SourcePoint.Y);
                    //XAttribute xSW = new XAttribute("sw", allFrames[i].SourceSize.Width);
                    //XAttribute xSH = new XAttribute("sh", allFrames[i].SourceSize.Height);

                    // Animations -> Animation -> Frame.CollisionRect
                    XAttribute xCX = new XAttribute("cx", collisionsizeXY.Width);
                    XAttribute xCY = new XAttribute("cy", collisionsizeXY.Height);
                    XAttribute xCW = new XAttribute("cw", collisionsizeXY.Width + allFrames[i].CollisionSize.Width);
                    XAttribute xCH = new XAttribute("ch", collisionsizeXY.Height + allFrames[i].CollisionSize.Height);
                    //XAttribute xCX = new XAttribute("cx", allFrames[i].CollisionPoint.X);
                    //XAttribute xCY = new XAttribute("cy", allFrames[i].CollisionPoint.Y);
                    //XAttribute xCW = new XAttribute("cw", allFrames[i].CollisionSize.Width);
                    //XAttribute xCH = new XAttribute("ch", allFrames[i].CollisionSize.Height);

                    // Animations -> Animation -> Frame.AnchorPoint
                    XAttribute xPX = new XAttribute("px", anchorsize.Width);
                    XAttribute xPY = new XAttribute("py", anchorsize.Height);
                    //XAttribute xPX = new XAttribute("px", allFrames[i].AnchorPoint.X);
                    //XAttribute xPY = new XAttribute("py", allFrames[i].AnchorPoint.Y);

                    // Animations -> Animation -> Frame.RotationPoint
                    XAttribute xRX = new XAttribute("rx", rotationsize.Width);
                    XAttribute xRY = new XAttribute("ry", rotationsize.Height);
                    //XAttribute xRX = new XAttribute("rx", allFrames[i].RotationPoint.X);
                    //XAttribute xRY = new XAttribute("ry", allFrames[i].RotationPoint.Y);

                    // Animations -> Animation -> Frame.Duration
                    XAttribute xDur = new XAttribute("duration", allFrames[i].Duration);

                    // Animations -> Animation -> Frame.Trigger
                    string trigger = allFrames[i].Trigger;


                    xFrame.Add(xSX);
                    xFrame.Add(xSY);
                    xFrame.Add(xSW);
                    xFrame.Add(xSH);

                    xFrame.Add(xCX);
                    xFrame.Add(xCY);
                    xFrame.Add(xCW);
                    xFrame.Add(xCH);

                    xFrame.Add(xPX);
                    xFrame.Add(xPY);

                    xFrame.Add(xRX);
                    xFrame.Add(xRY);

                    xFrame.Add(xDur);

                    xFrame.Value = trigger;

                    xAnimation.Add(xFrame);
                }

                XElement xSheetPath = new XElement("Sheet");

                XAttribute xPath = new XAttribute("path", sheetPath);
                xSheetPath.Add(xPath);


                xAnimation.Add(xSheetPath);


                xAnimations.Save(save.FileName);
            }
        }
        // Exit
        private void Exit()
        {
            Looping = false;
        }

        private void ImportFontSheet()
        {
            /*
            MessageBox.Show("Please choose an image file to import");


            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Title = "Import";
            dlg.Filter = "All Files|*.*|PNG Files|*.png|BMP Files|*.bmp|JPEG Files|*.jpg";
            dlg.FilterIndex = 1;


            // Go to correct default directory
            string defaultPath      = SubtractSomeFilePath(System.IO.Directory.GetCurrentDirectory(), 4);
            defaultPath             += "resource\\graphics";
            dlg.InitialDirectory    = defaultPath;
            //dlg.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\..\\Tacwars\\resource\\Images");


            string entire;
            string final;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                entire = dlg.FileName;
                final = GetName(entire);

                string filetype = GetFileType(final);
                filetype = filetype.ToLower();

                if (filetype != "png")
                {
                    if (filetype != "bmp")
                    {
                        if (filetype != "jpg")
                        {
                            string message = "Unable to import: " + final + ".\n\nIncorrect filetype was selected.";
                            MessageBox.Show(message);
                            return;
                        }
                    }
                }

                // Unload prev sheet
                if (sheetID != -1)
                {
                    m_TM.UnloadTexture(sheetID);
                    sheetID = -1;
                }

                // Load assets
                sheetID = m_TM.LoadTexture(entire);	// w/o color key
                sheetSize.Width = m_TM.GetTextureWidth(sheetID);
                sheetSize.Height = m_TM.GetTextureHeight(sheetID);

                // Setup the form properly
                SetupNewSheet(sheetSize, final);

                useFont = true;
            }

            */
        }



        // Data setup & selection
        private void SetupNewSheet(Size sheet_size, string sheet_name_final)
        {
            // auto scroll
            panel1.AutoScrollMinSize = sheet_size;



            // sheet name
            textBoxSheetName.Text = sheet_name_final;


            ///////////////////////////////////////////////////////////////////////////////////////////
            // Set the numUpDown.Maximum to prevent going further than the sheet
            ///////////////////////////////////////////////////////////////////////////////////////////

            // mouse
            numericUpDownMousePosX.Maximum = panel1.Size.Width; // (decimal)sheet_size.Width;
            numericUpDownMousePosY.Maximum = panel1.Size.Height; // (decimal)sheet_size.Height;


            // Frame Points
            numericUpDownFramePointX.Maximum = (decimal)sheet_size.Width;
            numericUpDownFramePointY.Maximum = (decimal)sheet_size.Height;

            // Frame Rectangles
            numericUpDownFrameRectX.Maximum = (decimal)sheet_size.Width;
            numericUpDownFrameRectY.Maximum = (decimal)sheet_size.Height;
            numericUpDownFrameRectW.Maximum = (decimal)sheet_size.Width;
            numericUpDownFrameRectH.Maximum = (decimal)sheet_size.Height;
        }
        private void SelectFrame(int index)
        {
            if (index < 0 || index >= allFrames.Count)
                return;

            Frame selectedFrame = allFrames[index];
            currFrame = selectedFrame;// allFrames[index];
            frameindex  = index;


            // anchor point
            textBoxAnchorPoint.Text     = selectedFrame.AnchorPoint.ToString();


            // rotation point
            textBoxRotationPoint.Text   = selectedFrame.RotationPoint.ToString();


            // source rect
            textBoxSourceSize.Text      = selectedFrame.SourceSize.Width.ToString() + " x " + selectedFrame.SourceSize.Height.ToString();
            textBoxSourcePoint.Text     = selectedFrame.SourcePoint.ToString();
            textBoxSourceEnd.Text       = selectedFrame.SourceEnd.ToString();


            // collision rect
            textBoxCollisionSize.Text   = selectedFrame.CollisionSize.Width.ToString() + " x " + selectedFrame.CollisionSize.Height.ToString();
            textBoxCollisionPoint.Text  = selectedFrame.CollisionPoint.ToString();
            textBoxCollisionEnd.Text    = selectedFrame.CollisionEnd.ToString();


            // trigger
            textBoxFrameTrigger.Text    = selectedFrame.Trigger;


            // duration
            numericUpDownFrameDuration.Value = (decimal)selectedFrame.Duration;
        }
        private void DeselectFrame()
        {
            //frameindex = -1;
            listBoxAllFrames.SelectedIndex = -1;

            Reset();
            currFrame = new Frame();
        }


        private void PopulateControls(Frame this_frame)
        {
            // points
            numericUpDownFramePointX.Value  = 0;
            numericUpDownFramePointY.Value  = 0;
            textBoxAnchorPoint.Text         = this_frame.AnchorPoint.ToString();
            textBoxRotationPoint.Text       = this_frame.RotationPoint.ToString();



            // rects
            numericUpDownFrameRectX.Value   = 0;
            numericUpDownFrameRectY.Value   = 0;
            numericUpDownFrameRectW.Value   = 0;
            numericUpDownFrameRectH.Value   = 0;

            textBoxSourcePoint.Text         = this_frame.SourcePoint.ToString();
            textBoxCollisionPoint.Text      = this_frame.CollisionPoint.ToString();

            textBoxSourceEnd.Text           = this_frame.SourceEnd.ToString();
            textBoxCollisionEnd.Text        = this_frame.CollisionEnd.ToString();

            textBoxSourceSize.Text          = this_frame.SourceSize.Width.ToString() + " x " + this_frame.SourceSize.Height.ToString();

            textBoxCollisionSize.Text       = this_frame.CollisionSize.Width.ToString() + " x " + this_frame.CollisionSize.Height.ToString();


            // trigger
            textBoxFrameTrigger.Text = this_frame.Trigger;


            // duration
            numericUpDownFrameDuration.Value = (decimal)this_frame.Duration;


            // other stuff


            // Finish the reset
        }


        private void UpdateButtonClicked(object sender, EventArgs e)
        {
            Frame frame = new Frame();

            frame = currFrame;

            frame.Trigger = textBoxFrameTrigger.Text;//currFrame.Trigger;

            frame.Duration = currFrame.Duration;
            
            int index           = listBoxAllFrames.SelectedIndex;
            allFrames[index]    = frame;
            /*
            frame.SourcePoint           = currFrame.SourcePoint;
            frame.SourceEnd             = currFrame.SourceEnd;
            frame.SourceSize            = currFrame.SourceSize;
            frame.SourceRectangle       = currFrame.SourceRectangle;
            
            frame.CollisionPoint        = currFrame.CollisionPoint;
            frame.CollisionEnd          = currFrame.CollisionEnd;
            frame.CollisionSize         = currFrame.CollisionSize;
            frame.CollisionRectangle    = currFrame.CollisionRectangle;
            
            frame.AnchorPoint           = currFrame.AnchorPoint;
            frame.RotationPoint         = currFrame.RotationPoint;
            
            frame.Trigger               = textBoxFrameTrigger.Text;//currFrame.Trigger;
            
            frame.Duration              = currFrame.Duration;


            int index           = listBoxAllFrames.SelectedIndex;
            allFrames[index]    = frame;
            */
            //Reset();
            //currFrame = new Frame();
        }




        // Form control manipulation
        private void LockAllControls()
        {
            // points
            checkBoxAnchorPoint.Checked         = false;
            checkBoxAnchorPoint.Enabled         = false;
            checkBoxRotationPoint.Checked       = false;
            checkBoxRotationPoint.Enabled       = false;

            numericUpDownFramePointX.Enabled    = false;
            numericUpDownFramePointY.Enabled    = false;


            // rects
            checkBoxSourceRectangle.Checked     = false;
            checkBoxSourceRectangle.Enabled     = false;
            checkBoxCollisionRectangle.Checked  = false;
            checkBoxCollisionRectangle.Enabled  = false;

            numericUpDownFrameRectX.Enabled     = false;
            numericUpDownFrameRectY.Enabled     = false;
            numericUpDownFrameRectW.Enabled     = false;
            numericUpDownFrameRectH.Enabled     = false;


            // trigger
            textBoxFrameTrigger.Enabled = false;


            // duration
            numericUpDownFrameDuration.Enabled = false;


            // other stuff
            buttonAddFrame.Enabled      = false;
            buttonRemoveFrame.Enabled   = false;
            //buttonUpdateFrame.Enabled   = false;
        }
        private void UnlockAllControls()
        {
            // points
            checkBoxAnchorPoint.Enabled         = true;
            checkBoxRotationPoint.Enabled       = true;

            numericUpDownFramePointX.Enabled    = true;
            numericUpDownFramePointY.Enabled    = true;


            // rects
            checkBoxSourceRectangle.Enabled     = true;
            checkBoxCollisionRectangle.Enabled  = true;

            numericUpDownFrameRectX.Enabled     = true;
            numericUpDownFrameRectY.Enabled     = true;
            numericUpDownFrameRectW.Enabled     = true;
            numericUpDownFrameRectH.Enabled     = true;


            // trigger
            textBoxFrameTrigger.Enabled = true;


            // duration
            numericUpDownFrameDuration.Enabled = true;


            // other stuff
            //buttonAddFrame.Enabled = true;
            //buttonRemoveFrame.Enabled = true;
        }
        private void LockButtons()
        {
            buttonAddFrame.Enabled      = false;
            buttonRemoveFrame.Enabled   = false;
            //buttonUpdateFrame.Enabled   = false;
        }
        private void UnlockButtons()
        {
            buttonAddFrame.Enabled      = true;
            buttonRemoveFrame.Enabled   = true;
            //buttonUpdateFrame.Enabled   = true;
        }




        // Get the actual name of the tile set WITHOUT the folder locations
        private string GetName(string entire)
        {
            // entire = "C:\Users\fullsail\Desktop\Windows_Tools_Programming_March_2014\Other\Images\TileSets\default.bmp"

            string[] split  = entire.Split(new Char[] { '\\', '\n' });
            string final    = split[split.Length - 1];

            // final = "default.bmp"
            return final;
        }
        // Get the actual name of the tile set WITHOUT the filetype
        private string GetFileType(string name_dot_type)
        {
            string name = name_dot_type;
            // name = "default.bmp"

            string[] split  = name.Split(new Char[] { '.', '\n' });
            string final    = split[split.Length - 1];

            return final;
        }
        // Remove some of the paths in the file path, similar to a "../" to step back by 1 path
        private string SubtractSomeFilePath(string filepath, int howmuch)
        {
            // entire = "C:\Users\fullsail\Desktop\Structure_of_Game_Production_I_September_2014\ProjectRepo\repo4\silent-strike\Tools\SS-AnimationEditor\AnimationEditor\AnimationEditor"

            string[] split = filepath.Split(new Char[] { '\\', '\n' });

            string subtract = "";
            for (int i = 0; i < split.Length - howmuch; i++)
            {
                subtract += split[i] + "\\";
            }


            // subtract = "C:\Users\fullsail\Desktop\Structure_of_Game_Production_I_September_2014\ProjectRepo\repo4\silent-strike"
            return subtract;
        }


        // Get the correct size of a rect when the end point is behind/above the start point
        private SizeF GetCorrectRectSize(PointF end, PointF start)
        {
            // source size
            PointF size = new PointF();
            size.X = end.X - start.X;
            size.Y = end.Y - start.Y;

            if (size.X < 0.0f)
                size.X *= -1.0f;
            if (size.Y < 0.0f)
                size.Y *= -1.0f;

            SizeF correct = new SizeF(size.X, size.Y);
            return correct;

            /*
            PointF size = new PointF();
            size.X = currFrame.SourceEnd.X - currFrame.SourcePoint.X;
            size.Y = currFrame.SourceEnd.Y - currFrame.SourcePoint.Y;

            if (size.X < 0.0f)
                size.X *= -1.0f;
            if (size.Y < 0.0f)
                size.Y *= -1.0f;

            currFrame.SourceSize = new SizeF(size.X, size.Y);
            //textBoxSourceSize.Text = currFrame.SourceSize.ToString();
            textBoxSourceSize.Text = currFrame.SourceSize.Width.ToString() + " x " + currFrame.SourceSize.Height.ToString();
            */
        }



        // Drawing stuff
        private void DrawCrossMark(PointF currframePoint, Panel panel, int rectOffset, int lineWidth, Color color)
        {
            Point pointOffset   = new Point((int)currframePoint.X, (int)currframePoint.Y);
            pointOffset.X       += panel.AutoScrollPosition.X;
            pointOffset.Y       += panel.AutoScrollPosition.Y;


            //Rectangle rectangle = new Rectangle(pointOffset.X, pointOffset.Y, rectOffset, rectOffset);
            //rectangle.X         = pointOffset.X - rectOffset;
            //rectangle.Y         = pointOffset.Y - rectOffset;
            //rectangle.Width     = (pointOffset.X + rectOffset) - pointOffset.X;
            //rectangle.Height    = (pointOffset.Y + rectOffset) - pointOffset.Y;

            //rectangle.Location.Offset(-rectOffset, -rectOffset);
            //int right = rectangle.Right;

            Rectangle rectangle = Rectangle.FromLTRB(pointOffset.X - rectOffset, pointOffset.Y - rectOffset, pointOffset.X + rectOffset, pointOffset.Y + rectOffset);

            m_D3D.DrawHollowRect(rectangle, color, 1);
            m_D3D.DrawLine(rectangle.Left, rectangle.Top, rectangle.Right, rectangle.Bottom, color, lineWidth);
            m_D3D.DrawLine(rectangle.Right, rectangle.Top, rectangle.Left, rectangle.Bottom, color, lineWidth);

            /*
            Point pointOffset = new Point((int)currframePoint.X, (int)currframePoint.Y);
            pointOffset.X += panel.AutoScrollPosition.X;
            pointOffset.Y += panel.AutoScrollPosition.Y;

            Size wah = new Size(rectOffset * 2, rectOffset * 2);
            Rectangle rectangle = new Rectangle(pointOffset, wah);
            rectangle.Offset(lineWidth, lineWidth);

            m_D3D.DrawHollowRect(rectangle, color, 1);
            m_D3D.DrawLine(rectangle.Left, rectangle.Top, rectangle.Right, rectangle.Bottom, color, lineWidth);
            m_D3D.DrawLine(rectangle.Left + wah.Width, rectangle.Top, rectangle.Right - wah.Height, rectangle.Bottom, color, lineWidth);
            */
            /*
            Point anchorOffset  = new Point((int)currFrame.AnchorPoint.X, (int)currFrame.AnchorPoint.Y);
            anchorOffset.X      += panel1.AutoScrollPosition.X;
            anchorOffset.Y      += panel1.AutoScrollPosition.Y;
            
            int         anchorRectOffset    = 10;
            int         anchorWidth         = 2;
            Size        wahA                = new Size(anchorRectOffset*2, anchorRectOffset*2);
            Rectangle   anchor              = new Rectangle(anchorOffset, wahA);
            
            m_D3D.DrawHollowRect(anchor, anchorColor, 1);
            m_D3D.DrawLine(anchor.Left, anchor.Top, anchor.Right, anchor.Bottom, anchorColor, anchorWidth);
            m_D3D.DrawLine(anchor.Left + anchorRectOffset*2, anchor.Top, anchor.Right - anchorRectOffset*2, anchor.Bottom, anchorColor, anchorWidth);
            */

            /*
            Point rotationOffset    = new Point((int)currFrame.RotationPoint.X, (int)currFrame.RotationPoint.Y);
            rotationOffset.X        += panel1.AutoScrollPosition.X;
            rotationOffset.Y        += panel1.AutoScrollPosition.Y;
            
            int         rotationRectOffset  = 8;
            int         rotationWidth       = 2;
            Size        wahR                = new Size(rotationRectOffset*2, rotationRectOffset*2);
            Rectangle   rotation            = new Rectangle(rotationOffset, wahR);
            
            m_D3D.DrawHollowRect(rotation, rotationColor, rotationWidth);
            m_D3D.DrawLine(rotation.Left, rotation.Top, rotation.Right, rotation.Bottom, rotationColor, rotationWidth);
            m_D3D.DrawLine(rotation.Left + rotationRectOffset*2, rotation.Top, rotation.Right - rotationRectOffset*2, rotation.Bottom, rotationColor, rotationWidth);
            */

            /*
            Point sourcerectOffset  = new Point((int)currFrame.SourcePoint.X, (int)currFrame.SourcePoint.Y);
            sourcerectOffset.X      += panel1.AutoScrollPosition.X;
            sourcerectOffset.Y      += panel1.AutoScrollPosition.Y;
            
            int         sourcerectRectOffset    = 8;
            int         sourcerectWidth         = 2;
            Size        wahSR                   = new Size(sourcerectRectOffset*2, sourcerectRectOffset*2);
            Rectangle   sourcerect              = new Rectangle(sourcerectOffset, wahSR);
            
            m_D3D.DrawHollowRect(sourcerect, sourcepointColor, sourcerectWidth);
            m_D3D.DrawLine(sourcerect.Left, sourcerect.Top, sourcerect.Right, sourcerect.Bottom, sourcepointColor, sourcerectWidth);
            m_D3D.DrawLine(sourcerect.Left + sourcerectRectOffset * 2, sourcerect.Top, sourcerect.Right - sourcerectRectOffset * 2, sourcerect.Bottom, sourcepointColor, sourcerectWidth);
            */
        }
        private void DrawRectangle(PointF start, SizeF size, Panel panel, int lineWidth, Color color)
        {
            Rectangle sRect = new Rectangle();

            sRect.X         = (int)start.X;
            sRect.Y         = (int)start.Y;
            sRect.Width     = (int)size.Width;
            sRect.Height    = (int)size.Height;

            Point asp = new Point(panel.AutoScrollPosition.X, panel.AutoScrollPosition.Y);

            sRect.Offset(asp.X + lineWidth, asp.Y + lineWidth);
            m_D3D.DrawHollowRect(sRect, color, 2);


            /*
            Rectangle sRect = new Rectangle();
            sRect.X         = (int)currFrame.SourcePoint.X;
            sRect.Y         = (int)currFrame.SourcePoint.Y;
            sRect.Width     = (int)currFrame.SourceSize.Width;
            sRect.Height    = (int)currFrame.SourceSize.Height;
            Point asp = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
            sRect.Offset(asp.X + 8, asp.Y + 8);
            m_D3D.DrawHollowRect(sRect, sourcerectColor, 2);
            */
            /*
            Rectangle sRect = new Rectangle();
            sRect.X         = (int)currFrame.CollisionPoint.X;
            sRect.Y         = (int)currFrame.CollisionPoint.Y;
            sRect.Width     = (int)currFrame.SourceSize.Width;
            sRect.Height    = (int)currFrame.SourceSize.Height;
            Point asp = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
            sRect.Offset(asp.X + 8, asp.Y + 8);
            m_D3D.DrawHollowRect(sRect, collisionrectColor, 2);
            */
        }
        private void DrawLinedRectangle(PointF start, PointF end, Panel panel, int lineWidth, Color color)
        {
            Point cS    = new Point((int)start.X, (int)start.Y);
            Point cE    = new Point((int)end.X, (int)end.Y);

            Point asp   = new Point(panel.AutoScrollPosition.X, panel.AutoScrollPosition.Y);

            cS.Offset(asp.X, asp.Y);
            cE.Offset(asp.X, asp.Y);

            m_D3D.DrawLine(cS.X, cS.Y, cE.X, cS.Y, color, 2);
            m_D3D.DrawLine(cS.X, cS.Y, cS.X, cE.Y, color, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cE.X, cS.Y, color, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cS.X, cE.Y, color, 2);
            /*
            Point cS = new Point((int)start.X, (int)start.Y);
            Point cE = new Point((int)end.X, (int)end.Y);

            Point asp = new Point(panel.AutoScrollPosition.X, panel.AutoScrollPosition.Y);

            cS.Offset(asp.X + lineWidth, asp.Y + lineWidth);
            cE.Offset(asp.X + lineWidth, asp.Y + lineWidth);

            m_D3D.DrawLine(cS.X, cS.Y, cE.X, cS.Y, color, 2);
            m_D3D.DrawLine(cS.X, cS.Y, cS.X, cE.Y, color, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cE.X, cS.Y, color, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cS.X, cE.Y, color, 2);
            */
            /*
            Point sS    = new Point((int)currFrame.SourcePoint.X, (int)currFrame.SourcePoint.Y);
            Point sE    = new Point((int)currFrame.SourceEnd.X, (int)currFrame.SourceEnd.Y);
            sS.Offset(asp.X + 8, asp.Y + 8);
            sE.Offset(asp.X + 8, asp.Y + 8);
            m_D3D.DrawLine(sS.X, sS.Y, sE.X, sS.Y, sourcerectColor, 2);
            m_D3D.DrawLine(sS.X, sS.Y, sS.X, sE.Y, sourcerectColor, 2);
            m_D3D.DrawLine(sE.X, sE.Y, sE.X, sS.Y, sourcerectColor, 2);
            m_D3D.DrawLine(sE.X, sE.Y, sS.X, sE.Y, sourcerectColor, 2);
            */
            /*
            Point cS    = new Point((int)currFrame.CollisionPoint.X, (int)currFrame.CollisionPoint.Y);
            Point cE    = new Point((int)currFrame.CollisionEnd.X, (int)currFrame.CollisionEnd.Y);
            Point asp   = new Point(panel1.AutoScrollPosition.X, panel1.AutoScrollPosition.Y);
            cS.Offset(asp.X + 8, asp.Y + 8);
            cE.Offset(asp.X + 8, asp.Y + 8);
            m_D3D.DrawLine(cS.X, cS.Y, cE.X, cS.Y, collisionrectColor, 2);
            m_D3D.DrawLine(cS.X, cS.Y, cS.X, cE.Y, collisionrectColor, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cE.X, cS.Y, collisionrectColor, 2);
            m_D3D.DrawLine(cE.X, cE.Y, cS.X, cE.Y, collisionrectColor, 2);
            */
        }

        // Removes the extra offset added to the mouse postion, that draws rectangles incorrectly
        private void panel1_Resize(object sender, EventArgs e)
        {
            if(m_D3D != null)
                 m_D3D.Resize(panel1, panel1.ClientSize.Width, panel1.ClientSize.Height, false);
        }


        // Open Color Dialog box
        private Color SummonColorBox(Color fixColor)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = fixColor;

            if (dlg.ShowDialog() == DialogResult.OK)
                fixColor = dlg.Color;

            return fixColor;
        }





    }



    public class Frame
    {
        // Def ctor
        /*
        */
        public Frame()
        {
            sourcePoint     = new PointF(0.0f, 0.0f);
            sourceSize      = new SizeF(0.0f, 0.0f);
            collisionPoint  = new PointF(0.0f, 0.0f);
            collisionSize   = new SizeF(0.0f, 0.0f);
            anchorPoint     = new PointF(0.0f, 0.0f);
            trigger         = "NONE";
            duration        = 0.25f;
        }


        // Gets & Sets -> Accessors & Mutators
        public PointF SourcePoint
        {
            get
            {
                PointF p = new PointF(sourcePoint.X, sourcePoint.Y);
                return p;
            }
            set
            {
                sourcePoint.X = value.X;
                sourcePoint.Y = value.Y;
            }
        }
        public PointF SourceEnd
        {
            get
            {
                PointF p = new PointF(sourceEnd.X, sourceEnd.Y);
                return p;
            }
            set
            {
                sourceEnd.X = value.X;
                sourceEnd.Y = value.Y;
            }
        }
        public SizeF SourceSize
        {
            get
            {
                SizeF s = new SizeF(sourceSize.Width, sourceSize.Height);
                return s;
            }
            set
            {
                sourceSize.Width    = value.Width;
                sourceSize.Height   = value.Height;
            }
        }
        public PointF CollisionPoint
        {
            get
            {
                PointF p = new PointF(collisionPoint.X, collisionPoint.Y);
                return p;
            }
            set
            {
                collisionPoint.X = value.X;
                collisionPoint.Y = value.Y;
            }
        }
        public PointF CollisionEnd
        {
            get
            {
                PointF p = new PointF(collisionEnd.X, collisionEnd.Y);
                return p;
            }
            set
            {
                collisionEnd.X = value.X;
                collisionEnd.Y = value.Y;
            }
        }
        public SizeF CollisionSize
        {
            get
            {
                SizeF s = new SizeF(collisionSize.Width, collisionSize.Height);
                return s;
            }
            set
            {
                collisionSize.Width = value.Width;
                collisionSize.Height = value.Height;
            }
        }
        public PointF AnchorPoint
        {
            get
            {
                PointF p = new PointF(anchorPoint.X, anchorPoint.Y);
                return p;
            }
            set
            {
                anchorPoint.X = value.X;
                anchorPoint.Y = value.Y;
            }
        }
        public PointF RotationPoint
        {
            get
            {
                PointF p = new PointF(rotationPoint.X, rotationPoint.Y);
                return p;
            }
            set
            {
                rotationPoint.X = value.X;
                rotationPoint.Y = value.Y;
            }
        }
        public string Trigger
        {
            get { return trigger; }
            set { trigger = value; }
        }
        public float Duration
        {
            get { return duration; }
            set { duration = value; }
        }


        // Other methods
        public RectangleF SourceRectangle
        {
            get
            {
                RectangleF r = new RectangleF(sourcePoint.X, sourcePoint.Y, sourceSize.Width, sourceSize.Height);
                return r;
            }
            set
            {
                sourceRect.X        = value.X;
                sourceRect.Y        = value.Y;
                sourceRect.Width    = value.Width;
                sourceRect.Height   = value.Height;
            }
        }
        public RectangleF CollisionRectangle
        {
            get
            {
                RectangleF r = new RectangleF(collisionPoint.X, collisionPoint.Y, collisionSize.Width, collisionSize.Height);
                return r;
            }
            set
            {
                collisionRect.X         = value.X;
                collisionRect.Y         = value.Y;
                collisionRect.Width     = value.Width;
                collisionRect.Height    = value.Height;
            }
        }



        // Fields
        PointF      sourcePoint;
        PointF      sourceEnd;
        SizeF       sourceSize;
        RectangleF  sourceRect;

        PointF      collisionPoint;
        PointF      collisionEnd;
        SizeF       collisionSize;
        RectangleF  collisionRect;


        PointF      anchorPoint;
        PointF      rotationPoint;
        string      trigger;
        float       duration;
    }


}
