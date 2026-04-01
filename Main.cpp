#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <conio.h>  // Windows 專用，Linux/macOS 需改用 termios

using namespace std;
using namespace chrono;

const int WIDTH = 40;
const int HEIGHT = 20;
const int LANE_WIDTH = 6;
const int LANES = 3;
const int PLAYER_X = WIDTH / 2;

int playerY = HEIGHT - 2;
int score = 0;
bool gameRunning = true;

struct Obstacle {
    int x;
    int y;
};

vector<Obstacle> obstacles;

void draw() {
    system("cls"); // 清屏，Linux 可用 "clear"

    // 畫上方邊界
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << endl;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // 畫車道分隔線 (每 LANE_WIDTH 一豎)
            bool isLaneDivider = (x % LANE_WIDTH == 0) && (y > 0 && y < HEIGHT - 1);
            if (isLaneDivider) {
                cout << "|";
                continue;
            }

            bool isPlayer = (y == playerY && x == PLAYER_X);
            bool isObstacle = false;
            for (auto& obs : obstacles) {
                if (obs.y == y && obs.x == x) {
                    isObstacle = true;
                    break;
                }
            }

            if (isPlayer) cout << "A";      // 玩家車
            else if (isObstacle) cout << "V"; // 敵方車
            else cout << " ";
        }
        cout << endl;
    }

    // 畫下方邊界
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "\nScore: " << score << endl;
}

void update() {
    // 移動障礙物
    for (auto& obs : obstacles) {
        obs.y++;
    }

    // 移除超出畫面的障礙物並加分
    obstacles.erase(remove_if(obstacles.begin(), obstacles.end(),
        [&](Obstacle& obs) {
            if (obs.y >= HEIGHT) {
                score++;
                return true;
            }
            return false;
        }), obstacles.end());

    // 隨機生成新障礙物
    if (rand() % 15 == 0) { // 降低生成頻率
        int lane = rand() % LANES;
        int xPos = lane * LANE_WIDTH + LANE_WIDTH / 2;
        obstacles.push_back({xPos, 0});
    }

    // 碰撞檢測
    for (auto& obs : obstacles) {
        if (obs.y == playerY && obs.x == PLAYER_X) {
            gameRunning = false;
        }
    }
}

void input() {
    if (_kbhit()) { // 檢查是否有按鍵
        char key = _getch();
        if (key == 'a' && PLAYER_X - LANE_WIDTH >= 0) {
            // 左移一個車道
            // 需確保新位置不超出邊界
        }
        // 簡化：直接移動固定距離
        // 完整版可根據按鍵調整 PLAYER_X
    }
}

int main() {
    srand(time(0));
    playerY = HEIGHT - 2;

    auto lastTime = steady_clock::now();
    const int FPS = 10;
    const milliseconds frameDuration(1000 / FPS);

    while (gameRunning) {
        auto currentTime = steady_clock::now();
        auto elapsed = currentTime - lastTime;

        if (elapsed >= frameDuration) {
            lastTime = currentTime;
            input();
            update();
            draw();
        }
        this_thread::sleep_for(milliseconds(1));
    }

    cout << "\nGame Over! Final Score: " << score << endl;
    return 0;
}
