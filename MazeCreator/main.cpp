#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <random>

#include "edge.h"
#include "vertex.h"

using namespace std;

random_device randomDev;

auto compareFunction = [](Edge* left, Edge* right)
{
    return left->GetKey() < right->GetKey();
};

typedef priority_queue<Edge*, vector<Edge*>, decltype(compareFunction)> wallQueue;

const uint32_t mazeWidth = 10,
               mazeHeight = 10;


void PrintMaze(Vertex* topLeft)
{
    auto current = topLeft,
         next = current->Bottom ? current->Bottom->A : nullptr;
    
    while(current)
    {
        stringstream verticals;
        while(current->Right)
        {
            cout << '*' << current->Right->ToChar();
            
            if(current->Bottom)
                verticals << current->Bottom->ToChar() << ' ';
            
            current = current->Right->A;
        }
        cout << '*' << endl;
        if(current->Bottom)
            cout << verticals.str() << current->Bottom->ToChar() << endl;
        
        current = next;
        if(current)
            next = current->Bottom ? current->Bottom->A : nullptr;
    }
    cout << endl;
}

void AddRelevantWalls(wallQueue& queue, Vertex* vertex)
{
    for (auto i = 0; i < Vertex::Length; i++) {
        auto edge = vertex->IndexOf(i);
        if(edge && !edge->IsInMaze())
            queue.push(edge);
    }
}

void PlaceEnterance(Vertex* topLeft, wallQueue& queue)
{
    bool zeroX = randomDev() % 2 == 0;
    
    auto x = zeroX ? 0 : randomDev() % (mazeWidth - 1),
    y = zeroX ? randomDev() % (mazeHeight - 1) : 0;

    auto current = topLeft;
    for(auto i = 0; i < x; i++)
        current = current->Right->A;

    for(auto i = 0; i < y; i++)
        current = current->Bottom->A;
    
    current->Visited = true;
    
    auto edge = zeroX ? current->Bottom : current->Right;
    edge->Include();
    edge->A->Visited = true;
    
    AddRelevantWalls(queue, current);
    AddRelevantWalls(queue, edge->A); PrintMaze(topLeft);
}

void BuildMaze(Vertex* topLeft, wallQueue& queue)
{
    while(!queue.empty())
    {
        auto edge = queue.top();
        queue.pop();
        
        if(edge->A->Visited && edge->B->Visited)
        {
            edge->Exclude(); PrintMaze(topLeft);
            continue;
        }
        
        auto vertex = edge->A->Visited ? edge->B : edge->A;
        vertex->Visited = true;
        edge->Include(); PrintMaze(topLeft);
        
        AddRelevantWalls(queue, vertex);
    }
}

Vertex* AllocateMaze()
{
    Vertex *topLeft = new Vertex(),
           *currentRowFirst = topLeft,
           *currentRowCursor = topLeft,
           *previousRowCursor = nullptr;

    for(auto y = 0; y < mazeHeight; y++)
    {
        if(y)
        {
            currentRowFirst = new Vertex();
            currentRowCursor = currentRowFirst;
        }
        
        for(auto x = 0; x < mazeWidth; x++)
        {
            auto next = currentRowCursor;
            if(x < mazeWidth - 1)
            {
                auto leftToRight = new Edge('-', randomDev());
                next = new Vertex();
                
                currentRowCursor->Right = leftToRight;
                next->Left = leftToRight;
                leftToRight->A = next;
                leftToRight->B = currentRowCursor;
            }
            
            if(previousRowCursor)
            {
                auto topToBottom = new Edge('|', randomDev());
                currentRowCursor->Top = topToBottom;
                previousRowCursor->Bottom = topToBottom;
                topToBottom->A = currentRowCursor;
                topToBottom->B = previousRowCursor;
                
                if(previousRowCursor->Right)
                    previousRowCursor = previousRowCursor->Right->A;
            }
            
            currentRowCursor = next;
        }
        
        previousRowCursor = currentRowFirst;
    }
    
    return topLeft;
}


int main(int argc, const char * argv[]) {
    wallQueue queue(compareFunction);
    auto topLeft = AllocateMaze();
    
    PlaceEnterance(topLeft, queue);
    BuildMaze(topLeft, queue);
    PrintMaze(topLeft);
    
    return 0;
}
