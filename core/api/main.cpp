#include <iostream>
#include "../blockchain/blockchain.h"

void menu(){
    std::cout <<"----------------------"<<std::endl;
    std::cout <<"1 - check database content"<<std::endl;
    std::cout <<"2 - initialize blockchain (genesis block)"<<std::endl;
    std::cout <<"3 - add block"<<std::endl;
    std::cout <<"4 - view blockchain"<<std::endl;
    std::cout <<"5 - validate blockchain"<<std::endl;
    std::cout <<"6 - exit"<<std::endl;
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
            std::cout<< "Initializing blockchain... Creating genesis block "<<std::endl;
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
            std::cout<< "Displaying blockchain contents "<<std::endl;
            std::cout<< "DONE."<<std::endl;
            break;
        case 5:
            std::cout<< "validating the blockchain "<<std::endl;
            std::cout<< "DONE."<<std::endl;
            break;
        case 6:
            std::cout<< "EXITING, GOODBYE"<<std::endl;
            exit(0);
        
        default:
            std::cout <<"Enter legit choice" <<std::endl;
            break;
        }
    } while (choice != 6);
    
    return 0;
}
