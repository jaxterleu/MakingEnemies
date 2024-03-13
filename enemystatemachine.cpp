#include <raylib.h>
#include <raymath.h>
#include "enemy.hpp"
#include <cstdlib>
#include <string>
#include <iostream>

void Enemy::Update(float delta_time){
    current_state->Update(*this, delta_time);
}

void Enemy::Draw(){
    DrawRectangle(position.x-width/2, position.y-height/2, width, height, color);
    DrawCircleLines(position.x, position.y, detectradius, YELLOW);
    DrawCircleLines(position.x, position.y, aggroradius, BLUE);
    DrawCircleLines(position.x, position.y, attackradius, RED);
}

void Enemy::SetState(EnemyState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

Enemy::Enemy(Vector2 pos, int w, int h, float aggrorad, float detectrad, float atkrad, float spd){
    position = pos;
    width = w;
    height = h;
    aggroradius = aggrorad;
    detectradius = detectrad;
    attackradius = atkrad;
    speed = spd;
    health = 5;
    timer = 0;
    SetState(&wandering);
}

void EnemyWandering::Enter(Enemy& enemy){
    enemy.color = SKYBLUE;
    enemy.timer = 0;
}
void EnemyChasing::Enter(Enemy& enemy){
    enemy.color = YELLOW;
}
void EnemyReadying::Enter(Enemy& enemy){
    enemy.color = ORANGE;
}
void EnemyAttacking::Enter(Enemy& enemy){
    enemy.color = RED;
}

void EnemyWandering::Update(Enemy& enemy, float delta_time){
    enemy.velocity = Vector2Zero();
    int n = 4;

    // Get random number between 0-3 once timer runs out
    if(enemy.timer <= 0){
        enemy.currentdir = rand() % n;
        enemy.timer = 3;
        std::cout<<"enemy's current direction is: "<<enemy.currentdir<<"\n";
    }
    // Move toward number until timer depletes
    else{
        if(enemy.currentdir == 0){
            enemy.velocity.y -= enemy.speed * delta_time;
        }
        else if (enemy.currentdir == 1)
        {
            enemy.velocity.y += enemy.speed * delta_time;
        }
        else if (enemy.currentdir == 2)
        {
            enemy.velocity.x -= enemy.speed * delta_time;
        }
        else if (enemy.currentdir == 3)
        {
            enemy.velocity.x += enemy.speed * delta_time;
        }     
    }
    enemy.timer -= delta_time;  

    enemy.position = Vector2Add(enemy.position, enemy.velocity);
    // If enemy detects player here, make collision
}

void EnemyChasing::Update(Enemy& enemy, float delta_time){

}
void EnemyReadying::Update(Enemy& enemy, float delta_time){
    
}
void EnemyAttacking::Update(Enemy& enemy, float delta_time){
    
}