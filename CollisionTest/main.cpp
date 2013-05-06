#include <SFML/Graphics.hpp>
#include <entity.h>
#include <scene.h>
#include <shape.h>
#include <circle.h>
#include <iostream>

unsigned int WINDOW_X = 0;
unsigned int WINDOW_Y = 0;

class TestEntity: public Entity
{
public:
    unsigned int team;
    sf::Shape graphics;
    TestEntity() : Entity() { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    TestEntity(float a, float b) : Entity(a, b) { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    TestEntity(float a, float b, Shape* shape) : Entity(a, b, shape) { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    TestEntity(float a, float b, Shape* shape, sf::Shape graphic) : Entity(a, b, shape) { graphics = graphic; }
    void move();
    void collide(Entity* other);
    void update(float dt);
};

void TestEntity::update(float dt)
{
    dx += ddx*dt + gravity_x*dt;
    dy += ddy*dt + gravity_y*dt;
}

void TestEntity::collide(Entity* other)
{
    std::cout << "Entity " << this->id << " collided with entity " << other->id << std::endl;
}

void TestEntity::move()
{
    x += dx;
    y += dy;
    if (x < -(this->shape->maxSize))
    {
        x = WINDOW_X + shape->maxSize;
    }
    else if (x > WINDOW_X + this->shape->maxSize)
    {
        x = -(this->shape->maxSize);
    }
    if (y < -(this->shape->maxSize))
    {
        y = WINDOW_Y + shape->maxSize;
    }
    else if (y > WINDOW_Y + this->shape->maxSize)
    {
        y = -(this->shape->maxSize);
    }
    graphics.SetPosition(x, y);
}

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");

    sf::Vector2f WindowSize(App.GetWidth(), App.GetHeight());

    Scene::initSingleton();

    WINDOW_X = App.GetWidth();
    WINDOW_Y = App.GetHeight();

    // Use circles for now.
    sf::Shape GreenCircle = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0));
    GreenCircle.Move(WindowSize.x/2, WindowSize.y/2);
    TestEntity GreenCirclePhysics(GreenCircle.GetPosition().x, GreenCircle.GetPosition().y, (Shape*)new Circle(10.0), GreenCircle);
    //GreenCirclePhysics.setGravity(0, 4.9);
    GreenCirclePhysics.Setdx(0.49);
    //GreenCirclePhysics.Setdy(4.9);

    Scene::singleton().addEntity((Entity*)&GreenCirclePhysics);

    sf::Shape RedCircle = sf::Shape::Circle(0, 0, 10, sf::Color(255, 0, 0));
    RedCircle.Move(WindowSize.x*(3/4), WindowSize.y/2);
    TestEntity RedCirclePhysics(RedCircle.GetPosition().x, RedCircle.GetPosition().y, (Shape*)new Circle(10.0), RedCircle);
    //RedCirclePhysics.setGravity(0, 4.9);
    RedCirclePhysics.Setdx(-0.49);
    //RedCirclePhysics.Setdy(4.9);

    Scene::singleton().addEntity((Entity*)&RedCirclePhysics);


    sf::Clock Clock;

	// Start the game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
            else if (Event.Type == sf::Event::KeyPressed)
            {
                if (Event.Key.Code == sf::Key::Escape)
                {
                    App.Close();
                }
                if (Event.Key.Code == sf::Key::Space)
                {
                    GreenCirclePhysics.Setdx(4.9);
                    //GreenCirclePhysics.Setdy(4.9);
                }
            }
        }

        // Clear screen
        App.Clear();

        float Time = Clock.GetElapsedTime();
        Clock.Reset();
        Scene::singleton().update(Time);
        /*
        if (GreenCirclePhysics.GetX() > WindowSize.x + 10)
        {
            GreenCirclePhysics.SetX(-10);
        }
        if (GreenCirclePhysics.GetX() < -10)
        {
            GreenCirclePhysics.SetX(WindowSize.x+10);
        }
        if (GreenCirclePhysics.GetY() > WindowSize.y - 10)
        {
            GreenCirclePhysics.SetY(WindowSize.y - 10);
            GreenCirclePhysics.Setdy(-0.5*GreenCirclePhysics.Getdy());
        }
        if (GreenCirclePhysics.GetY() < 0)
        {
            //GreenCirclePhysics.SetY(0);
            //GreenCirclePhysics.Setdy(-0.5*GreenCirclePhysics.Getdy());
        }
        //GreenCirclePhysics.setForce(-0.5*GreenCirclePhysics.Getdx()*GreenCirclePhysics.Getdx(), -0.5*GreenCirclePhysics.Getdy()*GreenCirclePhysics.Getdy());
        GreenCircle.SetPosition(GreenCirclePhysics.GetX(), GreenCirclePhysics.GetY());

        // Draw the GreenCircle
        App.Draw(GreenCircle);

        if (RedCirclePhysics.GetX() > WindowSize.x + 10)
        {
            RedCirclePhysics.SetX(-10);
        }
        if (RedCirclePhysics.GetX() < -10)
        {
            RedCirclePhysics.SetX(WindowSize.x+10);
        }
        if (RedCirclePhysics.GetY() > WindowSize.y - 10)
        {
            RedCirclePhysics.SetY(WindowSize.y - 10);
            RedCirclePhysics.Setdy(-0.5*RedCirclePhysics.Getdy());
        }
        if (RedCirclePhysics.GetY() < 0)
        {
            //RedCirclePhysics.SetY(0);
            //RedCirclePhysics.Setdy(-0.5*RedCirclePhysics.Getdy());
        }
        //RedCirclePhysics.setForce(-0.5*RedCirclePhysics.Getdx()*RedCirclePhysics.Getdx(), -0.5*RedCirclePhysics.Getdy()*RedCirclePhysics.Getdy());
        RedCircle.SetPosition(RedCirclePhysics.GetX(), RedCirclePhysics.GetY());

        // Draw the RedCircle
        App.Draw(RedCircle);*/
        for (std::map<unsigned int, Entity*>::iterator i = Scene::singleton()._entmap.begin(); i != Scene::singleton()._entmap.end(); i++)
        {
            TestEntity* ent = dynamic_cast<TestEntity*>((*i).second);
            if (ent)
            {
                App.Draw(ent->graphics);
            }
        }

        // Update the window
        App.Display();
    }

    return EXIT_SUCCESS;
}
