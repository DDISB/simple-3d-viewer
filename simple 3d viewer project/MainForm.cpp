#include "MainForm.h"

#include <Windows.h>

using namespace simple3dviewerproject;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm);
    return 0;
}


System::Void simple3dviewerproject::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    //blackSolidThinPen = gcnew Pen(Color::Black);
    blackSolidThinPen = gcnew Pen(Color::Tomato);
    blackSolidThinPen->Width = 1;

    bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
    canvas = Graphics::FromImage(bmp);

    
    return System::Void();
}

System::Void simple3dviewerproject::MainForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{    
    canvas->Clear(Color::White);
    canvas->DrawLine(blackSolidThinPen, 100, 100, 400, 400);

    pictureBox1->Image = bmp;

    return System::Void();
}

