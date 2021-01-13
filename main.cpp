/*
    Гребенков Д. И.
    Группа М8О-207Б-19.
    Вариант 12.

Разработать шаблоны классов согласно варианту задания.  Параметром шаблона должен являться скалярный
тип данных задающий тип данных для оси координат. Классы должны иметь публичные поля. Фигуры являются
фигурами вращения, т.е. равносторонними (кроме трапеции и прямоугольника). Для хранения координат 
фигур необходимо использовать шаблон std::pair.
Создать шаблон динамической коллекцию, согласно варианту задания:
1. Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr). 
Опционально использование std::unique_ptr;
2. В качестве параметра шаблона коллекция должна принимать тип данных - фигуры;
3. Реализовать forward_iterator по коллекции;
4. Коллекция должны возвращать итераторы begin() и  end();
5. Коллекция должна содержать метод вставки на позицию итератора insert(iterator);
6. Коллекция должна содержать метод удаления из позиции итератора erase(iterator);
7. При выполнении недопустимых операций (например выход за границы коллекции или удаление 
несуществующего элемента) необходимо генерировать исключения;
8. Итератор должен быть совместим со стандартными алгоритмами (например, std::count_if)
9. Коллекция должна содержать метод доступа:
стек – pop, push, top;
очередь – pop, push, top;
список, Динамический массив – доступ к элементу по оператору [].
10.	Реализовать программу, которая:
позволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять 
в коллекцию;
позволяет удалять элемент из коллекции по номеру элемента;
выводит на экран введенные фигуры с помощью std::for_each;
выводит на экран количество объектов, у которых площадь меньше   заданной (с помощью std::count_if).

	Вариант задания: трапеция, список.
*/
#include <iostream>
#include <memory>
#include <cmath>
#include <algorithm>

template <typename T>
class Trapezoid {
    public:
        using type = T;
        std::pair <T, T> left_top, left_bottom, right_bottom;
        Trapezoid () {}
        Trapezoid (T ltx, T lty, T lbx, T lby, T rbx, T rby) {
            left_top.first = ltx;
            left_top.second = lty;
            left_bottom.first = lbx;
            left_bottom.second = lby;
            right_bottom.first = rbx;
            right_bottom.second = rby;
        };
};

template <typename T>
void Vertices (Trapezoid <T> tr) {
    std::cout << "(" << tr.left_top.first << ", " << tr.left_top.second << ")"
    << "(" << tr.left_bottom.first << ", " << tr.left_bottom.second << ")"
    << "(" << tr.right_bottom.first << ", " << tr.right_bottom.second << ")"
    << "(" << tr.right_bottom.first - tr.left_top.first + tr.left_bottom.first << 
    ", " << tr.left_top.second << ")";
}

template <typename T>
T Square (Trapezoid <T> tr) {
    return ((tr.right_bottom.first - tr.left_top.first) * (tr.left_top.second - tr.left_bottom.second));
}

template <typename T>
class ListMember {
    public:
        using type = T;
        Trapezoid <T> figure;
        std::shared_ptr <ListMember <T> > next;
        bool barr;
        ListMember () {
            barr = true;
        }
        ListMember (T ltx, T lty, T lbx, T lby, T rbx, T rby) {
            figure.left_top.first = ltx;
            figure.left_top.second = lty;
            figure.left_bottom.first = lbx;
            figure.left_bottom.second = lby;
            figure.right_bottom.first = rbx;
            figure.right_bottom.second = rby;
            barr = false;
        }
};

template <typename T>
class Iterator {
    public:
        std::weak_ptr <ListMember <T> > it;
        Iterator (std::weak_ptr <ListMember <T> > al) {
            it = al;
        }
        Iterator * operator++ () {
            std::shared_ptr <ListMember <T> > temp = it.lock ();
            if (temp->next) {
                it = temp->next;
            }
            else {
                throw "ERROR: Trying to go out of range.";
            }
            return this;
        }
        Trapezoid <T> operator* () {
            std::shared_ptr <ListMember <int> > temp = it.lock ();
            if (!temp->next) {
                throw "ERROR: Trying to dereference nullptr.";
            }
            return (temp->next->figure);
        }
        bool operator!= (Iterator <T> second) {
            bool res;
            std::shared_ptr <ListMember <T> > one = it.lock ();
            std::shared_ptr <ListMember <T> > another = second.it.lock ();
            if (one.get () == another.get ()) {
                res = false;
            }
            else {
                res = true;
            }
            return res;
        }
};

