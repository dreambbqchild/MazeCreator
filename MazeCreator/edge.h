//
//  Header.h
//  MazeCreator
//
//  Created by Paige Rudnick on 2/26/16.
//  Copyright © 2016 Paige Rudnick. All rights reserved.
//

#ifndef Edge_h
#define Edge_h

class Vertex;

class Edge {
private:
    uint32_t key;
    char passageWay;
    char type;
public:
    Vertex *A, *B;
    uint32_t GetKey() { return key; }
    void Include() { type = passageWay; }
    void Exclude() { type = ' '; }
    bool IsInMaze() { return type != '?'; }
    char ToChar() { return type; }
    
    
    Edge(char passageWay, uint32_t key) : A(nullptr), B(nullptr), passageWay(passageWay), key(key), type('?') {}
};

#endif /* Edge_h */
