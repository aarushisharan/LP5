#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<omp.h>

using namespace std;

class Graph
{
  public:

  int V;
  vector<vector<int>>adj; //adjacency list

  Graph(int V): V(V), adj(V){}

  void addEdge(int u, int v)
  {
    adj[u].push_back(v);
    adj[v].push_back(u); // for undirected graph
  }

  void ParallelBfs(int start);
  void ParallelDfs(int start);

};

void Graph::ParallelBfs(int start)
{
  vector<int> visited(V,0);
  queue<int>q;

  visited[start]=true;
  q.push(start);

  cout<<"Parallel BFS Starting from node: "<<start<<": "<<endl;

  while(!q.empty())
  {
    //process one level of BFS
    int size= q.size(); //Number of nodes in the current level (important for level-order processing).
    vector<int>nextlevel; // Stores the nodes discovered at this level to be processed in the next round.

    #pragma omp parallel for  //Parallel loop to process each node in the current level.                
    for(int i=0;i<size;i++)   //Each thread handles one node from the queue.
    {
      int node; 

      #pragma omp critical //critical to access (safely pop) from shared memory. Only one thread at a time
      {
        node=q.front();
        q.pop();
      }

      #pragma omp critical 
      {
        cout<<node<<" ";
      }

      #pragma omp parallel for
      for(int j=0;j<adj[node].size();j++)
      {
        int neighbor= adj[node][j];
        #pragma omp critical 
        {
          if (!visited[neighbor])
          {
            visited[neighbor]=true;
            nextlevel.push_back(neighbor);
          }
        }
      }

    }

    for (int n:nextlevel)
    {
      q.push(n); //Add next level nodes to the queue
    }
  }

  cout<<endl;


}

void ParallelDFSUtil(int node, vector<vector<int>>& adj, vector<int>& visited)
{
  #pragma omp critical
    cout<<node<<" ";
    visited[node]=true;

    #pragma omp parallel for
    for(int i=0;i<adj[node].size();i++)
    {
      int neighbor= adj[node][i];
      if(!visited[neighbor])
      {
        bool expected=false;
        if(__sync_bool_compare_and_swap(&visited[neighbor], expected, true)){
          ParallelDFSUtil(neighbor, adj, visited);
        }

      }
    }
  
}

void Graph:: ParallelDfs(int start)
{
  vector<int>visited(V,0);
  cout<<"Parallel DFS starting from node: "<<start<<" "<<endl;
  
  #pragma omp parallel
  {
    #pragma omp single nowait 
    {
      ParallelDFSUtil(start, adj, visited);
    }
  }
  cout<<endl;

}

int main() {
  int V, E;
  cout << "Enter the number of vertices: ";
  cin >> V;

  cout << "Enter the number of edges: ";
  cin >> E;

  Graph g(V);

  cout << "Enter " << E << " edges (format: u v):" << endl;
  for (int i = 0; i < E; ++i) {
      int u, v;
      cin >> u >> v;
      g.addEdge(u, v);
  }

  int start;
    cout << "Enter starting node for BFS and DFS: ";
    cin >> start;

    g.ParallelBfs(start);
    g.ParallelDfs(start);



  return 0;
}