#ifndef IAPOSSIBLEMOVES_H
#define IAPOSSIBLEMOVES_H

#include "../Board/Board.h"
#include "../Board/PawnPos.h"
#include "../Traces/Traces.h"
#include "../ProgramVariables/ProgramVariables.h"

class IAPossibleMoves
{
    public:
        IAPossibleMoves();

        bool CanIGrabBlack(const unsigned short number, const Board & board) const;
        bool CheckHitTopLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitTopRightBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomRightBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopRightBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomRightBlack(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CanIPutHereBlack(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const;
        bool IsKillBlack(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const;
        bool OutOfBoardBlack(const unsigned short x, const unsigned short y) const;
        bool CanIGrabWhite(const unsigned short number, const Board & board) const;

        bool CheckHitTopLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitTopRightWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckHitBottomRightWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutTopRightWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const;
        bool CheckPutBottomRightWhite(const unsigned short x, const unsigned short y, const Board & board) const;

        bool CheckHitTopLeftWhite(const unsigned short number, const Board & board) const;
        bool CheckHitTopRightWhite(const unsigned short number, const Board & board) const;
        bool CheckHitBottomLeftWhite(const unsigned short number, const Board & board) const;
        bool CheckHitBottomRightWhite(const unsigned short number, const Board & board) const;
        bool CheckPutTopLeftWhite(const unsigned short number, const Board & board) const;
        bool CheckPutTopRightWhite(const unsigned short number, const Board & board) const;
        bool CheckPutBottomLeftWhite(const unsigned short number, const Board & board) const;
        bool CheckPutBottomRightWhite(const unsigned short number, const Board & board) const;

        void KillHitTopLeftWhite(const unsigned short number, Board &board);
        void KillHitTopRightWhite(const unsigned short number, Board &board);
        void KillHitBottomLeftWhite(const unsigned short number, Board &board);
        void KillHitBottomRightWhite(const unsigned short number, Board &board);

        bool CheckHitTopLeftBlack(const unsigned short number, const Board & board) const;
        bool CheckHitTopRightBlack(const unsigned short number, const Board & board) const;
        bool CheckHitBottomLeftBlack(const unsigned short number, const Board & board) const;
        bool CheckHitBottomRightBlack(const unsigned short number, const Board & board) const;
        bool CheckPutTopLeftBlack(const unsigned short number, const Board & board) const;
        bool CheckPutTopRightBlack(const unsigned short number, const Board & board) const;
        bool CheckPutBottomLeftBlack(const unsigned short number, const Board & board) const;
        bool CheckPutBottomRightBlack(const unsigned short number, const Board & board) const;

        void KillHitTopLeftBlack(const unsigned short number, Board & board);
        void KillHitTopRightBlack(const unsigned short number, Board & board);
        void KillHitBottomLeftBlack(const unsigned short number, Board & board);
        void KillHitBottomRightBlack(const unsigned short number, Board & board);

        bool CanIPutHereWhite(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const;
        bool IsKillWhite(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const;
        bool OutOfBoardWhite(const unsigned short x, const unsigned short y) const;
};


#endif // IAPOSSIBLEMOVES_H
