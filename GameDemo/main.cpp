#include <SFML/Graphics.hpp>
#include <scene.h>
#include <sstream>
#include <math.h>
#include <cstddef>
#include <iostream>
#include <time.h>

#define PI 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122
#define RAD2DEG 180.0/PI
#define DEG2RAD PI/180.0

unsigned int WINDOW_X = 0;
unsigned int WINDOW_Y = 0;
bool GameOver = false;

class GameEntity: public Entity
{
public:
    unsigned int team;
    sf::Shape graphics;
    GameEntity() : Entity() { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    GameEntity(float a, float b) : Entity(a, b) { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    GameEntity(float a, float b, Shape* shape) : Entity(a, b, shape) { graphics = sf::Shape::Circle(0, 0, 10, sf::Color(0, 255, 0)); }
    GameEntity(float a, float b, Shape* shape, sf::Shape graphic) : Entity(a, b, shape) { graphics = graphic; }
    virtual void collide(Entity* other) = 0;
};

class Enemy : public GameEntity
{
public:
    unsigned int generation;
    Enemy() : GameEntity() { team = 2; generation = 0; }
    Enemy(float a, float b) : GameEntity(a, b) { team = 2; generation = 0; }
    Enemy(float a, float b, Shape* shape) : GameEntity(a, b, shape) { team = 2; generation = 0; }
    Enemy(float a, float b, Shape* shape, sf::Shape graphic) : GameEntity(a, b, shape, graphic) { team = 2; generation = 0; }
    Enemy(float a, float b, Shape* shape, sf::Shape graphic, int gen) : GameEntity(a, b, shape, graphic) { team = 2; this->generation = gen; }
    void collide(Entity* other);
    void move();
    void update(float dt);
};

void Enemy::collide(Entity* other)
{
    GameEntity* OtherEnt = dynamic_cast<GameEntity*>(other);
    if (OtherEnt)
    {
        if (((GameEntity*)other)->team != 2)
        {
            std::cout << "Explod" << std::endl;
            if (generation < 2)
            {
                float speed = sqrt((this->dx*this->dx)+(this->dy*this->dy));
                for (int i = 0; i < 4; i++)
                {
                    int angle = rand() % 360;
                    sf::Shape GreenCircle = sf::Shape::Circle(0, 0, this->shape->minSize/2, sf::Color(0, 255, 0));
                    GreenCircle.Move(this->x, this->y);
                    Enemy* subEnemy = new Enemy(this->x, this->y, (Shape*)new Circle(this->shape->minSize/2), GreenCircle, this->generation+1);
                    subEnemy->dx = cos(angle*DEG2RAD)*speed*2;
                    subEnemy->dy = sin(angle*DEG2RAD)*speed*2;
                    Scene::singleton().addEntity((Entity*)subEnemy);
                }
            }
            Scene::singleton().delEntity((Entity*)this);
        }
    }
}

void Enemy::move()
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

void Enemy::update(float dt)
{
    dx += ddx*dt + gravity_x*dt;
    dy += ddy*dt + gravity_y*dt;
}

class Missile : public GameEntity
{
public:
    float angle;
    Missile() : GameEntity() { team = 1; }
    Missile(float a, float b, float angle) : GameEntity(a, b)
    {
        team = 1;
        this->angle = angle;
        this->shape = (Shape*)new Circle(6.0);
        sf::Shape graphic;
        graphic.AddPoint(-5, 6,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));
        graphic.AddPoint(5, 6,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));
        graphic.AddPoint(0, -6,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));
        graphic.Move(a, b);
        graphic.SetRotation(angle);
        this->angle += 180;
        this->ddy = 0.5*cos((this->angle)*DEG2RAD);
        this->ddx = 0.5*sin((this->angle)*DEG2RAD);
        this->graphics = graphic;
    }
    void collide(Entity* other);
    void move();
    void update(float dt);
};

void Missile::move()
{
    x += dx;
    y += dy;
    if ((x < -(this->shape->maxSize)) || (x > WINDOW_X + this->shape->maxSize) || (y < -(this->shape->maxSize)) || (y > WINDOW_Y + this->shape->maxSize))
    {
        Scene::singleton().delEntity((Entity*)this);
    }
    graphics.SetPosition(x, y);
}

void Missile::collide(Entity* other)
{
    if (((GameEntity*)other)->team != 1)
    {
        std::cout << "GAME OVER" << std::endl;
        Scene::singleton().delEntity((Entity*)this);
    }
}

void Missile::update(float dt)
{
    dx += ddx*dt + gravity_x*dt;
    dy += ddy*dt + gravity_y*dt;
}

