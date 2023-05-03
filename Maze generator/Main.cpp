#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

template <typename T>
class Stack
{
private:
	const int SIZE = 1000;
	T* stack;
	int top;

public:
	Stack()
	{
		this->stack = new T[SIZE];
		this->top = -1;
	}

	~Stack()
	{
		delete[] stack;
	}

	bool IsEmpty()
	{
		return top == -1;
	}

	bool IsFull()
	{
		return top == SIZE - 1;
	}

	void Push(T value)
	{
		if (!IsFull())
		{
			top++;
			stack[top] = value;
		}
		else
		{
			throw std::runtime_error("Stack overflow");
		}
	}

	T Pop()
	{
		if (!IsEmpty())
		{
			return stack[top--];
		}
		else
		{
			throw std::runtime_error("Stack underflow");
		}
	}

	std::vector<T> ToVector()
	{
		std::vector<T> result;

		for (int i = 0; i <= top; i++)
		{
			result.push_back(stack[i]);
		}

		return result;
	}
};

class UndirectedGraph
{
private:
	int numberOfVertices;
	std::vector<std::vector<int>> adjacencyMatrix;
	int gridDimension;

public:
	UndirectedGraph(int numberOfVertices) : numberOfVertices(numberOfVertices), gridDimension(sqrt(numberOfVertices))
	{
		adjacencyMatrix.resize(numberOfVertices);

		for (auto& row : adjacencyMatrix)
		{
			row.resize(numberOfVertices);
		}
	}

	bool InvalidVertex(int vertex)
	{
		return (vertex < 0) || (vertex >= numberOfVertices);
	}

	void AddEdge(int from, int to)
	{
		if (InvalidVertex(from) || InvalidVertex(to))
		{
			throw std::runtime_error("Invalid vertex number");
		}

		if (from == to)
		{
			adjacencyMatrix[from][to] = 2;
		}
		else
		{
			adjacencyMatrix[from][to] = adjacencyMatrix[to][from] = 1;
		}
	}

	void Display()
	{
		std::cout << "Adjacency matrix:\n\n";

		for (int i = 0; i < numberOfVertices; i++)
		{
			for (int j = 0; j < numberOfVertices; j++)
			{
				std::cout << adjacencyMatrix[i][j] << "  ";
			}

			std::cout << "\n";
		}
	}

	void CreateGridRepresentation()
	{
		for (int i = 0; i < numberOfVertices; i++)
		{
			// Left edge
			if (i % gridDimension != 0)
			{
				AddEdge(i, i - 1);
			}

			// Right edge
			if (i % gridDimension != (gridDimension - 1))
			{
				AddEdge(i, i + 1);
			}

			// Top edge
			if (i >= gridDimension)
			{
				AddEdge(i, i - gridDimension);
			}

			// Bottom edge
			if (i <= (numberOfVertices - 1) - gridDimension)
			{
				AddEdge(i, i + gridDimension);
			}
		}
	}

	friend class MazeGenerator;
};

class Edge
{
private:
	int sourceLabel;
	int destinationLabel;
	sf::RectangleShape edge;

public:
	Edge() : sourceLabel(-1), destinationLabel(-1), edge()
	{

	}

	friend class MazeGenerator;
	friend class GridCell;
	friend class Vertex;
};

class Vertex
{
private:
	int label;
	Edge leftEdge;
	Edge rightEdge;
	Edge topEdge;
	Edge bottomEdge;

public:
	Vertex() : label(-1)
	{

	}

	std::vector<int> GetNeighbors()
	{
		std::vector<int> neighbors;
		
		if (leftEdge.destinationLabel != -1)
		{
			neighbors.push_back(leftEdge.destinationLabel);
		}

		if (rightEdge.destinationLabel != -1)
		{
			neighbors.push_back(rightEdge.destinationLabel);
		}

		if (topEdge.destinationLabel != -1)
		{
			neighbors.push_back(topEdge.destinationLabel);
		}

		if (bottomEdge.destinationLabel != -1)
		{
			neighbors.push_back(bottomEdge.destinationLabel);
		}

		return neighbors;
	}

