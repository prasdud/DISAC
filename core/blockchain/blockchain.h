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
        int exit = sqlite3_open("/home/prasdud/playground/c++/disac/core/blockchain/example.db", &DB);
        if (exit) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
            std::exit(-1);
        } else {
            std::cout << "Opened database successfully!" << std::endl;
        }
    }

    /**Add a destructor here maybe?
     * 
     * 
     * 
     */
    
    // getters



    // setters

    void addNewBlock(Block& new_block) {

        // Set previous hash based on the last block in the database
        if (new_block.getStudentId().empty() || new_block.getStudentName().empty() || 
            new_block.getCertHash().empty() || new_block.getPrevHash().empty()) {
            std::cerr << "Error: Block is missing required information." << std::endl;
            return; // Early exit if validation fails
        }
        std::string prevHash = getLastBlockHash();
        new_block.setPrevHash(prevHash);
        
        // Update the current hash
        new_block.updateCurrHash();

        // Add block to the database
        addBlockToDatabase(new_block);
    }

    void displayDatabaseContents() const {
        const char* query = "SELECT * FROM blockchain;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare select statement: " << sqlite3_errmsg(DB) << std::endl;
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string student_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // Correct index for student_id
            std::string block_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));   // Correct index for block_hash
            std::string prev_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));    // Correct index for prev_hash
            std::string cert_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));    // Correct index for cert_hash

            std::cout << "\nHash: " << block_hash << ", Prev Hash: " << prev_hash 
                    << ", Student ID: " << student_id 
                    << ", Certificate Hash: " << cert_hash << std::endl << std::endl;
        }

        sqlite3_finalize(stmt);
    }

    std::string getLastBlockHash() const {
        const char* query = "SELECT block_hash FROM blockchain ORDER BY id DESC LIMIT 1;";
        sqlite3_stmt* stmt;
        std::string lastHash = "0000000"; // Default for genesis block

        if (sqlite3_prepare_v2(DB, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
            return lastHash; // Return default hash
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            lastHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }

        sqlite3_finalize(stmt);
        return lastHash;
    }

    void addBlockToDatabase(const Block& block) {
        char* errMsg;
        
        // Simplified SQL Insert Statement (no transactions, no prepared statements)
        std::string insertSql = "INSERT INTO blockchain (student_id, block_hash, prev_hash, certificate_hash, timestamp) VALUES ('" 
                                + block.getStudentId() + "', '" 
                                + block.getCurrHash() + "', '" 
                                + block.getPrevHash() + "', '" 
                                + block.getCertHash() + "', " 
                                + std::to_string(block.getTimestamp()) + ");";

        // Execute the SQL statement directly
        if (sqlite3_exec(DB, insertSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Block inserted successfully!" << std::endl;
        }
    }

    bool validateBlockchain(){
        
    }

private:
    std::vector<Block> chain;
    bool isChainValid() const;
    sqlite3* DB;

};