
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

const int M = 20;
const int N = 10;
const int PIECE_NUM = 4;

int field[M][N] = {0};

struct Point
{int x, y;} a[4], b[4];

int figures[7][4] = {

    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

void move_block(int &dx);
void rotate_block();
void tick(float &timer);
bool check();
void rebound();
void check_line();
void gen_rand(int &colorNum);

int main(){

    RenderWindow window(VideoMode(320, 480), "Tetris!");

    Texture t;
    t.loadFromFile("images/tiles.png");

    Texture t2;
    t2.loadFromFile("images/background.png");

    Sprite s(t);
    s.setTextureRect(IntRect(0,0,18,18));

    Sprite background(t2);
    background.setTextureRect(IntRect(0,0,320,480));

    int dx = 0; bool rotate = 0; int colorNum = 1;
    float timer = 0, delay = 0.3; float time;

    Clock clock;

    while(window.isOpen()){

        time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while(window.pollEvent(e)){
            if(e.type == Event::Closed)
                window.close();

            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Up) rotate = true;
                else if(e.key.code == Keyboard::Left) dx = -1;
                else if(e.key.code == Keyboard::Right) dx = 1;
            }
        } // END EVENT LOOP

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

        if(dx != 0)
            move_block(dx);
        if(rotate)
            rotate_block();
        if(timer > delay){
            tick(timer);

            if(!check()){
                gen_rand(colorNum);
            }
        }

        check_line();

        dx = 0; rotate = 0; delay = 0.3;

        window.clear(Color::White);
        window.draw(background);

        for(int i = 0; i < M; i++){
            for(int j = 0; j < N; j++){
                if(field[i][j] == 0)
                    continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                window.draw(s);
            }
        }

        for(int i = 0; i < PIECE_NUM; i++){
            s.setTextureRect(IntRect(colorNum*18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            window.draw(s);
        }

        window.display();
    }
    // END GAME LOOP

    return 0;
}

void move_block(int &dx){
    for(int i = 0; i < PIECE_NUM; i++){
        b[i] = a[i];
        a[i].x += dx;
    }

    if(!check())
        rebound();
}

void rotate_block(){
    Point p = a[1]; // CENTER OF ROTATION
    for(int i = 0; i < PIECE_NUM; i++){
        int x = a[i].y - p.y;
        int y = a[i].x - p.x;
        a[i].x = p.x - x;
        a[i].y = p.y + y;
    }
    if(!check())
        rebound();
}

void tick(float &timer){
    for(int i = 0; i < PIECE_NUM; i++){
        b[i] = a[i];
        a[i].y += 1;
    }

    timer = 0;
}

bool check(){
    for(int i = 0; i < PIECE_NUM; i++){
        if(a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if(field[a[i].y][a[i].x])
            return 0;
    }
    return 1;
}

void rebound(){
    for(int i = 0; i < PIECE_NUM; i++)
        a[i] = b[i];
}

void gen_rand(int &colorNum){
    for(int i = 0; i < PIECE_NUM; i++)
        field[b[i].y][b[i].x] = colorNum;

    colorNum = 1 + std::rand() % 7;
    int n = std::rand() % 7;
    for(int i = 0; i < PIECE_NUM; i++){
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }
}

void check_line() {
    int k = M - 1;

    for ( int i = M - 1; i > 0; i-- ) {
        int count = 0;
        for ( int j = 0; j < N; j++ ) {
            if ( field[ i ][ j ] )
                count++;

            field[ k ][ j ] = field[ i ][ j ];
        }

        if ( count < N )
            k--;
    }
}
