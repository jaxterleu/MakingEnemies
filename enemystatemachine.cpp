#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include "enemy.hpp"
#include <cstdlib>
#include <string>
#include <iostream>

void Enemy::Update(float delta_time){
    current_state->Update(*this, delta_time);
}

void Enemy::Draw(){
    // If current state is readying, orient towards player
    if(current_state != &wandering){
        //get direction towards rec
        Rectangle rec;
        rec.x = position.x;
        rec.y = position.y;
        rec.width = width;
        rec.height = height;
        Vector2 dir = Vector2Subtract(targetpos, position);
        float angle = atan2f(dir.y, dir.x) * RAD2DEG;
        DrawRectanglePro(rec, {(float)width/2,(float)height/2}, angle, color);
    }
    else{
        DrawRectangle(position.x-width/2, position.y-height/2, width, height, color);
    }
    
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
    health = 2;
    timer = 0;
    detected = false;
    insideaggro = false;
    insideattack = false;
    isattacking = false;
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
    enemy.velocity = Vector2Zero();
    enemy.color = GREEN;
    enemy.timer = 2;
}
void EnemyAttacking::Enter(Enemy& enemy){
    enemy.isattacking = true;
    enemy.velocity = Vector2Zero();
    Vector2 dir = Vector2Subtract(enemy.targetpos, enemy.position);
    dir = Vector2Normalize(dir);
    enemy.velocity = Vector2Scale(dir, 3);
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
    // If enemy detects player here, chasing
    if(enemy.detected){
        enemy.SetState(&enemy.chasing);
    }
}

void EnemyChasing::Update(Enemy& enemy, float delta_time){
    enemy.velocity = Vector2Zero();
    Vector2 dir = Vector2Subtract(enemy.targetpos, enemy.position);
    dir = Vector2Normalize(dir);
    enemy.velocity = Vector2Scale(dir, delta_time * enemy.speed);
    enemy.position = Vector2Add(enemy.position, enemy.velocity); 

    if(enemy.detected == false && enemy.insideattack == false){
        enemy.SetState(&enemy.wandering);
    }

    if(enemy.insideattack){
        enemy.SetState(&enemy.readying);
    }
}
void EnemyReadying::Update(Enemy& enemy, float delta_time){
    if(enemy.timer > 0){
        enemy.timer -= delta_time;
    }
    else{
        enemy.SetState(&enemy.attacking);
    }
}
void EnemyAttacking::Update(Enemy& enemy, float delta_time){
    enemy.position = Vector2Add(enemy.position, enemy.velocity);
    enemy.velocity = Vector2Subtract(enemy.velocity, Vector2Scale(enemy.velocity, 10.0f*delta_time));

    if(Vector2Length(enemy.velocity) < 0.2){
        enemy.isattacking = false;
        enemy.SetState(&enemy.wandering);
    }
}