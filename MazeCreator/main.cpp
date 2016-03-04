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

uint32_t mazeWidth = 40,
         mazeHeight = 10;


Vertex* startingVertex = nullptr;

struct VertexInfo {
    Vertex* vertex;
    uint32_t distance;
};

void PrintMaze(Vertex* topLeft)
{
    auto current = topLeft,
         next = current->Bottom ? current->Bottom->A : nullptr;
    
    while(current)
    {
        stringstream verticals;
        while(current->Right)
        {
            cout << current->Label << current->Right->ToChar();
            
            if(current->Bottom)
                verticals << current->Bottom->ToChar() << ' ';
            
            current = current->Right->A;
        }
        cout << current->Label << endl;
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
    for (auto i = 0; i < Vertex::EdgeCount; i++) {
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
    
    current->Label = 'S';
    startingVertex = current;
    
    auto edge = zeroX ? current->Bottom : current->Right;
    edge->Include();
    edge->A->Label = '*';
    
    AddRelevantWalls(queue, current);
    AddRelevantWalls(queue, edge->A);
}

void BuildMaze(Vertex* topLeft, wallQueue& queue)
{
    while(!queue.empty())
    {
        auto edge = queue.top();
        queue.pop();
        
        if(edge->A->Label && edge->B->Label)
        {
            edge->Exclude();
            continue;
        }
        
        auto vertex = edge->A->Label ? edge->B : edge->A;
        vertex->Label = '*';
        edge->Include();
        
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

VertexInfo FindFurthestVertex(Vertex* current, Vertex* last = nullptr, uint32_t distanceSoFar = 0)
{
    VertexInfo result = { current, distanceSoFar };
    for (auto i = 0; i < Vertex::EdgeCount; i++)
    {
        auto edge = current->IndexOf(i);
        if(edge && edge->ToChar() != ' ')
        {
            auto next = edge->A == current ? edge->B : edge->A;
            if(next != last)
            {
                auto info = FindFurthestVertex(next, current, distanceSoFar + 1);

                if(info.distance > result.distance)
                    result = info;
            }
        }
    }
    
    return result;
}

int main(int argc, const char * argv[]) {
    for(auto i = 0; i < argc; i++){
        if(!strcmp("/h", argv[i]))
            mazeHeight = atoi(argv[++i]);
        
        if(!strcmp("/w", argv[i]))
            mazeWidth = atoi(argv[++i]);
    }
    
    
    wallQueue queue(compareFunction);
    auto topLeft = AllocateMaze();
    
    PlaceEnterance(topLeft, queue);
    BuildMaze(topLeft, queue);
    
    auto furthest = FindFurthestVertex(startingVertex);
    furthest.vertex->Label = 'E';
    
    PrintMaze(topLeft);
    
    cout << furthest.distance << endl;
    
    return 0;
}
