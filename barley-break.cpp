#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

struct position
{
	unsigned short int x;
	unsigned short int y;
};

enum esituation {empty_to_dest, bad_zone, rotating, last_in_row, last_two_raws};

class field
{
private:
	int FIELD_SIZE;
	vector<vector<unsigned short int>> _field;
	position empty;
	esituation situation;
    int vector, steps, width, height, bone;
public:
	bool delay;
	field(int size = 4)
	{
		FIELD_SIZE = size;
		_field.resize(FIELD_SIZE);
		for (int i = 0 ; i < FIELD_SIZE ; i++)
		{
			_field[i].resize(FIELD_SIZE);
		}
		situation = empty_to_dest;
        vector = 1;
        steps = 0;
		bone = 1;
	}  

	position pos_calculate(int bone)
	{
		position p_bone;
		for (int i = 0 ; i <= FIELD_SIZE - 1 ; i++)
		{
			for (int j = 0 ; j <= FIELD_SIZE - 1 ; j++)
			{
				if (_field[i][j] == bone)
				{
					p_bone.x = j;
					p_bone.y = i;
					break;
				}
			}
		}
		return p_bone;
	}

	position dest_calculate(int bone)
	{
		position p_dest;
		p_dest.x = (bone - 1) % FIELD_SIZE;
        p_dest.y = (bone - 1) / FIELD_SIZE;
		return p_dest;
	}

	void generate()
	{
		unsigned short int i, j;
		for (i = 0 ; i <= FIELD_SIZE - 1 ; i++)
		{
			for (j = 0 ; j <= FIELD_SIZE - 1 ; j++)
			{
				_field[j][i] = 0;
			}
		}
		for (i = 1 ; i <= FIELD_SIZE * FIELD_SIZE - 1 ; )
		{
			unsigned short int x = rand() % FIELD_SIZE, y = rand() % FIELD_SIZE;
			if (!_field[x][y])
			{
				_field[x][y] = i++;
			}
		}
		for (i = 0 ; i <= FIELD_SIZE - 1 ; i++)
		{
			for (j = 0 ; j <= FIELD_SIZE - 1 ; j++)
			{
				if (!_field[i][j])
				{
					empty.x = j;
					empty.y = i;
				}
			}
		}
	}

	bool check_win()
    {
		if (bone == FIELD_SIZE * (FIELD_SIZE - 1))
		{
			return true;
		}
		else
		{
			return false;
		}
    }

