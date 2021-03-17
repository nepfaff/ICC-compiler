#pragma once

#include "ast.hpp"

/*
    C only supports limited constants so each one will get it's own node
*/
class AST_ConstInt
    : public AST
{
private:
    int value;
public:
    AST_ConstInt(int _value);

    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
};

class AST_ConstFloat
    : public AST
{
private:
    float value;
public:
    AST_ConstFloat(float _value);

    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
};

class AST_ConstDouble
    : public AST
{
private:
    double value;
public:
    AST_ConstDouble(double _value);

    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
};

class AST_ConstChar
    : public AST
{
private:
    char value;
public:
    AST_ConstChar(char _value);

    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
};

/*
   I made the change you suggested by adding an assignment AST node
   This node will be created if an identifier acting as a variable is
   detected in the parser

   I cahnged it to use a string as the name instead of an AST node
*/
class AST_Variable
    : public AST
{
private:
    std::string name;

public:
    AST_Variable(std::string* _name);

    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
    int getBytes() override;

    /*
        reg is the register that contains the new value.
        It should contian $.
        Example: If register is $v0, then reg = "$v0".
        - I made the change so it was compatible with the helper function for saving and reading variables
    */
    void updateVariable(std::ostream &assemblyOut, Frame* currentFrame, std::string reg) override;
};

class AST_Type
    : public AST
{
private:
    std::string name;
    int bytes;
public:
    AST_Type(std::string* name);
    static std::unordered_map<std::string, int> size_of_type;
    
    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    int getBytes() override;
    std::string getTypeName() override;

    // dont need destructor as it holds no pointers
};

class AST_ArrayType
    : public AST
{
private:
    AST* type;
    int size;
    int bytes;
public:
    AST_ArrayType(AST* _type, int _size);
    
    void generateFrames(Frame* _frame = nullptr) override;
    AST* deepCopy() override;
    void compile(std::ostream &assemblyOut) override;
    AST* getType() override;
    int getBytes() override;

    ~AST_ArrayType();
};
