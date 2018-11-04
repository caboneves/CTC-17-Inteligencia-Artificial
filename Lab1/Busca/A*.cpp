
// A*
 
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
#include <unordered_set>
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

unordered_set<int> open_set;
unordered_set<int> closed_set;
priority_queue< pair<float,int>, vector< pair<float,int> >, greater< pair<float,int> > > pq;




float heuristic(float x1,float y1,float x2,float y2){
	return (float)(sqrt(pow(x1-x2,2)+pow(y1-y2,2)));
	//return abs(x2-x1)+abs(y2-y1);
}

float dist(float x1,float y1,float x2,float y2){
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
	void addNeighbor(int x){
		neighbors.push_back(x);
	}

};


int bestF(const vector<node>& nodes){

	int index = -1;
	int ans = INF;
	for(int ind:open_set){
		if(ans> nodes[ind].f ){
			ans = nodes[ind].f;
			index = ind;
		}
	}
	return index;

}

int main() {

	//setup
	ifstream file;
	file.open("australia.txt");
	string line;
	getline(file,line);
    string id,city,xs,ys,state,population;
    int source,dest;
    vector<node> nodes;
    node trash(0,0,0,"");
    nodes.push_back(trash); //1 indexed
    while (getline(file,id,',')){

      getline(file,city,',');
      if (city == "Alice Springs") source = stoi(id);
      if (city == "Yulara") dest = stoi(id);
    	  vector<int> tmp;
    	  getline(file,xs,',');
    	  getline(file,ys,',');
    	  getline(file,state,',');
    	  getline(file,population,'\n');
    	  node n(stoi(id),stof(xs),stof(ys),city);
    	  nodes.push_back(n);

    }

    for(auto& x:nodes){
    		if(x.id > 1 and x.id%2 == 0){
    			x.addNeighbor(x.id+2);
    			x.addNeighbor(x.id-1);
    			nodes[x.id+2].addNeighbor(x.id);
    			nodes[x.id-1].addNeighbor(x.id);



    		}
    		if (x.id > 2 and x.id%2 == 1){
    			x.addNeighbor(x.id-2);
    			x.addNeighbor(x.id+1);
    			nodes[x.id-2].addNeighbor(x.id);
    			nodes[x.id+1].addNeighbor(x.id);
    		}
    }


    nodes[source].g = nodes[source].f = 0;
    open_set.insert(source);
    pq.push(make_pair(0,source));
    vector<int> tmp = nodes[source].neighbors;
    map<int,int> father;
    father[source] = source;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    while (!pq.empty()){
    		pair<float,int> p = pq.top();
    		pq.pop();
    		int index = p.second;
    		closed_set.insert(index);
    		node tmp = nodes[index];

    		if (tmp.id == dest){
    			break;
    		}
    		else{
    			if(nodes[index].f == p.first){
    			vector<int> neighbors = tmp.neighbors;
    			for (int i=0;i<neighbors.size();i++){
    				if(closed_set.find(neighbors[i])!=closed_set.end()) continue; //already processed
    				int u = neighbors[i];
    				float tmpG = tmp.g + dist(tmp.x,tmp.y,nodes[u].x,nodes[u].y);
    				if(tmpG>=nodes[u].g) continue;
    				nodes[u].g = tmpG;
    				father[u] = index;
    				nodes[u].f = nodes[u].g+heuristic(nodes[u].x,nodes[u].y,nodes[dest].x,nodes[dest].y);
    				pq.push(make_pair(nodes[u].f,u));
    			}

    			}
    		}

    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Tempo de execucao: "<< duration<<endl;
    int p = dest;
    vector<int> path;
    cout<<"Caminho A* : "<<endl;
    while (father[p]!=p){
    	path.push_back(p);
    	p = father[p];
    }
    path.push_back(source);
    for (int i=path.size()-1;i>=0;i--){
    		if(i==0)
    		cout <<nodes[path[i]].city<<endl;
    		else cout <<nodes[path[i]].city<<" -- ";
    }
    cout<<"Numero de nós percorridos: "<<path.size()<<endl;
    cout<<endl<<"Custo: "<<nodes[dest].g<<endl;

	return 0;

}
