//
//  vertex.h
//  MazeCreator
//
//  Created by Paige Rudnick on 2/26/16.
//  Copyright © 2016 Paige Rudnick. All rights reserved.
//

#ifndef vertex_h
#define vertex_h

class Vertex {
public:
    static const uint16_t EdgeCount = 4;
    
    char Label;
    Edge *Left, *Top, *Right, *Bottom;
    Vertex() : Left(nullptr), Top(nullptr), Right(nullptr), Bottom(nullptr), Label('\0') {}
    
    
    Edge* IndexOf(int i)
    {
        switch(i)
        {
            case 0: return Left;
            case 1: return Top;
            case 2: return Right;
            case 3: return Bottom;
        }
        return nullptr;
    }
};

#endif /* vertex_h */