	friend class MazeGenerator;
	friend class GridCell;
};

class GridCell
{
private:
	sf::RectangleShape rectangle;
	Vertex vertex;
	sf::RectangleShape leftWall;
	bool displayLeftWall;
	sf::RectangleShape rightWall;
	bool displayRightWall;
	sf::RectangleShape topWall;
	bool displayTopWall;
	sf::RectangleShape bottomWall;
	bool displayBottomWall;
	sf::CircleShape circle;
	bool displayCircle;

public:
	GridCell(sf::RectangleShape& model) : displayLeftWall(true), displayRightWall(true), displayTopWall(true), displayBottomWall(true), displayCircle(false)
	{
		rectangle.setFillColor(model.getFillColor());
		rectangle.setSize(model.getSize());
		rectangle.setPosition(model.getPosition());
	}

private:
	void SetEdgesPositions()
	{
		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recHalfSizeX = rectangle.getSize().x / 2;
		float recHalfSizeY = rectangle.getSize().y / 2;

		vertex.leftEdge.edge.setPosition(recPosX - recHalfSizeX, recPosY + recHalfSizeY - (vertex.leftEdge.edge.getSize().y / 2));

		vertex.rightEdge.edge.setPosition(recPosX + recHalfSizeX, recPosY + recHalfSizeY - (vertex.rightEdge.edge.getSize().y / 2));

		vertex.topEdge.edge.setPosition(recPosX + recHalfSizeX - (vertex.topEdge.edge.getSize().x / 2), recPosY - recHalfSizeY);

		vertex.bottomEdge.edge.setPosition(recPosX + recHalfSizeX - (vertex.bottomEdge.edge.getSize().x / 2), recPosY + recHalfSizeY);
	}

	void SetWallsSizes()
	{
		sf::Vector2f horizontalWallSize(vertex.leftEdge.edge.getSize());
		sf::Vector2f verticalWallSize(vertex.topEdge.edge.getSize());

		leftWall.setSize(verticalWallSize);
		rightWall.setSize(verticalWallSize);
		topWall.setSize(horizontalWallSize);
		bottomWall.setSize(horizontalWallSize);

		//unsigned int r = (unsigned int)(rand() % 256);
		//unsigned int g = (unsigned int)(rand() % 256);
		//unsigned int b = (unsigned int)(rand() % 256);

		//sf::Color randomColor(r, g, b);

		//leftWall.setFillColor(randomColor);
		//rightWall.setFillColor(randomColor);
		//topWall.setFillColor(randomColor);
		//bottomWall.setFillColor(randomColor);
	}

	void SetWallsPositions()
	{
		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recSizeX = rectangle.getSize().x;
		float recSizeY = rectangle.getSize().y;

		leftWall.setPosition(recPosX - (leftWall.getSize().x / 2), recPosY);

		rightWall.setPosition(recPosX + recSizeX - (rightWall.getSize().x / 2), recPosY);

		topWall.setPosition(recPosX, recPosY - (topWall.getSize().y / 2));

		bottomWall.setPosition(recPosX, recPosY + recSizeY - (bottomWall.getSize().y / 2));
	}

	void SetUpCircle(sf::Color color)
	{
		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recHalfSizeX = rectangle.getSize().x / 2;
		float recHalfSizeY = rectangle.getSize().y / 2;

		float radius = 0.25f * rectangle.getSize().x;

		circle.setRadius(radius);
		circle.setOrigin(sf::Vector2f(radius, radius));
		circle.setPosition(recPosX + recHalfSizeX, recPosY + recHalfSizeY);
		circle.setFillColor(color);
	}

	friend class MazeGenerator;
};

