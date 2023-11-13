#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::SetCompatibleTextRenderingDefault(false);
    Application::EnableVisualStyles();
    simple3dviewerproject::MainForm form;
    Application::Run(% form);
    return 0; 
}

//Глобальные переменные
std::vector<Coord> Vertices;
std::vector<std::vector<int>> Faces;
unsigned int NumberOfVertices, NumberIfFaces, NumberOfEdges;




//
//Функции
//

System::Void simple3dviewerproject::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
    /*
    Настройка ручки, получение размера холста, создание bitmap и canvas
    */
    blackSolidThinPen = gcnew Pen(Color::Black);
    blackSolidThinPen->Width = 1;
    greenSolidThinPen = gcnew Pen(Color::Green);
    greenSolidThinPen->Width = 1;
    rnadColorPen = gcnew Pen(Color::Green);
    rnadColorPen->Width = 1;

    width = pictureBox1->Width;
    height = pictureBox1->Height;
    halfWidth = width / 2;
    halfHeight = height / 2;

    bmp = gcnew Bitmap(width, height);
    canvas = Graphics::FromImage(bmp);
    
    //Считываем данные о фигуре из файла    
    using namespace std;
    ifstream file;
    //file.open("cube.off", ios_base::in);
    file.open("handv2.off", ios_base::in);

    //Если открытие файла прошло успешно
    if (file.is_open())
    {
        //Первая строка OFF
        string s;
        double x;
        file >> s;

        //Вторая строка: кол-во вершин, поверхностей, граней
        file >> NumberOfVertices >> NumberIfFaces >> NumberOfEdges;

        //Задаем размер вектора вершин
        Vertices.resize(NumberOfVertices);

        //Записываем вершины в вектор
        for (unsigned int i = 0; i < NumberOfVertices; i++)
        {
            file >> Vertices[i].x >> Vertices[i].y >> Vertices[i].z;
        }

        //Задаем размер вектора граней
        Faces.resize(NumberIfFaces);

        //Записываем информацию о гранях в вектор
        int colSize;
        for (unsigned int i = 0; i < NumberIfFaces; i++)
        {
            file >> colSize;
            //colSize += 3;
            Faces[i].resize(colSize);
            for (int j = 0; j < colSize; j++)
            {
                file >> Faces[i][j];
            }
            //file >> x >> x >> x;
        }
    }
    else
    {
        MessageBox::Show("File error", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

    file.close();
    
    return System::Void();
}

/// <summary>
/// Переводит координаты из одного вида в другой
/// </summary>
/// <returns></returns>
Coord simple3dviewerproject::MainForm::changeCoordinates(const Coord& OriginalCoord)
{
    Coord coord;

    coord.x = -round(halfHeight * OriginalCoord.x) + halfWidth;
    coord.y = -round(halfHeight * OriginalCoord.y) + halfHeight;
    coord.z = round(100 * OriginalCoord.z);

    return coord;
}

/// <summary>
/// Рисует фигуру на основе векторов вершин и поверхностей
/// </summary>
/// <returns></returns>
System::Void simple3dviewerproject::MainForm::drawEdging()
{
    canvas->Clear(Color::White);

    std::vector<Coord> newVertices (NumberOfVertices);

    for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        newVertices[i] = changeCoordinates(Vertices[i]);
    }

    int x1, x2, y1, y2;
    int j;
    for (unsigned int i = 0; i < NumberIfFaces; i++)
    {
        for (j = 0; j < Faces[i].size(); j++)
        {
            x1 = static_cast<int>(newVertices[Faces[i][j]].x);
            y1 = static_cast<int>(newVertices[Faces[i][j]].y);
            x2 = static_cast<int>(newVertices[Faces[i][(j + 1) % (Faces[i].size())]].x);
            y2 = static_cast<int>(newVertices[Faces[i][(j + 1) % (Faces[i].size())]].y);
            canvas->DrawLine(blackSolidThinPen, x1, y1, x2, y2);
        }
    }

    pictureBox1->Image = bmp;

    return System::Void();
}