class Player : public GameEntity
{
public:
    float MouseX;
    float MouseY;
    float angle;
    float GunCooldown;
    Player() : GameEntity() { GunCooldown = 0; MouseX = 0; MouseY = 0; team = 1; }
    Player(float a, float b) : GameEntity(a, b) { GunCooldown = 0; MouseX = 0; MouseY = 0; team = 1; }
    Player(float a, float b, Shape* shape) : GameEntity(a, b, shape) { GunCooldown = 0; MouseX = 0; MouseY = 0; team = 1; }
    Player(float a, float b, Shape* shape, sf::Shape graphic) : GameEntity(a, b, shape, graphic) { GunCooldown = 0; MouseX = 0; MouseY = 0; team = 1; }
    void UpdateRotation();
    void collide(Entity* other);
    void move();
    void update(float dt);
    bool fire;
};

void Player::UpdateRotation()
{
    float DiffX = MouseX - this->GetX();
    float DiffY = MouseY - this->GetY();
    float MouseAngle = 0;
    if (DiffX != 0)
    {
        MouseAngle = atan(-DiffY/DiffX)*RAD2DEG + (DiffX < 0 ? 180 : 0);
    }
    else if (DiffY > 0)
    {
        MouseAngle = 270;
    }
    else
    {
        MouseAngle = 90;
    }
    angle = MouseAngle - 90;
    graphics.SetRotation(angle);
}

void Player::collide(Entity* other)
{
    if (((GameEntity*)other)->team != 1)
    {
        GameOver = true;
        Scene::singleton().delEntity((Entity*)this);
    }
}

void Player::update(float dt)
{
    dx += ddx*dt + gravity_x*dt;
    dy += ddy*dt + gravity_y*dt;
    this->UpdateRotation();
    if (GunCooldown > 0)
    {
        GunCooldown -= dt;
    }
    while (GunCooldown <= 0 && fire)
    {
        Missile* missile = new Missile(this->x, this->y, this->angle);
        Scene::singleton().addEntity((Entity*)missile);
        GunCooldown += 0.5f;
    }
}

