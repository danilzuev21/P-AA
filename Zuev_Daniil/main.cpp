 
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct Vertex
{
    char name;
    bool is_visited;
    map<char, Vertex*> descedants;
    map<char, int> lengthes;
    map<char, int> mod_lengthes;
    map<char, bool> is_entered;
    map<char, bool> is_back;
    Vertex(char c):is_visited(0),name(c){}
};

class Graph
{
private:
    char begin;
    char end;
    int min_b;
    map<char, Vertex*> vertexes;
    string current_path;
public:
    Graph():min_b(10000){}
    void readCons()
    {
        char f, s;
        int n, length;
        cin >> n >> begin >> end;
        for(int i = 0; i<n; i++)
        {
            cin >> f;
            cin >> s;
            cin >> length;
            if(!vertexes[f])
                vertexes[f] = new Vertex(f);
            if(!vertexes[s])
                vertexes[s] = new Vertex(s);
            vertexes[f]->descedants[s] = vertexes[s];
            vertexes[f]->lengthes[s] = length;
            vertexes[f]->mod_lengthes[s] = 0;
            vertexes[f]->is_entered[s] = 1;
            vertexes[f]->is_back[s] = 0;
            if(vertexes[s]->descedants[f] == nullptr)
            {
                vertexes[s]->descedants[f] = vertexes[f];
                vertexes[s]->lengthes[f] = length;
                vertexes[s]->mod_lengthes[f] = 0;
                vertexes[s]->is_entered[f] = 0;
                vertexes[s]->is_back[f] = 1;
            }
        }
    }
    void path_saturation(Vertex* current)
    {
        for(map<char, Vertex*>::iterator it = current->descedants.begin(); it!= current->descedants.end(); ++it)
        {
            if(it->second->is_visited)
                continue;
            current_path+=it->first;
            if(it->first == end)
                FFA();
            else
                if(!(it->second->is_visited) && !(current->is_back[it->first]))
                {
                    it->second->is_visited = 1;
                    path_saturation(it->second);
                }
            it->second->is_visited = 0;
            current_path.pop_back();
        }
        int min_b_tmp = min_b;
        for(map<char, Vertex*>::iterator it = current->descedants.begin(); it!= current->descedants.end(); ++it)
        {
            if(it->second->is_visited)
                continue;
            current_path+=it->first;
            if(it->first == end)
                FFA();
            else
                if(!(it->second->is_visited))
                {
                    if(current->is_back[it->first] && vertexes[it->first]->mod_lengthes[current->name] < min_b)
                        min_b = vertexes[it->first]->mod_lengthes[current->name];
                    it->second->is_visited = 1;
                    path_saturation(it->second);
                }
            min_b = min_b_tmp;
            it->second->is_visited = 0;
            current_path.pop_back();
        }
    }

    void FFA()
    {
        int length;
        int min_length = 10000;
        int max_length = 10000;
        for(int i = 0; i < current_path.length() -1; i++)
        {
            length = vertexes[current_path[i]]->lengthes[current_path[i+1]];
            if(length < min_length)
                min_length = length;
            if(length - vertexes[current_path[i]]->mod_lengthes[current_path[i+1]] < max_length)
                max_length = length - vertexes[current_path[i]]->mod_lengthes[current_path[i+1]];
            if(max_length == 0)
                return;
        }
        if(min_length > max_length)
            min_length = max_length;
        if(min_length > min_b)
            min_length = min_b;
        for(int i = 0; i < current_path.length() -1; i++)
            vertexes[current_path[i]]->mod_lengthes[current_path[i+1]] += min_length;
    }
    void print()
    {
        int sum = 0;
        for(map<char, int>::iterator it = vertexes[begin]->mod_lengthes.begin(); it!=vertexes[begin]->mod_lengthes.end(); ++it)
            sum+=it->second;
        cout<<sum<<endl;
        for(map<char, Vertex*>::iterator it = vertexes.begin(); it!=vertexes.end(); ++it)
        {
            for(map<char, Vertex*>::iterator it2 = it->second->descedants.begin();it2 != it->second->descedants.end(); ++it2)
                if(vertexes[it->first]->is_entered[it2->first])
                {
                    cout<<it->first<<' '<<it2->first<<' '<<((it->second->mod_lengthes[it2->first] - it2->second->mod_lengthes[it->first])<0 ? 0 : (it->second->mod_lengthes[it2->first] - it2->second->mod_lengthes[it->first]))<<endl;
                }
        }
    }
    void start()
    {
        current_path+=begin;
        vertexes[begin]->is_visited = 1;
        path_saturation(vertexes[begin]);
        print();
    }
};

int main()
{
    Graph p;
    p.readCons();
    p.start();
}
