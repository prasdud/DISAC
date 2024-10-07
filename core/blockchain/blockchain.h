#pragma once
#include "block.h"
#include <vector>
#include <sqlite3.h>

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
        const char* insertSql = 
            "INSERT INTO blockchain (student_id, block_hash, prev_hash, certificate_hash, timestamp) "
            "VALUES ('GENESIS', '901131d838b17aac0f7885b81e03cbdc9f5157a00343d30ab22083685ed1416a', '0000000', '0000000', '1');";
            char* errorMessage;
            int rc = sqlite3_exec(DB, sql, nullptr, nullptr, &errorMessage);
            rc = sqlite3_exec(DB, insertSql, nullptr, nullptr, &errorMessage);

                if (rc != SQLITE_OK) {
                std::cerr << "Insert error: " << errorMessage << std::endl;
                sqlite3_free(errorMessage);
            } else {
                std::cout << "Data inserted successfully" << std::endl;
            }            
            
            return Block("GENESIS", "0000000");        
            //return Block("GENESIS", "901131d838b17aac0f7885b81e03cbdc9f5157a00343d30ab22083685ed1416a", "0000000", "0000000", "1");
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