//===================================================================
// Name        : part2.cpp
// Description : convert plaintext to rot13 cipher and caesar cipher 
//===================================================================



#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

//PROTOTYPES
string rot13(string input);    
string caesar(string input, string argument);

int missingCheck[26] = { }; //global array to be use by caesar function and storing given dictonary 

int main(int argc, char *argv[]) 
{
    if (argc < 2)    //terminate program if 2nd argument is missing or only program is provided 
    {
        cout << "MISSING COMMAND" << endl;
        return -1;
    }
    if (strcmp(argv[1], "-r") && strcmp(argv[1], "-g") && strcmp(argv[1], "-e") && strcmp(argv[1], "-d"))    //terminate program if valid argument is not provided 
    {
        cout << argv[1] << " NOT A VALID COMMAND" << endl;
        return -1;
    }
    
    if (strcmp(argv[1], "-r") == 0 && argc == 3)        //if they picked rot13 cipher and also provided input file 
    {    
        //cout << "you have provided file name " << argv[2] << endl;
        ifstream infile;
        string line;
        infile.open(argv[2]);
        if (infile.is_open() == false)        //unvalid file provided 
        {
            cout << argv[2] << " FILE COULD NOT BE OPENED "  << endl;
        }
        else
        {
            while (getline(infile, line))        //fetch line by line input
            {
                string temp;
                temp = rot13(line);
                cout << temp << endl;
            }
            infile.close();
        }
    }
    else if (!strcmp(argv[1], "-r"))        //if file name was not provided then get input from standard input 
    {
        string line, temp;
        while (getline(cin, line))
        {
            temp = rot13(line);
            cout << temp << endl;
        }
        
    }
    else if (!strcmp(argv[1], "-g"))
    {
        int pool[26];
        for (int i = 0; i < 26 ; i++)
        {
            pool[i] = i;
        }
        
        srand(time(NULL));
        int poolsize = 26;
        
        for (int i =  0; i < 26; i++)
        {
            int index = rand() % poolsize;
            
            while ((i + 'a') == (pool[index] + 'a'))        //shuffle until plaintext and cypertext is unique 
            {
                index = rand() % poolsize;
            }
            
            cout << char(i + 'a') << char(pool[index] + 'a') << " ";
            
            pool[index] = pool[poolsize - 1];    //take out already printed random letter 
            poolsize--;
            
        }
       
        cout << endl;
        return -1;
    }
    else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "-d"))
    {
        if (argc < 3) 
        {
            cout << "NO DICTIONARY GIVEN" << endl;
            return -1;
        }
        ifstream indict;
        indict.open(argv[2]);
        if (indict.is_open() == false)
        {
            cout << argv[2] << " DICTIONARY COULD NOT BE OPENED" << endl;
            return -1;
        }
        else 
        {
            string input;
            int duplicateCheck[26] = { };
            while (getline(indict,input))
            {
                istringstream iss(input);
                string word;
                while (iss >> word)        //READING DICTONARY PAIR BY PAIR AND CHECKING CORRECTNESS 
                {
                    if (word.size() != 2)
                    {
                        cout << "FORMATTING ERROR " << word << endl;
                        return -1;
                    }
                    else if (word[0] == word[1])
                    {
                        cout << "MAPPING ERROR " << word[0] << endl;
                        return -1;
                    }
                    int tmp = word[1];
                    if (duplicateCheck[tmp%97] == 1)
                    {
                        cout << "DUPLICATE CIPHERTEXT " << word[1] << endl;
                        return -1;
                    }
                    else 
                    {
                        duplicateCheck[tmp%97] = 1;
                    }
                    
                    missingCheck[int(word[0])%97] = int(word[1]);
                }
               
            }
            for (int i = 0; i < 26; i++)        //CHECKING IF THE DICTIONARY MISSING ANY PAIN LETTER 
            {
                if (missingCheck[i] == 0)
                {
                    cout << "MISSING LETTER " << char(i+97) << endl;
                    return -1;
                }
                else 
                {
                    continue;
                }
            }
        }
        indict.close();
        
        if ( argc == 4)        //READING INPUT FILE AND CHECKING IT'S CORRECTNESS 
        {
            ifstream infile;
            infile.open(argv[3]);
            if (infile.is_open() == false)
            {
                cout << argv[3] << " FILE COULD NOT BE OPENED" << endl;
                return -1;
            }
            else
            {
                string line, output;
                while (getline(infile, line))    //READ LINE BY LINE FROM INPUT FILE AND PASSING IT TO FUNCTION BASED ON GIVEN ARGUMENT 
                {
                    if ( !strcmp(argv[1], "-e"))
                    {
                        output = caesar(line, argv[1]);
                        cout << output << endl;
                    }
                    else if ( !strcmp(argv[1], "-d"))
                    {
                        output = caesar(line, argv[1]);
                        cout << output << endl;
                    }
                }
            }
        }
        else if (argc == 3)        //READING INPUT FROM STANDARD INPUT AND PASSING IT TO THE CAESAR FUNCTION FOR INCRIPTION
        {
            string line, output;
            while (getline(cin, line))
            {
                output = caesar(line, argv[1]);
                cout << output << endl;
            }
        }
        
    }
    else if (argc > 4)    //INDICATE TOO MANY ARGUMENT 
    {
        cout << "TOO MANY ARGUMENTS" << endl;
        return -1;
    }
    
    return 0;
}

//Rot13 function to convert plaintext to ciphertext using rot13 cipher 
string rot13(string input)
{
    string output;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            output += char(97 + (((int(input[i]) + 13) % 97) % 26));  
        }
        else if (input[i] >= 'A' && input[i] <= 'Z')
        {
            output += char(65 + (((int(input[i]) + 13) % 65) % 26));  
        }
        else
        {
            output += input[i];
        }
    }
    return output;
}

//CAESAR FUNCTION TAKES IN A STRING AND ARGUMENT AND RETUNRS CIPHER TEXT BASED ON PLAIN LETTER MAPPING PROVIDED BY THE USER 
string caesar(string input, string argument)
{
    string output;
    if (argument == "-e")        //ENCRYPT TEXT BASED ON PROVIDED DICTONARY 
    {
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] >= 'a' && input[i] <= 'z')
            {
                output += char(missingCheck[input[i]%97]);  
            }
            else if (input[i] >= 'A' && input[i] <= 'Z')
            {
                output += char(65 + (missingCheck[input[i]%65] % 97));   
            }
            else
            {
                output += input[i];
            }
        }
    }
    else        //DECRYPT TEXT BASED ON CIPHER TEXT MAPPING TO PLAIN TEXT
    {
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] >= 'a' && input[i] <= 'z')
            {
                for (int j = 0; j < 26; j++)
                {
                    if (char(missingCheck[j])== input[i])
                    {
                        output += char(97+j); 
                    }
                }
            }
            else if (input[i] >= 'A' && input[i] <= 'Z')
            {
                for (int j = 0; j < 26; j++)
                {
                    if (char(65+(missingCheck[j]%97))== input[i])
                    {
                        output += char(65+j); 
                    }
                }
            }
            else
            {
                output += input[i];
            }
        }
    }
    
    return output;
}