template <typename T>
class List {
    public:
        List (std::weak_ptr <ListMember <T> > b) {
            first = b;
            last = b;
            iter = b;
            count = 0;
        }
        int count;
        using type = T;
        std::weak_ptr <ListMember <T> > iter;
        std::weak_ptr <ListMember <T> > first;
        std::weak_ptr <ListMember <T> > last;
        Iterator <T> begin () {
            return (Iterator <T> (first));
        }
        Iterator <T> end () {
            return (Iterator <T> (last));
        }
        void insert () {
            std::shared_ptr <ListMember <T> > temp = iter.lock ();
            T ltx, lty, lbx, lby, rbx, rby;
            std::cout << "Enter the data of the figure: " << std::endl;
            std::cin >> ltx >> lty >> lbx >> lby >> rbx >> rby;
            std::shared_ptr <ListMember <T> > curr (new ListMember <T> (ltx, lty, lbx, lby, rbx, rby));
            count ++;
            if (temp->next) {
                std::shared_ptr <ListMember <T> > copy = temp->next;
                temp->next = curr;
                curr->next = copy;
            }
            else {
                temp->next = curr;
                last = curr;
            }
        }
        void erase () {
            std::shared_ptr <ListMember <T> > temp = iter.lock ();
            if (temp->next) {
                temp->next = temp->next->next;
                count --;
                if ((!temp->next) && (!temp->barr)) {
                    last = temp;
                }
            }
            else {
                throw "ERROR: Trying to delete nonexistent element.";
            }
        }
        List * operator++ () {
            std::shared_ptr <ListMember <T> > temp = iter.lock ();
            if (temp->next) {
                iter = temp->next;
            }
            else {
                throw "ERROR: Trying to go out of range.";
            }
            return this;
        }
        std::weak_ptr <ListMember <T> > operator[] (int idx) {
            std::weak_ptr <ListMember <T> > pointer = first;
            std::shared_ptr <ListMember <T> > temp;
            temp = pointer.lock ();
            if ((idx < 0) || (idx > count)) {
                throw "ERROR: Index is out of range.";
            }
            else {
                for (int i = 0; i < idx; i ++) {
                    temp = pointer.lock ();
                    pointer = temp->next;
                }
                temp = pointer.lock ();
            }
            return (temp);
        }
        Trapezoid <T> look () {
            std::shared_ptr <ListMember <T> > temp = iter.lock ();
            if (!temp->next) {
                throw "ERROR: Trying to receive value from nullptr.";
            }
            return (temp->next->figure);
        }
};

double max;

template <typename T>
bool SquareCheck (Trapezoid <T> tr) {
    bool res;
    if (Square <T> (tr) < max) {
        res = true;
    }
    else {
        res = false;
    }
    return res;
}

void ShowMenu () {
    std::cout << "0. Print menu." << std::endl;
    std::cout << "1. Show list." << std::endl;
    std::cout << "11. Go to the beginning." << std::endl;
    std::cout << "12. Go to the ending." << std::endl;
    std::cout << "13. Go to the next element." << std::endl;
    std::cout << "14. Go to element with entered number." << std::endl;
    std::cout << "2. Add element in iterator position." << std::endl;
    std::cout << "3. Erase element in iterator position." << std::endl;
    std::cout << "4. Print figures with \"for_each\"." << std::endl;
    std::cout << "5. Print the number of objects with an area less than the specified one."
    << std::endl;
    std::cout << "9. Quit the program." << std::endl;
}

template <class InputIt, class UnaryPredicate>
int count_if (InputIt first, InputIt last, UnaryPredicate p)
{
    int ret = 0;
    for (; first != last; ++ first) {
        if (p (*first)) {
            ret ++;
        }
    }
    return ret;
}

int main () {
    int option = 0;
    std::shared_ptr <ListMember <int> > p (new ListMember <int> ());
    List <int> list (p);
    ShowMenu ();
    while (option != 9) {
        std::cout << "Enter option: ";
        std::cin >> option;
        switch (option) {
            case 0: {
                ShowMenu ();
                break;
            }
            case 1: {
                if (list.count == 0) {
                    std::cout << "List is empty." << std::endl;
                }
                else try {
                    std::weak_ptr <ListMember <int> > temp = list.iter;
                    std::shared_ptr <ListMember <int> > check = temp.lock ();
                    std::shared_ptr <ListMember <int> > position;
                    list.iter = list.first; //list.iter = list.begin ();
                    for (int i = 0; i < list.count; i ++) {
                        position = list.iter.lock ();
                        std::cout << "  -> ";
                        if (check == position) {
                            std::cout << "|||";
                        }
                        Vertices (list.look ());
                        if (check == position) {
                            std::cout << "|||";
                        }
                        std::cout << std::endl;
                        ++list;
                        list.iter = position->next;
                    }
                    if (check->next.get () == nullptr) {
                        std::cout << "  -> |||       |||" << std::endl;
                    }
                    else {
                        std::cout << std::endl;
                    }
                    list.iter = temp;
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 11: {
                list.iter = list.first;
                break;
            }
            case 12: {
                list.iter = list.last;
                break;
            }
            case 13: {
                try {
                    ++list;
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 14: {
                try {
                    int idx;
                    std::cout << "Enter index: ";
                    std::cin >> idx;
                    list.iter = list[idx];
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 2: {
                try {
                    list.insert ();
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 3: {
                try {
                    list.erase ();
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 4: {
                try {
                    std::for_each (list.begin (), list.end (), Vertices <int>);
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 5: {
                try {
                    int number = 0;
                    std::cout << "Enter square: ";
                    std::cin >> max;
                    number = count_if (list.begin (), list.end (), SquareCheck <int>);
                    std::cout << "Number of objects with an area less than the specified one: "
                    << number << std::endl;
                }
                catch (const char* exc) {
                    std::cerr << exc << std::endl;
                }
                break;
            }
            case 9: {
                break;
            }
            default: {
                std::cout << "ERROR: Incorrect command entered." << std::endl;
                break;
            }
        }
    }
    return 0;
}