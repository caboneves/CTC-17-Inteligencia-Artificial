#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

high_resolution_clock::time_point t1 ;
high_resolution_clock::time_point t2 ;
#define REP(i, a, b) \
for (int i = int(a); i < int(b); i++)
#define N 50

int Queens[N] = {};
char tab[N][N] ;

int len(int vec[]){
    int i=0;
    while(vec[i] != -1) i++;
    return i;
}

int getRand(int mod){
    return random() % mod;
}

void generateTab(){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            tab[i][j] = '-';
        }
    }
    for(int i=0;i<N;i++) tab[i][Queens[i]] = 'Q';
}


int isValid(int vec[], int curlinha, int curcoluna){
    int i;
    if(len(vec)){
        for(i=0;i<len(vec);i++) if(i==curlinha || vec[i]==curcoluna || abs(i-curlinha)==abs(vec[i]-curcoluna)) return 0;
        return 1;
    } else return 1;
    return 0;
}

int getpeso(int vec[]){
    int peso = 0;
    int queen;
    for(queen=0;queen<N;queen++){    
        int nextqueen;
        for(nextqueen=queen+1;nextqueen<N;nextqueen++){       
            if(vec[queen] == vec[nextqueen] || abs(queen-nextqueen)==abs(vec[queen]-vec[nextqueen])){   //se tem colisao
                peso++;
            }
        }
    }
    return peso;
}

void hillClimbing(int vec[]){
    int peso = getpeso(vec);
    if (peso == 0){
            t2 = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>( t2 - t1 ).count();
                string time = "Tempo duracao Hill Climbing para N = " + to_string(N) + " : " + to_string(duration) + "microsegundos\n";
                string s2 = "solution" + to_string(N) +".txt";
                ofstream sol;
                sol.open(s2);
                sol << time;
                generateTab();

                for (int i = 0;i<N;i++){
                    string s3;
                        for(int j=0;j<N;j++){
                     if (j==N-1) s3 += tab[i][j];
                           else  {
                              s3 += tab[i][j];
                              s3 += ' ';
                    }
                              }
                              s3 += '\n';

                              sol << s3;
                            }
                            sol.close();


                exit(0);
    } else {
        int nextlinha[] = {-1};
        int nextcoluna[] = {-1};
        int nextpeso = peso;
        int queen;
        for(queen=0;queen<N;queen++){                            //for each linha
            int origcoluna = vec[queen];                             //save the original colunaumn
            int validcoluna;
            for(validcoluna = 0; validcoluna<N;validcoluna++){            //para cada coluna valida
                if(validcoluna != origcoluna){
                    vec[queen] = validcoluna;                        //coloca rainha na proxima coluna
                    int newpeso = getpeso(vec);               //pega o peso do tabuleiro
                    if(newpeso < nextpeso){                    //se eh um movimento melhor
                        int i;
                        for(i=0;i<len(nextlinha);i++){            //clear tabuleiro
                            nextlinha[i] = (int)NULL;
                            nextcoluna[i] = (int)NULL;
                        }
                        nextlinha[0] = queen;
                        nextlinha[1] = -1;
                        nextcoluna[0] = validcoluna;
                        nextcoluna[1] = -1;
                        nextpeso = newpeso;

                        hillClimbing(vec);
                    } else if (newpeso == nextpeso){
                        int leng = len(nextlinha);
                        nextlinha[leng] = queen;
                        nextlinha[leng+1] = -1;
                        nextcoluna[leng] = validcoluna;
                        nextcoluna[leng+1] = -1;
                    }

                }
            }
            vec[queen] = origcoluna;
        }
        //quando acabarmos de procurar no tabuleiro
        if (nextcoluna[0] != -1 && nextlinha[0] != -1){          //se encontramos um movimento melhor
            int i;
            for(i=0;i<len(nextlinha);i++){                 //para cada movimento que de um melhor peso
                vec[nextlinha[i]] = nextcoluna[i];             //do it
                hillClimbing(vec);                        //recursao
            }
        } else {

            int i;
            for(i=0;i<N;i++) Queens[i] = getRand(N);
            hillClimbing(Queens);
        }
    }
}

int main(int argc, const char * argv[]){
    //Leitura arquivo
    string s = "queens" + to_string(N) + ".txt",line;
    ifstream file;
    file.open(s);
    int linha = 0;
    while(getline(file,line)){
        int spaces = 0;
        REP(col,0,line.size()){
            if (line[col] == 'Q'){
                Queens[linha++] = col-spaces;
            }
            if(line[col] == ' ') spaces++;
        }
    }
    file.close();
    // Queens eh um vetor tal que a posicao i representa a linha e o conteudo
    // e Queens[i] representa coluna de uma rainha


    ///comeca a contar tempo
    t1 = high_resolution_clock::now();


    hillClimbing(Queens);
    return 0;
}


