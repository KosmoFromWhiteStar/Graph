#include <iostream>
#include <vector>

class IGraph {
private:
	std::vector<std::vector<int>> some_vec;
public:
	virtual ~IGraph() {}
	IGraph() {};
	IGraph(IGraph* _oth) {};

	// Метод принимает вершины начала и конца ребра и добавляет ребро
	virtual void AddEdge(int from, int to) = 0; 

	// Метод должен считать текущее количество вершин
	virtual int VerticesCount() const = 0; 
										   
	// Для конкретной вершины метод выводит в вектор «вершины» все вершины, в которые можно дойти по ребру из данной
	virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; 

	// Для конкретной вершины метод выводит в вектор «вершины» все вершины, из которых можно дойти по ребру в данную
	virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; 
	
	virtual void display() = 0;
	virtual std::vector<std::vector<int>> get_vec() const = 0;
};


class MatrixGraph : public IGraph
{
private:
	std::vector<std::vector<int>> matrix;
	
	// i[j] = -(j[i])
	// Exemple
	//     1   2   3  4
	//   +-------------
	// 1 | 0   1   1  0
	// 2 | -1  0   1  2
	// 3 | -1 -1   0  1
	// 4 | 0  -2  -1  0 

public:

	MatrixGraph() {}
	MatrixGraph(MatrixGraph* _oth) {
		matrix = _oth->matrix;
	};
	MatrixGraph(IGraph* obj) {};
	MatrixGraph(class ListGraph& oth);
	MatrixGraph& operator=(const MatrixGraph& oth)
	{
		this->matrix = oth.matrix;
	}
	MatrixGraph& operator=(class ListGraph& oth);

	std::vector<std::vector<int>> get_vec() const override
	{
		return matrix;
	}

	void AddEdge(int from, int to) override
	{
		if (from < 0 || to < 0)
		{
			std::cerr << "Unvalid num\n";
			return;
		}
		if (from > matrix.size())
		{
			matrix.resize(from);
			for (int i = 0; i < matrix.size(); i++)
			{
				matrix[i].resize(from);
			}
		}
		// Лень выводить в отдельную функцию
		if (to > matrix.size())
		{
			matrix.resize(to);
			for (int i = 0; i < matrix.size(); i++)
			{
				matrix[i].resize(to);
			}
		}

		//Будет логическая ошибка при таком сценарии:
		// 1 -> 2 | matrix[1][2]++; matrix[2][1]--;
		// 1 <- 2 | matrix[1][2]--; matrix[2][1]++;
		
		//convert to index vector
		//num Vertex = index + 1
		from--;
		to--;

		matrix[from][to]++;
		matrix[to][from]--;
	}

	int VerticesCount() const override
	{
		return matrix.size();
	}

	void GetNextVertices(int vertex, std::vector<int>& vertices) const override
	{
		if (vertex < 0 || vertex > matrix.size())
		{
			std::cout << "Unvalid num" << std::endl;
			return;
		}
		for (int i = 0; i < matrix.size(); i++)
		{
			if (matrix[vertex][i] != 0)
			{
				vertices.push_back(i);
			}
		}
	}

	void GetPrevVertices(int vertex, std::vector<int>& vertices) const override
	{
		if (vertex < 0 || vertex > matrix.size())
		{
			std::cout << "Unvalid num" << std::endl;
			return;
		}
		
		for (int i = 0; i < matrix.size(); i++)
		{
			if (matrix[vertex][i] < 0)
			{
				vertices.push_back(i);
			}
		}
	}

	void display() override 
	{
		for (int i = 0; i < matrix.size(); i++)
		{
			std::cout << i+1 << "| ";
			for (int j = 0; j < matrix.size(); j++)
			{
				std::cout << matrix[i][j] << '\t';
			}
			std::cout << std::endl;
		}
	}

	~MatrixGraph() {	};
};


class ListGraph : public IGraph
{
private:

	// size list = count vertex. 
	// inserted vectors veretex linked. 
	///[V] - vertex
	std::vector<std::vector<int>> list;


public:
	ListGraph() {};
	ListGraph(ListGraph* _oth)
	{
		list = _oth->list;
	}
	ListGraph(IGraph* _oth)
	{
		
	}
	~ListGraph() {	 };
	ListGraph(const MatrixGraph& anoth)
	{
		const std::vector<std::vector <int>> another_vec = anoth.get_vec();
		list.resize(another_vec.size());
		for (int i = 0; i < another_vec.size(); i++)
		{
			for (int j = 0; j < another_vec[i].size(); j++)
			{
				if (another_vec[i][j] > 0)
					for (int k = another_vec[i][j];
						k > 0; k--)list[i].push_back(j);
			}
		}
	}

