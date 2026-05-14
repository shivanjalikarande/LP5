#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;


class Graph
{
    public:
        int vertices;
        vector<vector<int>>adjMatrix;
        vector<bool>visited;

        Graph(int v)
        {
            vertices = v;
            adjMatrix.resize(v);
            visited.resize(v, false);
        }

        void addEdge(int u, int v)
        {
            adjMatrix[u].push_back(v);
            adjMatrix[v].push_back(u);
        }

        void resetVisited()
        {
            fill(visited.begin(), visited.end(), false);
        }

        // BFS

        //a. Sequential BFS
        void serialBFS(int start)
        {
            queue<int>qu;
            qu.push(start);
            visited[start]= true;

            while(!qu.empty())
            {
                int node = qu.front();
                qu.pop();

                //cout<<node<<" ";
                
                for(auto adjNode : adjMatrix[node])
                {
                    if(!visited[adjNode])
                    {
                        visited[adjNode] = true;
                        qu.push(adjNode);
                    }
                }
            }
        }

        //b. Prallel BFS
        void parallelBFS(int start)
        {
            queue<int>qu;
            qu.push(start);
            visited[start] = true;

            while(!qu.empty())
            {
                int size = qu.size();

                vector<int>currNodes;
                for(int i=0;i<size;i++)
                {
                    int node = qu.front();
                    //cout<<node<<" ";
                    currNodes.push_back(node);
                    qu.pop();
                }

                #pragma omp parallel for
                for(int i=0;i<currNodes.size();i++)
                {
                    int node = currNodes[i];

                    for(auto adjNode : adjMatrix[node])
                    {
                        #pragma omp critical
                        {
                            if(!visited[adjNode])
                            {
                                visited[adjNode] = true;
                                qu.push(adjNode);
                            }
                        }
                    }
                }
            }
        } 

        //DFS

        //a. Sequential DFS
        void serialDFS(int start)
        {
            stack<int>st;
            st.push(start);
            visited[start] = true;

            while(!st.empty())
            {
                int node = st.top();
                st.pop();
                //cout<<node<<" ";

                for(auto adjNode : adjMatrix[node])
                {
                    if(!visited[adjNode])
                    {
                        visited[adjNode] = true;
                        st.push(adjNode);
                    }
                }
            }
        }

        //b. Parallel DFS

        void parallelDFSUtil(int node)
        {
            if(visited[node]) return;

            visited[node] = true;
            //cout<<node<<" ";

            #pragma omp parallel for
            for(auto adjNode : adjMatrix[node])
            {
                #pragma omp critical
                {
                    if(!visited[adjNode])
                    {
                        #pragma omp task
                        {
                            parallelDFSUtil(adjNode);
                        }
                    }
                }
            }

        }

        void parallelDFS(int start)
        {
            #pragma omp parallel
            {
                #pragma omp single
                {
                    parallelDFSUtil(start);
                }
            }
        }
};

int main()
{
    int vertices = 100000;
    // cout<<"Enter no. of vertices: ";
    // cin>>vertices;

    Graph g(vertices);

    cout<<"Adding edges..";
    /*
    g.addEdge(0,1);
    g.addEdge(1,2);
    g.addEdge(1,3);
    g.addEdge(2,5);
    g.addEdge(1,4);
    g.addEdge(2,6);
    */

    for(int i=0;i<9999;i++)
    {
        g.addEdge(i, i+1);
    }

    
    double start, end;

    //searial BFS
    start = omp_get_wtime();
    g.serialBFS(0);
    end = omp_get_wtime();
    cout<<"\nSequential BFS Time: "<<(end-start) * 1000<<" ms\n"<<endl;

    g.resetVisited();

    //searial BFS
    start = omp_get_wtime();
    g.parallelBFS(0);
    end = omp_get_wtime();
    cout<<"\nParallel BFS Time: "<<(end-start) * 1000<<" ms\n"<<endl;

    g.resetVisited();

    //searial BFS
    start = omp_get_wtime();
    g.serialDFS(0);
    end = omp_get_wtime();
    cout<<"\nSequential DFS Time: "<<(end-start) * 1000<<" ms\n"<<endl;

    g.resetVisited();

    //searial BFS
    start = omp_get_wtime();
    g.parallelDFS(0);
    end = omp_get_wtime();
    cout<<"\nParallel DFS Time: "<<(end-start) * 1000<<" ms\n"<<endl;



    return 0;
}