class MazeGenerator
{
private:
	int n;
	sf::RenderWindow& window;
	int windowWidth;
	int windowHeight;
	UndirectedGraph graph;
	sf::Vector2f mazeAreaPosition;
	sf::Vector2f mazeAreaSize;
	std::vector<GridCell> grid;
	std::vector<int> solution;
	int startCellIndex;
	int endCellIndex;

public:
	MazeGenerator(int n, sf::RenderWindow& window) : n(n), window(window), graph(n * n)
	{
		windowWidth = window.getSize().x;
		windowHeight = window.getSize().y;

		sf::RectangleShape area(sf::Vector2f(0.8f * windowHeight, 0.8f * windowHeight));
		mazeAreaSize = area.getSize();

		area.setPosition(0.5f * (windowWidth - mazeAreaSize.x), 0.1f * windowHeight);
		mazeAreaPosition = area.getPosition();
	}

	void GenerateMaze()
	{
		graph.CreateGridRepresentation();
		SetUpGrid();
		SetUpVerticesAndEdges();
		SetUpWalls();
		solution = SetUpMaze();
	}

	void RegenerateMaze()
	{
		grid[startCellIndex].displayCircle = false;
		grid[endCellIndex].displayCircle = false;

		for (auto& gridCell : grid)
		{
			gridCell.displayLeftWall = true;
			gridCell.displayRightWall = true;
			gridCell.displayTopWall = true;
			gridCell.displayBottomWall = true;
		}

		solution = SetUpMaze();
	}
	
	void ShowMaze(bool showSolution)
	{
		window.clear();

		DrawMaze();
		DrawStartEnd();

		if (showSolution)
		{
			DrawSolution(solution);
		}

		window.display();
	}

private:
	void SetUpGrid()
	{
		srand(time(0));

		sf::Vector2f gridCellSize = mazeAreaSize;
		gridCellSize.x /= n;
		gridCellSize.y /= n;

		sf::Vector2f gridCellPosition = mazeAreaPosition;

		for (int i = 0; i < n; i++)
		{
			gridCellPosition.x = mazeAreaPosition.x;

			for (int j = 0; j < n; j++)
			{
				sf::RectangleShape model;

				model.setSize(gridCellSize);
				model.setPosition(gridCellPosition);

				unsigned int r = (unsigned int)(rand() % 256);
				unsigned int g = (unsigned int)(rand() % 256);
				unsigned int b = (unsigned int)(rand() % 256);

				sf::Color randomColor(r, g, b);

				model.setFillColor(randomColor);

				grid.push_back(GridCell(model));

				gridCellPosition.x += gridCellSize.x;
			}

			gridCellPosition.y += gridCellSize.y;
		}
	}

	void DrawGrid()
	{
		for (auto& gridCell : grid)
		{
			window.draw(gridCell.rectangle);
		}
	}

	void SetUpVerticesAndEdges()
	{
		sf::Vector2f gridCellSize = mazeAreaSize;
		gridCellSize.x /= n;
		gridCellSize.y /= n;

		sf::Vector2f horizontalEdgeDimensions(gridCellSize.x, 0.05f * gridCellSize.y);
		sf::Vector2f verticalEdgeDimensions(0.05f * gridCellSize.x, gridCellSize.y);

		for (int i = 0; i < grid.size(); i++)
		{
			Edge leftEdge;
			Edge rightEdge;
			Edge topEdge;
			Edge bottomEdge;

			leftEdge.edge.setSize(horizontalEdgeDimensions);
			rightEdge.edge.setSize(horizontalEdgeDimensions);
			topEdge.edge.setSize(verticalEdgeDimensions);
			bottomEdge.edge.setSize(verticalEdgeDimensions);

			sf::Color yellow = sf::Color::Yellow;
			leftEdge.edge.setFillColor(yellow);
			rightEdge.edge.setFillColor(yellow);
			topEdge.edge.setFillColor(yellow);
			bottomEdge.edge.setFillColor(yellow);

			leftEdge.sourceLabel = i;
			rightEdge.sourceLabel = i;
			topEdge.sourceLabel = i;
			bottomEdge.sourceLabel = i;

			for (int j = 0; j < graph.numberOfVertices; j++)
			{
				if ((i - 1 >= 0) && (graph.adjacencyMatrix[i][i - 1] == 1))
				{
					leftEdge.destinationLabel = i - 1;
				}

				if ((i + 1 < graph.numberOfVertices) && (graph.adjacencyMatrix[i][i + 1] == 1))
				{
					rightEdge.destinationLabel = i + 1;
				}

				if ((i - graph.gridDimension >= 0) && (graph.adjacencyMatrix[i][i - graph.gridDimension] == 1))
				{
					topEdge.destinationLabel = i - graph.gridDimension;
				}

				if ((i + graph.gridDimension < graph.numberOfVertices) && (graph.adjacencyMatrix[i][i + graph.gridDimension] == 1))
				{
					bottomEdge.destinationLabel = i + graph.gridDimension;
				}
			}

			Vertex vertex;
			vertex.label = i;
			vertex.leftEdge = leftEdge;
			vertex.rightEdge = rightEdge;
			vertex.topEdge = topEdge;
			vertex.bottomEdge = bottomEdge;

			grid[i].vertex = vertex;
			grid[i].SetEdgesPositions();
		}
	}