void Player::move()
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
    srand(time(0));

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");

    sf::Vector2f WindowSize(App.GetWidth(), App.GetHeight());

    Scene::initSingleton();

    WINDOW_X = App.GetWidth();
    WINDOW_Y = App.GetHeight();

    // Use circles for now.
    sf::Shape GreenCircle = sf::Shape::Circle(0, 0, 30, sf::Color(0, 255, 0));
    GreenCircle.Move(-30, -30);
    Enemy* AsteroidOne = new Enemy(GreenCircle.GetPosition().x, GreenCircle.GetPosition().y, (Shape*)new Circle(30.0), GreenCircle);

    int angle = rand() % 360;
    AsteroidOne->Setdx(cos(angle*DEG2RAD)*0.5);
    AsteroidOne->Setdy(sin(angle*DEG2RAD)*0.5);

    Scene::singleton().addEntity((Entity*)AsteroidOne);

    sf::Shape GreenCircleTwo = sf::Shape::Circle(0, 0, 30, sf::Color(0, 255, 0));
    GreenCircleTwo.Move(WindowSize.x + 30, WindowSize.y + 30);
    Enemy* AsteroidTwo = new Enemy(GreenCircle.GetPosition().x, GreenCircle.GetPosition().y, (Shape*)new Circle(30.0), GreenCircle);

    angle = rand() % 360;
    AsteroidTwo->Setdx(cos(angle*DEG2RAD)*0.5);
    AsteroidTwo->Setdy(sin(angle*DEG2RAD)*0.5);

    Scene::singleton().addEntity((Entity*)AsteroidTwo);

    /*sf::Shape RedCircle = sf::Shape::Circle(0, 0, 30, sf::Color(255, 0, 0));
    RedCircle.Move(WindowSize.x*(3/4), WindowSize.y/2);
    Player* RedCirclePhysics = new Player(RedCircle.GetPosition().x, RedCircle.GetPosition().y, (Shape*)new Circle(30.0), RedCircle);
    //RedCirclePhysics.setGravity(0, 4.9);
    RedCirclePhysics->Setdx(-0.49);
    //RedCirclePhysics.Setdy(4.9);

    Scene::singleton().addEntity((Entity*)RedCirclePhysics);*/

    sf::String Text;
    Text.SetFont(sf::Font::GetDefaultFont());
    Text.SetSize(50);
    Text.SetText("State: ");
    Text.Move(0, App.GetHeight()/2);

    sf::String GameOverText;
    GameOverText.SetFont(sf::Font::GetDefaultFont());
    GameOverText.SetSize(50);
    GameOverText.SetText("Game Over");
    GameOverText.Move(WindowSize.x/3, WindowSize.y/2);

    sf::Shape PlayerPolygon;
    PlayerPolygon.AddPoint(-10, 12,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));
    PlayerPolygon.AddPoint(10, 12,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));
    PlayerPolygon.AddPoint(0, -12,  sf::Color(255, 0, 0),     sf::Color(0, 128, 128));

    PlayerPolygon.Move(WindowSize.x/2, WindowSize.y/2);

    Player* PlayerOne = new Player(PlayerPolygon.GetPosition().x, PlayerPolygon.GetPosition().y, (Shape*)new Circle(), PlayerPolygon);
    Scene::singleton().addEntity((Entity*)PlayerOne);

    sf::Clock Clock;

    const sf::Input& Input = App.GetInput();

    unsigned int state = 0;
    unsigned int GameState = 3;

    bool leftPressed = false;
    bool rightPressed = false;

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
            }
        }

        // Clear screen
        App.Clear();

        if (!leftPressed && Input.IsMouseButtonDown(sf::Mouse::Left))
        {
            leftPressed = true;
            if (state > 0) state--;
            if (state == GameState)
            {
                Clock.Reset();
            }
        }
        else if (leftPressed && !Input.IsMouseButtonDown(sf::Mouse::Left))
        {
            leftPressed = false;
        }
        if (!rightPressed && Input.IsMouseButtonDown(sf::Mouse::Right))
        {
            state++;
            rightPressed = true;
            if (state == GameState)
            {
                Clock.Reset();
            }
        }
        else if (rightPressed && !Input.IsMouseButtonDown(sf::Mouse::Right))
        {
            rightPressed = false;
        }

        PlayerOne->MouseX = Input.GetMouseX();
        PlayerOne->MouseY = Input.GetMouseY();
        PlayerOne->fire = Input.IsKeyDown(sf::Key::Space);
        //PlayerOne.UpdateRotation();
        //PlayerPolygon.SetRotation(PlayerOne.angle);

        std::stringstream stateString("");
        switch(state)
        {
            case 0:
            stateString << "     Eric Nelson" << std::endl << "Video Game Demo";
            break;
            case 1:
            stateString << "Why a video game demo?" << std::endl;
            break;
            case 2:
            stateString << "                 How I did it:" << std::endl;
            stateString << "Simple and Fast Multimedia Library" << std::endl << "        (http://www.sfml-dev.org)" << std::endl;
            stateString << "     Home-grown Physics library.";
            break;
        }

        if (state == GameState)
        {
            float Time = Clock.GetElapsedTime();
            Clock.Reset();
            Scene::singleton().update(Time);
            for (std::map<unsigned int, Entity*>::iterator i = Scene::singleton()._entmap.begin(); i != Scene::singleton()._entmap.end(); i++)
            {
                GameEntity* ent = dynamic_cast<GameEntity*>((*i).second);
                std::cout << ent->id << std::endl;
                if (ent)
                {
                    App.Draw(ent->graphics);
                }
            }
            if (GameOver)
            {
                App.Draw(GameOverText);
            }
        }
        else if (state == 4)
        {
            sf::String AcknowledgementsTitle("Acknowledgements:");
            sf::FloatRect f = AcknowledgementsTitle.GetRect();
            AcknowledgementsTitle.SetCenter(f.GetWidth()/2, 0);
            AcknowledgementsTitle.SetPosition(WINDOW_X/2, 25);
            App.Draw(AcknowledgementsTitle);
            sf::String Acknowledgements;
            Acknowledgements.SetText("The Presidential Scholars Program\n           President Schwaller\n            Dr. Tony Molinero\n             Dr. Chris Kelson\n              Dr. Brian Ladd\n            Dr. Jason Howald\n           Dr. Michael Walters\n              Scott Buettner");
            f = Acknowledgements.GetRect();
            Acknowledgements.SetCenter(f.GetWidth()/2, 0);
            Acknowledgements.SetPosition(WINDOW_X/2, 100);
            App.Draw(Acknowledgements);
        }
        else
        {
            Text.SetText(stateString.str());
            sf::FloatRect f = Text.GetRect();
            Text.SetCenter(f.GetWidth()/2, f.GetHeight()/2);
            Text.SetPosition(WINDOW_X/2, WINDOW_Y/2);

            App.Draw(Text);
        }

        // Update the window
        App.Display();
    }

    return EXIT_SUCCESS;
}
