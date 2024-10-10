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

        //chain.push_back(createGenesisBlock());
    }

    // ~Blockchain(){
    //     sqlite3_close(DB);
    // }

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

    Block getLastBlock() const {
        if (chain.empty()) {
            throw std::runtime_error("Blockchain is empty.");
        }
        return chain.back();
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

        // Begin transaction
        if (sqlite3_exec(DB, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Failed to begin transaction: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return;
        }

        const char* insertSql = "INSERT INTO blockchain (student_id, block_hash, prev_hash, certificate_hash, timestamp) VALUES (?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, insertSql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(DB) << std::endl;
            sqlite3_exec(DB, "ROLLBACK;", nullptr, nullptr, &errMsg);
            sqlite3_free(errMsg);
            return;
        }

        // Bind parameters
        sqlite3_bind_text(stmt, 1, block.getStudentId().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, block.getCurrHash().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, block.getPrevHash().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, block.getCertHash().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 5, block.getTimestamp());

        // Execute the statement
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to insert block: " << sqlite3_errmsg(DB) << std::endl;
            sqlite3_exec(DB, "ROLLBACK;", nullptr, nullptr, &errMsg);
            sqlite3_free(errMsg);
        } else {
            // Commit the transaction
            if (sqlite3_exec(DB, "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
                std::cerr << "Failed to commit transaction: " << errMsg << std::endl;
                sqlite3_free(errMsg);
            }
        }

        sqlite3_finalize(stmt);
    }

private:
    std::vector<Block> chain;
    bool isChainValid() const;
    sqlite3* DB;

    bool blockchainExists() const {
        const char* query = "SELECT COUNT(*) FROM blockchain WHERE student_id = 'GENESIS';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(DB, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
            return false;
        }

        bool exists = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = (sqlite3_column_int(stmt, 0) > 0);
        }

        sqlite3_finalize(stmt);
        return exists;
    }



};