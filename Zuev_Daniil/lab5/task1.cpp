#include <iostream>
#include <map>


#define SIZE 5

using namespace std;


int num_of_symb(char symb)
{
    switch(symb)
    {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        case 'N': return 4;
        default: return -1;
    }
}


struct node
{
    node* descendants[SIZE];
    node* transition[SIZE];
    node* parent;
    node* suff_link;
    node* compressed_suff_link;
    bool is_pat;
    int pat_num;
    int pat_length;
    char symb;
    node():parent(nullptr),suff_link(nullptr),compressed_suff_link(nullptr),is_pat(0),pat_num(-1)
    {
        for(int i = 0; i < SIZE; i++)
        {
            descendants[i] = nullptr;
            transition[i] = nullptr;
        }
    }
};

node* transition_func(node*,char);

node* suff_func(node* current)
{
    if(current->suff_link)
        return current->suff_link;
    if(current->parent == nullptr)
         current->suff_link  = current;
    else if(current->parent->parent == nullptr)
         current->suff_link = current->parent;
    else
        current->suff_link = transition_func(suff_func(current->parent), current->symb);
    return current->suff_link;
}

node* transition_func(node* current, char symb)
{
    if(current->transition[num_of_symb(symb)])
        return current->transition[num_of_symb(symb)];
    if(current->descendants[num_of_symb(symb)])
        current->transition[num_of_symb(symb)] = current->descendants[num_of_symb(symb)];
    else if(current->parent == nullptr)
        current->transition[num_of_symb(symb)] = current;
    else
        current->transition[num_of_symb(symb)] = transition_func(suff_func(current), symb);
    return current->transition[num_of_symb(symb)];
}

node* compressed_func(node* current)
{
    if(current->compressed_suff_link)
        return current->compressed_suff_link;
    if(suff_func(current)->is_pat || suff_func(current)->parent == nullptr)
        current->compressed_suff_link = suff_func(current);
    else
        current->compressed_suff_link = compressed_func(suff_func(current));
    return current->compressed_suff_link;
}

void add_string_to_trie(const string& s, node* root, int num)
{
    node* current = root;
    for(int i = 0; i<s.length(); i++)
    {
        if(current->descendants[num_of_symb(s[i])] == nullptr)
        {
            current->descendants[num_of_symb(s[i])] = new node;
            current->descendants[num_of_symb(s[i])]->parent = current;
        }
        current = current->descendants[num_of_symb(s[i])];
        current->symb = s[i];
    }
    current->is_pat = 1;
    current->pat_num = num;
    current->pat_length = s.length();
}

void search(string s, node* root, map<int,map<int,bool>>&  result)
{
    node* current = root;
    node* compressed_suff;
    for(int i = 0; i<s.length(); i++)
    {
        current = transition_func(current, s[i]);
        if(current->is_pat)
            result[i - current->pat_length+2][current->pat_num+1] = 1;
        compressed_suff = current;
        while(compressed_suff != root)
        {
            compressed_suff = compressed_func(compressed_suff);
            if(compressed_suff->is_pat)
                result[i - compressed_suff->pat_length+2][compressed_suff->pat_num+1] = 1;
        }
    }
}

int main()
{
    string T;
    int n;
    cin>>T>>n;
    string P[3000];
    node* root;
    root = new node;
    root->symb = '#';
    root->suff_link = root;
    for(int i = 0; i<n;i++)
    {
        cin>>P[i];
        add_string_to_trie(P[i], root, i);
    }
    map<int, map<int, bool>> result;
    search(T, root, result);
    for(map<int, map<int,bool>>::iterator i = result.begin(); i != result.end(); i++)
        for(map<int,bool>::iterator j = i->second.begin(); j != i->second.end(); j++)
            cout<< i->first<< ' ' << j->first << endl;
    return 0;
}