System::Void simple3dviewerproject::MainForm::drawEdging(std::vector<Coord>& vec)
{
    canvas->Clear(Color::White);

    std::vector<Coord> newVertices(NumberOfVertices);

    for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        newVertices[i] = changeCoordinates(vec[i]);
    }

    int x1, x2, y1, y2;
    int j;
    for (unsigned int i = 0; i < NumberIfFaces; i++)
    {
        for (j = 0; j < Faces[i].size(); j++)
        {
            x1 = static_cast<int>(newVertices[Faces[i][j]].x);
            y1 = static_cast<int>(newVertices[Faces[i][j]].y);
            x2 = static_cast<int>(newVertices[Faces[i][(j + 1) % (Faces[i].size())]].x);
            y2 = static_cast<int>(newVertices[Faces[i][(j + 1) % (Faces[i].size())]].y);
            canvas->DrawLine(blackSolidThinPen, x1, y1, x2, y2);
        }
    }

    pictureBox1->Image = bmp;

    return System::Void();
}

void simple3dviewerproject::MainForm::filledTriangle(Coord c1, Coord c2, Coord c3)
{
    c1 = changeCoordinates(c1);
    c2 = changeCoordinates(c2);
    c3 = changeCoordinates(c3);
    

    //rnadColorPen->Color = Color::FromArgb(rand() % 255, rand() % 255, rand() & 255);
    //rnadColorPen->Color = color;

    if (c2.y > c3.y)
    {
        std::swap(c3.y, c2.y);
        std::swap(c3.x, c2.x);
    }
    if (c1.y > c2.y)
    {
        std::swap(c2.y, c1.y);
        std::swap(c2.x, c1.x);
    }
    if (c2.y > c3.y)
    {
        std::swap(c3.y, c2.y);
        std::swap(c3.x, c2.x);
    }

    double dx1, dx2;
    c2.y == c1.y ? dx1 = 0 : dx1 = (-(c1.x - c2.x) / (c2.y - c1.y));
    c3.y == c1.y ? dx2 = 0 : dx2 = (-(c1.x - c3.x) / (c3.y - c1.y));

    double lx, rx;
    lx = c1.x;
    rx = c1.x;

    for (int i = c1.y; i <= c2.y; i++)
    {
        canvas->DrawLine(rnadColorPen, rx, i, lx, i);
        lx += dx1;
        rx += dx2;
    }

    c3.y == c1.y ? dx1 = 0 : dx1 = (-(c3.x - c1.x) / (c3.y - c1.y));
    c3.y == c2.y ? dx2 = 0 : dx2 = (-(c3.x - c2.x) / (c3.y - c2.y));
    lx = c3.x;
    rx = c3.x;

    for (int i = c3.y; i > c2.y; i--)
    {
        lx += dx1;
        rx += dx2;
        canvas->DrawLine(rnadColorPen, rx, i, lx, i);
    }

    pictureBox1->Image = bmp;
}

