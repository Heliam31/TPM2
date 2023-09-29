#include <iostream>
#include <ostream>
using namespace std;

class Expr {
public:
virtual ~Expr() {};
virtual void print(ostream& out) = 0;
virtual float eval(float x) = 0;
};

class Xvar: public Expr {
public:
virtual void print(ostream& out) { out << "x"; }
virtual float eval(float x) { return x; }
};

class Const: public Expr{
    public:
        Const(float k) {this->_k = k;}
        virtual void print(ostream& out) { out << "k"; }
        virtual float eval(float x) { return _k; }
    private:
        float _k;
};
int main (){
    Xvar fx();
    fx.eval(1);
    float k = 3;
    Const fk(k);
    fk.eval(10);
    return 0;
}