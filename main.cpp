#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"
#include "enemystatemachine.cpp"

const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

//for camera snapping
struct Square{
    Vector2 position;
    float width;
    Color color;
};

int main(){
    
    // initiating the camera type
    int cam_type;

    // instantiating view for snapping
    Square view;
    view.position = {100, 100};
    view.width = 300;
    view.color = BLACK;

    //camera settings
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];
    camera_view.target = {0, 0};
    camera_view.offset = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    camera_view.zoom = 1.0f;

    Player player({WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, 25.0f, 200.0, 1.0, 0.25);
    Enemy enemy({200,200}, 50, 50, 300.0f, 160.0f, 90.0f, 50.0f);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Aruego-Berones-Leuterio_Homework03");

    while(!WindowShouldClose()){
        float delta_time = GetFrameTime();

        // update player
        player.Update(delta_time);
        enemy.Update(delta_time);

        //cameria view collision
        if (player.position.x+player.radius >= view.position.x + view.width){
            view.position.x = player.position.x+player.radius - view.width;
        }
        if (player.position.y+player.radius >= view.position.y + view.width){
            view.position.y = player.position.y+player.radius - view.width;
        }
        if (player.position.x-player.radius <= view.position.x){
            view.position.x = player.position.x-player.radius;
        }
        if (player.position.y-player.radius <= view.position.y){
            view.position.y = player.position.y-player.radius;
        }

        if (IsKeyUp(KEY_W) && IsKeyUp(KEY_A) && IsKeyUp(KEY_S) && IsKeyUp(KEY_D)){
            //if player not at center of view
            if (player.position.x > view.position.x+(view.width/2)){
                    view.position.x = view.position.x + (player.speed/2)*delta_time;
                }
                if (player.position.x < view.position.x+(view.width/2)){
                    view.position.x = view.position.x - (player.speed/2)*delta_time;
                }
                if (player.position.y > view.position.y+(view.width/2)){
                    view.position.y = view.position.y + (player.speed/2)*delta_time;
                }
                if (player.position.y < view.position.y+(view.width/2)){
                    view.position.y = view.position.y - (player.speed/2)*delta_time;
                }
        }

        // Collision Step
        // Detect Radius Check
        if (player.radius+enemy.detectradius >= Vector2Distance(player.position, enemy.position)){
            enemy.detected = true;
            enemy.targetpos = player.position;
        }
        else if(player.radius+enemy.detectradius >= Vector2Distance(player.position, enemy.position) && player.radius+enemy.aggroradius <= Vector2Distance(player.position, enemy.position)){
            enemy.detected = false;
        }

        // Aggro radius check
        if(player.radius+enemy.aggroradius >= Vector2Distance(player.position, enemy.position)){
            enemy.targetpos = player.position;
        }
        else{
            enemy.detected = false;
        }

        // Attack radius check
        if (player.radius+enemy.attackradius >= Vector2Distance(player.position, enemy.position)){
            enemy.insideattack = true;
            enemy.targetpos = player.position;
        }
        else{
            enemy.insideattack = false;
        }

        // update camera
        camera_view.target = {view.position.x+(view.width/2), view.position.y+(view.width/2)};

        BeginDrawing();
        ClearBackground(BLACK);
        // Check if player loses
        if (player.health == 0){
            view.position.x = WINDOW_WIDTH/2;
            view.position.y = WINDOW_HEIGHT/2;
            ClearBackground(BLACK);
            DrawText("You Lose", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, WHITE);
        }
        // Check if player wins
        else if ((enemy.health) == 0){
            view.position.x = WINDOW_WIDTH/2;
            view.position.y = WINDOW_HEIGHT/2;
            ClearBackground(BLACK);
            DrawText("You Win", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, WHITE);
        }
        else {
            player.DrawPlayerHealth();
            // establish camera view
            BeginMode2D(camera_view);
            player.Draw();
            enemy.Draw();
        }
        EndMode2D();
        EndDrawing();

    }
    CloseWindow();
    return 0;
}