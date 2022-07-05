#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <list>

// add edge to adjacency matrix
void add_edge(std::vector<int> adj[], int src, int dest)
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}

// generic BFS
bool BFS(std::vector<int> adj[], int src, int dest, int verts, int pred[], int dist[])
{
    std::list<int> queue;

    bool visited[4030];
 
    for (int i = 0; i < verts; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        pred[i] = -1;
    }
 
    visited[src] = true;
    dist[src] = 0;
    queue.push_back(src);
 
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++) {
            if (visited[adj[u][i]] == false) {
                visited[adj[u][i]] = true;
                dist[adj[u][i]] = dist[u] + 1;
                pred[adj[u][i]] = u;
                queue.push_back(adj[u][i]);
 

                if (adj[u][i] == dest)
                    return true;
            }
        }
    }
    return false;
}


void printShortestDistance(std::vector<int> adj[], int src, int dest, int verts, std::map<int, std::string> words4Reverse)
{
    int pred[4030], dist[4030];
 
    if (BFS(adj, src, dest, verts, pred, dist) == false) {
        std::cout << "No path between points";
        return;
    }
 
    std::vector<int> path;
    int crawl = dest;
    path.push_back(crawl);
    while (pred[crawl] != -1) {
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }
 
    std::cout << "Shortest path length is " << dist[dest];
 
    std::cout << "\nPath is: ";
    for (int i = path.size() - 1; i >= 0; i--) {
        std::cout << words4Reverse.find(path[i]-1)->second;
        if (i != 0) {
            std::cout  << " -> ";
        }
    }

}

void printVec(std::vector<std::string>& vector) {
    if (vector.size() > 0) {
        for (int i = 0; i<vector.size(); i++) {
            std::cout << vector.at(i) << " ";
        }
        std::cout <<"\n";
    }
}

// find all words in words4 that are within a letter of input word start
std::vector<std::string> findSimilar(std::string start, std::map<std::string, int>& words4, std::set<std::string>& searched) {
    std::vector<std::string> similars;

    for (int i = 0; i<4; i++) {
        for (int j = 0; j<26; j++) {
            std::string find = start;

            if (i == 0) {find = std::string() + char(j+97) + start[1] + start[2] + start[3];}
            if (i == 1) {find = std::string() + start[0] + char(j+97) + start[2] + start[3];}
            if (i == 2) {find = std::string() + start[0] + start[1] + char(j+97) + start[3];}
            if (i == 3) {find = std::string() + start[0] + start[1] + start[2] + char(j+97);}

            if (words4.count(find) && searched.find(start) == searched.end() && find != start) {
                similars.push_back(find);
            }
        }   
    }
    searched.insert(start);
    return similars;
}

// recursively generate points by going through all "connected" words using the findSimilar function 
// 
int buildGraph(std::vector<int> adj[], std::string start, std::map<std::string, int>& words4, std::set<std::string>& searched) {
    std::vector<std::string> similar = findSimilar(start, words4, searched);

    if (similar.size() == 0) {return -1;}

    // remove duplicates from vector
    std::set<std::string> s (similar.begin(), similar.end());
    similar.assign(s.begin(), s.end());
    // printVec(similar);

    for (int i = 0; i<similar.size(); i++) {
        add_edge(adj, words4.find(start)->second+1, words4.find(similar.at(i))->second+1);
        // std::cout << "{" << words4.find(start)->second+1 << " " << start<< ", " << words4.find(similar.at(i))->second+1 << " " << similar.at(i) << "} ";
        buildGraph(adj, similar.at(i), words4, searched);
    }
    return 0;
}


int main() {
    std::map<std::string, int> words4;
     // can get rid of if i can figure out how to reverse search a map (or use better data structure)
    std::map<int, std::string> words4Reverse;
    std::set<std::string> searched;

    std::vector<int> adj[4030];

    std::string start;
    std::string end;

    std::cout << "Starting word: ";
    std::cin >> start;
    std::cout << "Ending word: ";
    std::cin >> end;
    std::cout << "Going from " << start << " to " << end << "\n";

    std::ifstream words;
    words.open("words4.txt");

    std::string line;
    words >> line;

    int index = 0;
    // read words from words.txt into file
    while (!words.eof()) {
        words4.insert(std::pair<std::string, int>(line, index));
        words4Reverse.insert(std::pair<int, std::string>(index, line));
        words >> line;
        index++;
    }

    buildGraph(adj, start, words4, searched);
    printShortestDistance(adj, words4.find(start)->second+1, words4.find(end)->second+1, 4030, words4Reverse);
    return 0;
}