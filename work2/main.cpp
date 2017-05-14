#include<iostream>
#include<fstream>
#include<windows.h>
using namespace std;

const string path_to_files = "src\\data";

template<typename T>
class Iterator{
        typedef struct Node{
            T element;
            Node* next;
            Node* prev;
        } Node;
    public:
        Iterator():m_this(nullptr){}
        Iterator(void *p){
            m_this = (Node*)p;
        }
        Iterator<T>& operator++(){
            if(m_this)
                m_this = m_this->next;
            return *this;
        }
        Iterator<T>& operator--(){
            if(m_this)
                m_this = m_this->prev;
            return *this;
        }
        Iterator<T>& operator++(int){
            Iterator<T> temp = Iterator<T>(m_this);
            if(m_this)
                m_this = m_this->next;
            return temp;
        }
        Iterator<T>& operator--(int){
            Iterator<T> temp = Iterator<T>(m_this);
            if(m_this)
                m_this = m_this->prev;
            return temp;
        }
        bool operator==(const Iterator& it) { return this->m_this == it.m_this; }
        operator bool()const {return m_this;}
        void operator=(const Iterator& it){
            this->m_this = it.m_this;
        }
        friend ostream& operator<< (ostream& os, const Iterator<T>& it) {
            if(it.m_this)
                os << it.m_this->element;
            return os;
        }
        inline T* get_value() const {
            if(!m_this) return nullptr;
            return &m_this->element;
        }
    private:
        Node* m_this;
};

template <class T>
class Lst{
    public:
        Lst(): m_begin(nullptr), m_end(nullptr), m_size(0){}
        void init(string filename){
            ifstream fin; fin.open(filename);
            T el;
            if(fin.is_open())
                while(!fin.eof()){
                    fin >> el;
                    this->push_back(el);
                }
            fin.close();
        }
        inline int get_size(){return m_size;}
        void push_front(T new_element){
            Node* temp = new Node;
            temp ->prev = nullptr;
            temp->next = m_begin;
            temp->element = new_element;
            if(m_begin) {
                m_begin->prev = temp;
            } else {
                m_end = temp;
            }
            m_begin = temp;
            ++m_size;
        }
        void push_back(T new_element){
            Node* temp = new Node;
            temp->prev = m_end;
            temp->next = nullptr;
            temp->element = new_element;
            if(m_end) {
                m_end->next = temp;
            } else {
                m_begin = temp;
            }
            m_end = temp;
            ++m_size;
        }
        void operator +=(Lst& second_lst){
            second_lst.m_begin->prev = this->m_end;
            this->m_end->next = second_lst.m_begin;
            this->m_end = second_lst.m_end;
            this->m_size += second_lst.m_size;
            second_lst = Lst();
        }
        friend ostream& operator<< (ostream& os, const Lst<T>& lst) {
            Node* N = lst.m_begin;
            os << "NULLPTR<->";
            while(N){
                os << (N->element) << "<->";
                N = N->next;
            }
            os << "NULLPTR\n";
            return os;
        }
        Iterator<T> search(T el) {
            Node* temp = this->m_begin;
            while(temp){
                if(temp->element == el) {
                    return Iterator<T>(temp);
                }
                temp = temp->next;
            }
            return Iterator<T>(nullptr);
        }
        void sort(){
            if(m_size > 1){
                for(int i=0 ;i<m_size-1; i++){
                    Node* temp1 = m_begin;
                    Node* temp2 = m_begin->next;
                    for(int j=0; j<m_size-1; j++){
                        if( (temp1->element) > (temp2->element) )
                            swap(temp1->element, temp2->element);
                        temp1 = temp1->next;
                        temp2 = temp2->next;
                    }
                }
            }
        }
        Lst<T> grep(bool(*func)(T)){
            Lst<T> result;
            Node* temp = this->m_begin;
            while(temp){
                if(func(temp->element)) {
                    result.push_back(temp->element);
                }
                temp = temp->next;
            }
            return result;
        }
        Iterator<T> begin(){
            return Iterator<T>(m_begin);
        }
        Iterator<T> end(){
            return Iterator<T>(m_end);
        }
        void clear(){
            Node* temp = this->m_begin;
            while(m_begin){
                temp = temp->next;
                delete m_begin;
                m_begin = temp;
            }
            m_begin = m_end = nullptr;
            m_size = 0;
        }
        bool operator==(Lst& L){
            if(this->m_size != L.m_size) return false;
            Node* temp1 = this->m_begin;
            Node* temp2 = L.m_begin;
            while(temp1){
                if(temp1->element != temp2->element) return false;
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
            return true;
        }
        ~Lst<T>(){
            clear();
        }
    private:
        typedef struct Node{
            T element;
            Node* next;
            Node* prev;
        } Node;
        Node* m_begin;
        Node* m_end;
        int m_size;
};

class MyComplex{
    public:
        MyComplex():m_re(0), m_im(0){}
        MyComplex(const double R, const double I) : m_re(R), m_im(I){}
        friend ostream& operator << (ostream& os, const MyComplex& num){
            os << "{" << num.m_re << " + i*" << num.m_im << "}" ;
            return os;
        }
        friend istream& operator>>(istream& is, MyComplex& num){
            is >> num.m_re;
            is >> num.m_im;
            return is;
        }
        double get_re(){return m_re;}
        double get_im(){return m_im;}
        void set_re(double R){m_re = R;}
        void get_re(double I){m_im = I;}
        bool operator>(const MyComplex& num){ return this->m_re > num.m_re; }
        bool operator<(const MyComplex& num){ return this->m_re < num.m_re; }
        bool operator==(const MyComplex& num){ return this->m_re == num.m_re && this->m_im == num.m_im; }
        bool operator!=(const MyComplex& num){ return this->m_re != num.m_re || this->m_im != num.m_im; }

