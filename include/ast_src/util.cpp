#include "util.hpp"

std::string generateUniqueLabel(const std::string &labelName) {
    static int uniqueLabelCount = 0;
    return labelName + std::to_string(uniqueLabelCount++);
}

void regToVar(std::ostream &assemblyOut, Frame* frame, const std::string& reg, const std::string& var, const std::string& reg_2){
    std::pair<int, int> varAddress = frame->getVarAddress(var);
    std::string varType = frame->getVarType(var)->getTypeName();

    // check if global variable => cannot be reached using stack
    if (varAddress.first == -1 && varAddress.second == -1) {
        if (varType == "float") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "s.s " << reg << ", 0($t6)" << std::endl;
        } else if (varType == "double") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "s.d " << reg << ", 0($t6)" << std::endl;
        } else if (varType == "char") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "sb " << reg << ", 0($t6)" << std::endl;
        } else {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "sw " << reg << ", 0($t6)" << std::endl;
        }
        return;
    }
    
    // coppy frame pointer to t6 and recurse back expected number of frames
    assemblyOut << "move $t6, $fp" << std::endl;
    for(int i = 0; i < varAddress.first; i++){
        assemblyOut << "lw $t6, 12($t6)" << std::endl;
    }
    
    // store register data into variable's memory address
    if (varType == "float") {
        if(reg[1] == 'f'){
            assemblyOut << "s.s " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
        }
        else{
            assemblyOut << "sw " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
        }
    } else if (varType == "double") {
        if(reg[1] == 'f'){
            assemblyOut << "s.d " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
        }
        else{
            assemblyOut << "sw " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
            assemblyOut << "sw " << reg_2 << ", -" << varAddress.second - 4 << "($t6)" << std::endl;
        }
    } else if (varType == "char"){
        assemblyOut << "sb " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    } else {
        assemblyOut << "sw " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    }
}

void varToReg(std::ostream &assemblyOut, Frame* frame, const std::string& reg, const std::string& var){
    std::pair<int, int> varAddress = frame->getVarAddress(var);
    std::string varType = frame->getVarType(var)->getTypeName();

    // check if global variable => cannot be reached using stack
    if (varAddress.first == -1 && varAddress.second == -1) {
        if (varType == "float") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "l.s " << reg << ", 0($t6)" << std::endl;
        } else if (varType == "double") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "l.d " << reg << ", 0($t6)" << std::endl;
        } else if (varType == "char") {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "lb " << reg << ", 0($t6)" << std::endl;
        } else {
            assemblyOut << "la $t6, " << var << std::endl;
            assemblyOut << "lw " << reg << ", 0($t6)" << std::endl;
        }
        return;
    }
    
    // coppy frame pointer to t6 and recurse back expected number of frames
    assemblyOut << "move $t6, $fp" << std::endl;
    for(int i = 0; i < varAddress.first; i++){
        assemblyOut << "lw $t6, 12($t6)" << std::endl;
    }
    
    // load from memory into register
    if (varType == "float") {
        assemblyOut << "l.s " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    } else if (varType == "double") {
        assemblyOut << "l.d " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    } else if (varType == "char") {
        assemblyOut << "lb " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    } else {
        assemblyOut << "lw " << reg << ", -" << varAddress.second << "($t6)" << std::endl;
    }
}

void varAddressToReg(std::ostream &assemblyOut, Frame* frame, const std::string& reg, const std::string& var){
    std::pair<int, int> varAddress = frame->getVarAddress(var);

    // check if global variable => cannot be reached using stack
    if (varAddress.first == -1 && varAddress.second == -1) {
        assemblyOut << "la " << reg << ", " << var << std::endl;
        return;
    }
    
    // coppy frame pointer to t6 and recurse back expected number of frames
    assemblyOut << "move $t6, $fp" << std::endl;
    for(int i = 0; i < varAddress.first; i++){
        assemblyOut << "lw $t6, 12($t6)" << std::endl;
    }
    
    // store variable address into register
    assemblyOut << "addiu " << reg << ", $t6, -" << varAddress.second << std::endl;
}

void valueToVarLabel(std::ostream &assemblyOut, std::string varLabel, char value) {
    assemblyOut << ".data" << std::endl;
    assemblyOut << ".align 2" << std::endl;
    assemblyOut << ".type " << varLabel << ", @object" << std::endl;
    assemblyOut << ".size " << varLabel << ", 1" << std::endl;

    assemblyOut << varLabel << ":" << std::endl;
    assemblyOut << ".byte " << (int)value << std::endl;
    assemblyOut << ".text " << std::endl;
}

void valueToVarLabel(std::ostream &assemblyOut, std::string varLabel, int value) {
    assemblyOut << ".data" << std::endl;
    assemblyOut << ".align 2" << std::endl;
    assemblyOut << ".type " << varLabel << ", @object" << std::endl;
    assemblyOut << ".size " << varLabel << ", 4" << std::endl;

    assemblyOut << varLabel << ":" << std::endl;
    assemblyOut << ".word " << value << std::endl;
    assemblyOut << ".text " << std::endl;
}

void valueToVarLabel(std::ostream &assemblyOut, std::string varLabel, float value) {
    assemblyOut << ".data" << std::endl;
    assemblyOut << ".align 2" << std::endl;
    assemblyOut << ".type " << varLabel << ", @object" << std::endl;
    assemblyOut << ".size " << varLabel << ", 4" << std::endl;

    assemblyOut << varLabel << ":" << std::endl;
    ieee754Float.fnum = value;
    assemblyOut << ".word " << ieee754Float.num << std::endl;
    assemblyOut << ".text " << std::endl;
}

void valueToVarLabel(std::ostream &assemblyOut, std::string varLabel, double value) {
    assemblyOut << ".data" << std::endl;
    assemblyOut << ".align 2" << std::endl;
    assemblyOut << ".type " << varLabel << ", @object" << std::endl;
    assemblyOut << ".size " << varLabel << ", 8" << std::endl;

    assemblyOut << varLabel << ":" << std::endl;
    ieee754Double.dnum = value;
    assemblyOut << ".word " << (ieee754Double.num >> 32) << std::endl;
    assemblyOut << ".word " << (ieee754Double.num & 0xFFFFFFFF) << std::endl;
    assemblyOut << ".text " << std::endl;
}

bool hasEnding(const std::string &fullString, const std::string &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}
