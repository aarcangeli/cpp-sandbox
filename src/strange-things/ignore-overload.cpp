#include <iostream>

class Quadrilateral {
public:
    virtual void display() {
        std::cout << "I am a Quadrilateral" << std::endl;
    }
};

class Square : public Quadrilateral {
public:
    void display() override {
        std::cout << "I am a Square" << std::endl;
    }
};

// This program demonstrates the fact that
//   you can invoke directly a method of a base class
//   ignoring the fact that it is virtual
int main() {
    Square square;

    square.Quadrilateral::display();
    (&square)->Quadrilateral::display();

    Square *squareHeap = new Square;
    squareHeap->Quadrilateral::display();

    Quadrilateral *squareAsQuadrilateral = new Square;
    squareAsQuadrilateral->Quadrilateral::display();

    return 0;
}
