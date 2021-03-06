#include "Collider.hpp"
#include <iostream>
#include <cassert>
#include <Application.hpp>
#include <vector>

void Collider::clamping(Vec2d& center)
{
    auto worldSize = getApp().getWorldSize();
    auto width = worldSize.x; // largeur
    auto height = worldSize.y; // hauteur

    while (center.x > width) {
        center.x -= width;
    }
    while (center.x < 0) {
        center.x += width;
    }
    while (center.y > height) {
        center.y -= height;
    }
    while (center.y < 0) {
        center.y += height;
    }
}
Collider::Collider(Vec2d cent, double r) //[?] les arguments peuvent etre mit par reference q1.3?
    : radius_(r)
    , center_(cent)

{
    assert(r >= 0); // Si le rayon du Collider est inférieur a 0, erreur.
    clamping(center_); // On s'assure que le centre est bien dans le monde torique.
}

double Collider::getRadius() const
{
    return radius_;
}

Vec2d Collider::getPosition() const
{
    return center_;
}

Collider::Collider(Collider const& copie)
    : radius_(copie.radius_)
    , center_(copie.center_)
{
}

Collider& Collider::operator=(Collider const& recopie)
{
    center_ = recopie.center_;
    radius_ = recopie.radius_;
    return *this;
}

Vec2d Collider::directionTo(const Vec2d& to) const
{
    auto worldSize = getApp().getWorldSize();
    auto width = worldSize.x; // largeur
    auto height = worldSize.y; // hauteur
    Vec2d current(to.x + width - center_.x, to.y + height - center_.y);
    Vec2d good(current);

    for (int i(0); i < 3; i++) { // boucle iterant sur toutes les possibilites de vecteurs (9)  pour choisir le meilleur
        for (int i(0); i < 3; i++) {
            if (good.length() > current.length()) {
                good = current;
            }
            current.y -= width;
        }
        current.x -= width;
        current.y = to.y + height - center_.y;
    }
    return good;
}

Vec2d Collider::directionTo(const Collider& too) const
{
    return directionTo(too.center_);
}

double Collider::distanceTo(const Vec2d& to) const
{
    return (directionTo(to)).length();
}

double Collider::distanceTo(const Collider& too) const
{
    return (directionTo(too)).length();
}

void Collider::move(const Vec2d& dx)
{
    Vec2d newcentre(center_ + dx);
    clamping(newcentre);
    center_ = newcentre;
}

Collider& Collider::operator+=(Vec2d const& to)
{
    move(to);
    return *this;
}



bool Collider::isColliderInside(const Collider& other) const
{


    if ((radius_ >= other.radius_) and (distanceTo(other) <= radius_ - other.radius_)) {
        return true;
    }

    return false;
}

bool Collider::isColliding(const Collider& other) const
{
    if (distanceTo(other) <= radius_ + other.radius_) {
        return true;
    }

    return false;
}

bool Collider::isPointInside(const Vec2d& other) const
{
    if (radius_ >= distanceTo(other)) {
        return true;
    }

    return false;
}

bool Collider::operator>(Collider const& other) const
{
    return isColliderInside(other);
}

bool Collider::operator|(Collider const& other) const
{
    return isColliding(other);
}

bool Collider::operator>(Vec2d const& pts) const
{
    return isPointInside(pts);
}

std::ostream& operator<<(std::ostream& sortie, const Collider& Obj)
{
    sortie << "Collider: position = (" << Obj.center_.x << ", " << Obj.center_.y << ") radius = " << Obj.radius_;

    return sortie;
}