System::Void simple3dviewerproject::MainForm::drawPolygons(std::vector<Coord> Vertices)
{
    /*Matrix mt(4, 4);

    mt(0, 0) = 1;
    mt(1, 1) = 1;
    mt(2, 3) = 1 / 3;
    mt(3, 3) = 1;*/

    /*for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        Vertices[i] = mt.mult4x(Vertices[i]);
        Vertices[i].x = Vertices[i].x / Vertices[i].d;
        Vertices[i].y = Vertices[i].y / Vertices[i].d;
    }*/

    canvas->Clear(Color::Black);

    //std::vector<Coord> newVertices (NumberOfVertices);

    /*for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        newVertices[i] = changeCoordinates(Vertices[i]);
    }*/

    //for (unsigned int i = 0; i < NumberIfFaces; i++)
    //{
    //    rnadColorPen->Color = Color::FromArgb(Faces[i][3], Faces[i][4], Faces[i][5]);
    //    //color = color.ToArgb(Faces[i][3], Faces[i][4], Faces[i][5]);
    //    filledTriangle(newVertices[Faces[i][0]], newVertices[Faces[i][1]], newVertices[Faces[i][2]]);
    //}

    /*for (unsigned int i = 0; i < NumberOfVertices; i++)
    {
        Vertices[i] = changeCoordinates(Vertices[i]);
    }*/

    Coord c, v1, v2;
    double intenc;
    double l;
    for (unsigned int i = 0; i < NumberIfFaces; i++)
    {
        v1.x = Vertices[Faces[i][1]].x - Vertices[Faces[i][0]].x;
        v1.y = Vertices[Faces[i][1]].y - Vertices[Faces[i][0]].y;
        v1.z = Vertices[Faces[i][1]].z - Vertices[Faces[i][0]].z;

        v2.x = Vertices[Faces[i][2]].x - Vertices[Faces[i][0]].x;
        v2.y = Vertices[Faces[i][2]].y - Vertices[Faces[i][0]].y;
        v2.z = Vertices[Faces[i][2]].z - Vertices[Faces[i][0]].z;

        //векторное произведение
        c.x = v1.y * v2.z - v1.z * v2.y;
        c.y = v1.z * v2.x - v1.x * v2.z;
        c.z = v1.x * v2.y - v1.y * v2.x;

        l = sqrt(c.x * c.x + c.y * c.y + c.z * c.z);

        c.x = c.x / l;
        c.y = c.y / l;
        c.z = c.z / l;
        
        //скалярное произведение
        intenc = -1 * c.z;
        intenc *= 255;
        if (intenc > 255)
            intenc = 255;
        if (intenc > 0)
        {
            rnadColorPen->Color = Color::FromArgb(intenc, intenc, intenc);
            filledTriangle(Vertices[Faces[i][0]], Vertices[Faces[i][1]], Vertices[Faces[i][2]]);
        }
        //rnadColorPen->Color = Color::FromArgb(255 - Faces[i][3], 255 - Faces[i][4], 255 - Faces[i][5]);
        //color = color.ToArgb(Faces[i][3], Faces[i][4], Faces[i][5]);
    }

    pictureBox1->Image = bmp;
    return System::Void();
}

/// <summary>
/// Смещает фигуру на XYZ
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

void simple3dviewerproject::MainForm::rotateY(std::vector<Coord>& vec, double a)
{
    a = a * 3.14 / 180;

    Matrix mt(3, 3);
    mt(0, 0) = cos(-a);
    mt(0, 2) = -sin(-a);
    mt(1, 1) = 1;
    mt(2, 0) = sin(-a);
    mt(2, 2) = cos(-a);

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult3x(vec[i]);
    }    
}

void simple3dviewerproject::MainForm::rotateX(std::vector<Coord>& vec, double a)
{
    a = a * 3.14 / 180;

    Matrix mt(3, 3);
    mt(0, 0) = 1;
    mt(1, 1) = cos(a);
    mt(1, 2) = -sin(a);
    mt(2, 1) = sin(a);
    mt(2, 2) = cos(a);

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult3x(vec[i]);
    }
}

void simple3dviewerproject::MainForm::rotateZ(std::vector<Coord>& vec, double a)
{
    a = a * 3.14 / 180;

    Matrix mt(3, 3);
    mt(0, 0) = cos(a);
    mt(0, 1) = -sin(a);
    mt(1, 0) = sin(a);
    mt(1, 1) = cos(a);
    mt(2, 2) = 1;

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult3x(vec[i]);
    }
}

void simple3dviewerproject::MainForm::viewFromAbove(std::vector<Coord> vec)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].y = vec[i].z;
    }

    drawEdging(vec);
}

void simple3dviewerproject::MainForm::sideView(std::vector<Coord> vec)
{
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i].x = vec[i].z;
    }

    drawEdging(vec);
}

void simple3dviewerproject::MainForm::isometry(std::vector<Coord> vec)
{
    double p = 45 * 3.14 / 180;
    double f = 35.264 * 3.14 / 180;

    Matrix mt(4, 4);

    mt(0, 0) = cos(p);
    mt(0, 1) = sin(f) * sin(p);
    mt(1, 1) = cos(f);
    mt(2, 0) = sin(p);
    mt(2, 1) = -sin(f) * cos(p);
    mt(3, 3) = 1;

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult4x(vec[i]);
    }

    drawEdging(vec);
}

