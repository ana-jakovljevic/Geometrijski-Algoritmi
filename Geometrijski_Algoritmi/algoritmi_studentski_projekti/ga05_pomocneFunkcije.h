#ifndef GA05_POMOCNEFUNKCIJE_H
#define GA05_POMOCNEFUNKCIJE_H

#include "ga05_EdgeDQ.h"


double Det4x4(double* col_0, double* col_1, double* col_2, double* col_3){
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_3[0];
    double e = col_0[1];
    double f = col_1[1];
    double g = col_2[1];
    double h = col_3[1];
    double i = col_0[2];
    double j = col_1[2];
    double k = col_2[2];
    double l = col_3[2];
    double m = col_0[3];
    double n = col_1[3];
    double o = col_2[3];
    double p = col_3[3];

    // Compute 3x3 determinants
    double adet = a * ((f * k * p) - (f * l * o) - (g * j * p) + (g * l * n) + (h * j * o) - (h * k * n));
    double bdet = b * ((e * k * p) - (e * l * o) - (g * i * p) + (g * l * m) + (h * i * o) - (h * k * m));
    double cdet = c * ((e * j * p) - (e * l * n) - (f * i * p) + (f * l * m) + (h * i * n) - (h * j * m));
    double ddet = d * ((e * j * o) - (e * k * n) - (f * i * o) + (f * k * m) + (g * i * n) - (g * j * m));

    // Return their alternating sum
    double det = adet - bdet + cdet - ddet;
    return det;
}
double Det3x3(double* col_0, double* col_1, double* col_2){
    // Gets the determinant of a 3x3 matrix, where the arguments are 3-long column vectors

    // Names all the objects in the matrix for my convenience
    double a = col_0[0];
    double b = col_1[0];
    double c = col_2[0];
    double d = col_0[1];
    double e = col_1[1];
    double f = col_2[1];
    double g = col_0[2];
    double h = col_1[2];
    double i = col_2[2];

    // Return the alternating sum of the 2x2 determinants of the coproducts
    double det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    return det;
}
bool CCW(QPointF a, QPointF b, QPointF c){
    float a_x = a.x();
    float a_y = a.y();
    float b_x = b.x();
    float b_y = b.y();
    float c_x = c.x();
    float c_y = c.y();

    // Set up a matrix
    double m[3][3] = { { a_x, b_x, c_x }, { a_y, b_y, c_y }, { 1, 1, 1 } };

    // Return true if our determinant is positive
    return Det3x3(m[0], m[1], m[2]) > 0;
}

QPointF Circumcenter(QPointF a, QPointF b, QPointF c){
    auto d = 2* (a.x() * (b.y() - c.y()) + b.x() * (c.y() - a.y()) + c.x() * (a.y() - b.y()));
    auto a_lengthsquared = a.x()*a.x() + a.y()*a.y();
    auto b_lengthsquared = b.x()*b.x() + b.y()*b.y();
    auto c_lengthsquared = c.x()*c.x() + c.y()*c.y();

    qreal x = static_cast<qreal>(a_lengthsquared * (b.y() - c.y()) + b_lengthsquared * (c.y() - a.y()) + c_lengthsquared * (a.y() - b.y())) / d;
    qreal y = static_cast<qreal>(a_lengthsquared * (c.x() - b.x()) + b_lengthsquared * (a.x() - c.x()) + c_lengthsquared * (b.x() - a.x())) / d;
    return QPointF(x,y);
}
bool in_circle(QPointF a, QPointF b, QPointF c, QPointF d){
    auto a_lengthsquared = a.x()*a.x()+a.y()*a.y();
    auto b_lengthsquared = b.x()*b.x()+b.y()*b.y();
    auto c_lengthsquared = c.x()*c.x()+c.y()*c.y();
    auto d_lengthsquared = d.x()*d.x()+d.y()*d.y();
    double m[4][4] = {	{ a.x(), b.x(), c.x(), d.x() },
                         { a.y(), b.y(), c.y(), d.y() },
                         { a_lengthsquared, b_lengthsquared, c_lengthsquared, d_lengthsquared},
                            { 1, 1, 1, 1 } };

        // Return true if our determinant is positive
        return Det4x4(m[0], m[1], m[2], m[3]) > 0;
}
bool right_of(EdgeDQ* e, QPointF p){
    return CCW(p, e->destination(), e->origin());

}
bool left_of( EdgeDQ* e, QPointF p){
    return CCW(p, e->origin(), e->destination());

}
bool Valid(EdgeDQ* e, EdgeDQ *base_edge){
    return right_of(base_edge,e->destination());

}

#endif // GA05_POMOCNEFUNKCIJE_H
