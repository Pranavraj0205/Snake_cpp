#include <iostream.h>
#include <vector>
#include <cstdlib>
#include <conio> // For _getch() and _kbhit()
#include <unistd> // For usleep() on Unix-based systems
#include <ctime> // For seeding rand

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

class Snake {
private:
    vector<pair<int, int>> snake; // Snake body segments
    pair<int, int> fruit; // Fruit position
    char direction; // Current direction of the snake
    bool gameOver; // Game state
    int score; // Score

public:
    Snake() {
        setup();
    }

    void setup() {
        srand(static_cast<unsigned>(time(0))); // Seed random number generator
        snake.push_back(make_pair(HEIGHT / 2, WIDTH / 2)); // Start snake at the center
        direction = 'd'; // Start moving to the right
        score = 0;
        gameOver = false;
        placeFruit();
    }

    void placeFruit() {
        fruit.first = rand() % HEIGHT ?rand() % HEIGHT : 1; // Random y position
        fruit.second = rand() % WIDTH ?rand() % WIDTH : 1; // Random x position
        
    }

    void draw() {
        system("cls"); // Clear console (Windows: system("cls")
        for (int i = 0; i < HEIGHT + 2; ++i) {
            for (int j = 0; j < WIDTH + 2; ++j) {
                if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
                    cout << '#'; // Draw boundaries
                } else if (i == snake[0].first && j == snake[0].second) {
                    cout << '@'; // Snake head
                } else if (i == fruit.first && j == fruit.second) {
                    cout << '*'; // Fruit
                } else {
                    bool isBody = false;
                    for (int k = 1; k < snake.size(); ++k) {
                        if (snake[k].first == i && snake[k].second == j) {
                            cout << 'o'; // Snake body
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody) {
                        cout << ' ';
                    }
                }
            }
            cout << endl;
        }
        cout << "Score: " << score << endl;
        cout << "Controls: W(up) A(left) S(down) D(right) X(exit)" << endl;
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': direction = 'w'; break; // Up
                case 'a': direction = 'a'; break; // Left
                case 's': direction = 's'; break; // Down
                case 'd': direction = 'd'; break; // Right
                case 'x': gameOver = true; break; // Exit
            }
        }
    }

    void run() {
        pair<int, int> newHead = snake[0];

        switch (direction) {
            case 'w': newHead.first--; break; // Move up
            case 's': newHead.first++; break; // Move down
            case 'a': newHead.second--; break; // Move left
            case 'd': newHead.second++; break; // Move right
        }

        // Check for collisions with boundaries
        if (newHead.first < 1 || newHead.first > HEIGHT || newHead.second < 1 || newHead.second > WIDTH) {
            gameOver = true;
            return;
        }

        // Check for collisions with itself
        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake[i] == newHead) {
                gameOver = true;
                return;
            }
        }

        snake.insert(snake.begin(), newHead); // Add new head

        // Check if snake has eaten the fruit
        if (newHead == fruit) {
            score++;
            placeFruit(); // Place new fruit
        } else {
            snake.pop_back(); // Remove last segment if no fruit eaten
        }
    }

    bool isGameOver() {
        return gameOver;
    }

    int getScore() { // Add this method
    return score;
    }
};

int main() {
    Snake s;
    while (!s.isGameOver()) {
        s.draw();
        s.input();
        s.run();
        usleep(300000); // Control speed of the snake
    }
    cout << "Game Over! Your score: " << s.getScore() << endl;
    return 0;
}