	void DrawVerticesAndEdges()
	{
		for (auto& gridCell : grid)
		{
			if (gridCell.vertex.leftEdge.destinationLabel != -1)
			{
				window.draw(gridCell.vertex.leftEdge.edge);
			}
			
			if (gridCell.vertex.rightEdge.destinationLabel != -1)
			{
				window.draw(gridCell.vertex.rightEdge.edge);
			}

			if (gridCell.vertex.topEdge.destinationLabel != -1)
			{
				window.draw(gridCell.vertex.topEdge.edge);
			}

			if (gridCell.vertex.bottomEdge.destinationLabel != -1)
			{
				window.draw(gridCell.vertex.bottomEdge.edge);
			}
		}
	}

	void SetUpWalls()
	{
		for (auto& gridCell : grid)
		{
			gridCell.SetWallsSizes();
			gridCell.SetWallsPositions();
		}
	}

	void DrawWalls()
	{
		for (auto& gridCell : grid)
		{
			window.draw(gridCell.leftWall);
			window.draw(gridCell.rightWall);
			window.draw(gridCell.topWall);
			window.draw(gridCell.bottomWall);
		}
	}

	std::vector<int> SetUpMaze()
	{
		Stack<int> stack;
		std::vector<bool> visited(grid.size());
		int currentCellIndex = rand() % grid.size();
		int lastCellIndex = currentCellIndex;

		while (lastCellIndex == currentCellIndex)
		{
			lastCellIndex = rand() % grid.size();
		}

		startCellIndex = currentCellIndex;
		endCellIndex = lastCellIndex;

		grid[currentCellIndex].SetUpCircle(sf::Color::Red);
		grid[currentCellIndex].displayCircle = true;

		grid[lastCellIndex].SetUpCircle(sf::Color::Green);
		grid[lastCellIndex].displayCircle = true;

		visited[currentCellIndex] = true;
		stack.Push(currentCellIndex);

		bool foundDestination = false;
		std::vector<int> solution;

		while (!stack.IsEmpty())
		{
			currentCellIndex = stack.Pop();
			GridCell currentCell = grid[currentCellIndex];
			std::vector<int> neighbors = currentCell.vertex.GetNeighbors();
			std::vector<int> unvisitedNeighbors;

			for (auto& neighbor : neighbors)
			{
				if (visited[neighbor] == false)
				{
					unvisitedNeighbors.push_back(neighbor);
				}
			}

			if (unvisitedNeighbors.size() > 0)
			{
				stack.Push(currentCellIndex);
				int randomNeighborIndex = unvisitedNeighbors[rand() % unvisitedNeighbors.size()];
				GridCell randomNeighbor = grid[randomNeighborIndex];

				if (currentCell.vertex.leftEdge.destinationLabel == randomNeighbor.vertex.label)
				{
					grid[currentCellIndex].displayLeftWall = false;
				}
				if (currentCell.vertex.rightEdge.destinationLabel == randomNeighbor.vertex.label)
				{
					grid[currentCellIndex].displayRightWall = false;
				}
				if (currentCell.vertex.topEdge.destinationLabel == randomNeighbor.vertex.label)
				{
					grid[currentCellIndex].displayTopWall = false;
				}
				if (currentCell.vertex.bottomEdge.destinationLabel == randomNeighbor.vertex.label)
				{
					grid[currentCellIndex].displayBottomWall = false;
				}

				if (randomNeighbor.vertex.leftEdge.destinationLabel == currentCell.vertex.label)
				{
					grid[randomNeighborIndex].displayLeftWall = false;
				}
				if (randomNeighbor.vertex.rightEdge.destinationLabel == currentCell.vertex.label)
				{
					grid[randomNeighborIndex].displayRightWall = false;
				}
				if (randomNeighbor.vertex.topEdge.destinationLabel == currentCell.vertex.label)
				{
					grid[randomNeighborIndex].displayTopWall = false;
				}
				if (randomNeighbor.vertex.bottomEdge.destinationLabel == currentCell.vertex.label)
				{
					grid[randomNeighborIndex].displayBottomWall = false;
				}

				if (currentCellIndex == lastCellIndex)
				{
					solution = stack.ToVector();
				}

				visited[randomNeighborIndex] = true;
				stack.Push(randomNeighborIndex);
			}
		}

		return solution;
	}

