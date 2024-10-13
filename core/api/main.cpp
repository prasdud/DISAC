#include <iostream>
#include "../blockchain/blockchain.h"

void menu(){
    std::cout <<"----------------------"<<std::endl;
    std::cout <<"1 - Check database content"<<std::endl;
    std::cout <<"2 - Verify DB connection"<<std::endl;
    std::cout <<"3 - Add block to blockchain"<<std::endl;
    std::cout <<"4 - Validate blockchain"<<std::endl;
    std::cout <<"5 - Quit"<<std::endl;
}

int main(int argc, char const *argv[])
{
    Blockchain blockchain; 
    int choice = 0;
    std::cout <<"Enter choice" <<std::endl;
    do
    {
        menu();
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            std::cout<< "Displaying database contents "<<std::endl;
            blockchain.displayDatabaseContents();
            std::cout<< "DONE."<<std::endl;
            break;
        case 2:
            std::cout<< "Connecting to DB... Verifying connection.."<<std::endl;
            blockchain = Blockchain(); 
            std::cout<< "DONE."<<std::endl;
            break;
        case 3:{
                std::cout<< "Adding block "<<std::endl;
                std::string prevHash = blockchain.getLastBlockHash();
                Block block(prevHash);
                blockchain.addNewBlock(block);
                std::cout<< "DONE."<<std::endl;
                break;
        }
        case 4:
            std::cout<< "Validating the blockchain "<<std::endl;
            std::cout<< "DONE."<<std::endl;
            break;
        case 5:
            std::cout<< "EXITING, GOODBYE"<<std::endl;
            exit(0);
        
        default:
            std::cout <<"Enter legit choice" <<std::endl;
            break;
        }
    } while (choice != 6);
    
    return 0;
}
