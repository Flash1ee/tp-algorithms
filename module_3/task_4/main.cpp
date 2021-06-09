#include <array>
#include <queue>
#include <unordered_map>
#include <assert.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <set>

using std::array;
using std::queue;
using std::unordered_map;
using std::swap;
using std::string;
using std::cout;
using std::ostream;

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const array<char, FieldSize> FinishField({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});

class GameState {
public:
    GameState(const array<char, FieldSize> &_field);

    bool CanMoveLeft() const;

    bool CanMoveUp() const;

    bool CanMoveRight() const;

    bool CanMoveDown() const;

    GameState MoveLeft() const;

    GameState MoveUp() const;

    GameState MoveRight() const;

    GameState MoveDown() const;

    bool IsFinish() const;

    int getManhattanDistance();

    int getHeuristicDistance() const { return this->heuristicDistance; }

    char getLastMove() const { return this->lastMove; }

    bool operator==(const GameState &state) const { return field == state.field; }

    bool operator!=(const GameState &state) const { return !(*this == state); }

    bool operator<(const GameState &state) const {
        return heuristicDistance < state.getHeuristicDistance();
    }

    void updateHeuristic() {
        this->heuristicDistance = getManhattanDistance() + ((field[FieldSize - 1] == FieldSize - 1 ||
                                                             field[FieldSize - 1] == FieldSize - SideSize) ? 0 : 2);
    }

    void saveLastMove(char action) { this->lastMove = action; }

private:
    array<char, FieldSize> field;
    char zeroPos;
    int heuristicDistance;
    char lastMove;

    friend ostream &operator<<(ostream &out, const GameState &state);

    friend struct StateHasher;
};

ostream &operator<<(ostream &out, const GameState &state) {
    for (int y = 0; y < SideSize; ++y) {
        for (int x = 0; x < SideSize; ++x) {
            out << static_cast<int>(state.field[y * SideSize + x]) << " ";
        }
        out << std::endl;
    }
    return out;
}

int GameState::getManhattanDistance() {
    int res = 0;
    for (int i = 0; i < FieldSize; ++i) {
        int number = (field[i] + FieldSize - 1) % FieldSize;
        res += abs(number - i) % SideSize + abs((number / SideSize - i / SideSize));
    }
    return res;
}

GameState::GameState(const array<char, FieldSize> &_field) :
        field(_field),
        zeroPos(-1),
        heuristicDistance(0),
        lastMove('S') {
    for (unsigned int i = 0; i < field.size(); ++i)
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    updateHeuristic();
    assert(zeroPos != -1);
}

bool GameState::CanMoveLeft() const {
    return zeroPos % SideSize != 0;
}

bool GameState::CanMoveUp() const {
    return zeroPos >= SideSize;
}

bool GameState::CanMoveRight() const {
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveDown() const {
    return zeroPos < FieldSize - SideSize;
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos], newState.field[newState.zeroPos - 1]);
    --newState.zeroPos;
    newState.saveLastMove('R');
    newState.updateHeuristic();
    return newState;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos], newState.field[newState.zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    newState.saveLastMove('D');
    newState.updateHeuristic();
    return newState;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos], newState.field[newState.zeroPos + 1]);
    ++newState.zeroPos;
    newState.saveLastMove('L');
    newState.updateHeuristic();
    return newState;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());
    GameState newState(*this);
    swap(newState.field[newState.zeroPos], newState.field[newState.zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    newState.saveLastMove('U');
    newState.updateHeuristic();
    return newState;
}

bool GameState::IsFinish() const {
    return field == FinishField;
}

struct StateHasher {
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        memcpy(&hash, &state.field[0], sizeof(hash));
        return hash;
    }
};

