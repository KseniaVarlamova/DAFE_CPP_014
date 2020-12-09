#include <iostream>

using namespace std;

class vector
{
    int sz;                         // размер
    double* el;                     // указатель на элементы
public:
    vector(int s) :sz{s}, el{new double [sz]}     // конструктор, выделяет память
    {
        for(int i = 0; i<sz; ++i) el[i] = 0.0;    // инициализация
    }
    int size() const {return sz; }              // количество элементов
    ~vector() {delete [] el;}                   // деструктор, освобождает память
    vector (initializer_list<double> lst)       // конструктор со списком инициализации
    : sz (lst.size()), el {new double [sz]}     // неициализированная память
    {
        copy (lst.begin (), lst.end () , el) ;  // инициализация
    }
    vector (const vector&) ;                    // 1) копирующий конструктор
    vector& operator = (const vector&);         // 2) копирующее присваивание
    vector (vector&& a) ;                       // 3) перемещающий конструктор
    vector& operator = (vector&&) ;             // 4) перемещающее присваивание
    double& operator [] (int n)  { return el [n] ; }            // для обращения по индексу
    const double& operator [] (int n) const  { return el[n] ;}; // для обращения по индексу
};

// 1)
vector::vector(const vector& arg): sz{arg.sz}, el{new double[arg.sz]}   // размещает элементы, инициализирует их путём копирования
{
    copy(arg.el, arg.el+arg.sz, el);
}

// 2)
vector& vector::operator = (const vector& a)  // делает вектор копией
{
    double *p = new double [a.sz];
    copy (a.el, a.el+a.sz ,p);
    delete [] el;
    el = p;
    sz = a.sz;
    return *this;
}

// 3)
vector::vector (vector&& a)
: sz{a.sz}, el{a.el} // копирует el и sz из а
{
    a.sz = 0; // делает вектор а пустым
    a.el = nullptr;
}

// 4)
vector& vector::operator = (vector&& a) // перемещает а в текущий вектор
{
    delete[] el;          // освобождение старой памяти
    el = a.el;            // копирование el и sz из а
    sz = a.sz;
    a.el = nullptr;       // делает вектор а пустым
    a.sz = 0;
    return *this;         // возврат ссылки
}

int main()
{
    vector v1 (20);
    cout << "v1 = {";
    for (int i = 0; i < v1.size(); i++)
    {
        v1[i] = i * i;
        cout << v1[i] << ", ";
    }
    cout << "}" << endl;
    vector v2 = {1.5,3.3,4.6,10,5};
    cout << "size v1: " << v2.size() << "; 2nd element: " << v2[2]<< endl;
    v1[5] = v2[2];
    cout << v1[5] << " = " << v2[2] << endl;
    v1 = v2;
    cout << "New size v1: " << v1.size() << "; new 2nd element: " << v1[2] << endl;
    v2[2] = 10;
    cout << "new 2nd element: " << v2[2] << "; old 2nd element: " << v1[2] << endl;
    const vector v3 = {1,2,3,4,5};

    return 0;
}
