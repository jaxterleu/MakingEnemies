#include <raylib.h>
#include <raymath.h>
#include <string>
#include "Player.hpp"
#include <stdio.h>

void Player::Update(float delta_time){
    current_state->Update(*this, delta_time);
}

void Player::Draw() {
    DrawCircleV(position, radius, color);
}

void Player::DrawPlayerHealth() {
    DrawText(std::to_string(health).c_str(), 10, 10, 40, WHITE);
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Player::SetCounter(float dcount){
    dodgecount = dcount;
}

float Player::GetCounter(){
    return dodgecount;
}

void Player::SetACounter(float acount){
    attackcount = acount;
}

float Player::GetACounter(){
    return attackcount;
}

Player::Player(Vector2 pos, float rad, float spd, float atime, float dtime){
    position = pos;
    radius = rad;
    speed = spd;
    attacktime = atime;
    dodgetime = dtime;
    health = 5;
    SetState(&idle);
    SetCounter(0.0);
}

void PlayerIdle::Enter(Player& player) {
    player.color = PINK;
}

void PlayerMoving::Enter(Player& player) {
    player.color = GREEN;
}

void PlayerBlocking::Enter(Player& player){
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        player.color = BLUE;
    }
}

void PlayerAttacking::Enter(Player& player){
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        player.color = RED;
    }
}

void PlayerDodging::Enter(Player& player){
    if (IsKeyDown(KEY_SPACE)){
        player.color = WHITE;
    }
}

void PlayerIdle::Update(Player& player, float delta_time){

    //honestly just out conditions
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)){
        player.SetState(&player.moving);
    };

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        player.SetState(&player.blocking);
    };

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        player.SetState(&player.attacking);
    };
}

void PlayerMoving::Update(Player& player, float delta_time){
    
    if(IsKeyDown(KEY_W)){
        player.velocity.y = player.speed * -1 * delta_time;
    }
    if(IsKeyDown(KEY_A)){
        player.velocity.x = player.speed * -1 * delta_time;
    }
    if(IsKeyDown(KEY_S)){
        player.velocity.y = player.speed * delta_time;
    }
    if(IsKeyDown(KEY_D)){
        player.velocity.x = player.speed * delta_time;
    }
    if (!(IsKeyDown(KEY_W) || IsKeyDown(KEY_S))){
        player.velocity.y = 0;
    }
    if (!(IsKeyDown(KEY_A) || IsKeyDown(KEY_D))){
        player.velocity.x = 0;
    }

    player.position = Vector2Add(player.position, player.velocity);


    //out condition/s
    if (IsKeyPressed(KEY_SPACE)){
        player.SetState(&player.dodging);
    }
    
    if (Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    }
    
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        player.SetState(&player.blocking);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        player.SetState(&player.attacking);
    }
}

void PlayerBlocking::Update(Player& player, float delta_time){
    //not allowed to move
    player.velocity.y = 0;
    player.velocity.x = 0;

    //out condition
    if (IsMouseButtonUp(MOUSE_BUTTON_RIGHT)){
        player.SetState(&player.idle);
    }
}

void PlayerDodging::Update(Player& player, float delta_time){

    //for a set amt of frames/time/distance
        //go faster in direction last done
    
    if (player.velocity.x > 0){
        player.velocity.x = player.speed * 5 * delta_time;
    }
    else if (player.velocity.x < 0){
        player.velocity.x = player.speed * -5 * delta_time;
    }

    if (player.velocity.y > 0){
        player.velocity.y = player.speed * 5  * delta_time;
    }
    else if (player.velocity.y < 0){
        player.velocity.y = player.speed * -5 * delta_time;
    }

    player.position = Vector2Add(player.position, player.velocity);
    player.SetCounter(player.GetCounter()+delta_time);
    
    printf("velo: x%f y%f - %f > %f\n", player.velocity.x, player.velocity.y, player.dodgetime, player.GetCounter());



    //out condition/s
    if (player.GetCounter() > player.dodgetime) {
        player.SetState(&player.idle);
        player.SetCounter(0.0);
        player.velocity = Vector2Zero();
    }

    if(IsKeyPressed(KEY_TAB)){
        player.SetState(&player.idle);
    }
    

}

void PlayerAttacking::Update(Player& player, float delta_time){

    player.SetACounter(player.GetACounter()+delta_time);

    //not allowed to move
    player.velocity.y = 0;
    player.velocity.x = 0;

    //out condition
    if (player.GetACounter() > player.attacktime){
        player.SetState(&player.idle);
        player.SetACounter(0.0);
    }
}