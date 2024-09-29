#pragma once
#include "block.h"
#include <vector>

class Blockchain{
public:
    Blockchain();


private:
    std::vector<Block> chain;
    bool isChainValid() const;
    Block createGenesisBlock() const;   //genesis block is the first block in the blockchain
    void addBlock(Block& newBlock){
        //std::string prev_hash = newBlock.getPrevHash();
        //std::string curr_hash = newBlock.getBlockHash();

        //previous hash of the new block = hash of last block
        newBlock.setPrevHash(chain.back().getCurrHash());

        //recalculate block hash now that prev hash has been set
        newBlock.updateCurrHash();
        chain.push_back(newBlock);
    }
};