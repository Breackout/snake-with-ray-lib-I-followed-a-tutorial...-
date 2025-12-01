#include<iostream>
#include<raylib.h>
#include<raymath.h>
#include<deque>

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 10;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

bool EventTriger(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake{
public:
    std::deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle snake = {offset+x*cellSize, offset+y*cellSize, float(cellSize), float(cellSize)};
            DrawRectangleRounded(snake, 0.5, 6, darkGreen);
        }
    }

    void Update(){

        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true){
            addSegment = false;
        }else{
            body.pop_back();
        }
    }

    void Reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1, 0};
    }
};

class Food{
public:
    Vector2 position = {5, 6};
    Texture2D texture; 

    Food(std::deque<Vector2> snakeBody){
        Image image = LoadImage("Graphics/food1.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture, offset+position.x*cellSize, offset+position.y*cellSize, WHITE);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody){

        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody)){
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;

    /*
    Sound eatSound;
    Sound wallSound;

    Game(){
        InitAudioDevice();
        eatSound = LoadSound("Sound/eat.mp3");
        wallSound = LoadSound("Sound/wall.mp3");
    }

    ~Game(){
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }
    */

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        if(running){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        //PlaySound(wallSound);
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            //PlaySound(eatSound);
        } 
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void CheckCollisionWithTail(){
        std::deque<Vector2> headLessSnake = snake.body;
        headLessSnake.pop_front();
        if(ElementInDeque(snake.body[0], headLessSnake)){
            GameOver();
        }
    }
};

int main(){
    SetTargetFPS(60);
    InitWindow(2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "SNAKE");

    Game game = Game();

    while(!WindowShouldClose()){
        BeginDrawing();

        if(EventTriger(0.2)){
            game.Update();
        }

        if(IsKeyDown(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0, -1};
            game.running = true;
        }else if(IsKeyDown(KEY_DOWN)  && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;
        }else if(IsKeyDown(KEY_LEFT)  && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }else if(IsKeyDown(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{float(offset)-5, float(offset)-5, float(cellSize)*cellCount+10, float(cellSize)*cellCount+10},5 ,darkGreen);
        DrawText("Retro Snake", offset-5, 20, 40, darkGreen);
        DrawText(std::to_string(game.score).c_str(), offset-5, offset+cellSize*cellCount+10, 40, darkGreen);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
