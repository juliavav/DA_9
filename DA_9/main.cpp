#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <queue>

const uint64_t INF = LLONG_MAX;

struct TEdge {
	uint32_t to;
	int32_t weight;
};

struct TGraph {
	std::vector<int64_t> delta;
	std::vector<std::vector<TEdge>> vertices;
	uint32_t numOfVertices;
	uint32_t numOfEdges;
};

void ReadGraph(TGraph&, int&, int&);
void Dijkstra(TGraph&, int, std::vector<int64_t>);
bool BellmanFord(TGraph&);
void PrintGraph(TGraph&, int, int);

int main() {
	int start;
	int finish;
	TGraph graph;
	ReadGraph(graph, start, finish);
	if (BellmanFord(graph) == false) {
		std::cout << "Negative cycle\n";
		return 0;
	}
	for (int j = 0; j < graph.numOfVertices; ++j) {
		for (int k = 0; k < graph.vertices[j].size(); ++k) {
			graph.vertices[j][k].weight += graph.delta[j] - graph.delta[graph.vertices[j][k].to];
		}
	}

	std::vector<long long> lineOfResMatrix(graph.numOfVertices);
	for (int j = 0; j < graph.numOfVertices; ++j) {
		Dijkstra(graph, j, lineOfResMatrix);
		for (int k = 0; k < graph.numOfVertices - 1; k++) {
			if (lineOfResMatrix[k] == LLONG_MAX) {
				std::cout << "inf ";
			}
			else {
				std::cout << lineOfResMatrix[k] - (graph.delta[j] - graph.delta[k]) << " ";
			}
		}

		if (lineOfResMatrix[graph.numOfVertices - 1] == LLONG_MAX) {
			std::cout << "inf";
		}
		else {
			std::cout << lineOfResMatrix[graph.numOfVertices - 1] -
				(graph.delta[j] - graph.delta[graph.numOfVertices - 1]);
		}

		std::cout << "\n";
	}
	system("pause");
	return 0;
}

void Dijkstra(TGraph& graph, int start, std::vector<int64_t> dist) {
	dist.resize(graph.numOfVertices);
	TEdge temp;
	// Using lambda to compare elements.
	//auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
	//std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
	auto cmp = [](TEdge& e1, TEdge& e2) { return e1.weight > e2.weight; };
	std::priority_queue<TEdge, std::vector<TEdge>, decltype(cmp)> prQueue(cmp);

	for (int i = 0; i < graph.numOfVertices; ++i) {
		dist[i] = INF;
	}
	dist[start] = 0;
	temp.weight = 0;//77777
	temp.to = start;
	prQueue.push(temp);

	while (!prQueue.empty()) {
		int active = prQueue.top().to;
		prQueue.pop();
		std::vector<TEdge>::iterator i;
		for (i = graph.vertices[active].begin(); i != graph.vertices[active].end(); ++i) {
			int to = (*i).to;
			int weight = (*i).weight;

			if (dist[to] > dist[active] + weight) {
				dist[to] = dist[active] + weight;
				temp.to = to;
				temp.weight = dist[to];
				prQueue.push(temp);
			}
		}
	}
}

bool BellmanFord(TGraph& graph) {
	graph.delta.resize(++graph.numOfVertices, INF);
	graph.delta[graph.delta.size() - 1] = 0;
	TEdge temp;
	temp.weight = 0;
	for (int i = 0; i < graph.numOfVertices; ++i) {
		temp.to = i;
		graph.vertices[graph.numOfVertices-1].push_back(temp);
	}
	bool changed;
	for (int k = 0; k < graph.numOfVertices; ++k) {
		changed = false;
		for (int i = 0; i < graph.vertices.size(); ++i) {
			for (int j = 0; j < graph.vertices[i].size(); ++j) {
				if (graph.delta[i] < INF) {
					if (graph.delta[graph.vertices[i][j].to] >
						graph.delta[i] + graph.vertices[i][j].weight) {
						graph.delta[graph.vertices[i][j].to] = graph.delta[i] +
							graph.vertices[i][j].weight;
						changed = true;
					}
				}
			}
		}
	}
	--graph.numOfVertices;
	graph.vertices.pop_back();

	if (changed == true) {
		return false;
	}

	return true;
}

void ReadGraph(TGraph& graph, int& start, int& finish) {
	TEdge temp1;
	TEdge temp2;

	std::cin >> graph.numOfVertices
		>> graph.numOfEdges;
	graph.vertices.resize(graph.numOfVertices + 1);//for BF
	for (int i = 0; i < graph.numOfEdges; ++i) {
		std::cin >> temp1.to
			>> temp2.to
			>> temp2.weight;
		temp1.weight = temp2.weight;
		--temp1.to;
		--temp2.to;
		graph.vertices[temp1.to].push_back(temp2);
		graph.vertices[temp2.to].push_back(temp1);
	}
}

void PrintGraph(TGraph& graph, int start, int finish) {
	for (int i = 0; i < graph.numOfVertices; ++i) {
		std::cout << "> Vertex " << i + 1 << std::endl;
		for (int j = 0; j < graph.vertices[i].size(); ++j) {
			std::cout << j + 1 << ") Vertex " << graph.vertices[i][j].to + 1 << std::endl;
			std::cout << j + 1 << ") Weight " << graph.vertices[i][j].weight << std::endl;
			std::cout << "===========" << std::endl;
		}
	}
}
