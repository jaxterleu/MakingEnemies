#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>

class Enemy;

class EnemyState{
public:
    virtual ~EnemyState() {}
    virtual void Enter(Enemy& enemy) = 0; // Base state class
    virtual void Update(Enemy& enemy, float delta_time) = 0; 
};

class EnemyWandering : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyChasing : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyReadying : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class EnemyAttacking : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float delta_time);
};

class Enemy {
public:

    Vector2 position, targetpos;
    int width, height, currentdir;
    float aggroradius, detectradius, attackradius;
    bool detected, insideaggro, insideattack, isattacking, is_hit;
    Color color;
    int health;


    Vector2 velocity;
    float speed;

    // timer for attack and dodge
    float timer;

    Enemy(Vector2 pos, int width, int height, float aggrorad, float detectrad, float atkrad, float spd);

    void Update(float delta_time);

    void Draw();

    void SetState(EnemyState* new_state);
    
    EnemyWandering wandering;
    EnemyAttacking attacking;
    EnemyChasing chasing;
    EnemyReadying readying;

private:
    EnemyState* current_state;
};

#endif