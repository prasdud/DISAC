#pragma once
#include "block.h"
#include <vector>

class Blockchain{
public:
    Blockchain(){
        chain.push_back(createGenesisBlock());
    }

    void addNewBlock(Block& new_block){
        addBlock(new_block);
    }

    Block getLastBlock() const {
        if (chain.empty()) {
            throw std::runtime_error("Blockchain is empty.");
        }
        return chain.back();
    }

private:
    std::vector<Block> chain;
    bool isChainValid() const;
    Block createGenesisBlock() const{   //genesis block is the first block in the blockchain
        return Block("Genesis", "000000");
    }
    
    void addBlock(Block& newBlock){
        //std::string prev_hash = newBlock.getPrevHash();
        //std::string curr_hash = newBlock.getBlockHash();
        if (!chain.empty())
        {
            //previous hash of the new block = hash of last block
            newBlock.setPrevHash(chain.back().getCurrHash());

        }
        
        //recalculate block hash now that prev hash has been set
        newBlock.updateCurrHash();
        chain.push_back(newBlock);
    }
};