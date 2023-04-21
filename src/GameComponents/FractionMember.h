#ifndef SWTOR_IFRACTIONMEMBER_H
#define SWTOR_IFRACTIONMEMBER_H

enum Fraction {
    None,
    Jedi,
    Sith,
};

class FractionMember
{
private:
    Fraction _fraction;
public:
    Fraction getFraction() {
        return _fraction;
    }
protected:
    void changeFraction(Fraction newFraction){
        _fraction = newFraction;
    }
};



#endif //SWTOR_IFRACTIONMEMBER_H