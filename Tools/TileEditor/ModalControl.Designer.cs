namespace TileEditor
{
    partial class ModalControl
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.numericUpDownMapC = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownMapR = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.numericUpDownTileC = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTileR = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTileH = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTileW = new System.Windows.Forms.NumericUpDown();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.helpProvider1 = new System.Windows.Forms.HelpProvider();
            this.buttonApply = new System.Windows.Forms.Button();
            this.buttonReset = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMapC)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMapR)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileC)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileR)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileH)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileW)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.numericUpDownMapC);
            this.groupBox1.Controls.Add(this.numericUpDownMapR);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(210, 82);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Map Size";
            // 
            // numericUpDownMapC
            // 
            this.numericUpDownMapC.Location = new System.Drawing.Point(109, 50);
            this.numericUpDownMapC.Name = "numericUpDownMapC";
            this.numericUpDownMapC.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownMapC.TabIndex = 7;
            this.numericUpDownMapC.ValueChanged += new System.EventHandler(this.numericUpDownMapC_ValueChanged);
            // 
            // numericUpDownMapR
            // 
            this.numericUpDownMapR.Location = new System.Drawing.Point(109, 23);
            this.numericUpDownMapR.Name = "numericUpDownMapR";
            this.numericUpDownMapR.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownMapR.TabIndex = 6;
            this.numericUpDownMapR.ValueChanged += new System.EventHandler(this.numericUpDownMapR_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 52);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Columns";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(19, 25);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Rows";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.numericUpDownTileC);
            this.groupBox2.Controls.Add(this.numericUpDownTileR);
            this.groupBox2.Controls.Add(this.numericUpDownTileH);
            this.groupBox2.Controls.Add(this.numericUpDownTileW);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(12, 100);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(210, 130);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tile Size";
            // 
            // numericUpDownTileC
            // 
            this.numericUpDownTileC.Location = new System.Drawing.Point(109, 98);
            this.numericUpDownTileC.Name = "numericUpDownTileC";
            this.numericUpDownTileC.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownTileC.TabIndex = 7;
            this.numericUpDownTileC.ValueChanged += new System.EventHandler(this.numericUpDownTileC_ValueChanged);
            // 
            // numericUpDownTileR
            // 
            this.numericUpDownTileR.Location = new System.Drawing.Point(109, 72);
            this.numericUpDownTileR.Name = "numericUpDownTileR";
            this.numericUpDownTileR.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownTileR.TabIndex = 6;
            this.numericUpDownTileR.ValueChanged += new System.EventHandler(this.numericUpDownTileR_ValueChanged);
            // 
            // numericUpDownTileH
            // 
            this.numericUpDownTileH.Location = new System.Drawing.Point(109, 46);
            this.numericUpDownTileH.Name = "numericUpDownTileH";
            this.numericUpDownTileH.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownTileH.TabIndex = 5;
            this.numericUpDownTileH.ValueChanged += new System.EventHandler(this.numericUpDownTileH_ValueChanged);
            // 
            // numericUpDownTileW
            // 
            this.numericUpDownTileW.Location = new System.Drawing.Point(109, 20);
            this.numericUpDownTileW.Name = "numericUpDownTileW";
            this.numericUpDownTileW.Size = new System.Drawing.Size(54, 20);
            this.numericUpDownTileW.TabIndex = 4;
            this.numericUpDownTileW.ValueChanged += new System.EventHandler(this.numericUpDownTileW_ValueChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(19, 100);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(47, 13);
            this.label8.TabIndex = 3;
            this.label8.Text = "Columns";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(19, 74);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(34, 13);
            this.label7.TabIndex = 2;
            this.label7.Text = "Rows";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(19, 48);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Height";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(19, 22);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Width";
            // 
            // buttonApply
            // 
            this.buttonApply.Location = new System.Drawing.Point(12, 236);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(103, 23);
            this.buttonApply.TabIndex = 2;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // buttonReset
            // 
            this.buttonReset.Location = new System.Drawing.Point(121, 236);
            this.buttonReset.Name = "buttonReset";
            this.buttonReset.Size = new System.Drawing.Size(101, 23);
            this.buttonReset.TabIndex = 3;
            this.buttonReset.Text = "Reset";
            this.buttonReset.UseVisualStyleBackColor = true;
            this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
            // 
            // ModalControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(234, 267);
            this.Controls.Add(this.buttonReset);
            this.Controls.Add(this.buttonApply);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "ModalControl";
            this.Text = "Map/Tile Properties";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMapC)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMapR)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileC)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileR)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileH)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTileW)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown numericUpDownMapC;
        private System.Windows.Forms.NumericUpDown numericUpDownMapR;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericUpDownTileC;
        private System.Windows.Forms.NumericUpDown numericUpDownTileR;
        private System.Windows.Forms.NumericUpDown numericUpDownTileH;
        private System.Windows.Forms.NumericUpDown numericUpDownTileW;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.HelpProvider helpProvider1;
        private System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.Button buttonReset;
    }
}