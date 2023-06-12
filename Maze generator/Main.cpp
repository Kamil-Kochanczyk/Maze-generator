#include <iostream>
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

public:
	UndirectedGraph(int numberOfVertices) : numberOfVertices(numberOfVertices)
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

	void Clear()
	{
		for (auto& row : adjacencyMatrix)
		{
			std::fill(row.begin(), row.end(), 0);
		}
	}

	void ToGridRepresentation()
	{
		Clear();

		int gridDimension = sqrt(numberOfVertices);

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

class GridCell
{
private:
	class Edge
	{
	public:
		int sourceLabel;
		int destinationLabel;
		sf::RectangleShape edge;

	public:
		Edge() : sourceLabel(-1), destinationLabel(-1), edge(sf::Vector2f(0, 0))
		{

		}
	};

	class Vertex
	{
	public:
		int label;
		Edge leftEdge;
		Edge rightEdge;
		Edge topEdge;
		Edge bottomEdge;

	public:
		Vertex() : label(-1), leftEdge(), rightEdge(), topEdge(), bottomEdge()
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
	};

private:
	sf::RectangleShape rectangle;
	Vertex vertex;
	sf::RectangleShape leftWall;
	sf::RectangleShape rightWall;
	sf::RectangleShape topWall;
	sf::RectangleShape bottomWall;
	sf::CircleShape circle;
	bool displayLeftWall;
	bool displayRightWall;
	bool displayTopWall;
	bool displayBottomWall;
	bool displayCircle;

public:
	GridCell(sf::RectangleShape& model) : rectangle(model.getSize()), 
										  vertex(), 
										  leftWall(sf::Vector2f(0, 0)), 
										  rightWall(sf::Vector2f(0, 0)), 
										  topWall(sf::Vector2f(0, 0)), 
										  bottomWall(sf::Vector2f(0, 0)), 
										  circle(0), 
										  displayLeftWall(true), 
										  displayRightWall(true), 
										  displayTopWall(true), 
										  displayBottomWall(true), 
										  displayCircle(false)
	{
		rectangle.setFillColor(model.getFillColor());
		rectangle.setPosition(model.getPosition());

		SetUpVertex();
		SetUpWalls();
		SetUpCircle();
	}

private:
	void SetUpVertex()
	{
		sf::Vector2f horizontalEdgeSize(rectangle.getSize().x, 0.05f * rectangle.getSize().y);
		sf::Vector2f verticalEdgeSize(0.05f * rectangle.getSize().x, rectangle.getSize().y);

		vertex.leftEdge.edge.setSize(horizontalEdgeSize);
		vertex.rightEdge.edge.setSize(horizontalEdgeSize);
		vertex.topEdge.edge.setSize(verticalEdgeSize);
		vertex.bottomEdge.edge.setSize(verticalEdgeSize);

		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recHalfSizeX = rectangle.getSize().x / 2;
		float recHalfSizeY = rectangle.getSize().y / 2;

		vertex.leftEdge.edge.setPosition(recPosX - recHalfSizeX, recPosY + recHalfSizeY - (vertex.leftEdge.edge.getSize().y / 2));

		vertex.rightEdge.edge.setPosition(recPosX + recHalfSizeX, recPosY + recHalfSizeY - (vertex.rightEdge.edge.getSize().y / 2));

		vertex.topEdge.edge.setPosition(recPosX + recHalfSizeX - (vertex.topEdge.edge.getSize().x / 2), recPosY - recHalfSizeY);

		vertex.bottomEdge.edge.setPosition(recPosX + recHalfSizeX - (vertex.bottomEdge.edge.getSize().x / 2), recPosY + recHalfSizeY);
	}

	void SetUpWalls()
	{
		sf::Vector2f horizontalWallSize(rectangle.getSize().x, 0.05f * rectangle.getSize().y);
		sf::Vector2f verticalWallSize(0.05f * rectangle.getSize().x, rectangle.getSize().y);

		leftWall.setSize(verticalWallSize);
		rightWall.setSize(verticalWallSize);
		topWall.setSize(horizontalWallSize);
		bottomWall.setSize(horizontalWallSize);

		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recSizeX = rectangle.getSize().x;
		float recSizeY = rectangle.getSize().y;

		leftWall.setPosition(recPosX - (leftWall.getSize().x / 2), recPosY);

		rightWall.setPosition(recPosX + recSizeX - (rightWall.getSize().x / 2), recPosY);

		topWall.setPosition(recPosX, recPosY - (topWall.getSize().y / 2));

		bottomWall.setPosition(recPosX, recPosY + recSizeY - (bottomWall.getSize().y / 2));

		//unsigned int r = (unsigned int)(rand() % 256);
		//unsigned int g = (unsigned int)(rand() % 256);
		//unsigned int b = (unsigned int)(rand() % 256);

		//sf::Color randomColor(r, g, b);

		//leftWall.setFillColor(randomColor);
		//rightWall.setFillColor(randomColor);
		//topWall.setFillColor(randomColor);
		//bottomWall.setFillColor(randomColor);
	}

	void SetUpCircle()
	{
		float recPosX = rectangle.getPosition().x;
		float recPosY = rectangle.getPosition().y;

		float recHalfSizeX = rectangle.getSize().x / 2;
		float recHalfSizeY = rectangle.getSize().y / 2;

		float radius = 0.25f * rectangle.getSize().x;

		circle.setRadius(radius);
		circle.setOrigin(sf::Vector2f(radius, radius));
		circle.setPosition(recPosX + recHalfSizeX, recPosY + recHalfSizeY);
	}

	friend class MazeGenerator;
};

class MazeGenerator
{
private:
	int gridDimension;
	sf::RenderWindow& window;
	int windowWidth;
	int windowHeight;
	UndirectedGraph gridRepresentation;
	sf::Vector2f gridSize;
	sf::Vector2f gridPosition;
	std::vector<GridCell> grid;
	int startCellIndex;
	int endCellIndex;
	std::vector<int> mazeSolution;

public:
	MazeGenerator(int n, sf::RenderWindow& window) : gridDimension(n), 
													 window(window), 
													 windowWidth(window.getSize().x), 
													 windowHeight(window.getSize().y), 
													 gridRepresentation(n * n), 
													 gridSize(0.8f * windowHeight, 0.8f * windowHeight), 
													 gridPosition(0.5f * (windowWidth - gridSize.x), 0.1f * windowHeight), 
													 startCellIndex(-1), 
													 endCellIndex(-1), 
													 mazeSolution(0) 
	{
		gridRepresentation.ToGridRepresentation();
	}

	void GenerateMaze()
	{
		SetUpFoundation();
		SetUpPaths();
		SetUpWalls();
		mazeSolution = SetUpMaze();
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

		mazeSolution = SetUpMaze();
	}
	
	void ShowMaze(bool showSolution)
	{
		window.clear();

		DrawMaze();

		if (showSolution)
		{
			DrawSolution();
		}

		window.display();
	}

	void Test()
	{
		window.clear();

		SetUpFoundation();
		DrawFoundation();

		SetUpPaths();
		DrawPaths();

		SetUpWalls();
		DrawWalls();

		window.display();
	}

private:
	void SetUpFoundation()
	{
		sf::Vector2f gridCellSize = gridSize;
		gridCellSize.x /= gridDimension;
		gridCellSize.y /= gridDimension;

		sf::Vector2f gridCellPosition = gridPosition;

		sf::RectangleShape model;
		model.setSize(gridCellSize);

		for (int i = 0; i < gridDimension; i++)
		{
			gridCellPosition.x = gridPosition.x;

			for (int j = 0; j < gridDimension; j++)
			{
				model.setPosition(gridCellPosition);

				//unsigned int r = (unsigned int)(rand() % 256);
				//unsigned int g = (unsigned int)(rand() % 256);
				//unsigned int b = (unsigned int)(rand() % 256);
				//sf::Color randomColor(r, g, b);
				//model.setFillColor(randomColor);
				model.setFillColor(sf::Color::Black);

				grid.push_back(GridCell(model));

				gridCellPosition.x += gridCellSize.x;
			}

			gridCellPosition.y += gridCellSize.y;
		}
	}

	void DrawFoundation()
	{
		for (auto& gridCell : grid)
		{
			window.draw(gridCell.rectangle);
		}
	}

	void SetUpPaths()
	{
		for (int i = 0; i < grid.size(); i++)
		{
			grid[i].vertex.label = i;

			grid[i].vertex.leftEdge.edge.setFillColor(sf::Color::Yellow);
			grid[i].vertex.rightEdge.edge.setFillColor(sf::Color::Yellow);
			grid[i].vertex.topEdge.edge.setFillColor(sf::Color::Yellow);
			grid[i].vertex.bottomEdge.edge.setFillColor(sf::Color::Yellow);

			grid[i].vertex.leftEdge.sourceLabel = i;
			grid[i].vertex.rightEdge.sourceLabel = i;
			grid[i].vertex.topEdge.sourceLabel = i;
			grid[i].vertex.bottomEdge.sourceLabel = i;

			// Left neighbor
			if ((i - 1 >= 0) && (gridRepresentation.adjacencyMatrix[i][i - 1] == 1))
			{
				grid[i].vertex.leftEdge.destinationLabel = i - 1;
			}

			// Right neighbor
			if ((i + 1 < gridRepresentation.numberOfVertices) && (gridRepresentation.adjacencyMatrix[i][i + 1] == 1))
			{
				grid[i].vertex.rightEdge.destinationLabel = i + 1;
			}

			// Top neighbor
			if ((i - gridDimension >= 0) && (gridRepresentation.adjacencyMatrix[i][i - gridDimension] == 1))
			{
				grid[i].vertex.topEdge.destinationLabel = i - gridDimension;
			}

			// Bottom neighbor
			if ((i + gridDimension < gridRepresentation.numberOfVertices) && (gridRepresentation.adjacencyMatrix[i][i + gridDimension] == 1))
			{
				grid[i].vertex.bottomEdge.destinationLabel = i + gridDimension;
			}
		}
	}

	void DrawPaths()
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
			gridCell.leftWall.setFillColor(sf::Color::White);
			gridCell.rightWall.setFillColor(sf::Color::White);
			gridCell.topWall.setFillColor(sf::Color::White);
			gridCell.bottomWall.setFillColor(sf::Color::White);
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
		std::vector<int> solution;

		startCellIndex = rand() % grid.size();
		endCellIndex = startCellIndex;

		while (endCellIndex == startCellIndex)
		{
			endCellIndex = rand() % grid.size();
		}

		grid[startCellIndex].circle.setFillColor(sf::Color::Red);
		grid[startCellIndex].displayCircle = true;

		grid[endCellIndex].circle.setFillColor(sf::Color::Green);
		grid[endCellIndex].displayCircle = true;

		visited[startCellIndex] = true;
		stack.Push(startCellIndex);

		while (!stack.IsEmpty())
		{
			int currentCellIndex = stack.Pop();
			std::vector<int> neighbors = grid[currentCellIndex].vertex.GetNeighbors();
			std::vector<int> unvisitedNeighbors;

			if (currentCellIndex == endCellIndex)
			{
				stack.Push(currentCellIndex);
				solution = stack.ToVector();
				stack.Pop();
			}

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

				if (grid[currentCellIndex].vertex.leftEdge.destinationLabel == grid[randomNeighborIndex].vertex.label)
				{
					grid[currentCellIndex].displayLeftWall = false;
				}
				if (grid[currentCellIndex].vertex.rightEdge.destinationLabel == grid[randomNeighborIndex].vertex.label)
				{
					grid[currentCellIndex].displayRightWall = false;
				}
				if (grid[currentCellIndex].vertex.topEdge.destinationLabel == grid[randomNeighborIndex].vertex.label)
				{
					grid[currentCellIndex].displayTopWall = false;
				}
				if (grid[currentCellIndex].vertex.bottomEdge.destinationLabel == grid[randomNeighborIndex].vertex.label)
				{
					grid[currentCellIndex].displayBottomWall = false;
				}

				if (grid[randomNeighborIndex].vertex.leftEdge.destinationLabel == grid[currentCellIndex].vertex.label)
				{
					grid[randomNeighborIndex].displayLeftWall = false;
				}
				if (grid[randomNeighborIndex].vertex.rightEdge.destinationLabel == grid[currentCellIndex].vertex.label)
				{
					grid[randomNeighborIndex].displayRightWall = false;
				}
				if (grid[randomNeighborIndex].vertex.topEdge.destinationLabel == grid[currentCellIndex].vertex.label)
				{
					grid[randomNeighborIndex].displayTopWall = false;
				}
				if (grid[randomNeighborIndex].vertex.bottomEdge.destinationLabel == grid[currentCellIndex].vertex.label)
				{
					grid[randomNeighborIndex].displayBottomWall = false;
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

			if (gridCell.displayCircle)
			{
				window.draw(gridCell.circle);
			}
		}
	}

	void DrawSolution()
	{
		for (int i = 0; i < mazeSolution.size() - 1; i++)
		{
			if (grid[mazeSolution[i]].vertex.leftEdge.destinationLabel == mazeSolution[i + 1])
			{
				window.draw(grid[mazeSolution[i]].vertex.leftEdge.edge);
			}

			if (grid[mazeSolution[i]].vertex.rightEdge.destinationLabel == mazeSolution[i + 1])
			{
				window.draw(grid[mazeSolution[i]].vertex.rightEdge.edge);
			}

			if (grid[mazeSolution[i]].vertex.topEdge.destinationLabel == mazeSolution[i + 1])
			{
				window.draw(grid[mazeSolution[i]].vertex.topEdge.edge);
			}

			if (grid[mazeSolution[i]].vertex.bottomEdge.destinationLabel == mazeSolution[i + 1])
			{
				window.draw(grid[mazeSolution[i]].vertex.bottomEdge.edge);
			}
		}
	}
};

int main()
{
	srand(time(0));
	const unsigned int windowWidth = (unsigned int)(sf::VideoMode::getDesktopMode().width / 1.25);
	const unsigned int windowHeight = (unsigned int)(sf::VideoMode::getDesktopMode().height / 1.25);
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Maze Generator", sf::Style::Titlebar | sf::Style::Close);
	int gridDimension = 20;	// 2 <= gridDimension <= 34
	MazeGenerator mazeGenerator(gridDimension, window);
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
					mazeGenerator.RegenerateMaze();
					showSolution = false;
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

//int main()
//{
//	srand(time(0));
//	const unsigned int windowWidth = (unsigned int)(sf::VideoMode::getDesktopMode().width / 1.25);
//	const unsigned int windowHeight = (unsigned int)(sf::VideoMode::getDesktopMode().height / 1.25);
//	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Maze Generator", sf::Style::Titlebar | sf::Style::Close);
//	int gridDimension = 3;	// gridDimension >= 2
//	MazeGenerator mazeGenerator(gridDimension, window);
//	bool showSolution = false;
//
//	mazeGenerator.Test();
//
//	while (window.isOpen())
//	{
//		sf::Event someEvent;
//
//		while (window.pollEvent(someEvent))
//		{
//			if (someEvent.type == sf::Event::Closed)
//			{
//				window.close();
//			}
//		}
//	}
//
//	return 0;
//}
