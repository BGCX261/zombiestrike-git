using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TileEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());


            // Create the UI form
            Form1 myForm = new Form1();
            myForm.Show();

            // Initialize the form
            myForm.Initialize();


            // "Message" loop (actually C# events)
            while (myForm.Running)
            {
                // Update & render the form
                myForm.Refresh();
                myForm.RenderAll();

                // Process the UI events
                Application.DoEvents();
            }

            // Terminate the form
            myForm.Terminate();
        }
    }
}