	bool check_shape()
	{
		unsigned short int temp, now, summ = 0;
		for (int i = 0 ; i <= FIELD_SIZE - 1 ; i++)
		{
			for (int j = 0 ; j <= FIELD_SIZE - 1 ; j++)
			{
				now = _field[i][j];
				for (int q = i ; q <= FIELD_SIZE - 1 ; q++)
				{
					temp = j;
					if (q > i)
					{
						temp = 0;
					}
					for (int z = temp ; z <= FIELD_SIZE - 1 ; z++)
					{

						if ((_field[q][z]) && (_field[q][z] < now))
						{
							summ++;
						}
					}
				}
			}
		}
		summ += empty.y;
		if (summ % 2)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void print()
	{
		for (int i = 0; i < FIELD_SIZE; i++)
		{
			for (int j = 0 ; j < FIELD_SIZE; j++)
			{
				if (_field[i][j])
				{
					printf("%s%2d%s", "[", _field[i][j], "] ");
				}
				else
				{
					printf("%4s", "[  ] ");
				}
			}
			printf("\n\n");
		}
	}

	void moveup()
	{
		if (empty.y)
		{
			swap(_field[empty.y][empty.x], _field[empty.y - 1][empty.x]);
			empty.y = empty.y--;
			system("cls");
			print();
		}
	}

	void movedown()
	{
		if (empty.y < FIELD_SIZE - 1)
		{
			swap(_field[empty.y][empty.x], _field[empty.y + 1][empty.x]);
			empty.y = empty.y++;
			system("cls");
			print();
		}
	}

	void moveright()
	{
		if (empty.x < FIELD_SIZE - 1)
		{
			swap(_field[empty.y][empty.x], _field[empty.y][empty.x + 1]);
			empty.x = empty.x++;
			system("cls");
			print();
		}
	}

	void moveleft()
	{
		if (empty.x)
		{
			swap(_field[empty.y][empty.x], _field[empty.y][empty.x - 1]);
			empty.x = empty.x--;
			system("cls");
			print();
		}
	}

	bool move_empty()
	{
		char c;
		c = shag();
		if (empty.x > dest_calculate(bone).x)
			{
                moveleft();
                return false;
			}
		if (empty.x < dest_calculate(bone).x)
			{
                moveright();
                return false;
			}
		if (empty.y < dest_calculate(bone).y)
			{
                movedown();
                return false;
			}
		if (empty.y > dest_calculate(bone).y)
			{
                moveup();
                return false;
			}
		return true;
	}

	bool bad_zone_movedown()
	{
		position dest = dest_calculate(bone);
		movedown();
		if ((pos_calculate(bone).x == dest_calculate(bone).x) && (pos_calculate(bone).y == dest_calculate(bone).y) && !(bone % FIELD_SIZE))
			{
				bone++;
				situation = empty_to_dest;
				return false;
			}
		dest.y++;
		rotate1(dest);
		return true;
	}

	void rotate2(position dest)
	{
		char c;
		width = abs(pos_calculate(bone + FIELD_SIZE).x - dest.x);
		height = abs(pos_calculate(bone + FIELD_SIZE).y - dest.y);
		if (width == 0)
			width++;
		if (height == 0)
			height++;
		steps = 0;
		while (!((pos_calculate(bone + FIELD_SIZE).x == dest.x) && (pos_calculate(bone + FIELD_SIZE).y == dest.y)))
		{
			c = shag();
			
			if (vector % 2 == 0)
			{
				if (steps == width) 
				{
		            vector++;
					steps = 0;
				}
			} 
			else
			{	
			    if (steps == height) 
				{
					vector++;
					steps = 0;
				}
			}	
			if (vector > 4)
			{
				vector = 1;
			}
				switch (vector)
				{
					case 1: {movedown();   break;}
					case 2: {moveright();    break;}
					case 3: {moveup();    break;}
					case 4: {moveleft();      break;}
				}
			steps++;
		}
	}

	void rotate1(position dest)
	{
		char c;
		width = abs(pos_calculate(bone).x - dest.x);
		height = abs(pos_calculate(bone).y - dest.y);
		if (width == 0)
			width++;
		if (height == 0)
			height++;
		steps = 0;
		while (!((pos_calculate(bone).x == dest.x) && (pos_calculate(bone).y == dest.y)))
		{
			c = shag();
			
			if (vector % 2 == 0)
			{
				if (steps == width) 
				{
		            vector++;
					steps = 0;
				}
			} 
			else
			{	
			    if (steps == height) 
				{
					vector++;
					steps = 0;
				}
			}	
			if (vector > 4)
			{
				vector = 1;
			}
				switch (vector)
				{
					case 1: {movedown();   break;}
					case 2: {moveleft();    break;}
					case 3: {moveup();    break;}
					case 4: {moveright();      break;}
				}
			steps++;
		}
	}

	void rotate(position dest)
	{
		char c;
		width = abs(pos_calculate(bone).x - dest.x);
		height = abs(pos_calculate(bone).y - dest.y);
		if (width == 0)
			width++;
		if (height == 0)
			height++;
		steps = 0;
		while (!((pos_calculate(bone).x == dest.x) && (pos_calculate(bone).y == dest.y)))
		{
			c = shag();		
			if (vector % 2 == 1)
			{
				if (steps == width) 
				{
		            vector++;
					steps = 0;
				}
			} 
			else
			{	
			    if (steps == height) 
				{
					vector++;
					steps = 0;
				}
			}	
			if (vector > 4)
			{
				vector = 1;
			}
			switch (vector)
			{
				case 1: {moveright();   break;}
				case 2: {movedown();    break;}
				case 3: {moveleft();    break;}
				case 4: {moveup();      break;}
			}
			steps++;
		}
	}

	void steps_emulate(char *command) 
	{
		int i = 0;
		char c;
		while (command[i] != '\0') 
		{
			c = shag();
	        switch(command[i]) {
				case 'l' : {moveleft();break;}
				case 'u' : {moveup();break;}
				case 'd' : {movedown();break;}
				case 'r' : {moveright();break;}
			}
			i++;
		}
	}

	void step() 
	{
		char c;
		c = shag();
		while (empty.x > 0)
		{
			c = shag();
			moveleft();
		}
		while (empty.y > bone / FIELD_SIZE - 1)
		{
			c = shag();
			moveup();
		}
		while (empty.x < FIELD_SIZE - 1)
		{
			c = shag();
			moveright();
		}
		c = shag();
		movedown();
		c = shag();
		moveleft();
		c = shag();
		moveup();
		while (empty.x > 0)
		{
			c = shag();
			moveleft();
		}
		c = shag();
		movedown();
	}

	void solve()
	{
		if (!(bone % FIELD_SIZE))
		{
			situation = last_in_row;
		}
		if (bone > (FIELD_SIZE - 2) * FIELD_SIZE)
		{
			situation = last_two_raws;
		}
		switch (situation)
		{
		case last_in_row:
			{
				if (bad_zone_movedown())
				{
					step();
					bone++;
					situation = empty_to_dest;
					break;
				}
			}
		case empty_to_dest:
			{
				if (move_empty())
				{
					situation = bad_zone;
				}
				else
				{
					break;
				}				
			}
		case bad_zone:
			{
				if (pos_calculate(bone).x < dest_calculate(bone).x)
				{
					bad_zone_movedown();
					situation = empty_to_dest;
					break;
				}
				else
				{
					situation = rotating;
				}
			}
		case rotating:
			{
				rotate(dest_calculate(bone));
				bone++;
				situation = empty_to_dest;
				break;
			}
		case last_two_raws:
			{
				rotate(dest_calculate(bone));
				steps_emulate("dlur");
				if ((dest_calculate(bone).x == pos_calculate(bone + FIELD_SIZE).x) && (dest_calculate(bone).y == pos_calculate(bone + FIELD_SIZE).y))
				{
					steps_emulate("ldrruldlur");
				}
				rotate2(dest_calculate(bone + 1));
				steps_emulate("uldr");
				bone++;
				move_empty();
				break;
			}
		}
	}

	char shag()
	{
		if (!check_win())
		{
			if (!delay)
			{
				Sleep(50);
				return ' ';
			}
			else
			{
				char command = getch();
				return command;
			}
		}
		else
		{
			cout<<"WIN!!!!"<<endl;
			system("pause");
		}
	}
};

int main()
{
	srand(time(0));
	field pole(4);
	bool delay;
	do
	{
		pole.generate();
	} 
	while (pole.check_shape());
	cout<<"Auto(0) ili manual(chto nibud' drugoe)?"<<endl;
	cin>>pole.delay;
	pole.print();
	char command;
	do
	{
		command = pole.shag();
        switch (command)
		{
		case ' ':
			{
				pole.solve(); 
				break;
			}
		}
	}
	while (!pole.check_win());
	return 0;
}

