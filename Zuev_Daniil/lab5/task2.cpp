#include <iostream>
#include <map>

#define SIZE 6

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
        case '$': return 5;
        default: return 5;
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
    if(symb == joker)
        current->transition[num_of_symb(symb)] = current->descendants[num_of_symb(joker)];
    if(current->transition[num_of_symb(symb)])
        return current->transition[num_of_symb(symb)];
    if(symb == joker)
        current->transition[num_of_symb(symb)] = current->descendants[num_of_symb(joker)];
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

void add_string_to_trie(string s, node* root, int num, char joker)
{
    node* current = root;
    for(int i = 0; i<s.length(); i++)
    {
        if(current->descendants[num_of_symb(s[i])] == nullptr)
        {
            current->descendants[num_of_symb(s[i])] = new node;
            current->descendants[num_of_symb(s[i])]->parent = current;
        }
        if(s[i] == joker)
            for(int j = 0; j < SIZE-1; j++)
                current->transition[j] = current->descendants[num_of_symb(s[i])];
        current = current->descendants[num_of_symb(s[i])];
        current->symb = s[i];
    }
    current->is_pat = 1;
    current->pat_num = num;
    current->pat_length = s.length();
}

void search(string s, node* root, map<int,bool>&  result, size_t length)
{
    string result_i;
    node* current = root;
    node* compressed_suff;
    for(int i = 0; i<s.length(); i++)
    {
        current = transition_func(current, s[i]);
        if(current->is_pat)
            result[i - length+2] = 1;
        compressed_suff = current;
        while(compressed_suff != root)
        {
            compressed_suff = compressed_func(compressed_suff);
            if(compressed_suff->is_pat)
                result[i - length+2] = 1;
        }
    }
}

int main()
{
    string T;
    string P;
    char joker;
    cin>>T;
    node* root;
    root = new node;
    root->symb = '#';
    root->suff_link = root;
    cin>>P;
    cin>>joker;
    add_string_to_trie(P, root, 0, joker);
    map<int, bool> result;
    search(T, root, result, P.length());
    for(map<int, bool>::iterator i = result.begin(); i != result.end(); i++)
            cout<< i->first<< endl;
    return 0;
}
