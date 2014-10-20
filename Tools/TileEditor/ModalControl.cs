using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TileEditor
{
    public partial class ModalControl : Form
    {
        public event ApplyEventHandler Apply;

        public event ApplyEventHandler Reset;

        public ModalControl()
        {
            InitializeComponent();
        }

        private Size tileSize;

        public Size TileSize
        {
            get { return tileSize; }

            set { tileSize = value; }
        }

        private Size mapSize;

        public Size MapSize
        {
            get { return mapSize; }

            set { mapSize = value; }
        }

        private Size palSize;

        public Size PalSize
        {
            get { return PalSize; }

            set { palSize = value; }
        }

        public delegate void ApplyEventHandler(object sender, ApplyEventArgs e);
        public class ApplyEventArgs : EventArgs
        {
            private Size tSize;

            public Size TSize
            {
                get { return tSize; }

                set { tSize = value; }
            }

            private Size mSize;
            public Size MSize
            {
                get { return mSize; }

                set { mSize = value; }
            }

            private Size pSize;

            public Size PSize
            {
                get { return pSize; }

                set { pSize = value; }
            }

            public ApplyEventArgs(int tSizeW,int tSizeH, int mSizeW, int mSizeH, int pSizeW, int pSizeH)
            {
                this.tSize.Width = tSizeW;
                this.tSize.Height = tSizeH;
                this.mSize.Width = mSizeW;
                this.mSize.Height = mSizeH;
                this.pSize.Width = pSizeW;
                this.pSize.Height = pSizeH;
            }

          //  public ApplyEventArgs


        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            if (Apply != null)
            {
                Apply(this, new ApplyEventArgs((int)numericUpDownTileW.Value, (int)numericUpDownTileH.Value, (int)numericUpDownMapR.Value, (int)numericUpDownMapC.Value, (int)numericUpDownTileR.Value , (int)numericUpDownTileC.Value));
            }
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDownMapR_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownMapR.Value != 0)
            {
                mapSize.Width = (int)numericUpDownMapR.Value;
            }
        }

        private void numericUpDownMapC_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownMapC.Value != 0)
            {
                mapSize.Height = (int)numericUpDownMapR.Value;
            }
        }

        private void numericUpDownTileW_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownTileW.Value != 0)
            {
                tileSize.Width = (int)numericUpDownTileW.Value;
            }
        }

        private void numericUpDownTileH_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownTileH.Value != 0)
            {
                tileSize.Height = (int)numericUpDownTileH.Value;
            }
        }

        private void numericUpDownTileR_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownTileR.Value != 0)
            {
                palSize.Width = (int)numericUpDownTileR.Value;
            }
        }

        private void numericUpDownTileC_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownTileC.Value != 0)
            {
                palSize.Height = (int)numericUpDownTileC.Value;
            }
        }



    }
}
