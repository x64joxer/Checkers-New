#ifndef POSSIBLEMOVES_H
#define POSSIBLEMOVES_H

#include <list>
#include "Board.h"

class PossibleMoves
{
    public:
        PossibleMoves();
        bool CanIGrab(const unsigned short number, const Board & board);
        bool CheckHitTopLeft(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitTopRight(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomLeft(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomRight(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopLeft(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopRight(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomLeft(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomRight(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CanIPutHere(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const;
        bool IsKill(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const;

    private:

        bool OutOfBoard(const unsigned short x, const unsigned short y) const;

};

#endif // POSSIBLEMOVES_H