    private:
        double m_re, m_im;
};

template<typename T>
void CHECK(const T& expected, const T& value){
    if(value == expected){
        cout << "OK" << endl;
    }else{
        cout << "ERROR" << endl;
        cout << "\tGot     : " << value << "\n\tExpected: " << expected << endl;
    }
}

bool more_then_four(int num){ return num > 4;}
bool more_then_1000(int num){ return num > 1000;}
bool more_then_1000(MyComplex num){ return num.get_re()  > 1000;}

void test(){
    int i;

    cout << "TEST : iterator\t\t\t\t";
    Lst<int> l1;
    auto it1 = l1.begin();
  CHECK( it1.get_value(), (int*)nullptr );

    cout << "TEST : add && iterator\t\t\t";
    for(i = 0; i < 10; i++){l1.push_back(i);}
    i=0;
    int counter = 0;
    for(auto it = l1.begin(); it; ++it)
        counter += (*it.get_value() == i++);
  CHECK(10, counter);

    cout << "TEST : clear\t\t\t\t";
    l1.clear();
    it1 = l1.begin();
  CHECK( it1.get_value(), (int*)nullptr );

    cout << "TEST : add && iterator\t\t\t";
    for(i = 0; i < 10; i++){l1.push_front(i);}
    counter = i = 0;
    for(auto it = l1.end(); it; it--)
        counter += (*it.get_value() == i++);
  CHECK(10, counter);

    cout << "TEST : add && complex\t\t\t";
    Lst<MyComplex> l2;
    auto it2 = l2.begin();
    for(i=0; i<10; i++)
        l2.push_back( MyComplex(i+0.1, i+0.5) );
    counter = i = 0;
    for(auto it = l2.begin(); it; ++it, ++i)
        counter += (*it.get_value() == MyComplex(i+0.1, i+0.5));
  CHECK(10, counter);


    cout << "TEST : size1:\t\t\t\t";
    l1.clear();
    Lst<int> l3;
    for(int i=0; i<10; i++){l1.push_back(i); l3.push_back(i+10);}
  CHECK(10, l1.get_size());

    cout << "TEST : size2:\t\t\t\t";
  CHECK(10, l3.get_size());

    cout << "TEST : concatenation size 1:\t\t";
    l1 += l3;
  CHECK(20, l1.get_size());

    cout << "TEST : concatenation size 2:\t\t";
  CHECK(0, l3.get_size());
    cout << "TEST : concatenation\t\t\t";
    counter = i = 0;
    for(auto it = l1.begin(); it; ++it, ++i)
        counter += (*it.get_value() == i);
  CHECK(20, counter);

    cout << "TEST : search  test 1\t\t\t";
    int searched = 19;
    auto elem = l1.search(19);
  CHECK(searched, *elem.get_value());

    cout << "TEST : search  test 2\t\t\t";
  CHECK(l1.end(), elem);

    cout << "TEST : sort test\t\t\t";
    int arr[] = {9,7,5,8,6,4,3,5,1};
    int sorted_arr[] = {1,3,4,5,5,6,7,8,9};
    for(i=0; i<9; i++)
        l3.push_back(arr[i]);
    l3.sort();
    counter = i = 0;
    for(auto it = l3.begin(); it; ++it, ++i)
        counter += (*it.get_value() == sorted_arr[i]);
  CHECK(9, counter);

    cout << "TEST : grep\t\t\t\t";
    Lst<int> l4 = l3.grep(more_then_four);
    counter = i = 0;
    for(auto it = l4.begin(); it; ++it, ++i)
        counter += (*it.get_value() == sorted_arr[i+3]);
  CHECK(6, counter);
}

Lst<int>& list_int(void *ptr) { *(Lst<int>*)ptr;  }
Lst<MyComplex>& list_complex(void *ptr) { *(Lst<MyComplex>*)ptr;  }

int main(){
    char answer = 0;

    while(1){
        answer = 0;
        while(answer != '2' && answer != '1'){
            system("cls");
            cout << "1. test\n2.work with list.\n\tEnter number of operation: ";
            cin >> answer;
            if(answer == 'q') return 0;
        }
        if(answer == '1'){
            test();
        }else{
            answer = 0;
            while(answer != '2' && answer != '1'){
                system("cls");
                cout << "1. int\n2.complex.\n\tChoose type of data: ";
                cin >> answer;
                if(answer == 'q') return 0;
            }
            if(answer == '1'){
                answer = 0;
                while(answer != '2' && answer != '1'){
                    system("cls");
                    cout << "1. int_num1.txt\n2. int_num2.txt.\n\tEnter file with data: ";
                    cin >> answer;
                    if(answer == 'q') return 0;
                }
                string path = path_to_files + "\\int_num" + answer + ".txt";
                Lst<int> L;
                L.init(path);

                answer = 0;
                while(answer > '4' || answer < '1'){
                    system("cls");
                    cout << "1. print list\n2.search element\n3.Concatenation\n4.get sublist\tChoose number operation: ";
                    cin >> answer;
                    if(answer == 'q') return 0;
                }
                if(answer == '1') {
                    cout << L;
                } else if(answer == '2') {
                    int el;
                    cin >> el;
                    auto it = L.search(el);
                    if(it)
                        cout << "There are this element" << endl;
                    else
                        cout << "No this element in list" << endl;
                } else if(answer == '3') {
                    cout << "for example second list from " << path_to_files << "\\int_num3.txt" << endl;
                    Lst<int> L2;
                    L2.init(path_to_files + "\\int_num3.txt");
                    cout << "L1: " << L << "L2: " << L2;
                    L += L2;
                    cout << "L1+L2: " << L;
                } else if(answer == '4') {
                    cout << "for example grep num more then 1000" << endl;
                    auto subL = L.grep(more_then_1000);
                    cout << subL;
                }

            } else {
                answer = 0;
                while(answer != '2' && answer != '1'){
                    system("cls");
                    cout << "1. int_complex1.txt\n2. int_complex2.txt.\n\tEnter file with data: ";
                    cin >> answer;
                    if(answer == 'q') return 0;
                }
                string path = path_to_files + "\\int_complex" + answer + ".txt";
                Lst<MyComplex> L;
                L.init(path);

                answer = 0;
                while(answer > '4' || answer < '1'){
                    system("cls");
                    cout << "1. print list\n2.search element\n3.Concatenation\n4.get sublist\tChoose number operation: ";
                    cin >> answer;
                    if(answer == 'q') return 0;
                }
                if(answer == '1') {
                    cout << L;
                } else if(answer == '2') {
                    MyComplex el;
                    cin >> el;
                    auto it = L.search(el);
                    if(it)
                        cout << "There are this element" << endl;
                    else
                        cout << "No this element in list" << endl;
                } else if(answer == '3') {
                    cout << "for example second list from " << path_to_files << "\\int_complex3.txt" << endl;
                    Lst<MyComplex> L2;
                    L2.init(path_to_files + "\\int_complex3.txt");
                    cout << "L1: " << L << "L2: " << L2;
                    L += L2;
                    cout << "L1+L2: " << L;
                } else if(answer == '4') {
                    cout << "for example grep num more then 1000" << endl;
                    auto subL = L.grep(more_then_1000);
                    cout << subL;
                }

            }
        }
        system("pause");
    }

    return 0;
}








