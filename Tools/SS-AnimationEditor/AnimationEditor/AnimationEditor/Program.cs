using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AnimationEditor
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


            // Make the Form
            Form1 theForm = new Form1();

            // Display the Form
            theForm.Show();

            // Initialize the form
            theForm.Initialize();


            // Starting time
            int nNow = System.Environment.TickCount;



            // Start the main program loop
            while (theForm.Looping)
            {
                // Elapsed time
                int nBefore = nNow;
                nNow = System.Environment.TickCount;
                float dt = (nNow - nBefore) / 1000.0f;

                // Call our forms Update function
                theForm.UpdateForm(dt);

                // Call our forms Render function
                theForm.RenderAll();

                // Handle events
                Application.DoEvents();
            }


            // Terminate the form
            theForm.Terminate();
        }
    }
}
