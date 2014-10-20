namespace TileEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }

            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadTilesetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.undoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.cutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pasteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.Layers = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBoxMapSettings = new System.Windows.Forms.GroupBox();
            this.buttonSetMapSize = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.numericMapCol = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.numericMapRows = new System.Windows.Forms.NumericUpDown();
            this.tilePanel = new System.Windows.Forms.Panel();
            this.groupBoxTileSettings = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.numericUpDownSheetHeight = new System.Windows.Forms.NumericUpDown();
            this.numericSheetWidth = new System.Windows.Forms.NumericUpDown();
            this.labelTileHeight = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.numericTileHeight = new System.Windows.Forms.NumericUpDown();
            this.numericTileWidth = new System.Windows.Forms.NumericUpDown();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.button5 = new System.Windows.Forms.Button();
            this.label18 = new System.Windows.Forms.Label();
            this.textBoxStartState = new System.Windows.Forms.TextBox();
            this.comboBoxAIStates = new System.Windows.Forms.ComboBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.textBoxNumEvents = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.numericNumEnemies = new System.Windows.Forms.NumericUpDown();
            this.numericObjID = new System.Windows.Forms.NumericUpDown();
            this.buttonNameObjTile = new System.Windows.Forms.Button();
            this.textBoxObjName = new System.Windows.Forms.TextBox();
            this.numericWaypoint = new System.Windows.Forms.NumericUpDown();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonDeleteAllWaypoints = new System.Windows.Forms.Button();
            this.buttonDeleteWaypoint = new System.Windows.Forms.Button();
            this.buttonDeleteAllEvents = new System.Windows.Forms.Button();
            this.buttonDeleteEvent = new System.Windows.Forms.Button();
            this.WayPoints = new System.Windows.Forms.ListBox();
            this.deleteAll = new System.Windows.Forms.Button();
            this.deleteSelected = new System.Windows.Forms.Button();
            this.objectPanel = new System.Windows.Forms.Panel();
            this.Objects = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.addEvent = new System.Windows.Forms.Button();
            this.eventString = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.EventTable = new System.Windows.Forms.ListBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.buttonAddTileEvent = new System.Windows.Forms.Button();
            this.listBoxTileEvents = new System.Windows.Forms.ListBox();
            this.label9 = new System.Windows.Forms.Label();
            this.listBoxCollisionObjects = new System.Windows.Forms.ListBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxEventTrigger = new System.Windows.Forms.TextBox();
            this.TriggerCheckBox = new System.Windows.Forms.CheckBox();
            this.CollisionCheckBox = new System.Windows.Forms.CheckBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.label16 = new System.Windows.Forms.Label();
            this.textBoxCurrNodeTag = new System.Windows.Forms.TextBox();
            this.button4 = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.textBoxNodeTag = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.buttonAddEdge = new System.Windows.Forms.Button();
            this.numericEdgeIndex = new System.Windows.Forms.NumericUpDown();
            this.buttonDeleteAllEdges = new System.Windows.Forms.Button();
            this.buttonDeleteEdge = new System.Windows.Forms.Button();
            this.buttonDeleteAllNodes = new System.Windows.Forms.Button();
            this.buttonDeletePathNode = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.listBoxEdges = new System.Windows.Forms.ListBox();
            this.PathNodes = new System.Windows.Forms.ListBox();
            this.worldPanel = new System.Windows.Forms.Panel();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.Layers.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBoxMapSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericMapCol)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericMapRows)).BeginInit();
            this.groupBoxTileSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSheetHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSheetWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericTileHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericTileWidth)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericNumEnemies)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericObjID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWaypoint)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericEdgeIndex)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1299, 28);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem,
            this.loadTilesetToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
            this.newToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(167, 24);
            this.newToolStripMenuItem.Text = "&New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
            this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(167, 24);
            this.openToolStripMenuItem.Text = "&Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
            this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(167, 24);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(167, 24);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // loadTilesetToolStripMenuItem
            // 
            this.loadTilesetToolStripMenuItem.Name = "loadTilesetToolStripMenuItem";
            this.loadTilesetToolStripMenuItem.Size = new System.Drawing.Size(167, 24);
            this.loadTilesetToolStripMenuItem.Text = "Load Palette";
            this.loadTilesetToolStripMenuItem.Click += new System.EventHandler(this.loadTilesetToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.undoToolStripMenuItem,
            this.redoToolStripMenuItem,
            this.toolStripSeparator3,
            this.cutToolStripMenuItem,
            this.copyToolStripMenuItem,
            this.pasteToolStripMenuItem,
            this.toolStripSeparator4,
            this.selectAllToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(47, 24);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // undoToolStripMenuItem
            // 
            this.undoToolStripMenuItem.Name = "undoToolStripMenuItem";
            this.undoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.undoToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.undoToolStripMenuItem.Text = "&Undo";
            // 
            // redoToolStripMenuItem
            // 
            this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
            this.redoToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.redoToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.redoToolStripMenuItem.Text = "&Redo";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(162, 6);
            // 
            // cutToolStripMenuItem
            // 
            this.cutToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("cutToolStripMenuItem.Image")));
            this.cutToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.cutToolStripMenuItem.Name = "cutToolStripMenuItem";
            this.cutToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.cutToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.cutToolStripMenuItem.Text = "Cu&t";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
            this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.copyToolStripMenuItem.Text = "&Copy";
            // 
            // pasteToolStripMenuItem
            // 
            this.pasteToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("pasteToolStripMenuItem.Image")));
            this.pasteToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.pasteToolStripMenuItem.Name = "pasteToolStripMenuItem";
            this.pasteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.pasteToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.pasteToolStripMenuItem.Text = "&Paste";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(162, 6);
            // 
            // selectAllToolStripMenuItem
            // 
            this.selectAllToolStripMenuItem.Name = "selectAllToolStripMenuItem";
            this.selectAllToolStripMenuItem.Size = new System.Drawing.Size(165, 24);
            this.selectAllToolStripMenuItem.Text = "Select &All";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.customizeToolStripMenuItem,
            this.optionsToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(57, 24);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // customizeToolStripMenuItem
            // 
            this.customizeToolStripMenuItem.Name = "customizeToolStripMenuItem";
            this.customizeToolStripMenuItem.Size = new System.Drawing.Size(147, 24);
            this.customizeToolStripMenuItem.Text = "&Customize";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(147, 24);
            this.optionsToolStripMenuItem.Text = "&Options";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 28);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.Layers);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.worldPanel);
            this.splitContainer1.Size = new System.Drawing.Size(1299, 544);
            this.splitContainer1.SplitterDistance = 503;
            this.splitContainer1.TabIndex = 1;
            // 
            // Layers
            // 
            this.Layers.Controls.Add(this.tabPage1);
            this.Layers.Controls.Add(this.tabPage2);
            this.Layers.Controls.Add(this.tabPage3);
            this.Layers.Controls.Add(this.tabPage4);
            this.Layers.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Layers.Location = new System.Drawing.Point(0, 0);
            this.Layers.Name = "Layers";
            this.Layers.SelectedIndex = 0;
            this.Layers.Size = new System.Drawing.Size(503, 544);
            this.Layers.TabIndex = 0;
            this.Layers.SelectedIndexChanged += new System.EventHandler(this.Layers_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.groupBoxMapSettings);
            this.tabPage1.Controls.Add(this.tilePanel);
            this.tabPage1.Controls.Add(this.groupBoxTileSettings);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(495, 515);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Tile";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBoxMapSettings
            // 
            this.groupBoxMapSettings.Controls.Add(this.buttonSetMapSize);
            this.groupBoxMapSettings.Controls.Add(this.label5);
            this.groupBoxMapSettings.Controls.Add(this.numericMapCol);
            this.groupBoxMapSettings.Controls.Add(this.label6);
            this.groupBoxMapSettings.Controls.Add(this.numericMapRows);
            this.groupBoxMapSettings.Location = new System.Drawing.Point(218, 387);
            this.groupBoxMapSettings.Name = "groupBoxMapSettings";
            this.groupBoxMapSettings.Size = new System.Drawing.Size(142, 118);
            this.groupBoxMapSettings.TabIndex = 3;
            this.groupBoxMapSettings.TabStop = false;
            this.groupBoxMapSettings.Text = "Map Settings";
            // 
            // buttonSetMapSize
            // 
            this.buttonSetMapSize.Location = new System.Drawing.Point(9, 89);
            this.buttonSetMapSize.Name = "buttonSetMapSize";
            this.buttonSetMapSize.Size = new System.Drawing.Size(127, 23);
            this.buttonSetMapSize.TabIndex = 6;
            this.buttonSetMapSize.Text = "Set Map Size";
            this.buttonSetMapSize.UseVisualStyleBackColor = true;
            this.buttonSetMapSize.Click += new System.EventHandler(this.buttonSetMapSize_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 45);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(62, 17);
            this.label5.TabIndex = 5;
            this.label5.Text = "Columns";
            // 
            // numericMapCol
            // 
            this.numericMapCol.Location = new System.Drawing.Point(72, 45);
            this.numericMapCol.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericMapCol.Name = "numericMapCol";
            this.numericMapCol.Size = new System.Drawing.Size(64, 22);
            this.numericMapCol.TabIndex = 3;
            this.numericMapCol.ValueChanged += new System.EventHandler(this.numericMapCol_ValueChanged_1);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 20);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(42, 17);
            this.label6.TabIndex = 4;
            this.label6.Text = "Rows";
            // 
            // numericMapRows
            // 
            this.numericMapRows.Location = new System.Drawing.Point(72, 13);
            this.numericMapRows.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericMapRows.Name = "numericMapRows";
            this.numericMapRows.Size = new System.Drawing.Size(64, 22);
            this.numericMapRows.TabIndex = 2;
            this.numericMapRows.ValueChanged += new System.EventHandler(this.numericMapRows_ValueChanged_1);
            // 
            // tilePanel
            // 
            this.tilePanel.Location = new System.Drawing.Point(3, 3);
            this.tilePanel.Name = "tilePanel";
            this.tilePanel.Size = new System.Drawing.Size(360, 366);
            this.tilePanel.TabIndex = 0;
            this.tilePanel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.tilePanel_MouseClick);
            // 
            // groupBoxTileSettings
            // 
            this.groupBoxTileSettings.Controls.Add(this.label7);
            this.groupBoxTileSettings.Controls.Add(this.label8);
            this.groupBoxTileSettings.Controls.Add(this.numericUpDownSheetHeight);
            this.groupBoxTileSettings.Controls.Add(this.numericSheetWidth);
            this.groupBoxTileSettings.Controls.Add(this.labelTileHeight);
            this.groupBoxTileSettings.Controls.Add(this.label4);
            this.groupBoxTileSettings.Controls.Add(this.numericTileHeight);
            this.groupBoxTileSettings.Controls.Add(this.numericTileWidth);
            this.groupBoxTileSettings.Location = new System.Drawing.Point(8, 387);
            this.groupBoxTileSettings.Name = "groupBoxTileSettings";
            this.groupBoxTileSettings.Size = new System.Drawing.Size(204, 118);
            this.groupBoxTileSettings.TabIndex = 2;
            this.groupBoxTileSettings.TabStop = false;
            this.groupBoxTileSettings.Text = "Tile Settings";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(0, 99);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(90, 17);
            this.label7.TabIndex = 7;
            this.label7.Text = "Sheet Height";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(0, 76);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(85, 17);
            this.label8.TabIndex = 6;
            this.label8.Text = "Sheet Width";
            // 
            // numericUpDownSheetHeight
            // 
            this.numericUpDownSheetHeight.Location = new System.Drawing.Point(106, 97);
            this.numericUpDownSheetHeight.Name = "numericUpDownSheetHeight";
            this.numericUpDownSheetHeight.Size = new System.Drawing.Size(64, 22);
            this.numericUpDownSheetHeight.TabIndex = 5;
            this.numericUpDownSheetHeight.ValueChanged += new System.EventHandler(this.numericUpDownSheetHeight_ValueChanged);
            // 
            // numericSheetWidth
            // 
            this.numericSheetWidth.Location = new System.Drawing.Point(106, 74);
            this.numericSheetWidth.Name = "numericSheetWidth";
            this.numericSheetWidth.Size = new System.Drawing.Size(64, 22);
            this.numericSheetWidth.TabIndex = 4;
            this.numericSheetWidth.ValueChanged += new System.EventHandler(this.numericSheetWidth_ValueChanged);
            // 
            // labelTileHeight
            // 
            this.labelTileHeight.AutoSize = true;
            this.labelTileHeight.Location = new System.Drawing.Point(0, 49);
            this.labelTileHeight.Name = "labelTileHeight";
            this.labelTileHeight.Size = new System.Drawing.Size(76, 17);
            this.labelTileHeight.TabIndex = 3;
            this.labelTileHeight.Text = "Tile Height";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(0, 26);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 17);
            this.label4.TabIndex = 2;
            this.label4.Text = "Tile Width";
            // 
            // numericTileHeight
            // 
            this.numericTileHeight.Location = new System.Drawing.Point(106, 47);
            this.numericTileHeight.Name = "numericTileHeight";
            this.numericTileHeight.Size = new System.Drawing.Size(64, 22);
            this.numericTileHeight.TabIndex = 1;
            this.numericTileHeight.ValueChanged += new System.EventHandler(this.numericTileHeight_ValueChanged);
            // 
            // numericTileWidth
            // 
            this.numericTileWidth.Location = new System.Drawing.Point(106, 24);
            this.numericTileWidth.Name = "numericTileWidth";
            this.numericTileWidth.Size = new System.Drawing.Size(64, 22);
            this.numericTileWidth.TabIndex = 0;
            this.numericTileWidth.ValueChanged += new System.EventHandler(this.numericTileWidth_ValueChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.button5);
            this.tabPage2.Controls.Add(this.label18);
            this.tabPage2.Controls.Add(this.textBoxStartState);
            this.tabPage2.Controls.Add(this.comboBoxAIStates);
            this.tabPage2.Controls.Add(this.label17);
            this.tabPage2.Controls.Add(this.label14);
            this.tabPage2.Controls.Add(this.textBoxNumEvents);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Controls.Add(this.numericObjID);
            this.tabPage2.Controls.Add(this.buttonNameObjTile);
            this.tabPage2.Controls.Add(this.textBoxObjName);
            this.tabPage2.Controls.Add(this.numericWaypoint);
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Controls.Add(this.buttonDeleteAllWaypoints);
            this.tabPage2.Controls.Add(this.buttonDeleteWaypoint);
            this.tabPage2.Controls.Add(this.buttonDeleteAllEvents);
            this.tabPage2.Controls.Add(this.buttonDeleteEvent);
            this.tabPage2.Controls.Add(this.WayPoints);
            this.tabPage2.Controls.Add(this.deleteAll);
            this.tabPage2.Controls.Add(this.deleteSelected);
            this.tabPage2.Controls.Add(this.objectPanel);
            this.tabPage2.Controls.Add(this.Objects);
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.addEvent);
            this.tabPage2.Controls.Add(this.eventString);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.EventTable);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(495, 519);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Objects";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(373, 177);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(109, 23);
            this.button5.TabIndex = 32;
            this.button5.Text = "Confirm State";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(370, 212);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(57, 13);
            this.label18.TabIndex = 31;
            this.label18.Text = "Start State";
            // 
            // textBoxStartState
            // 
            this.textBoxStartState.Location = new System.Drawing.Point(366, 231);
            this.textBoxStartState.Name = "textBoxStartState";
            this.textBoxStartState.Size = new System.Drawing.Size(121, 20);
            this.textBoxStartState.TabIndex = 30;
            // 
            // comboBoxAIStates
            // 
            this.comboBoxAIStates.FormattingEnabled = true;
            this.comboBoxAIStates.Location = new System.Drawing.Point(367, 150);
            this.comboBoxAIStates.Name = "comboBoxAIStates";
            this.comboBoxAIStates.Size = new System.Drawing.Size(121, 21);
            this.comboBoxAIStates.TabIndex = 29;
            this.comboBoxAIStates.SelectedIndexChanged += new System.EventHandler(this.comboBoxAIStates_SelectedIndexChanged);
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(367, 122);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(82, 13);
            this.label17.TabIndex = 28;
            this.label17.Text = "New Start State";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(392, 331);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(84, 17);
            this.label14.TabIndex = 25;
            this.label14.Text = "Num Events";
            // 
            // textBoxNumEvents
            // 
            this.textBoxNumEvents.Location = new System.Drawing.Point(375, 347);
            this.textBoxNumEvents.Name = "textBoxNumEvents";
            this.textBoxNumEvents.ReadOnly = true;
            this.textBoxNumEvents.Size = new System.Drawing.Size(100, 22);
            this.textBoxNumEvents.TabIndex = 24;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label13);
            this.groupBox1.Controls.Add(this.numericNumEnemies);
            this.groupBox1.Location = new System.Drawing.Point(366, 16);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(109, 95);
            this.groupBox1.TabIndex = 23;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Spawner Options";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(6, 28);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(95, 17);
            this.label13.TabIndex = 24;
            this.label13.Text = "Num Enemies";
            // 
            // numericNumEnemies
            // 
            this.numericNumEnemies.Location = new System.Drawing.Point(9, 44);
            this.numericNumEnemies.Name = "numericNumEnemies";
            this.numericNumEnemies.Size = new System.Drawing.Size(56, 22);
            this.numericNumEnemies.TabIndex = 20;
            this.numericNumEnemies.ValueChanged += new System.EventHandler(this.numericUpDown3_ValueChanged);
            // 
            // numericObjID
            // 
            this.numericObjID.Location = new System.Drawing.Point(4, 248);
            this.numericObjID.Name = "numericObjID";
            this.numericObjID.Size = new System.Drawing.Size(100, 22);
            this.numericObjID.TabIndex = 18;
            // 
            // buttonNameObjTile
            // 
            this.buttonNameObjTile.Location = new System.Drawing.Point(18, 300);
            this.buttonNameObjTile.Name = "buttonNameObjTile";
            this.buttonNameObjTile.Size = new System.Drawing.Size(75, 23);
            this.buttonNameObjTile.TabIndex = 17;
            this.buttonNameObjTile.Text = "Add Name ";
            this.buttonNameObjTile.UseVisualStyleBackColor = true;
            this.buttonNameObjTile.Click += new System.EventHandler(this.buttonNameObjTile_Click_1);
            // 
            // textBoxObjName
            // 
            this.textBoxObjName.Location = new System.Drawing.Point(4, 274);
            this.textBoxObjName.Name = "textBoxObjName";
            this.textBoxObjName.Size = new System.Drawing.Size(100, 22);
            this.textBoxObjName.TabIndex = 16;
            // 
            // numericWaypoint
            // 
            this.numericWaypoint.Location = new System.Drawing.Point(116, 274);
            this.numericWaypoint.Name = "numericWaypoint";
            this.numericWaypoint.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericWaypoint.Size = new System.Drawing.Size(109, 22);
            this.numericWaypoint.TabIndex = 15;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(115, 300);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(110, 23);
            this.button1.TabIndex = 14;
            this.button1.Text = "Add Waypoint";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonDeleteAllWaypoints
            // 
            this.buttonDeleteAllWaypoints.Location = new System.Drawing.Point(116, 477);
            this.buttonDeleteAllWaypoints.Name = "buttonDeleteAllWaypoints";
            this.buttonDeleteAllWaypoints.Size = new System.Drawing.Size(110, 23);
            this.buttonDeleteAllWaypoints.TabIndex = 13;
            this.buttonDeleteAllWaypoints.Text = "Delete All";
            this.buttonDeleteAllWaypoints.UseVisualStyleBackColor = true;
            this.buttonDeleteAllWaypoints.Click += new System.EventHandler(this.buttonDeleteAllWaypoints_Click);
            // 
            // buttonDeleteWaypoint
            // 
            this.buttonDeleteWaypoint.Location = new System.Drawing.Point(116, 447);
            this.buttonDeleteWaypoint.Name = "buttonDeleteWaypoint";
            this.buttonDeleteWaypoint.Size = new System.Drawing.Size(110, 23);
            this.buttonDeleteWaypoint.TabIndex = 12;
            this.buttonDeleteWaypoint.Text = "Delete Selected";
            this.buttonDeleteWaypoint.UseVisualStyleBackColor = true;
            this.buttonDeleteWaypoint.Click += new System.EventHandler(this.buttonDeleteWaypoint_Click);
            // 
            // buttonDeleteAllEvents
            // 
            this.buttonDeleteAllEvents.Location = new System.Drawing.Point(250, 477);
            this.buttonDeleteAllEvents.Name = "buttonDeleteAllEvents";
            this.buttonDeleteAllEvents.Size = new System.Drawing.Size(110, 23);
            this.buttonDeleteAllEvents.TabIndex = 11;
            this.buttonDeleteAllEvents.Text = "Delete All";
            this.buttonDeleteAllEvents.UseVisualStyleBackColor = true;
            this.buttonDeleteAllEvents.Click += new System.EventHandler(this.buttonDeleteAllEvents_Click);
            // 
            // buttonDeleteEvent
            // 
            this.buttonDeleteEvent.Location = new System.Drawing.Point(250, 448);
            this.buttonDeleteEvent.Name = "buttonDeleteEvent";
            this.buttonDeleteEvent.Size = new System.Drawing.Size(110, 23);
            this.buttonDeleteEvent.TabIndex = 10;
            this.buttonDeleteEvent.Text = "Delete Selected";
            this.buttonDeleteEvent.UseVisualStyleBackColor = true;
            this.buttonDeleteEvent.Click += new System.EventHandler(this.buttonDeleteEvent_Click);
            // 
            // WayPoints
            // 
            this.WayPoints.FormattingEnabled = true;
            this.WayPoints.ItemHeight = 16;
            this.WayPoints.Location = new System.Drawing.Point(115, 331);
            this.WayPoints.Name = "WayPoints";
            this.WayPoints.Size = new System.Drawing.Size(110, 100);
            this.WayPoints.TabIndex = 9;
            // 
            // deleteAll
            // 
            this.deleteAll.Location = new System.Drawing.Point(3, 474);
            this.deleteAll.Name = "deleteAll";
            this.deleteAll.Size = new System.Drawing.Size(101, 23);
            this.deleteAll.TabIndex = 8;
            this.deleteAll.Text = "Delete All";
            this.deleteAll.UseVisualStyleBackColor = true;
            this.deleteAll.Click += new System.EventHandler(this.deleteAll_Click);
            // 
            // deleteSelected
            // 
            this.deleteSelected.Location = new System.Drawing.Point(3, 445);
            this.deleteSelected.Name = "deleteSelected";
            this.deleteSelected.Size = new System.Drawing.Size(101, 23);
            this.deleteSelected.TabIndex = 7;
            this.deleteSelected.Text = "Delete Selected";
            this.deleteSelected.UseVisualStyleBackColor = true;
            this.deleteSelected.Click += new System.EventHandler(this.deleteSelected_Click);
            // 
            // objectPanel
            // 
            this.objectPanel.Location = new System.Drawing.Point(7, 7);
            this.objectPanel.Name = "objectPanel";
            this.objectPanel.Size = new System.Drawing.Size(353, 231);
            this.objectPanel.TabIndex = 6;
            this.objectPanel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.objectPanel_MouseClick);
            // 
            // Objects
            // 
            this.Objects.FormattingEnabled = true;
            this.Objects.ItemHeight = 16;
            this.Objects.Location = new System.Drawing.Point(3, 344);
            this.Objects.Name = "Objects";
            this.Objects.Size = new System.Drawing.Size(101, 84);
            this.Objects.TabIndex = 5;
            this.Objects.SelectedIndexChanged += new System.EventHandler(this.Objects_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(275, 258);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(75, 17);
            this.label2.TabIndex = 4;
            this.label2.Text = "New Event";
            // 
            // addEvent
            // 
            this.addEvent.Location = new System.Drawing.Point(274, 300);
            this.addEvent.Name = "addEvent";
            this.addEvent.Size = new System.Drawing.Size(75, 23);
            this.addEvent.TabIndex = 3;
            this.addEvent.Text = "Add Event";
            this.addEvent.UseVisualStyleBackColor = true;
            this.addEvent.Click += new System.EventHandler(this.addEvent_Click);
            // 
            // eventString
            // 
            this.eventString.Location = new System.Drawing.Point(250, 274);
            this.eventString.Name = "eventString";
            this.eventString.Size = new System.Drawing.Size(110, 22);
            this.eventString.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(271, 331);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(96, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Added Events";
            // 
            // EventTable
            // 
            this.EventTable.FormattingEnabled = true;
            this.EventTable.ItemHeight = 16;
            this.EventTable.Location = new System.Drawing.Point(246, 347);
            this.EventTable.Name = "EventTable";
            this.EventTable.Size = new System.Drawing.Size(120, 84);
            this.EventTable.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.button2);
            this.tabPage3.Controls.Add(this.button3);
            this.tabPage3.Controls.Add(this.buttonAddTileEvent);
            this.tabPage3.Controls.Add(this.listBoxTileEvents);
            this.tabPage3.Controls.Add(this.label9);
            this.tabPage3.Controls.Add(this.listBoxCollisionObjects);
            this.tabPage3.Controls.Add(this.checkBox1);
            this.tabPage3.Controls.Add(this.label3);
            this.tabPage3.Controls.Add(this.textBoxEventTrigger);
            this.tabPage3.Controls.Add(this.TriggerCheckBox);
            this.tabPage3.Controls.Add(this.CollisionCheckBox);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(495, 519);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Collision";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(12, 372);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(101, 23);
            this.button2.TabIndex = 10;
            this.button2.Text = "Delete All";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(12, 343);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(101, 23);
            this.button3.TabIndex = 9;
            this.button3.Text = "Delete Selected";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // buttonAddTileEvent
            // 
            this.buttonAddTileEvent.Location = new System.Drawing.Point(228, 118);
            this.buttonAddTileEvent.Name = "buttonAddTileEvent";
            this.buttonAddTileEvent.Size = new System.Drawing.Size(128, 23);
            this.buttonAddTileEvent.TabIndex = 8;
            this.buttonAddTileEvent.Text = "Add Event";
            this.buttonAddTileEvent.UseVisualStyleBackColor = true;
            this.buttonAddTileEvent.Click += new System.EventHandler(this.buttonAddTileEvent_Click);
            // 
            // listBoxTileEvents
            // 
            this.listBoxTileEvents.FormattingEnabled = true;
            this.listBoxTileEvents.ItemHeight = 16;
            this.listBoxTileEvents.Location = new System.Drawing.Point(228, 151);
            this.listBoxTileEvents.Name = "listBoxTileEvents";
            this.listBoxTileEvents.Size = new System.Drawing.Size(128, 180);
            this.listBoxTileEvents.TabIndex = 7;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 132);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(76, 17);
            this.label9.TabIndex = 6;
            this.label9.Text = "Collidables";
            // 
            // listBoxCollisionObjects
            // 
            this.listBoxCollisionObjects.FormattingEnabled = true;
            this.listBoxCollisionObjects.ItemHeight = 16;
            this.listBoxCollisionObjects.Location = new System.Drawing.Point(9, 151);
            this.listBoxCollisionObjects.Name = "listBoxCollisionObjects";
            this.listBoxCollisionObjects.Size = new System.Drawing.Size(120, 180);
            this.listBoxCollisionObjects.TabIndex = 5;
            this.listBoxCollisionObjects.SelectedIndexChanged += new System.EventHandler(this.listBoxCollisionObjects_SelectedIndexChanged);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(228, 8);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(72, 21);
            this.checkBox1.TabIndex = 4;
            this.checkBox1.Text = "Eraser";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(263, 75);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(71, 17);
            this.label3.TabIndex = 3;
            this.label3.Text = "Tile Event";
            // 
            // textBoxEventTrigger
            // 
            this.textBoxEventTrigger.Location = new System.Drawing.Point(228, 91);
            this.textBoxEventTrigger.Name = "textBoxEventTrigger";
            this.textBoxEventTrigger.Size = new System.Drawing.Size(128, 22);
            this.textBoxEventTrigger.TabIndex = 2;
            // 
            // TriggerCheckBox
            // 
            this.TriggerCheckBox.AutoSize = true;
            this.TriggerCheckBox.Location = new System.Drawing.Point(116, 8);
            this.TriggerCheckBox.Name = "TriggerCheckBox";
            this.TriggerCheckBox.Size = new System.Drawing.Size(76, 21);
            this.TriggerCheckBox.TabIndex = 1;
            this.TriggerCheckBox.Text = "Trigger";
            this.TriggerCheckBox.UseVisualStyleBackColor = true;
            this.TriggerCheckBox.CheckedChanged += new System.EventHandler(this.TriggerCheckBox_CheckedChanged);
            // 
            // CollisionCheckBox
            // 
            this.CollisionCheckBox.AutoSize = true;
            this.CollisionCheckBox.Location = new System.Drawing.Point(8, 8);
            this.CollisionCheckBox.Name = "CollisionCheckBox";
            this.CollisionCheckBox.Size = new System.Drawing.Size(79, 21);
            this.CollisionCheckBox.TabIndex = 0;
            this.CollisionCheckBox.Text = "Collsion";
            this.CollisionCheckBox.UseVisualStyleBackColor = true;
            this.CollisionCheckBox.CheckedChanged += new System.EventHandler(this.CollisionCheckBox_CheckedChanged);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.label16);
            this.tabPage4.Controls.Add(this.textBoxCurrNodeTag);
            this.tabPage4.Controls.Add(this.button4);
            this.tabPage4.Controls.Add(this.label15);
            this.tabPage4.Controls.Add(this.textBoxNodeTag);
            this.tabPage4.Controls.Add(this.label12);
            this.tabPage4.Controls.Add(this.buttonAddEdge);
            this.tabPage4.Controls.Add(this.numericEdgeIndex);
            this.tabPage4.Controls.Add(this.buttonDeleteAllEdges);
            this.tabPage4.Controls.Add(this.buttonDeleteEdge);
            this.tabPage4.Controls.Add(this.buttonDeleteAllNodes);
            this.tabPage4.Controls.Add(this.buttonDeletePathNode);
            this.tabPage4.Controls.Add(this.label11);
            this.tabPage4.Controls.Add(this.label10);
            this.tabPage4.Controls.Add(this.listBoxEdges);
            this.tabPage4.Controls.Add(this.PathNodes);
            this.tabPage4.Location = new System.Drawing.Point(4, 25);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(495, 522);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Pathfinding";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(9, 323);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(55, 13);
            this.label16.TabIndex = 20;
            this.label16.Text = "Node Tag";
            // 
            // textBoxCurrNodeTag
            // 
            this.textBoxCurrNodeTag.Location = new System.Drawing.Point(12, 342);
            this.textBoxCurrNodeTag.Name = "textBoxCurrNodeTag";
            this.textBoxCurrNodeTag.ReadOnly = true;
            this.textBoxCurrNodeTag.Size = new System.Drawing.Size(100, 20);
            this.textBoxCurrNodeTag.TabIndex = 19;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(12, 282);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(100, 23);
            this.button4.TabIndex = 18;
            this.button4.Text = "Add Tag";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(12, 237);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(51, 13);
            this.label15.TabIndex = 17;
            this.label15.Text = "New Tag";
            // 
            // textBoxNodeTag
            // 
            this.textBoxNodeTag.Location = new System.Drawing.Point(12, 256);
            this.textBoxNodeTag.Name = "textBoxNodeTag";
            this.textBoxNodeTag.Size = new System.Drawing.Size(100, 20);
            this.textBoxNodeTag.TabIndex = 16;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(219, 10);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(109, 17);
            this.label12.TabIndex = 15;
            this.label12.Text = "New Edge Index";
            // 
            // buttonAddEdge
            // 
            this.buttonAddEdge.Location = new System.Drawing.Point(219, 55);
            this.buttonAddEdge.Name = "buttonAddEdge";
            this.buttonAddEdge.Size = new System.Drawing.Size(101, 23);
            this.buttonAddEdge.TabIndex = 14;
            this.buttonAddEdge.Text = "Add Edge";
            this.buttonAddEdge.UseVisualStyleBackColor = true;
            this.buttonAddEdge.Click += new System.EventHandler(this.buttonAddEdge_Click);
            // 
            // numericEdgeIndex
            // 
            this.numericEdgeIndex.Location = new System.Drawing.Point(219, 29);
            this.numericEdgeIndex.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericEdgeIndex.Name = "numericEdgeIndex";
            this.numericEdgeIndex.Size = new System.Drawing.Size(120, 22);
            this.numericEdgeIndex.TabIndex = 13;
            // 
            // buttonDeleteAllEdges
            // 
            this.buttonDeleteAllEdges.Location = new System.Drawing.Point(228, 238);
            this.buttonDeleteAllEdges.Name = "buttonDeleteAllEdges";
            this.buttonDeleteAllEdges.Size = new System.Drawing.Size(101, 23);
            this.buttonDeleteAllEdges.TabIndex = 12;
            this.buttonDeleteAllEdges.Text = "Delete All";
            this.buttonDeleteAllEdges.UseVisualStyleBackColor = true;
            this.buttonDeleteAllEdges.Click += new System.EventHandler(this.buttonDeleteAllEdges_Click);
            // 
            // buttonDeleteEdge
            // 
            this.buttonDeleteEdge.Location = new System.Drawing.Point(228, 209);
            this.buttonDeleteEdge.Name = "buttonDeleteEdge";
            this.buttonDeleteEdge.Size = new System.Drawing.Size(101, 23);
            this.buttonDeleteEdge.TabIndex = 11;
            this.buttonDeleteEdge.Text = "Delete Selected";
            this.buttonDeleteEdge.UseVisualStyleBackColor = true;
            this.buttonDeleteEdge.Click += new System.EventHandler(this.buttonDeleteEdge_Click);
            // 
            // buttonDeleteAllNodes
            // 
            this.buttonDeleteAllNodes.Location = new System.Drawing.Point(12, 180);
            this.buttonDeleteAllNodes.Name = "buttonDeleteAllNodes";
            this.buttonDeleteAllNodes.Size = new System.Drawing.Size(101, 23);
            this.buttonDeleteAllNodes.TabIndex = 10;
            this.buttonDeleteAllNodes.Text = "Delete All";
            this.buttonDeleteAllNodes.UseVisualStyleBackColor = true;
            this.buttonDeleteAllNodes.Click += new System.EventHandler(this.buttonDeleteAllNodes_Click);
            // 
            // buttonDeletePathNode
            // 
            this.buttonDeletePathNode.Location = new System.Drawing.Point(12, 151);
            this.buttonDeletePathNode.Name = "buttonDeletePathNode";
            this.buttonDeletePathNode.Size = new System.Drawing.Size(101, 23);
            this.buttonDeletePathNode.TabIndex = 9;
            this.buttonDeletePathNode.Text = "Delete Selected";
            this.buttonDeletePathNode.UseVisualStyleBackColor = true;
            this.buttonDeletePathNode.Click += new System.EventHandler(this.buttonDeletePathNode_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(216, 89);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(48, 17);
            this.label11.TabIndex = 3;
            this.label11.Text = "Edges";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 31);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(49, 17);
            this.label10.TabIndex = 2;
            this.label10.Text = "Nodes";
            // 
            // listBoxEdges
            // 
            this.listBoxEdges.FormattingEnabled = true;
            this.listBoxEdges.ItemHeight = 16;
            this.listBoxEdges.Location = new System.Drawing.Point(219, 108);
            this.listBoxEdges.Name = "listBoxEdges";
            this.listBoxEdges.Size = new System.Drawing.Size(120, 84);
            this.listBoxEdges.TabIndex = 1;
            // 
            // PathNodes
            // 
            this.PathNodes.FormattingEnabled = true;
            this.PathNodes.ItemHeight = 16;
            this.PathNodes.Location = new System.Drawing.Point(8, 50);
            this.PathNodes.Name = "PathNodes";
            this.PathNodes.Size = new System.Drawing.Size(120, 84);
            this.PathNodes.TabIndex = 0;
            this.PathNodes.SelectedIndexChanged += new System.EventHandler(this.listBoxPathNodes_SelectedIndexChanged);
            // 
            // worldPanel
            // 
            this.worldPanel.AutoSize = true;
            this.worldPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.worldPanel.BackColor = System.Drawing.SystemColors.Window;
            this.worldPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.worldPanel.Location = new System.Drawing.Point(0, 0);
            this.worldPanel.Name = "worldPanel";
            this.worldPanel.Size = new System.Drawing.Size(792, 544);
            this.worldPanel.TabIndex = 0;
            this.worldPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.worldPanel_MouseDown);
            this.worldPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.worldPanel_MouseMove);
            this.worldPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.worldPanel_MouseUp);
            // 
            // Form1
            // 
            this.ClientSize = new System.Drawing.Size(1299, 572);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing_1);
            this.Scroll += new System.Windows.Forms.ScrollEventHandler(this.Form1_Scroll);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.Layers.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.groupBoxMapSettings.ResumeLayout(false);
            this.groupBoxMapSettings.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericMapCol)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericMapRows)).EndInit();
            this.groupBoxTileSettings.ResumeLayout(false);
            this.groupBoxTileSettings.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSheetHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSheetWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericTileHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericTileWidth)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericNumEnemies)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericObjID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWaypoint)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericEdgeIndex)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem undoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem cutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem pasteToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem selectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem customizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ToolStripMenuItem loadTilesetToolStripMenuItem;
        private System.Windows.Forms.TabControl Layers;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Panel tilePanel;
        private System.Windows.Forms.Button deleteAll;
        private System.Windows.Forms.Button deleteSelected;
        private System.Windows.Forms.Panel objectPanel;
        private System.Windows.Forms.ListBox Objects;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button addEvent;
        private System.Windows.Forms.TextBox eventString;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox EventTable;
        private System.Windows.Forms.CheckBox TriggerCheckBox;
        private System.Windows.Forms.CheckBox CollisionCheckBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxEventTrigger;
        private System.Windows.Forms.GroupBox groupBoxTileSettings;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numericTileHeight;
        private System.Windows.Forms.NumericUpDown numericTileWidth;
        private System.Windows.Forms.Label labelTileHeight;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown numericUpDownSheetHeight;
        private System.Windows.Forms.NumericUpDown numericSheetWidth;
        private System.Windows.Forms.GroupBox groupBoxMapSettings;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numericMapCol;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.NumericUpDown numericMapRows;
        private System.Windows.Forms.ListBox WayPoints;
        private System.Windows.Forms.Button buttonDeleteAllEvents;
        private System.Windows.Forms.Button buttonDeleteEvent;
        private System.Windows.Forms.Button buttonDeleteAllWaypoints;
        private System.Windows.Forms.Button buttonDeleteWaypoint;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.ListBox listBoxCollisionObjects;
        private System.Windows.Forms.Button buttonAddTileEvent;
        private System.Windows.Forms.ListBox listBoxTileEvents;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown numericWaypoint;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ListBox PathNodes;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button buttonAddEdge;
        private System.Windows.Forms.NumericUpDown numericEdgeIndex;
        private System.Windows.Forms.Button buttonDeleteAllEdges;
        private System.Windows.Forms.Button buttonDeleteEdge;
        private System.Windows.Forms.Button buttonDeleteAllNodes;
        private System.Windows.Forms.Button buttonDeletePathNode;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ListBox listBoxEdges;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Button buttonNameObjTile;
        private System.Windows.Forms.TextBox textBoxObjName;
        private System.Windows.Forms.NumericUpDown numericObjID;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox textBoxNumEvents;
        private System.Windows.Forms.Button buttonSetMapSize;
        public System.Windows.Forms.Panel worldPanel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.NumericUpDown numericNumEnemies;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox textBoxCurrNodeTag;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBoxNodeTag;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.ComboBox comboBoxAIStates;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBoxStartState;


    }
}

