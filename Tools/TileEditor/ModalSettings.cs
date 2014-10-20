using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Data;
using System.Windows.Forms;
using System.Drawing;


namespace TileEditor
{
    class ModalSettings : Form
    {
        public event ApplyEventHandler aeh;

        public ModalSettings()
        {
            InitializeLifetimeService();
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

        public delegate void ApplyEventHandler(object sender, ApplyEventArgs ea);
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

        }

    }
}
