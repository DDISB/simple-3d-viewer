#include "MainForm.h"
#include <Windows.h>


using namespace simple3dviewerproject;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm);
    return 0;
}

std::vector<Coord> Vertices;
std::vector<std::vector<int>> Faces;

unsigned int NumberOfVertices, NumberIfFaces, NumberOfEdges;

System::Void simple3dviewerproject::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    /*
    ��������� �����, ��������� ������� ������, �������� bitmap � canvas
    */
    blackSolidThinPen = gcnew Pen(Color::Black);
    blackSolidThinPen->Width = 1;

    width = pictureBox1->Width;
    height = pictureBox1->Height;
    halfWidth = width / 2;
    halfHeight = height / 2;

    bmp = gcnew Bitmap(width, height);
    canvas = Graphics::FromImage(bmp);
    
    //��������� ������ � ������ �� �����    
    using namespace std;
    ifstream file;
    //file.open("cube.off", ios_base::in);
    file.open("hand.off", ios_base::in);

    //���� �������� ����� ������ �������
    if (file.is_open())
    {
        //������ ������ OFF
        string s;
        file >> s;

        //������ ������: ���-�� ������, ������������, ������
        file >> NumberOfVertices >> NumberIfFaces >> NumberOfEdges;

        //������ ������ ������� ������
        Vertices.resize(NumberOfVertices);

        //���������� ������� � ������
        for (unsigned int i = 0; i < NumberOfVertices; i++)
        {
            file >> Vertices[i].x >> Vertices[i].y >> Vertices[i].z;
        }

        //������ ������ ������� ������
        Faces.resize(NumberIfFaces);

        //���������� ���������� � ������ � ������
        int rowSize;
        for (unsigned int i = 0; i < NumberIfFaces; i++)
        {
            file >> rowSize;
            Faces[i].resize(rowSize);
            for (int j = 0; j < rowSize; j++)
            {
                file >> Faces[i][j];
            }
        }
    }
    else
    {
        throw ;
    }

    file.close();
    
    return System::Void();
}

/// <summary>
/// ��������� ���������� �� ������ ���� � ������
/// </summary>
/// <returns></returns>
Coord simple3dviewerproject::MainForm::changeCoordinates(const Coord& OriginalCoord)
{
    Coord coord;

    coord.x = round(halfHeight * OriginalCoord.x) + halfWidth;
    coord.y = round(halfHeight * OriginalCoord.y) + halfHeight;
    coord.z = round(100 * OriginalCoord.z);

    return coord;
}

/// <summary>
/// ������ ������ �� ������ �������� ������ � ������������
/// </summary>
/// <returns></returns>
System::Void simple3dviewerproject::MainForm::drawFigure()
{
    canvas->Clear(Color::White);

    std::vector<Coord> newVertices = Vertices;

    for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        newVertices[i] = changeCoordinates(Vertices[i]);
    }

    int x1, x2, y1, y2;
    int j;
    for (unsigned int i = 0; i < NumberIfFaces; i++)
    {
        for (j = 1; j < Faces[i].size(); j++)
        {
            x1 = static_cast<int>(newVertices[Faces[i][j - 1]].x);
            y1 = static_cast<int>(newVertices[Faces[i][j - 1]].y);
            x2 = static_cast<int>(newVertices[Faces[i][j]].x);
            y2 = static_cast<int>(newVertices[Faces[i][j]].y);
            canvas->DrawLine(blackSolidThinPen, x1, y1, x2, y2);
        }
        x1 = static_cast<int>(newVertices[Faces[i][0]].x);
        y1 = static_cast<int>(newVertices[Faces[i][0]].y);
        x2 = static_cast<int>(newVertices[Faces[i][j - 1]].x);
        y2 = static_cast<int>(newVertices[Faces[i][j - 1]].y);
        canvas->DrawLine(blackSolidThinPen, x1, y1, x2, y2);
    }

    pictureBox1->Image = bmp;

    return System::Void();
}

/// <summary>
/// ������� ������ �� XYZ
/// </summary>
/// <param name=""></param>
void simple3dviewerproject::MainForm::offsetCoordinates(std::vector<Coord>& vec, const double& x, const double& y, const double& z)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].x += x;
        vec[i].y += y;
        vec[i].z += z;
    }

    return System::Void();
}

void simple3dviewerproject::MainForm::scalingCoordinates(std::vector<Coord>& vec, const double& dx)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].x *= dx;
        vec[i].y *= dx;
        vec[i].z *= dx;
    }
    return System::Void();
}

System::Void simple3dviewerproject::MainForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    offsetCoordinates(Vertices, -0.1, 0, 0);

    drawFigure();    

    return System::Void();
}

System::Void simple3dviewerproject::MainForm::MainForm_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
    switch (e->KeyChar)
    {
    case 's':
        offsetCoordinates(Vertices, 0, 0.1, 0);
        drawFigure();
        break;
    case 'w':
        offsetCoordinates(Vertices, 0, -0.1, 0);
        drawFigure();
        break;
    case 'd':
        offsetCoordinates(Vertices, 0.1, 0, 0);
        drawFigure();
        break;
    case 'a':
        offsetCoordinates(Vertices, -0.1, 0, 0);
        drawFigure();
        break;
    case 'q':
        scalingCoordinates(Vertices, 0.9);
        drawFigure();
        break;
    case 'e':
        scalingCoordinates(Vertices, 1.1);
        drawFigure();
        break;
    case 'z':
        timer1->Enabled ? timer1->Enabled = false : timer1->Enabled = true;
        break;
    case '[':
        rotateY(Vertices, 10);
        drawFigure();
        break;
    default:
        e->Handled;
        break;
    }

    return System::Void();
}

System::Void simple3dviewerproject::MainForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
    /*offsetCoordinates*/(Vertices, 0.01, 0, 0);
    rotateY(Vertices, 5);
    drawFigure();
    return System::Void();
}

void simple3dviewerproject::MainForm::rotateY(std::vector<Coord>& vec, double a)
{
    a = a * 3.14 / 180;
    double x,z;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        x = vec[i].x;
        z = vec[i].z;
        vec[i].x = x * cos(a) + z * sin(a);
        vec[i].z = -x * sin(a) + z * cos(a);
    }
    
}