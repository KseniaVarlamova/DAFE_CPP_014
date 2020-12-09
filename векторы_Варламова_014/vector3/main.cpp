#include <iostream>
#include <string>
#include "std_lib_facilities.h"

using namespace std;

template <typename T, typename A>
class Vb
{
public:
    int sz;                         //размер вектора
    T *elem;                        //указатель на 1-ый элемент
    int space;                      //кол-во элементов + свободное место или слоты для неинициализированных элементов
    A alloc;                        //работает с памятью, вьделяемой для элементов
    Vb()
    {
        sz = 0;
        space = 0;
        elem = nullptr;
    }
    Vb(A &a, int n)
    {
        alloc = a;
        elem = a.allocate(n);
        sz = n;
        space = n;
    }
    ~Vb()
    {
        alloc.deallocate(elem, space);
    }
};

template <typename T, typename A = allocator<T>>
class vectop: private Vb<T,A>              //класс для всех типов Т
{
public:
    vectop()                              // конструктор
    {
        this->sz = 0;
        this->elem = nullptr;
        this->space = 0;
    };

    vectop(initializer_list<T> lst)       // конструктор со списком инициализации
    {
        this->sz = lst.size();
        this->elem = new T[this->sz];
        copy(lst.begin(), lst.end (), this->elem) ;  // инициализация
    }

    explicit vectop(int s) // конструктор, выделяет память
    {
        this->sz =  s;
        this->space = s;
        this->elem = new T[s];
    }

    int size() //возвращает количество элементов вектора
    {
        return this->sz;
    }

    ~vectop() // деструктор освобождает память
    {
        delete[] this->elem;
    }

    vectop(const vectop&);                     // 1) копирующий конструктор
    vectop& operator= (const vectop&);         // 2) копирующее присвоение
    vectop (vectop&& );                        // 3) перемещающий конструктор
    vectop& operator= (vectop&&);              // 4) Перемещающее присваивание
    T& operator [] (int n)  // Чтобы обращаться к нек. вектору по индексу v[i]
    {
        return this->elem[n];
    }
    const T& operator [] (int n) const          // обращение к const вектору по индексу v[i]
    {
        return this->elem[n];
    }

    void reserve(int newalloc);                     // 5) добавляет память для новых элементов
    int capacity () const
    {
        return this->space;
    }
    void resize(int newsize , T val = T());            // 6) изменение размера вектора
    void push_back (const T& val);                     // 7) добавление элемента

    T& at (int n);                                // 8) доступ с проверкой
    const T& at (int n) const;                    // доступ с проверкой
};

template <typename T, typename A>
vectop<T, A>::vectop(const vectop& arg)
{
    this->sz = arg.sz;
    this->elem = new T[arg.sz];
    copy(arg.elem, arg.elem+arg.sz, this->elem);
}

template <typename T, typename A>
vectop<T,A>&  vectop<T,A>::operator = (const vectop& a)  // делает этот вектор копией вектора а
{
    if (this == &a)
        return *this ;             // самоприсваивание
    if (a.sz <= this->space)                         // памяти достаточно
    {
        for (int i = 0; i<a.sz; ++i)
            this->elem[i] = a.elem[i];            // копирование
        this->sz = a.sz;
        return *this ;
    }
    T *p = new T [a.sz];  // выделение новой памяти
    for (int i = 0; i < a.sz; i++)
        p[i] = a.elem[i];           // копирование
    delete[] this->elem;                 // освобождение памяти
    this->space = a.sz;              // новый размер
    this->elem = p;                       // указатель на новые элементы
    return *this;                   // ссылка на текущий объект
}

template <typename T, typename A>
vectop<T,A>::vectop (vectop&& a)
{
    this->sz = a.sz;
    this->elem = a.elem;
    a.sz = 0;               // Делаем вектор а пустым
    a.elem = nullptr;
}

template <typename T, typename A>
vectop<T,A>&  vectop<T,A>::operator = (vectop&& a) // Перемещаем а в текущий вектор
{
    delete[] this->elem;          // Освобождение старой памяти
    this->elem = a.elem;          // Копирование elem и sz из а
    this->sz = a.sz;
    a.elem = nullptr;       // пустой вектор
    a.sz = 0;
    return *this;           // ссылка на себя
}

template <typename T, typename A>
void vectop<T,A>::reserve (int newalloc)
{
    if (newalloc <= this->space)
        return;                     // размер никогда не уменьшается

    Vb<T,A> b(this->alloc, newalloc);                                // выделение новой памяти
    uninitialized_copy(b.elem, &b.elem[this->sz], this->elem);       // копирование старых элементов
    for (int i = 0; i < this->sz; i++)
        this->alloc.destroy(&this->elem[i]);                        // удаление старых данных
    swap<Vb<T, A>>(*this, b) ;                                      // обмен преставлений
}

template <typename T, typename A>
void vectop<T,A>::resize (int newsize, T val)
{
    this->reserve(newsize) ;
    for (int i = this->sz ; i < newsize ; ++i)
        this->alloc.construct(&this->elem[i], val);
    for (int i = this->sz ; i < newsize ; ++i)
        this->alloc.destroy(&this->elem[i]);
    this->sz = newsize;
}

template <typename T, typename A>
void vectop<T,A>::push_back (const T& val)
{
    if (this->space == 0)
        reserve(8);                        // выделяем память для 8 элементов
    else if (this->sz == this->space)
        reserve(2 * this->space);              // выделяем дополнительную память
    this->alloc.construct(&this->elem[this->sz], val);             // добавляем val в конец вектора
    ++this->sz;                                       // увеличиваем количество элементов
}

template <typename T, typename A>
T& vectop<T,A>:: at (int n)
{
    if (n < 0 || this->sz <= n)
        throw out_of_range("Выход за диапазон значений");
    return this->elem[n];
}

int main()
{
    vectop<int> v1 = {1,2,3,4,5,6,7,8,9,10};
    vectop<string> v2 = {"a", "b", "c", "d", "e", "f", "g"};
    v1.resize(13, 100);
    for (int i = 0; i < v1.size(); i++)
        cout<<v1[i]<<' ';
    cout<<'\n';

    for(int i = 0; i < v2.size(); i++)
        cout<<v2[i]<<' ';
    cout<<'\n';
}