string Get15thSolution(const GameState &state) {
    std::set<GameState> stateSet;
    stateSet.insert(state);
    unordered_map<GameState, char, StateHasher> visited;
    visited[state] = 'S';
    bool hasSolution = false;
    while (!stateSet.empty()) {
        GameState tempState = *(stateSet.begin());
        stateSet.erase(tempState);
        if (tempState.IsFinish()) {
            hasSolution = true;
            break;
        }
        if (tempState.CanMoveLeft() && tempState.getLastMove() != 'L') {
            GameState newState = tempState.MoveLeft();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                stateSet.insert(newState);
            }
        }
        if (tempState.CanMoveUp() && tempState.getLastMove() != 'D') {
            GameState newState = tempState.MoveUp();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                newState.saveLastMove('D');
                stateSet.insert(newState);
            }
        }
        if (tempState.CanMoveRight() && tempState.getLastMove() != 'R') {
            GameState newState = tempState.MoveRight();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                newState.saveLastMove('L');
                stateSet.insert(newState);
            }
        }
        if (tempState.CanMoveDown() && tempState.getLastMove() != 'D') {
            GameState newState = tempState.MoveDown();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                newState.saveLastMove('U');
                stateSet.insert(newState);
            }
        }
    }

    if (!hasSolution)
        return string();

    string result;
    GameState tempState(FinishField);
    char move = visited[tempState];
    while (move != 'S') {
        result += move;
        switch (move) {
            case 'L':
                tempState = tempState.MoveRight();
                break;
            case 'U':
                tempState = tempState.MoveDown();
                break;
            case 'R':
                tempState = tempState.MoveLeft();
                break;
            case 'D':
                tempState = tempState.MoveUp();
                break;
        }
        move = visited[tempState];
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int run(std::istream &in, std::ostream &out) {
    array<char, FieldSize> data{};
    int tmp;
    for (int i = 0; i < FieldSize; ++i) {
        in >> tmp;
        data[i] = tmp;
    }
    GameState state(data);
    string solution = Get15thSolution(state);
    if (solution.empty()) {
        out << "-1" << std::endl;
        return -1;
    } else {
        out << state << std::endl;
    }
    for (char direction : solution) {
        switch (direction) {
            case 'L':
                state = state.MoveLeft();
                break;
            case 'U':
                state = state.MoveUp();
                break;
            case 'R':
                state = state.MoveRight();
                break;
            case 'D':
                state = state.MoveDown();
                break;
        }
        out << state << std::endl;
    }
    return 0;

}

void test_cases() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 5 0 1\n"
                 "6 9 7 2\n"
                 "14 11 8 4\n"
                 "15 10 13 12";
        auto res = run(input, output);
        assert(!res);
        assert(!output.str().empty());
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1 2 3 4\n"
                 "5 6 7 8\n"
                 "9 10 11 12\n"
                 "13 15 14 0";
        auto res = run(input, output);
        assert(res == -1);
//        assert(output.str() == "-1");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "0 1 2 3\n"
                 "5 6 7 4\n"
                 "9 10 11 8\n"
                 "13 14 15 12";
        auto res = run(input, output);
        assert(output.str() == "0 1 2 3 \n"
                               "5 6 7 4 \n"
                               "9 10 11 8 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 0 2 3 \n"
                               "5 6 7 4 \n"
                               "9 10 11 8 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 2 0 3 \n"
                               "5 6 7 4 \n"
                               "9 10 11 8 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 2 3 0 \n"
                               "5 6 7 4 \n"
                               "9 10 11 8 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 2 3 4 \n"
                               "5 6 7 0 \n"
                               "9 10 11 8 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 2 3 4 \n"
                               "5 6 7 8 \n"
                               "9 10 11 0 \n"
                               "13 14 15 12 \n"
                               "\n"
                               "1 2 3 4 \n"
                               "5 6 7 8 \n"
                               "9 10 11 12 \n"
                               "13 14 15 0 \n"
                               "\n");
        assert(!res);
    }

}

int main() {
    //run(std::cin, std::cout);
    test_cases();
    return 0;
}