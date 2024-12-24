#pragma once
#include <PIX3D.h>
#include <time.h>

using namespace PIX3D;

#define CELL_SIZE 30
#define HALF_CELL_SIZE CELL_SIZE / 2
#define CELL_COUNT 25


static float FromCellToPixel(float pos)
{
	return pos * CELL_SIZE + HALF_CELL_SIZE;
}

enum CellOrientation
{
	Horizontal = 0,
	Vertical,
	TopLeft,
	TopRight,
	BottomRight,
	BottomLeft
};

struct CellPosition
{
	int x, y;
	CellOrientation orientation = Horizontal;
};

class Snake
{
public:

	enum class Directions
	{
		RIGHT = 0,
		LEFT,
		UP,
		DOWN
	};

public:
	Snake()
	{
		Body.push_back({ 10, 10 });
		Body.push_back({ 9, 10 });
		Body.push_back({ 8, 10 });
	}
	
	~Snake() {}

	void LoadAssets()
	{
		// Head Assets
		{
			Head.RightTexture.LoadFromFile("assets/head_right.png");
			Head.LeftTexture.LoadFromFile("assets/head_left.png");
			Head.UpTexture.LoadFromFile("assets/head_up.png");
			Head.DownTexture.LoadFromFile("assets/head_down.png");

			Head.CurrentTexture = Head.RightTexture;
		}

		// Tail Assets
		{
			Tail.UpTexture.LoadFromFile("assets/tail_up.png");
			Tail.DownTexture.LoadFromFile("assets/tail_down.png");
			Tail.RightTexture.LoadFromFile("assets/tail_right.png");
			Tail.LeftTexture.LoadFromFile("assets/tail_left.png");
		}

		// Body Assets
		{
			SnakeBody.Horizontal.LoadFromFile("assets/body_horizontal.png");
			SnakeBody.Vertical.LoadFromFile("assets/body_vertical.png");
			SnakeBody.TopLeft.LoadFromFile("assets/body_topleft.png");
			SnakeBody.TopRight.LoadFromFile("assets/body_topright.png");
			SnakeBody.BottomLeft.LoadFromFile("assets/body_bottomleft.png");
			SnakeBody.BottomRight.LoadFromFile("assets/body_bottomright.png");
		}
	}

	void ProcessInput()
	{
		Right = Input::IsKeyPressed(KeyCode::D) || Input::IsKeyPressed(KeyCode::Right);
		Left = Input::IsKeyPressed(KeyCode::A) || Input::IsKeyPressed(KeyCode::Left);
		Up = Input::IsKeyPressed(KeyCode::W) || Input::IsKeyPressed(KeyCode::Up);
		Down = Input::IsKeyPressed(KeyCode::S) || Input::IsKeyPressed(KeyCode::Down);
	}

	void Update()
	{
		Move();

		CheckForDeath();
	}

