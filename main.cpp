#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"

const int WINDOW_WIDTH(800);
const int WINDOW_HEIGHT(600);
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

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Making Enemies");

    while(!WindowShouldClose()){
        float delta_time = GetFrameTime();

        // update player
        player.Update(delta_time);
        
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

        // update camera
        camera_view.target = {view.position.x+(view.width/2), view.position.y+(view.width/2)};

        BeginDrawing();
        ClearBackground(BLACK);
        // establish camera view
        BeginMode2D(camera_view);

        DrawRectangle(250, 300, 200, 500, PURPLE);
        DrawCircle(100, 400, 150, RED);
        player.Draw();

        EndDrawing();

    }
    CloseWindow();
    return 0;
}