#include <iostream>
#include <string>
using namespace std;

template <typename T>
T add(T a, T b){
    return (a+b);
}

template <typename T>
class A{
    public:
	A(int a):m_a(a){}
	void print(T a);
        A<T> operator+(A<T> &a);
	void print(){cout<<"m_a="<<m_a<<endl;}
    private:
	int m_a;
};

template <typename T>
void A<T>::print(T a){
    cout<<a<<endl;
}

template <typename T>
A<T> A<T>::operator+(A<T> &a){
	A<T> *nw = new A<T>(m_a+a.m_a);
	return *nw;
}

int main(){
    A<float> *a1=new A<float>(1);
    a1->print(1.2);
    cout<<add(1,2)<<endl;
    cout<<add(string("a"),string("b"))<<endl;
    A<float> *a2=new A<float>(2);
    A<float> a3=*a1+*a2;
    a1->print();
    a2->print();
    a3.print();
    return 0;
}
