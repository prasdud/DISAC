#pragma once
#include "block.h"
#include <vector>
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>


class Blockchain{
public:
    Blockchain(){
        
        // Open the SQLite database connection
        int exit = sqlite3_open("../db/example.db", &DB);
        if (exit) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
            std::exit(-1);
        } else {
            std::cout << "Opened database successfully!" << std::endl;
        }

        chain.push_back(createGenesisBlock());
    }

    ~Blockchain(){
        sqlite3_close(DB);
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
    sqlite3* DB;
    Block createGenesisBlock() const{   //genesis block is the first block in the blockchain
            const char* createTableSql =
                "CREATE TABLE IF NOT EXISTS blockchain ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "student_id TEXT NOT NULL, "
                "block_hash TEXT NOT NULL, "
                "prev_hash TEXT NOT NULL, "
                "certificate_hash TEXT NOT NULL, "
                "timestamp INTEGER);";

            char* errorMessage;
            
            int rc = sqlite3_exec(DB, createTableSql, nullptr, nullptr, &errorMessage);
            
            if (rc != SQLITE_OK) {
                std::cerr << "Create table error: " << errorMessage << std::endl;
                sqlite3_free(errorMessage);
                exit(-1);
            } else {
                std::cout << "Table created successfully" << std::endl;
            }
            
            const char* insertSql = 
            "INSERT INTO blockchain (student_id, block_hash, prev_hash, certificate_hash, timestamp) "
            "VALUES ('GENESIS', 'GENESIS', '0000000', '0000000', '1');";
            rc = sqlite3_exec(DB, insertSql, nullptr, nullptr, &errorMessage);

            if (rc != SQLITE_OK) {
            std::cerr << "Insert error: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            } 
            else {
                std::cout << "Data inserted successfully" << std::endl;
            }            
            
            return Block("0000000");        
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