void simple3dviewerproject::MainForm::dimetry(std::vector<Coord> vec)
{
    double p = 22.208 * 3.14 / 180;
    double f = 20.705 * 3.14 / 180;
    Matrix mt(4, 4);

    mt(0, 0) = cos(p);
    mt(0, 1) = sin(f) * sin(p);
    mt(1, 1) = cos(f);
    mt(2, 0) = sin(p);
    mt(2, 1) = -sin(f) * cos(p);
    mt(3, 3) = 1;

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult4x(vec[i]);
    }

    drawEdging(vec);
}

void simple3dviewerproject::MainForm::trimetry(std::vector<Coord> vec)
{
    double p = 40 * 3.14 / 180;
    double f = 55 * 3.14 / 180;
    Matrix mt(4, 4);

    mt(0, 0) = cos(p);
    mt(0, 1) = sin(f) * sin(p);
    mt(1, 1) = cos(f);
    mt(2, 0) = sin(p);
    mt(2, 1) = -sin(f) * cos(p);
    mt(3, 3) = 1;

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult4x(vec[i]);
    }

    drawEdging(vec);
}

void simple3dviewerproject::MainForm::singlePointProjection(std::vector<Coord> vec, const double& d)
{
    Matrix mt(4, 4);

    mt(0, 0) = 1;
    mt(1, 1) = 1;
    mt(2, 3) = 1 / d;
    mt(3, 3) = 1;

    for (unsigned int i = 0; i < vec.size(); i++)
    {
        vec[i] = mt.mult4x(vec[i]);
        vec[i].x = vec[i].x / vec[i].d;
        vec[i].y = vec[i].y / vec[i].d;
    }

    drawEdging(vec);
}

System::Void simple3dviewerproject::MainForm::MainForm_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
    switch (e->KeyChar)
    {
    case 's':
        offsetCoordinates(Vertices, 0, -0.1, 0);
        drawEdging();
        break;
    case 'w':
        offsetCoordinates(Vertices, 0, 0.1, 0);
        drawEdging();
        break;
    case 'd':
        offsetCoordinates(Vertices, -0.1, 0, 0);
        drawEdging();
        break;
    case 'a':
        offsetCoordinates(Vertices, 0.1, 0, 0);
        drawEdging();
        break;
    case 'q':
        scalingCoordinates(Vertices, 0.9);
        drawEdging();
        break;
    case 'e':
        scalingCoordinates(Vertices, 1.1);
        drawEdging();
        break;
    case '/':
        timer1->Enabled ? timer1->Enabled = false : timer1->Enabled = true;
        break;
    case '.':
        timer2->Enabled ? timer2->Enabled = false : timer2->Enabled = true;
        break;
    case '1':
        rotateY(Vertices, 10);
        drawEdging();
        break;
    case '2':
        rotateX(Vertices, 10);
        drawEdging();
        break;
    case '3':
        rotateZ(Vertices, 10);
        drawEdging();
        break;
    case 'z':
        drawEdging();
        break;
    case 'x':
        viewFromAbove(Vertices);
        break;
    case 'c':
        sideView(Vertices);
        break;
    case ']':
        isometry(Vertices);
        break;
    case '[':
        dimetry(Vertices);
        break;
    case 'p':
        trimetry(Vertices);
        break;
    case 'o':
        singlePointProjection(Vertices, 3);
        break;
    case 'i':
        drawPolygons(Vertices);
        break;
    default:
        e->Handled;
        break;
    }
    
    return System::Void();
}

System::Void simple3dviewerproject::MainForm::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
    rotateY(Vertices, 5);
    drawPolygons(Vertices);
    return System::Void();
}

System::Void simple3dviewerproject::MainForm::timer2_Tick(System::Object^ sender, System::EventArgs^ e)
{
    rotateY(Vertices, 5);
    rotateZ(Vertices, 5);
    singlePointProjection(Vertices, 3);
    return System::Void();
}