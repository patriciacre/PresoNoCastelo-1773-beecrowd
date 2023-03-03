#include <iostream>
#include <queue>
#include <vector>

using namespace std;

enum Cores {BRANCO, PRETO}; //cores para marcar os vertices

class vertice{
    public:
        vertice();
        vertice(Cores cor);
        virtual ~vertice() {};
        void setCor(Cores cor) {this->cor = cor;};
        Cores getCor() const {return this->cor;};
        void setAdj(int i) {this->adj.push_back(i);};
        int getTotaldeVizinhos() const {return this->adj.size();};
        int getVizinho(int i) const {return adj[i];}
    private:
        Cores cor;
        vector<int> adj;
};

vertice::vertice(){
    this->cor = BRANCO;
    adj = {};
}

vertice::vertice(Cores cor){
    this->cor = cor;
}

class grafo{
    public:
        grafo(int N, vector<vertice> &vert); // construtor parametrizado
        virtual ~grafo() {}; // destrutor
        bool bfs(int s); // busca em largura
        void setChaves(int c, int v); // setters e getters dos atributos
        vector<vector<int>> getChaves() const {return this->chaves;}
        void setU(vector<vertice> v) {this->u = v;};
        vector<vertice> getU() const {return this->u;};
        void setQ(queue<int> q) {this->q = q;};
        queue<int> getQ() const {return this->q;};
        int getnumV() const {return this->numV;}
        void setnumV(int n) {this->numV = n;}
        void setChavesObt(vector<int> v) {this->chavesObt = v;};
        vector<int> getChavesObt() const {return this->chavesObt;};
    private:
        vector<vertice> u; // vetor de vertices do grafo
        queue<int> q; // para fazer a bfs
        vector<vector<int>> chaves; // indica quais vértices possuem quais chaves
        vector<int> chavesObt; // chaves que já foram obtidas
        int numV; // número de vértices do grafo
};

grafo::grafo(int N, vector<vertice> &vert){
    u = vert;
    setnumV(N);
    q = {};
    chaves.resize(N+1);
    chavesObt.resize(N+1);
}

void grafo::setChaves(int c, int v){
    chaves[c].push_back(v);
}

bool grafo::bfs(int s){ // implementação da busca em largura
    int cont = 0; int a = 0;
    int aux;
    for (int i = 0 ; i < numV; i++){
        u[i].setCor(BRANCO); // inicialmente marco todos os vértices como BRANCOS, não visitados
    }
    u[s-1].setCor(PRETO); // o vértice inicial sera preto
    q = {}; // coloco a fila como vazia
    q.push(s); // coloca na fila a origem do grafo
    chavesObt[s] = 1; // indico que já tenho a chave da primeira sala
    int l = 0;
    while (!q.empty()){ // enquanto a fila não estiver vazia vou realizar a busca avançando em largura
        l = q.front(); // pego o primeiro item da fila
        q.pop(); // desenfilo
        if (chavesObt[l] == 1){ // se ja tiver a chave entro na sala
            cont++;
            a = 0;
            for (int i = 0; i<chaves[l].size(); i++){
                chavesObt[chaves[l][i]] = 1; // pego as chaves que tem na sala
            }
            for (int i = 0; i < u[l-1].getTotaldeVizinhos(); i++){ // para percorrer as adjacências da sala em questão
                aux = u[l-1].getVizinho(i);
                if (u[aux-1].getCor() == BRANCO){ // posso visitar o vértice somente se for branco
                    u[aux-1].setCor(PRETO); // marco como preto indicando que visitei
                    q.push(aux); // coloco o índice do vértice em questão na fila
                }
            }
        } else { // significa que não posso acessar a sala
             if (l == a){ // utilizo para verificar se estou inserindo uma sequencia de salas q não podem ser visitadas
                return false;
            } else {
                q.push(l); // se não for o caso, basta reinserir a sala no final da fila
            }
            if (a == 0) { // guardo em a o valor da primeira sala q não consegui visitar
                a = l;
            }
        }
    }
    return (cont == numV); // retorno se consegui visitar todos os vertices
}

int main() {
    int n, m, a, b, v2;
    vector<vertice> v;
    while (cin >> n >> m){ // lendo o número de salas e corredores
        v = {};
        v.resize(n);
		for(int i = 0; i < m; i++){ // lendo as arestas - corredores
			cin >> a >> b;
			v[a-1].setAdj(b); // grafo não direcional, posso ir de a para b ou de b para a
			v[b-1].setAdj(a); // devo subtrair 1 pois os grafos de exemplo começam em 1 mas os vetores começam em zero
		}
		grafo g(n, v);
		for(int i = 2; i <= n; i++) {
            cin >> v2; // ex: na sala v2 possuo a chave da sala i
            g.setChaves(v2, i); // armazeno os vértices onde são encontradas as chaves
		}
        bool result = g.bfs(1);
        if (result){ // verifico se consegui visitar todos os vertices
            cout << "sim" << endl;
        } else {
            cout << "nao" << endl;
        }
    }
    return 0;
}
