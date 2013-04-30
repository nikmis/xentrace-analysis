
#ifndef _pipes_AST_
#define _pipes_AST__

typedef void (HandlerType)(void);

class ExprAST {
    public:
        virtual ~ExprAST() {}
};


class OperatorExprAST : public ExprAST {
    HandlerType pHandler;
    public:
        OperatorExprAST(HandlerType handler) : pHandler(handler) {}
};

class PipeExprAST : public ExprAST {
    HandlerType pHandler;
    public:
        OperatorExprAST(HandlerType handler) : pHandler(handler) {}
};


#endif

