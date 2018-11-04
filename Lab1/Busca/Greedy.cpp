 //Greedy
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <set>
#include <stack>
#include <queue>
#include <math.h>
using namespace std;
using namespace std::chrono;

#define _CRT_SECURE_NO_DEPRECATE
typedef vector<int> vi;
typedef pair<int, int> ii;

typedef vector<ii> vii;
typedef set<int> si;
typedef map<string, int> msi;

#define REP(i, a, b) \
for (int i = int(a); i <= int(b); i++)

#define TRvi(c, it) \
for (vi::iterator it = (c).begin(); it != (c).end(); it++)
#define TRvii(c, it) \
for (vii::iterator it = (c).begin(); it != (c).end(); it++)
#define TRmsi(c, it) \
for (msi::iterator it = (c).begin(); it != (c).end(); it++)
#define INF 2000000000 // 2 billion
#define MEMSET_INF 127 // about 2B
#define MEMSET_HALF_INF 63 // about 1B


float distancia(float x1,float y1,float x2,float y2){
  return (float)(sqrt(pow(x1-x2,2)+pow(y1-y2,2)));
}

struct node{
	int id;
	float x;
	float y;
	float g;
	float f;
	string city;
	vector<int> neighbors;
	node(int id_,float x_,float y_,string city_){
		id = id_;
		x = x_;
		y = y_;
		g = f = INF;
		city = city_;
	}
	void addNeighbor(const vector<int>& v){
		neighbors = v;
	}

};



int main() {

	//setup
		ifstream file;
		file.open("australia.txt");
		string line;
		getline(file,line);
		vector<vi> list;
		vector<int> trash(0);
		list.push_back(trash);
	    string id,city,xs,ys,state,population;
	    int source,dest;
	    map<int,pair<float,float>> positions;
	    map<int,int> father;
	    map<int,string> IdCity;
	    int len = 0;
	    while (getline(file,id,',')){
	    	  len++;
	      getline(file,city,',');
	      if (city == "Alice Springs") source = stoi(id);
	      if (city == "Yulara") dest = stoi(id);
	    	  vector<int> tmp;
	    	  getline(file,xs,',');
	    	  getline(file,ys,',');
	    	  getline(file,state,',');
	    	  getline(file,population,'\n');
	    	  positions[stoi(id)] = make_pair(stof(xs),stof(ys));
	    	  IdCity[stoi(id)] = city;
	   }
	for(int i=0;i<len;i++) list.push_back(vi(0));

	for(int x=2;x<=len;x++){
		if (x%2 == 0){
			list[x].push_back(x+2);
			list[x].push_back(x-1);
			list[x+2].push_back(x);
			list[x-1].push_back(x);
		}
		else{
			list[x].push_back(x-2);
			list[x].push_back(x+1);
			list[x-2].push_back(x);
			list[x+1].push_back(x);

		}
	}


    vector< vector< pair<int,float> > > adjList; //int é o nó vizinho , o float eh a distancia
    vector< pair<int,float> > tmp2;
    tmp2.push_back(make_pair(-1,-1));
    adjList.push_back(tmp2);
    tmp2.clear();

    //para cada no vizinho , armazena a distancia ate destino
    for (int i=1;i< (int)list.size();i++){
        for (int j=0;j<(int)list[i].size();j++){
          float d = distancia(positions[dest].first,positions[dest].second,positions[list[i][j]].first,positions[list[i][j]].second);
          pair<int,float> aux = make_pair(list[i][j],d);
          tmp2.push_back(aux);
        }
        adjList.push_back(tmp2);
        tmp2.clear();
    }


    vector<float> dist((int)adjList.size(), INF);
    dist[source] = 0; // INF = 2.10^9 not MAX_INT to avoid overflow
    priority_queue< pair<float,int>, vector< pair<float,int> >, greater< pair<float,int> > > pq;
    pq.push(pair<float,int>(0, source)); // sort by distance
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    while (!pq.empty()) { // main loop
    pair<float , int > top = pq.top();
    pq.pop(); // greedy: pick shortest unvisited vertex
    float d = top.first;
    int u = top.second;
    if(u==dest) {
        break;
    }
    if (d == dist[u])
          for( vector< pair<int , float > > ::iterator it = adjList[u].begin();it!=adjList[u].end();it++){ // all outgoing edges from u
          int v = it->first;
          float weight_u_v = it->second;
          if ( weight_u_v < dist[v]) { // if can relax
          dist[v] = weight_u_v; // relax
          father[v] = u;
          pq.push(make_pair(dist[v], v)); // enqueue this neighbor
          }
          }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Tempo de execucao: "<< duration<<endl;
    int p = dest;
    float ans = 0;
          vector<int> path;
          father[source] = source;
          while (father[p]!=p){
            ans += distancia(positions[p].first,positions[p].second,positions[father[p]].first,positions[father[p]].second);
            path.push_back(p);
            p = father[p];
          }
      path.push_back(source);
      for (int i=path.size()-1;i>=0;i--){
          if(i==0)
          cout <<IdCity[path[i]];
          else cout <<IdCity[path[i]]<<" -- ";
      }
      cout<<endl<<"Numero de nós percorridos: "<<path.size()<<endl;

      cout <<endl<<"Custo: "<<ans<<endl;


  return 0;
}

