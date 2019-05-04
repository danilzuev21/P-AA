 
#include <vector>
#include <iostream>
#include <string>

using namespace std;

string KMP(string T, string P, size_t begin, size_t end, vector<int> p)
{
    string result;
    int k = 0;
    for (int i = begin; i <= end; ++i)
    {
        while ((k > 0) && (P[k] != T[i]))
            k = p[k - 1];
        if (P[k] == T[i])
            ++k;
        if(k == P.length())
            result += to_string(i - P.length() + 1) + ',';
    }
    return result;
}

vector<int> preffix_for_P(string P)
{
    int len = P.length();
    vector<int> p(len);
    p[0] = 0;
    int k = 0;
    for (int i = 1; i < len; ++i)
    {
        while ((k > 0) && (P[k] != P[i]))
            k = p[k - 1];
        if (P[k] == P[i])
            ++k;
        p[i] = k;
    }
    return p;
}

string init_func(string T, string P, size_t k)
{
    string result;
    size_t s_length = T.length() / k;
    vector<int> p = preffix_for_P(P);
    for(int i = 0; i < k-1;i++)
        result+=KMP(T, P, i*s_length,(i+1)*s_length+P.length() - 2, p);
    result+=KMP(T,P, (k-1)*s_length, T.length()-1, p);
    return result;
}

int main()
{
    string P;
    string T;
    size_t k;
    cin>>P>>T>>k;
    string task1_answ = init_func(T, P, k);
    cout<< "Task 1:" << endl;
    if(task1_answ.empty())
        cout<<-1<<endl;
    else
    {
        task1_answ.pop_back();
        cout << task1_answ << endl;
    }
    cout<<"Task 2:" << endl;
    if(T.length() != P.length())
        cout<<-1<<endl;
    else
    {
        string task2_answ = init_func(T+T, P, k);
        if(task2_answ.empty())
            cout<<-1<<endl;
        else
            cout << task2_answ[0] << endl;
    }
    return 0;
}
