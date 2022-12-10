#include <raylib.h>
#include <iostream>
#include <vector>

// contains a balls location and size
struct ball {
    public:
    float x, y;
    float h, v = 0;
    int radius;
    bool selected = false;
    bool releasing = false;
    float initialEnergy = 0;
    float distance = 0;
    float tempEnergy = 0;
    Color colour;

    void draw() {
        if (selected) {
            DrawCircle(x, y, radius, RED);
        } else {
            DrawCircle(x, y, radius, colour);
        }
    }

    float weight(int base = 5) {
        return radius / base;
    }

    bool touching(int x_, int y_, int w, int h) {
        if (x < x_ + w && x > x_ - w && y <= y_ + h && y >= y_ - h) {
            return true;
        } else {
            return false;
        }
    }

    bool above(int y_) {
        if ((y - radius) <= y_) {
            return true;
        } else {
            return false;
        }
    }
    
    bool below(int y_) {
        if ((y + radius) >= y_) {
            return true;
        } else {
            return false;
        }
    }

    ball(float x, float y, int radius, Color colour) : x(x), y(y), radius(radius), colour(colour) {}
};

int main() {
    const int width = 700;
    const int height = 700;
    const float gravity = 0.1;
    const float maxFall = 10;
    const float bounce = 1.5;
    const int fps = 60;
    const char * title = "Balls";

    std::vector<ball> balls = {
        ball(200.0, 600.0, 50, GREEN),
        ball(100.0, 200.0, 10, RAYWHITE)
    };

    InitWindow(width, height, title);
    SetTargetFPS(fps);

    while (!WindowShouldClose()) {
        // add new balls
        if (IsKeyReleased(KEY_EQUAL)) {
            srand(time(0));
            balls.push_back(ball(50 + (rand() % 600), 50 + (rand() % 600), 5 + (rand() % 50), YELLOW));
        }
        // check for clicked balls
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            for (auto& b : balls) {
                if (!b.selected) {
                    if (!b.above(0)) {
                        b.y -= 15 / b.weight();
                    }
                    b.v = 0;
                }
            }
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for (auto& b : balls) {
                Vector2 mp = GetMousePosition();
                if (b.touching(mp.x, mp.y, b.radius, b.radius)) {
                    //std::cout << "TOUCHING!" << std::endl;
                    b.selected = !b.selected;
                    b.v = 0;
                }
            }
        } else {
            // apply gravity to balls
            for (auto& b : balls) {
                if (b.selected) {
                    Vector2 mp = GetMousePosition();
                    b.x = mp.x;
                    b.y = mp.y;
                    b.initialEnergy = 0;
                    b.tempEnergy = 0;
                    continue;
                }
                if (b.releasing) {
                    if (b.tempEnergy < 1) {
                        b.tempEnergy = 0;
                        b.initialEnergy /= 2;
                        b.releasing = false;
                        continue;
                    }
                    b.tempEnergy /= 2;
                    b.v = -b.tempEnergy;
                } else {
                    if (b.v < maxFall * b.weight()) {
                        b.v += gravity * b.weight();
                        b.distance += b.v;
                        if (b.initialEnergy == 0 && b.below(height)) {
                            b.initialEnergy = b.distance / 2;
                            b.distance = 0;
                        }
                    }
                }
                if (b.below(height) && !b.releasing) {
                    b.y = height - b.radius;
                    b.v = 0;
                    if (b.initialEnergy > 1) {
                        b.tempEnergy = b.initialEnergy;
                        b.releasing = true;
                    }
                    std::cout << b.tempEnergy << std::endl;
                    continue;
                } else if (b.x > width - b.radius || b.x < b.radius) {
                    b.x = width / 2;
                    b.v = 0;
                }
                b.y += b.v;
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        // draw objects
        for (auto& b : balls) {
            b.draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}