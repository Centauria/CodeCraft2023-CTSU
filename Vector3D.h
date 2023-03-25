//
// Created by Centauria V. CHEN on 2023/3/11.
//

#ifndef CODECRAFTSDK_Vector3D_H
#define CODECRAFTSDK_Vector3D_H


class Vector3D
{
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D operator+(const Vector3D &v) const;
    Vector3D operator-(const Vector3D &v) const;
    Vector3D operator*(double t) const;
    Vector3D operator/(double t) const;
    Vector3D &operator+=(const Vector3D &v);
    Vector3D &operator*=(double t);
    Vector3D &operator/=(double t);

    [[nodiscard]] double norm() const;
    [[nodiscard]] double dot(const Vector3D &v) const;
    [[nodiscard]] Vector3D normalize() const;

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

Vector3D operator*(double t, const Vector3D &v);
inline bool operator==(const Vector3D &lhs, const Vector3D &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}


#endif//CODECRAFTSDK_Vector3D_H
