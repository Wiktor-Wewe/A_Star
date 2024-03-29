﻿#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <time.h>

#include "Node.h"
#include "SelfSortingQueue.h"

std::vector<std::vector<int>> loadMazeFromFile(char* path)
{
    std::vector<std::vector<int>> maze;

    std::fstream file(path, std::ios::in);
    if (file.good() == false) {
        return maze;
    }

    std::string buff;
    maze.push_back(std::vector<int>());

    int line = 0;
    while (!file.eof()) {
        std::getline(file, buff);
        for (auto x : buff) {
            if (x >= '0' && x <= '9') {
                maze[line].push_back(x-'0');
            }
        }

        if (!file.eof()) {
            line++;
            maze.push_back(std::vector<int>());
        }
    }

    return maze;
}

std::vector<Node::Position> astar(std::vector<std::vector<int>>& maze, Node::Position start, Node::Position end) 
{
    auto nStart = new Node(start);
    auto nEnd = new Node(end);

    SelfSortingQueue openList;
    std::vector<Node*> closedList;

    openList.push(nStart);

    while (!openList.empty()) {
        Node* current = openList.pop();
        closedList.push_back(current);

        if (*current == *nEnd) {
            std::vector<Node::Position> path;
            Node* node = current;
            while (node != nullptr) {
                path.push_back(node->position);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());

            delete nStart; delete nEnd;
            return path;
        }

        std::vector<Node::Position> directions = { Node::Position(0, 1), Node::Position(0, -1), Node::Position(1, 0), Node::Position(-1, 0) };

        for (const auto& direction : directions) {
            int x = current->position.x + direction.x;
            int y = current->position.y + direction.y;

            if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size() || maze[x][y] == 5) {
                continue;
            }

            Node* neighbor = new Node(Node::Position(x, y));
            neighbor->g = current->g + 1;
            neighbor->h = abs(x - nEnd->position.x) + abs(y - nEnd->position.y);
            neighbor->f = neighbor->g + neighbor->h;
            neighbor->parent = current;

            bool skip = false;
            for (auto closed : closedList) {
                if (*neighbor == *closed) {
                    skip = true;
                    break;
                }
            }

            if (skip) {
                delete neighbor;
                neighbor = nullptr;
                continue;
            }

            bool inOpen = false;
            for (auto open : openList.getVector()) {
                if (*neighbor == *open && neighbor->g > open->g) {
                    inOpen = true;
                    break;
                }
            }

            if (!inOpen) {
                openList.push(neighbor);
            }
            else {
                delete neighbor;
                neighbor = nullptr;
            }
        }
    }
    return {};
}

void showPath(std::vector<Node::Position>& path, std::vector<std::vector<int>> maze, Node::Position start, Node::Position end) 
{
    for (auto p : path) {
        maze[p.x][p.y] = 9;
    }

    printf(" ");
    for (int i = 0; i < maze[0].size(); i++) {
        if (i < 10) printf(" ");
        if (i == 10) printf(" ");
        printf("%i ", i);
    }
    printf("\n");

    for (int x = 0; x < maze.size(); x++) {
        if (x < 10) printf(" ");
        printf("%i", x);
        for (int y = 0; y < maze[x].size(); y++) {
            if (start.x == x && start.y == y) {
                printf("\x1b[43m  \x1b[0m ");
            }
            else if (end.x == x && end.y == y) {
                printf("\x1b[45m  \x1b[0m ");
            }
            else if (maze[x][y] == 0) {
                printf("\x1b[47m  \x1b[0m ");
            }
            else if (maze[x][y] == 5) {
                printf("\x1b[41m  \x1b[0m ");
            }
            else if (maze[x][y] == 9) {
                printf("\x1b[44m  \x1b[0m ");
            }
        }
        printf("\n");
    }
}

bool isPosistionPossible(Node::Position position, std::vector<std::vector<int>>& maze)
{
    if (maze[position.x][position.y] == 5) {
        return false;
    }
    return true;
}

Node::Position randPosition(std::vector<std::vector<int>>& maze)
{
    auto position = Node::Position(rand() % 20, rand() % 20);

    while (isPosistionPossible(position, maze) == false) {
        position.x = rand() % 20;
        position.y = rand() % 20;
    }

    return position;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("usage: A_Star.exe *pathToMaze.txt*\n\n");
        return 1;
    }

    srand(time(0));
    auto maze = loadMazeFromFile(argv[1]);

    if (maze.empty()) {
        printf("unable to load file: %s\n", argv[1]);
        return -1;
    }

    bool close = false;
    char input = 0;

    system("cls");
    while (!close) {
        auto start = randPosition(maze);
        auto end = randPosition(maze);
        auto result = astar(maze, start, end);
        printf("\x1b[43m  \x1b[0m start: x=%i y=%i\n", start.y, start.x);
        printf("\x1b[45m  \x1b[0m end: x=%i y=%i\n", end.y, end.x);
        showPath(result, maze, start, end);

        scanf_s("%c", &input);
        if (input == '0') {
            close = true;
        }
        system("cls");
    }
    
    return 0;
}