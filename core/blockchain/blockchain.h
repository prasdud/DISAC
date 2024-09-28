#pragma once
#include "block.h"
#include <vector>

class Blockchain{
public:
    Blockchain();
    void addBlock(const Block& newBlock);
    bool isChainValid() const;

private:
    std::vector<Block> chain;
    Block createGenesisBlock() const;   //genesis block is the first block in the blockchain
};