	void DrawMaze()
	{
		for (auto& gridCell : grid)
		{
			if (gridCell.displayLeftWall)
			{
				window.draw(gridCell.leftWall);
			}

			if (gridCell.displayRightWall)
			{
				window.draw(gridCell.rightWall);
			}

			if (gridCell.displayTopWall)
			{
				window.draw(gridCell.topWall);
			}

			if (gridCell.displayBottomWall)
			{
				window.draw(gridCell.bottomWall);
			}
		}
	}

	void DrawStartEnd()
	{
		for (auto& gridCell : grid)
		{
			if (gridCell.displayCircle)
			{
				window.draw(gridCell.circle);
			}
		}
	}

	void DrawSolution(std::vector<int> solution)
	{
		for (int i = 0; i < solution.size() - 1; i++)
		{
			GridCell currentGridCell = grid[solution[i]];

			if (currentGridCell.vertex.leftEdge.destinationLabel == solution[i + 1])
			{
				window.draw(grid[solution[i]].vertex.leftEdge.edge);
			}

			if (currentGridCell.vertex.rightEdge.destinationLabel == solution[i + 1])
			{
				window.draw(grid[solution[i]].vertex.rightEdge.edge);
			}

			if (currentGridCell.vertex.topEdge.destinationLabel == solution[i + 1])
			{
				window.draw(grid[solution[i]].vertex.topEdge.edge);
			}

			if (currentGridCell.vertex.bottomEdge.destinationLabel == solution[i + 1])
			{
				window.draw(grid[solution[i]].vertex.bottomEdge.edge);
			}
		}
	}
};

int main()
{
	const unsigned int windowWidth = (unsigned int)(sf::VideoMode::getDesktopMode().width / 1.25);
	const unsigned int windowHeight = (unsigned int)(sf::VideoMode::getDesktopMode().height / 1.25);
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "First SFML Project", sf::Style::Default);
	int n = 20;
	MazeGenerator mazeGenerator(n, window);
	bool showSolution = false;

	mazeGenerator.GenerateMaze();
	mazeGenerator.ShowMaze(showSolution);

	std::cout << "Press R to regenerate\n";
	std::cout << "Press S to show/hide solution\n";

	while (window.isOpen())
	{
		sf::Event someEvent;

		while (window.pollEvent(someEvent))
		{
			if (someEvent.type == sf::Event::KeyPressed)
			{
				switch (someEvent.key.code)
				{
				case sf::Keyboard::Key::R:
					showSolution = false;
					mazeGenerator.RegenerateMaze();
					mazeGenerator.ShowMaze(showSolution);
					break;
				case sf::Keyboard::Key::S:
					showSolution = !showSolution;
					mazeGenerator.ShowMaze(showSolution);
					break;
				}
			}
			else if (someEvent.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}

	return 0;
}
