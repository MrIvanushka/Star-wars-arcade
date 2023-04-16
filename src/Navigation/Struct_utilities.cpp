#include "Struct_utilities.hpp"

poly_intersection::poly_intersection() {};

poly_intersection::poly_intersection(const poly_intersection& other) : line(other.line) {};

poly_intersection& poly_intersection::operator=(const poly_intersection& other){
    this->line = other.line;
    return *this;
}

Node::Node(){}

Node::Node(const TPPLPoly_it& p, double score) : poly(p), f_score(score){}

Node::Node(const Node& other) : poly(other.poly), f_score(other.f_score) {}

Node& Node::operator=(const Node& other){
    poly = other.poly;
    f_score = other.f_score;
    return *this;
}

bool Node::operator<(const Node& other) const{
    return (other.f_score < f_score);
}
