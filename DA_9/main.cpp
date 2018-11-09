#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <queue>

const int64_t INF = LLONG_MAX;

struct TEdge {
	uint32_t to;
	int32_t weight;
};

struct TGraph {
	std::vector<int64_t> distances;
	std::vector<std::vector<TEdge>> vertices;
	uint32_t numOfVertices;
	uint32_t numOfEdges;
};

void ReadGraph(TGraph&, int&, int&);
void Dijkstra(TGraph&, int, std::vector<int64_t>&);
bool BellmanFord(TGraph&);
void PrintGraph(TGraph&, int, int);

int main() {
	int start;
	int finish;
	TGraph graph;
	ReadGraph(graph, start, finish);
	if (BellmanFord(graph)) {
		std::cout << "Negative cycle\n";
		return 0;
	}
	for (uint32_t j = 0; j < graph.numOfVertices; ++j) {
		for (uint32_t k = 0; k < graph.vertices[j].size(); ++k) {
			graph.vertices[j][k].weight += graph.distances[j] - graph.distances[graph.vertices[j][k].to];
		}
	}

	std::vector<int64_t> lineOfResMatrix(graph.numOfVertices);
	for (uint32_t j = 0; j < graph.numOfVertices; ++j) {
		Dijkstra(graph, j, lineOfResMatrix);
		for (uint32_t k = 0; k < graph.numOfVertices - 1; ++k) {
			if (lineOfResMatrix[k] == LLONG_MAX) {
				std::cout << "inf ";
			}
			else {
				std::cout << lineOfResMatrix[k] - (graph.distances[j] - graph.distances[k]) << " ";
			}
		}

		if (lineOfResMatrix[graph.numOfVertices - 1] == LLONG_MAX) {
			std::cout << "inf";
		}
		else {
			std::cout << lineOfResMatrix[graph.numOfVertices - 1] -
				(graph.distances[j] - graph.distances[graph.numOfVertices - 1]);
		}

		std::cout << "\n";
	}
	return 0;
}

void Dijkstra(TGraph& graph, int start, std::vector<int64_t>& distances) {
	// Using lambda to compare elements https://en.cppreference.com/w/cpp/container/priority_queue
 	auto comparing = [](TEdge& firstEdge, TEdge& secondEdge) { return firstEdge.weight > secondEdge.weight; };
	std::priority_queue<TEdge, std::vector<TEdge>, decltype(comparing)> priorityQueue(comparing);

	distances.resize(graph.numOfVertices);
	for (uint32_t i = 0; i < graph.numOfVertices; ++i) {
		distances[i] = INF;
	}

	distances[start] = 0;
	TEdge currentEdge{start,0};
	priorityQueue.push(currentEdge);

	while (!priorityQueue.empty()) {
		const uint32_t currentVertex = priorityQueue.top().to;
		priorityQueue.pop();

		std::vector<TEdge>::iterator i;
		for (i = graph.vertices[currentVertex].begin(); i != graph.vertices[currentVertex].end(); ++i) {
			const uint32_t relaxationVertex = (*i).to;
			const int32_t weight = (*i).weight;

			if (distances[relaxationVertex] > distances[currentVertex] + weight) {
				distances[relaxationVertex] = distances[currentVertex] + weight;
				currentEdge.to = relaxationVertex;
				currentEdge.weight = distances[relaxationVertex];
				priorityQueue.push(currentEdge);
			}
		}
	}
}

bool BellmanFord(TGraph& graph) {
	//add fictive vertex 
	graph.distances.resize(++graph.numOfVertices, INF);
	graph.distances[graph.distances.size() - 1] = 0;
	TEdge temp{0,0};
	for (uint32_t i = 0; i < graph.numOfVertices-1; ++i) {
		temp.to = i;
		graph.vertices[graph.numOfVertices-1].push_back(temp);
	}

	bool changed = false;
	for (uint32_t k = 0; k < graph.numOfVertices; ++k) {
		changed = false;
		for (uint32_t i = 0; i < graph.vertices.size(); ++i) {
			for (uint32_t j = 0; j < graph.vertices[i].size(); ++j) {
				if (graph.distances[i] < INF) {
					if (graph.distances[graph.vertices[i][j].to] >
						graph.distances[i] + graph.vertices[i][j].weight) {
						graph.distances[graph.vertices[i][j].to] = graph.distances[i] +
							graph.vertices[i][j].weight;
						changed = true;
					}
				}
			}
		}
	}
	--graph.numOfVertices;
	graph.vertices.pop_back();

	return changed;
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
		//graph.vertices[temp2.to].push_back(temp1);
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
