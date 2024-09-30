#include "collisionhandler.h"
#include "logger.h"

CollisionHandler::CollisionHandler()
{

}

CollisionHandler::~CollisionHandler()
{

}

void CollisionHandler::addBall(OctaBall* ball)
{
    balls.push_back(ball);
}

void CollisionHandler::addWall(Plane* wall)
{
    walls.push_back(wall);
}

void CollisionHandler::DetectCollision(std::vector<PhysicsObject*> pObjects)
{
    if (pObjects.size() > 1)
    {
        std::vector<std::vector<int>> tracker;
        // assume the radius is 1
        for (int i = 0; pObjects.size() > i; i++)
        {
            tracker.push_back(*new std::vector<int>());
            for (int j = 0; j <= i; j++)
                tracker[i].push_back(j);

            for (int j = 0; pObjects.size() > j; j++)
            {
                if (std::find(tracker[i].begin(), tracker[i].end(), j) != tracker[i].end())
                {
                    continue;
                }
                float distance = 0;
                if (pObjects[i]->type() != pObjects[j]->type())
                {
                    if (pObjects[j]->type() == 0) // if the pObjects is wall
                        distance = ((Plane *) pObjects[j])->distanceFromPoint(pObjects[i]->getPosition());
                    else // if the other pObjects is wall
                        distance = ((Plane *) pObjects[i])->distanceFromPoint(pObjects[j]->getPosition());
                }
                else // if the pObjects are equal (only both being balls matter)
                    distance = pObjects[i]->getPosition().distanceToPoint(pObjects[j]->getPosition());
                // mLogger->logText(std::to_string(distance));

                if (distance < pObjects[i]->getRadius() + pObjects[j]->getRadius())
                {
                    Collide(pObjects[i], pObjects[j]);
                }
            }
        }
    }
    else
    {
        // pObjects er ikke større en 1
    }
}

void CollisionHandler::Collide(PhysicsObject* object1, PhysicsObject* object2)
{
//    mLogger->logText("Collision!");

    // objektspesifike sprett (ball (1) or wall (0))
    // viss et objekt er ball (1) og den andre er en vegg (0)
    if ((object1->type() != object2->type()))
    {
        if (object1->type() == 0 && object2->type() == 1)
        {
            BallWallCollision((OctaBall *) object2, (Plane *) object1);
        }
        else
        {
            BallWallCollision((OctaBall *) object1, (Plane *) object2);
        }
    }
    // viss begge objekta er baller (1)
    else if ((object1->type() == 1 && object2->type() == 1))
    {
        BallBallCollision((OctaBall *) object1, (OctaBall *) object2);
    }
}

void CollisionHandler::BallWallCollision(OctaBall* ball, Plane* wall)
{
    // Spretter ballen vekk fra ballen på samme måte som lys reflekteres fra speil
    // Grader på inngangsvektoren blir reversert til å bli speilvendt i forhold til normalen
    ball->setVelocity(-2 * QVector3D().dotProduct(wall->getNormal(), ball->getVelocity()) * wall->getNormal() + ball->getVelocity());

    // Regner ut avstanden fra ballen til det nærmeste punktet på planet
    float distance = wall->distanceFromPoint(ball->getPosition());
    // Dytter ballen ut fra veggen like mye som han gikk inn i veggen
    ball->move(wall->getNormal() * (distance - ball->getRadius()));
}

void CollisionHandler::BallBallCollision(OctaBall* ball0, OctaBall* ball1)
{
    // Likt som i BallWallCollision, skal ballene vekk fra ballen like langt som de gikk inn i hverandre
    // "Normalen" i dette tilfellet vil være fra ballen sitt senter til senteret i den andre ballen (.normalized())
    // da de støtte på hverandre, slik at kollisjonen er litt mer dynamisk enn med en vegg
    mLogger->logText("Collision!");

    //    mLogger->logText("New velocity is " + std::to_string(object1->getVelocity().y()) + " and " + std::to_string(object2->getVelocity().y()));

    // Ballen skal også skifte hastighetvektor til å fortsette i retning fra kollisjonspunktet.
    // Jeg følger formlene som blir vist i 9.7.5 (hvor n er normalen funnet i forrige kommentar og d som er avstanden mellom ballene da de kolliderte)
    // og endrer hastighetsvektoren til summen

}

int CollisionHandler::type()
{
    return -1;
}
