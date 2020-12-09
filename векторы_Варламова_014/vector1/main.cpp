#include <iostream>

using namespace std;

class vector
{
    int sz;  // размер вектора
    double* el;  // указатель на элементы
public:
    vector(int r) :sz(r), el(new double [r])  // конструктор, выделяет память
    {
        for(int i = 0; i<r; ++i) el[i] = 0;  // инициализация
    }
    double get(int n) {return el[n];} // чтение
    void set(int n, double v)  {el[n] = v;} // запись
    int size() const {return sz; }  // количество элементов
    ~vector() {delete [] el;} // деструктор, освобождает память
};

int main() // тесты
{
    vector v1 (6);
    v1.set(1,1.5);
    v1.set(6,6.4);
    v1.set(2,3.7);
    v1.set(6,8.4);
    v1.set(4,4.2);
    v1.set(3,11.1);
    cout << "size = " << v1.size() << endl;
    for (int i = 0; i < v1.size(); i++)
        cout << i << " element = " << v1.get(i) << endl;
}