	ListGraph& operator=(const MatrixGraph& anoth)
	{
		const std::vector<std::vector <int>> another_vec = anoth.get_vec();
		list.resize(another_vec.size());
		for (int i = 0; i < another_vec.size(); i++)
		{
			for (int j = 0; j < another_vec[i].size(); j++)
			{
				if (another_vec[i][j] > 0)
					for (int k = another_vec[i][j];
						k > 0; k--)list[i].push_back(j);
			}
		}
		return *this;
	}
	ListGraph& operator=(const ListGraph& oth)
	{
		this->list = oth.list;
	}
	
	void AddEdge(int from, int to) override
	{
		if (from < 0 || to < 0)
		{
			std::cerr << "Unvalid num\n";
			return;
		}
		if (from < list.size() && to < list.size())
		{
			//add Edge From[V] - to[V]
			for (int i = 0; i < list[from].size(); i++)
			{
				if (list[from][i] == to)
				{
					list[from].push_back(to);
				}
			}
			return;
		}


		if (from > list.size())
		{
			list.resize(from);
		}
		if (to > list.size())
		{
			list.resize(to);
		}

		//convert to index vector
		//num Vertex = index + 1
		from--;
		to--;

		list[from].push_back(to);
		//list[to].push_back(from);
	}

	int VerticesCount() const override
	{
		return list.size();
	}

	void GetNextVertices(int vertex, std::vector<int>& vertices) const override
	{
		if (vertex < 0 || vertex > list.size())
		{
			std::cerr << "Unvalid num" << std::endl;
			return;
		}
		vertices = list[vertex];
	}

	void GetPrevVertices(int vertex, std::vector<int>& vertices) const override
	{
		for (int i = 0; i < list.size(); i++)
		{
			for (int j = 0; j < list[i].size(); j++)
			{
				if (vertex == list[i][j])
				{
					vertices.push_back(i);
					break;
				}
			}

		}
		
	}

	void display() override
	{
		for (int i = 0; i < list.size(); i++)
		{
			std::cout << i+1 << "|";
			for (int j = 0; j < list[i].size(); j++)
			{
				std::cout << list[i][j] + 1;
				if (j != list[i].size() - 1)
				{
					std::cout << "->";
				}
			}
			std::cout << std::endl;
		}
	}

	std::vector<std::vector<int>> get_vec() const override
	{
		return list;
	}
};


MatrixGraph& MatrixGraph::operator=(class ListGraph& oth)
{
	if (&oth == nullptr) { return *this; }

	const std::vector< std::vector <int>> oth_Vec = oth.get_vec();

	if (oth_Vec.size() <= 0){	return *this;	}

	matrix.resize(oth_Vec.size());
	
	for (int i = 0; i < oth_Vec.size(); i++)
	{
		matrix[i].resize(oth_Vec.size());
		for (int j = 0; j < oth_Vec[i].size(); j++)
		{
			matrix[i][ (oth_Vec[i][j]) ] ++;
			matrix[ (oth_Vec[i][j]) ][i] --;
		}
	}
	return *this;
}
MatrixGraph::MatrixGraph(class ListGraph& oth)
{
	if (&oth == nullptr) { return; }

	const std::vector< std::vector <int>> oth_Vec = oth.get_vec();

	if (oth_Vec.size() <= 0) { return ; }

	matrix.resize(oth_Vec.size());

	for (int i = 0; i < oth_Vec.size(); i++)
	{
		matrix[i].resize(oth_Vec.size());
		for (int j = 0; j < oth_Vec[i].size(); j++)
		{
			matrix[i][(oth_Vec[i][j])]++;
			//matrix[(oth_Vec[i][j])][i]--;
		}
	}
	for (int i = 0; i < oth_Vec.size(); i++)
	{
		for (int j = 0; j < oth_Vec[i].size(); j++)
		{

			matrix[(oth_Vec[i][j])][i]--;
		}
	}
}

int main(int, char**)
{
	MatrixGraph g1;

	// Example
	//     1   2   3   4
	//   +--------------
	// 1 |  0   1   1  0
	// 2 | -1   0   1  2
	// 3 | -1  -1   0  1
	// 4 |  0  -2  -1  0 
	
	g1.AddEdge(1, 2);
	g1.AddEdge(1, 3);
	g1.AddEdge(2, 3);
	g1.AddEdge(2, 4);
	g1.AddEdge(2, 4);
	g1.AddEdge(3, 4);
	//Do that

	g1.display();
	

	system("Pause");
	system("cls");


	ListGraph g2 = g1;
	//Check MatrixGraph -> ListGraph convert
	/*
	g2.AddEdge(1, 2);
	g2.AddEdge(1, 3);
	g2.AddEdge(2, 3);
	g2.AddEdge(2, 4);
	g2.AddEdge(2, 4);
	g2.AddEdge(3, 4);
	*/

	g2.display();

	system("Pause");
	system("cls");
	
	//Check ListGraph -> MatrixGraph convert
	// Same graph
	MatrixGraph g3 = g2;

	g3.display();

	return 0;
}