#include <iostream>
#include <string>

using namespace std;

int main()
{
    string line;
    while(getline(cin,line))
    {
        string t;
        for(auto ch:line) 
        {
            t.push_back(ch);
            if(ch=='\\') t.push_back('\\');
        }
        printf("puts(\"%s\");\n",t.c_str());
        
    }
}