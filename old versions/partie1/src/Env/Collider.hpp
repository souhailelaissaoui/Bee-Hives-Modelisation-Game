#include <Utility/Vec2d.hpp>

#ifndef Collider_H
#define Collider_H

class Collider
{
private:
    double radius_;
    Vec2d center_;
    void clamping(Vec2d& center); // Monde torique

public:
    Collider(Vec2d cent, double r); // constructeur
    double getRadius() const;
    Vec2d getPosition() const;
    Collider(Collider const& copie); // constructeur de copie
    Collider& operator=(Collider const& recopie);
    Vec2d directionTo(const Vec2d& to) const; // Donne le vecteur le plus court pour atteindre un point dans le monde torique
    Vec2d directionTo(const Collider& too) const; // Donne le vecteur le plus court pour atteindre le centre du collider passe en argument
    double distanceTo(const Vec2d& to) const; // Donne la mesure du meilleur vecteur fourni par directionTo
    double distanceTo(const Collider& too) const;
    void move(const Vec2d& dx); // Deplace le Collider courant
    Collider& operator+=(Vec2d const& to);
    bool isColliderInside(const Collider& other) const; // Tester si un collider passe en argument est dans le collider courant
    bool isPointInside(const Vec2d& other) const; // Meme chose avec un point
    bool isColliding(const Collider& other) const; // Tester si UN collider est en collision avec le collider courant
    bool operator>(Collider const& other) const; // meme principe de Iscolliderinside
    bool operator>(Vec2d const& pts) const; // meme principe aue isPointInside
    bool operator|(Collider const& other) const; // Meme principe de isColliding

    friend std::ostream& operator<<(std::ostream& sortie, const Collider& Obj); // Affiche le centre et le rayon du COllider
};

#endif