	void Draw()
	{
		// Draw Head
		{
			float xpos = FromCellToPixel(Body[0].x);
			float ypos = FromCellToPixel(Body[0].y);
			GL::GLPixelRenderer2D::DrawTexturedQuad(Head.CurrentTexture, xpos, ypos, CELL_SIZE, CELL_SIZE, {1.0f, 1.0f, 1.0f, 1.0f});
		}

		// Draw Body Parts
		{
			for (size_t i = 1; i < Body.size() - 1; i++)
			{
				CellPosition& BodyPart = Body[i];

				float xpos = FromCellToPixel(BodyPart.x);
				float ypos = FromCellToPixel(BodyPart.y);

				if(BodyPart.orientation == TopLeft)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.TopLeft, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
				else if (BodyPart.orientation == TopRight)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.TopRight, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
				else if (BodyPart.orientation == BottomLeft)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.BottomLeft, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
				else if (BodyPart.orientation == BottomRight)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.BottomRight, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
				else if (BodyPart.orientation == Horizontal)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.Horizontal, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
				else if (BodyPart.orientation == Vertical)
				{
					GL::GLPixelRenderer2D::DrawTexturedQuad(SnakeBody.Vertical, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
			}
		}

		// Draw Tail
		{
			auto& TailCell = Body[Body.size() - 1];
			
			float xpos = FromCellToPixel(TailCell.x);
			float ypos = FromCellToPixel(TailCell.y);

			if (Tail.Direction == Directions::UP)
				Tail.CurrentTexture = Tail.UpTexture;
			if (Tail.Direction == Directions::DOWN)
				Tail.CurrentTexture = Tail.DownTexture;
			if (Tail.Direction == Directions::RIGHT)
				Tail.CurrentTexture = Tail.RightTexture;
			if (Tail.Direction == Directions::LEFT)
				Tail.CurrentTexture = Tail.LeftTexture;

			GL::GLPixelRenderer2D::DrawTexturedQuad(Tail.CurrentTexture, xpos, ypos, CELL_SIZE, CELL_SIZE, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

	void Grow()
	{
		auto TailCell = Body[Body.size() - 1];
		Body.push_back({ TailCell.x, TailCell.y });
		Body[Body.size() - 1].orientation = Body[Body.size() - 2].orientation;
		SetCellOrintation();
	}

	void CheckForDeath()
	{
		auto& head = Body[0];

		for (size_t i = 1; i < Body.size(); i++)
		{
			if (head.x == Body[i].x && head.y == Body[i].y)
			{
				Dead = true;
				break;
			}
		}
	}

	void SetCellOrintation()
	{
		for (size_t i = 1; i < Body.size() - 1; i++)
		{
			auto& CurrentCell = Body[i];
			auto& PrevCell = Body[i - 1];
			auto& NextCell = Body[i + 1];

			if (PrevCell.y == NextCell.y)
				CurrentCell.orientation = Horizontal;

			else if (PrevCell.x == NextCell.x)
				CurrentCell.orientation = Vertical;
			
			else if (PrevCell.x < CurrentCell.x && NextCell.y < CurrentCell.y || NextCell.x < CurrentCell.x && PrevCell.y < CurrentCell.y)
				CurrentCell.orientation = TopLeft;

			else if (PrevCell.x > CurrentCell.x && NextCell.y < CurrentCell.y || NextCell.x > CurrentCell.x && PrevCell.y < CurrentCell.y)
				CurrentCell.orientation = TopRight;

			else if (NextCell.x > CurrentCell.x && PrevCell.y > CurrentCell.y || PrevCell.x > CurrentCell.x && NextCell.y > CurrentCell.y)
				CurrentCell.orientation = BottomRight;
			
			else if (NextCell.x < CurrentCell.x && PrevCell.y > CurrentCell.y || PrevCell.x < CurrentCell.x && NextCell.y > CurrentCell.y)
				CurrentCell.orientation = BottomLeft;
		}
	}

	void Move()
	{
		// set direction

		if (Right && Head.Direction != Directions::LEFT)
		{
			Head.Direction = Directions::RIGHT;
			Head.CurrentTexture = Head.RightTexture;
		}
		else if (Left && Head.Direction != Directions::RIGHT)
		{
			Head.Direction = Directions::LEFT;
			Head.CurrentTexture = Head.LeftTexture;
		}
		else if (Up && Head.Direction != Directions::DOWN)
		{
			Head.Direction = Directions::UP;
			Head.CurrentTexture = Head.UpTexture;
		}
		else if (Down && Head.Direction != Directions::UP)
		{
			Head.Direction = Directions::DOWN;
			Head.CurrentTexture = Head.DownTexture;
		}

		{
			auto oldbody = Body;

			CellPosition& HeadCell = Body[0];
			CellPosition FutureHeadPosition = HeadCell;

			switch (Head.Direction)
			{
			case Snake::Directions::RIGHT: { FutureHeadPosition.x++; } break;
			case Snake::Directions::LEFT: { FutureHeadPosition.x--; } break;
			case Snake::Directions::UP: { FutureHeadPosition.y--; } break;
			case Snake::Directions::DOWN: { FutureHeadPosition.y++; } break;
			default:
				break;
			}

			// collision check
			
			if (FutureHeadPosition.x >= CELL_COUNT ||
				FutureHeadPosition.x < 0 ||
				FutureHeadPosition.y < 0 ||
				FutureHeadPosition.y >= CELL_COUNT)
			{
				Dead = true;
				return;
			}

			// move head
			HeadCell = FutureHeadPosition;

			// move body
			for (size_t i = 1; i < Body.size(); i++)
			{
				CellPosition& BodyPart = Body[i];
				CellPosition PrevBodyPart = oldbody[i - 1];

				BodyPart.x = PrevBodyPart.x;
				BodyPart.y = PrevBodyPart.y;
			}
		}

		CellPosition& HeadCell = Body[0];

		SetCellOrintation();
		UpdateTail();
	}
	void UpdateTail()
	{
		auto& TailCell = Body[Body.size() - 1];
		auto& PrevCell = Body[Body.size() - 2];

		if (TailCell.y < PrevCell.y)
			Tail.Direction = Directions::UP;
		else if (TailCell.y > PrevCell.y)
			Tail.Direction = Directions::DOWN;
		else if (TailCell.x > PrevCell.x)
			Tail.Direction = Directions::RIGHT;
		else if (TailCell.x < PrevCell.x)
			Tail.Direction = Directions::LEFT;
	}

	bool IsDead() { return Dead; }

	void Destroy()
	{
		// Head Assets
		{
			Head.RightTexture.Destroy();
			Head.LeftTexture.Destroy();
			Head.UpTexture.Destroy();
			Head.DownTexture.Destroy();

			Head.CurrentTexture.Destroy();
		}

		// Tail Assets
		{
			Tail.UpTexture.Destroy();
			Tail.DownTexture.Destroy();
			Tail.RightTexture.Destroy();
			Tail.LeftTexture.Destroy();
		}

		// Body Assets
		{
			SnakeBody.Horizontal.Destroy();
			SnakeBody.Vertical.Destroy();
			SnakeBody.TopLeft.Destroy();
			SnakeBody.TopRight.Destroy();
			SnakeBody.BottomLeft.Destroy();
			SnakeBody.BottomRight.Destroy();
		}

		Body.clear();
	}

private:
	std::vector<CellPosition> Body;

	bool Dead = false;
	bool Right, Left, Up, Down = false;
	
	struct _Head
	{
		// textures
		GL::GLTexture RightTexture;
		GL::GLTexture LeftTexture;
		GL::GLTexture UpTexture;
		GL::GLTexture DownTexture;

		// Current Texture
		GL::GLTexture CurrentTexture;

		// Direction
		Directions Direction = Directions::RIGHT;
	}Head;

	struct _Tail
	{
		// Textures
		GL::GLTexture UpTexture;
		GL::GLTexture DownTexture;
		GL::GLTexture RightTexture;
		GL::GLTexture LeftTexture;

		// Current Texture
		GL::GLTexture CurrentTexture;

		// Direction
		Directions Direction = Directions::LEFT;
	}Tail;

	struct _Body
	{
		// Textures
		GL::GLTexture Horizontal;
		GL::GLTexture Vertical;
		GL::GLTexture TopLeft;
		GL::GLTexture TopRight;
		GL::GLTexture BottomLeft;
		GL::GLTexture BottomRight;
	}SnakeBody;

	friend class Food;
};

class Food
{
public:
	Food()
	{
		srand(time(NULL));

		FoodPosition = { 15, 3 };
	}

	void LoadAssets()
	{
		AppleTexture.LoadFromFile("assets/apple.png");
	}

	void Draw()
	{
		float x = FromCellToPixel(FoodPosition.x);
		float y = FromCellToPixel(FoodPosition.y);
		GL::GLPixelRenderer2D::DrawTexturedQuad(AppleTexture, x, y, CELL_SIZE * 1.2f, CELL_SIZE * 1.2f, {1.0f, 1.0f, 1.0f, 1.0f});
	}

	void Update(Snake& Snake)
	{
		CellPosition SnakeHead = Snake.Body[0];

		if (SnakeHead.x == FoodPosition.x && SnakeHead.y == FoodPosition.y)
		{
			do
			{
				FoodPosition.x = rand() % CELL_COUNT;
				FoodPosition.y = rand() % CELL_COUNT;
			} while (FoodOnSnake(Snake));

			Snake.Grow();
		}
	}

	bool FoodOnSnake(Snake& Snake)
	{
		for (size_t i = 0; i < Snake.Body.size(); i++)
		{
			auto& part = Snake.Body[i];
			if (part.x == FoodPosition.x && part.y == FoodPosition.y)
			{
				return true;
			}
		}

		return false;
	}

	void Destroy()
	{
		AppleTexture.Destroy();
	}
	
private:
	CellPosition FoodPosition;
	GL::GLTexture AppleTexture;
};
