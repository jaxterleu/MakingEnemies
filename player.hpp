#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>

class Player;

class PlayerState {
    public:
        virtual ~PlayerState() {} //destructor
        virtual void Enter (Player& player) = 0;
        virtual void Update (Player& player, float delta_time) = 0;
};

class PlayerIdle : public PlayerState{
    public:
        void Enter(Player& player);
        void Update (Player& player, float delta_time);
};

class PlayerMoving : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerBlocking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerDodging : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class PlayerAttacking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time);
};

class Player{
    public:
        Vector2 position;
        float radius;
        Color color;
        int health;

        bool is_hit, is_blocking, is_dodging, is_attacking;

        Vector2 velocity;
        float speed;
        float timer;

        float attacktime;
        float dodgetime;
        

        Player(Vector2 pos, float rad, float spd, float atime, float dtime); 
        // declaring not assigning

        void Update(float delta_time);

        void Draw();
        void DrawPlayerHealth();
        
        void SetState(PlayerState* new_state);

        void SetCounter(float dcount);
        float GetCounter();
        
        void SetACounter(float dcount);
        float GetACounter();

        PlayerIdle idle;
        PlayerMoving moving;
        PlayerBlocking blocking;
        PlayerDodging dodging;
        PlayerAttacking attacking;
        // instantiating so it exists somewhere ; you can put this anywhere as long as you can find it but its easier if it's here

    private:
        PlayerState* current_state;
        float dodgecount;
        float attackcount;
};

#endif