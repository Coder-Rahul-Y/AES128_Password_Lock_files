#include <fstream>  
#include <iostream>  
#include "AES128.h"  
#include<string.h>
#include<limits>
typedef bitset<8> BYTE;  
typedef bitset<32> word;  
using namespace std;
/** 
 *  Converting an array of char characters into binary 
 *  Save it in a BYTE array 
 */  
void charToByte(BYTE out[16], const char s[16])  
{   
    for(int i=0; i<16; ++i)    
        for(int j=0; j<8; ++j)    
            out[i][j]= ((s[i]>>j) & 1);    
}  
  
/** 
 *  Divide consecutive 128 bits into 16 groups and store them in a BYTE array 
 */  
void divideToByte(BYTE out[16], bitset<128>& data)  
{  
    bitset<128> temp;  
    for(int i=0; i<16; ++i)  
    {  
        temp = (data << 8*i) >> 120;  
        out[i] = temp.to_ulong();  
    }  
}  
  
/** 
 *  Merge 16 BYTE s into 128 consecutive bits 
 */  
bitset<128> mergeByte(BYTE in[16])  
{  
    bitset<128> res;  
    res.reset();  //Set 0  
    bitset<128> temp;  
    for(int i=0; i<16; ++i)  
    {  
        temp = in[i].to_ulong();  
        temp <<= 8*(15-i);  
        res |= temp;  
    }  
    return res;  
}  
  

//rahul y made function:
void askUserIntention(string &keyStr, string &toEnc, string &toDec, char &intention){
    cout<<"Do you want to encrypt or decrypt? (E/D)\n";
    do {
        cin>>intention;
        intention = toupper(intention);

        //clear cin state and ignore everything in input buffer.
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        if (intention == 'E'){
            cout<<"enter the filename you want to encrypt : ";
            // cin>>toEnc;
            getline(cin, toEnc);
        }
        else if (intention == 'D'){
            cout<<"\nenter the filename you want to decrypt :";
            // cin>>toDec;
            getline(cin, toDec);
        }else {
            cout<<"Please enter either \"E\" or \"D\" only";
        }
    }while(intention != 'E' && intention != 'D');

    cout<<"Enter password : ";
    getline(cin, keyStr);  
    cout<<"\n using \""<<keyStr<<"\" as password.\n";
}


int main()  
{  
    //rahul y code below few lines
    string keyStr, toEnc, toDec;
    char intention=' ';
    askUserIntention(keyStr, toEnc, toDec, intention);
    
    BYTE key[16];   
    charToByte(key, keyStr.c_str());  
    //Key expansion  
    word w[4*(Nr+1)];  
    KeyExpansion(key, w);  
  
    bitset<128> data;  
    BYTE plain[16];  
    ifstream in;  
    ofstream out;  

    if (intention == 'E'){
        //Encrypt the file flower.jpg into cipher.txt  
        in.open(toEnc, ios::binary);
        out.open(toEnc+".RYaes128", ios::binary);
        while(in.read((char*)&data, sizeof(data)))  
        {  
            divideToByte(plain, data);  
            encrypt(plain, w);  
            data = mergeByte(plain);  
            out.write((char*)&data, sizeof(data));  
            data.reset();  //Set 0  
        }  
        in.close();  
        out.close(); 
    }
    else if (intention == 'D'){
        //Decrypt cipher.txt and write the picture flower1.jpg  
        in.open(toDec, ios::binary);  
        out.open("DEC"+toDec.substr(0, toDec.find_last_of('.')), ios::binary);  
        while(in.read((char*)&data, sizeof(data)))  
        {  
            divideToByte(plain, data);  
            decrypt(plain, w);  
            data = mergeByte(plain);  
            out.write((char*)&data, sizeof(data));  
            data.reset();  //Set 0  
        }  
        in.close();  
        out.close();  
    }

    cout<<"\n\nDONE!\n\n";
  
    return 0;  
}  