#pragma once
class Node
{
public:
	struct Position {
		Position() {
			this->x = 0;
			this->y = 0;
		}

		Position(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int x;
		int y;

		Position& operator=(const Position& other) {
			if (this == &other) return *this;
			
			this->x = other.x;
			this->y = other.y;

			return *this;
		}
	};
	
	Node(Position position, Node* parent = nullptr) {
		this->parent = parent;
		this->position = position;
		this->g = 0;
		this->h = 0;
		this->f = 0;
	}

	bool operator==(const Node& other) const {
		if ((this->position.x == other.position.x) && (this->position.y == other.position.y)) {
			return true;
		}
		return false;
	}

	Node* parent;
	Position position;
	int g;
	int h;
	int f;
};
