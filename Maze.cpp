#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

int random(int size)
{
	return rand() % size;
}

bool isEqual(int a, int b)
{
	if (a == b)
		return true;
	else 
		return false;
}

class bloc
{
public:
	bloc()
	{
		num = 0;										// numero du bloc
		hex = 0xF;										// nombre hex a 1 bit car en binaire cela donne 1111, et chaque 1 represente un mur (L,B,R,T)
		
	}
	bloc(int j)
	{
		num = j;										// numero du bloc
		hex = 0xF;										// nombre hex a 1 bit car en binaire cela donne 1111, et chaque 1 represente un mur (L,B,R,T)
		sequence = -1;									// la sequence de labyrinthe auquel il apartient
	}
	int		num;										// numero du bloc
	short	hex;										// nombre hex a 1 bit car en binaire cela donne 1111, et chaque 1 represente un mur (L,B,R,T)
	int		weight;										// combien d'elements dans la sequence de labyrinthe
	vector<bloc> maze;
	int		sequence;									// la sequence du labyrinth auquel il appartient a

	bool rightEdge(int number, int longeur)				// checker si le bloc est sur le bord droit
		{
			if ((number + 1)% longeur == 0)
				return true;
			else 
				return false;
		}
		bool leftEdge(int number, int longeur)			// checker si le bloc est sur le bord gauche
		{
			if (number % longeur == 0)
				return true;
			else 
				return false;
		}
		bool topEdge(int number, int longeur)			// checker si le bloc est sur la ligne du haut
		{
			if (number < longeur)
				return true;
			else 
				return false;
		}
		bool bottomEdge(int number, int longeur, int hauteur) // checker si le bloc est sur la ligne du bas
		{
			if (number > ((hauteur*longeur) - longeur) - 1)
				return true;
			else 
				return false;
		}
};

class arrayOfMazes
{
private:
	vector<bloc> mazeVector;
	short compteur;
	
public:
	bool isMaze(int longeur, int hauteur) 
	{
		for (int i = 1; i < longeur*hauteur ; i++)
		{
			if (mazeVector[0].sequence != mazeVector[i].sequence || mazeVector[0].sequence == -1 || mazeVector[i].sequence == -1)
				return false;

		}
		return true;
	}

	arrayOfMazes(int size)											// cree un nombre de bloc egal a size pour former le labyrinth
	{
		compteur = 0;
		for (int i = 0; i < size; i++)
		{
			bloc* temp = new bloc(i);
			mazeVector.push_back(*temp);
		}

	}

	void mergeBloc(int rand, int longeur, int hauteur)
	{
		int wall = random(4);
		switch (wall)
		{
			case 0: 
			{
				if(mazeVector[rand].topEdge(rand,longeur))
					break;
				else
				{
					int bit1 = mazeVector[rand].hex;										// take hex of our bloc
					int bit2 = mazeVector[rand - longeur].hex;								// take hex of bloc above it
					bit1 &= 0x1;															// take 1st bit (top wall)
					bit2 &= 0x4;															// take 3rd bit (bottom wall)
					bit2 >>= 2;
					if (bit1 & bit2)														// if equal 1 then destroy walls
					{
						if ((mazeVector[rand].sequence == -1 && mazeVector[rand - longeur].sequence == -1))									// tout les deux dans des ensembles disjoints simples
						{
							mazeVector[rand].hex ^= 0x1;										// toggle 1st bit
							mazeVector[rand - longeur].hex ^= 0x4;								// toggle 3rd bit
							mazeVector[rand].sequence = compteur;								// union des deux dans un nouvel ensemble
							mazeVector[rand - longeur].sequence = compteur;						// union des deux dans un nouvel ensemble
							compteur++;															// monter le compteur de nouvel ensemble pour que le prochain na pas le meme nombre d'ensemble
						}
						else
							if (mazeVector[rand].sequence != mazeVector[rand - longeur].sequence)											// si les deux bloc ne font pas partit du meme labyrinthe
							{
								mazeVector[rand].hex ^= 0x1;										// toggle 1st bit
								mazeVector[rand - longeur].hex ^= 0x4;								// toggle 3rd bit
								if (mazeVector[rand].sequence < mazeVector[rand - longeur].sequence && mazeVector[rand].sequence != -1)		// si sequence de bloc rand plus petit que sequence rand-longeur mais est different de -1
								{
									int temp = mazeVector[rand].sequence;																	
									for (int i = 0; i < (longeur * hauteur); i++)															// parcourir tous les bloc pour voir quelle sont dans la meme sequence
									{
										if (mazeVector[i].sequence == temp)																	
											mazeVector[i].sequence = mazeVector[rand - longeur].sequence;									// remplacer tous les sequence par celle du bloc plus grand
									}

								}
								else
									if (mazeVector[rand].sequence == -1)																	// si sequence du bloc est ensemble disjoint
									{
										mazeVector[rand].sequence = mazeVector[rand - longeur].sequence;									// remplacer par sequence du bloc le plus grand
									}
									else
										if (mazeVector[rand].sequence > mazeVector[rand - longeur].sequence && mazeVector[rand - longeur].sequence != -1)// si sequence de bloc rand plus grand que sequence rand-longeur et rand longewur sequence est different de -1
										{
											int temp = mazeVector[rand - longeur].sequence;
											for (int i = 0; i < (longeur * hauteur); i++)
											{
												if (mazeVector[i].sequence == temp)															// tout le bloc ayant meme sequence que rand- longeur
													mazeVector[i].sequence = mazeVector[rand].sequence;										// = sequence de bloc rand
											}

										}
										else
											if (mazeVector[rand - longeur].sequence == -1)													// si rand-longeur sequence est ensemble disjoint
											{
												mazeVector[rand - longeur].sequence = mazeVector[rand].sequence;							// rand-longeur sequence = rand sequence
											}
								

							}
			
					}

				}
				break;
				
			}
			case 1:
			{
				if (mazeVector[rand].rightEdge(rand, longeur))
					break;
				else
				{
					int bit1 = mazeVector[rand].hex;										// take hex of our bloc
					int bit2 = mazeVector[rand + 1].hex;									// take hex of bloc right of it
					bit1 &= 0x2;															// take 2nd bit (right wall)
					bit2 &= 0x8;															// take 4th bit (left wall)
					bit2 >>= 2;
					if (bit1 & bit2)													// if equals 2 then we can destroy walls
					{
						if ((mazeVector[rand].sequence == -1 && mazeVector[rand + 1].sequence == -1))									// tout les deux dans des ensembles disjoints simples
						{
							mazeVector[rand].hex ^= 0x2;										// toggle 2nd bit
							mazeVector[rand + 1].hex ^= 0x8;									// toggle 4th bit
							mazeVector[rand].sequence = compteur;								// union des deux dans un nouvel ensemble
							mazeVector[rand + 1].sequence = compteur;							// union des deux dans un nouvel ensemble
							compteur++;															// monter le compteur de nouvel ensemble pour que le prochain na pas le meme nombre d'ensemble														
						}
						else
							if(mazeVector[rand].sequence != mazeVector[rand + 1].sequence)
							{
								mazeVector[rand].hex ^= 0x2;										// toggle 2nd bit
								mazeVector[rand +1 ].hex ^= 0x8;									// toggle 4th bit
								if (mazeVector[rand].sequence < mazeVector[rand + 1].sequence && mazeVector[rand].sequence != -1)
								{
									int temp = mazeVector[rand].sequence;
									for (int i = 0; i < (longeur * hauteur); i++)															// parcourir tous les bloc pour voir quelle sont dans la meme sequence
									{
										if (mazeVector[i].sequence == temp)
											mazeVector[i].sequence = mazeVector[rand + 1].sequence;											// remplacer tous les sequence par celle du bloc plus grand
									}
								}
								else
									if (mazeVector[rand].sequence == -1)																	// si sequence du bloc est ensemble disjoint
									{
										mazeVector[rand].sequence = mazeVector[rand + 1].sequence;											// remplacer par sequence du bloc le plus grand
									}
									else
										if (mazeVector[rand].sequence > mazeVector[rand + 1].sequence && mazeVector[rand + 1].sequence != -1)// si sequence de bloc rand plus grand que sequence rand+1 et rand+1 sequence est different de -1
										{
											int temp = mazeVector[rand + 1].sequence;
											for (int i = 0; i < (longeur * hauteur); i++)
											{
												if (mazeVector[i].sequence == temp)															// tout le bloc ayant meme sequence que rand+1
													mazeVector[i].sequence = mazeVector[rand].sequence;										// = sequence de bloc rand
											}

										}
										else
											if (mazeVector[rand + 1].sequence == -1)													// si rand-longeur sequence est ensemble disjoint
											{
												mazeVector[rand + 1].sequence = mazeVector[rand].sequence;								// rand+1 sequence = rand sequence
											}
							}
						
					}

				}
				break;
			}
			case 2:
			{
				if (mazeVector[rand].bottomEdge(rand, longeur, hauteur))
					break;
				else
				{
					int bit1 = mazeVector[rand].hex;										// take hex of our bloc
					int bit2 = mazeVector[rand + longeur].hex;								// take hex of bloc under it
					bit1 &= 0x4;															// take 3rd bit (bottom wall)
					bit2 &= 0x1;															// take 1st bit (top wall)
					bit2 <<= 2;
					if (bit1 & bit2)														// if both 1 then destroy walls
					{
						if ((mazeVector[rand].sequence == -1 && mazeVector[rand + longeur].sequence == -1))									// tout les deux dans des ensembles disjoints simples
						{
							mazeVector[rand].hex ^= 0x4;										// toggle 3rd bit
							mazeVector[rand + longeur].hex ^= 0x1;									// toggle 1st bit
							mazeVector[rand].sequence = compteur;								// union des deux dans un nouvel ensemble
							mazeVector[rand + longeur].sequence = compteur;							// union des deux dans un nouvel ensemble
							compteur++;															// monter le compteur de nouvel ensemble pour que le prochain na pas le meme nombre d'ensemble														
						}
						else
							if (mazeVector[rand].sequence != mazeVector[rand + longeur].sequence)
							{
								mazeVector[rand].hex ^= 0x4;										// toggle 3rd bit
								mazeVector[rand + longeur].hex ^= 0x1;									// toggle 1st bit
								if (mazeVector[rand].sequence < mazeVector[rand + longeur].sequence && mazeVector[rand].sequence != -1)
								{
									int temp = mazeVector[rand].sequence;
									for (int i = 0; i < (longeur * hauteur); i++)															// parcourir tous les bloc pour voir quelle sont dans la meme sequence
									{
										if (mazeVector[i].sequence == temp)
											mazeVector[i].sequence = mazeVector[rand + longeur].sequence;											// remplacer tous les sequence par celle du bloc plus grand
									}
								}
								else
									if (mazeVector[rand].sequence == -1)
									{
										mazeVector[rand].sequence = mazeVector[rand + longeur].sequence;
									}
									else
										if (mazeVector[rand].sequence > mazeVector[rand + longeur].sequence && mazeVector[rand + longeur].sequence != -1)// si sequence de bloc rand plus grand que sequence rand+longeur et rand+longeur sequence est different de -1
										{
											int temp = mazeVector[rand + longeur].sequence;
											for (int i = 0; i < (longeur * hauteur); i++)
											{
												if (mazeVector[i].sequence == temp)															// tout le bloc ayant meme sequence que rand + longeur
													mazeVector[i].sequence = mazeVector[rand].sequence;										// = sequence de bloc rand
											}

										}
										else
											if (mazeVector[rand + longeur].sequence == -1)													// si rand+longeur sequence est ensemble disjoint
											{
												mazeVector[rand + longeur].sequence = mazeVector[rand].sequence;								// rand+longeur sequence = rand sequence
											}
							}
						
					}

				}
				break;
			}
			case 3:
			{
				if (mazeVector[rand].leftEdge(rand, longeur))
					break;
				else
				{
					int bit1 = mazeVector[rand].hex;										// take hex of our bloc
					int bit2 = mazeVector[rand - 1].hex;									// take hex of bloc above it
					bit1 &= 0x8;															// take 4th bit (left wall)
					bit2 &= 0x2;															// take 2nd bit (right wall)
					bit2 <<= 2;
					if (bit1 & bit2)														// if both a 1 we can destroy
					{
						if ((mazeVector[rand].sequence == -1 && mazeVector[rand - 1].sequence == -1))									// tout les deux dans des ensembles disjoints simples
						{
							mazeVector[rand].hex ^= 0x8;										// toggle 4th bit
							mazeVector[rand - 1].hex ^= 0x2;									// toggle 2nd bit
							mazeVector[rand].sequence = compteur;								// union des deux dans un nouvel ensemble
							mazeVector[rand - 1].sequence = compteur;							// union des deux dans un nouvel ensemble
							compteur++;															// monter le compteur de nouvel ensemble pour que le prochain na pas le meme nombre d'ensemble														
						}
						else
							if (mazeVector[rand].sequence != mazeVector[rand - 1].sequence)
							{
								mazeVector[rand].hex ^= 0x8;										// toggle 4th bit
								mazeVector[rand - 1].hex ^= 0x2;									// toggle 2nd bit
								if (mazeVector[rand].sequence < mazeVector[rand - 1].sequence && mazeVector[rand].sequence != -1)
								{
									int temp = mazeVector[rand].sequence;
									for (int i = 0; i < (longeur * hauteur); i++)															// parcourir tous les bloc pour voir quelle sont dans la meme sequence
									{
										if (mazeVector[i].sequence == temp)
											mazeVector[i].sequence = mazeVector[rand - 1].sequence;											// remplacer tous les sequence par celle du bloc plus grand
									}
								}
								else
									if (mazeVector[rand].sequence == -1)
									{
										mazeVector[rand].sequence = mazeVector[rand - 1].sequence;
									}
									else
										if (mazeVector[rand].sequence > mazeVector[rand - 1].sequence && mazeVector[rand - 1].sequence != -1)// si sequence de bloc rand plus grand que sequence rand-1 et rand-1 sequence est different de -1
										{
											int temp = mazeVector[rand - 1].sequence;
											for (int i = 0; i < (longeur * hauteur); i++)
											{
												if (mazeVector[i].sequence == temp)															// tout le bloc ayant meme sequence que rand-1
													mazeVector[i].sequence = mazeVector[rand].sequence;										// = sequence de bloc rand
											}

										}
										else
											if (mazeVector[rand - 1].sequence == -1)													// si rand-1 sequence est ensemble disjoint
											{
												mazeVector[rand - 1].sequence = mazeVector[rand].sequence;								// rand-1 sequence = rand sequence
											}


							}

					}
				}
				break;
			}
			
		}
	}

	void print(int size, int longeur, int hauteur) // imprimer le labyrinth
	{
		mazeVector[size - 1].hex ^= 0x4;
		int x = 1;
		int y = 0;
		int z = 0;
		cout << "+  +";
		for (; x < longeur; x++)
		{
			cout << "--+";
		}
		cout << endl;
		for (int level = 1; level <= hauteur; level ++)
		{
			cout << "|";
			for (; y < longeur*level; y++) // rightwall
			{
				int bit = mazeVector[y].hex;
				bit &= 0x2;
				if (bit == 2)
				{
					cout << "  |";
				}
				else
					cout << "   ";
			}
			cout << endl;
			cout << "+";
			for (; z < longeur*level; z++) // bottom 
			{
				int bit = mazeVector[z].hex;
				bit &= 0x4;
				if (bit == 4)
				{
					cout << "--+";
				}
				else
					cout << "  +";
			}
			cout << endl;
		}

	}
};

int main()
{
	srand(time(NULL));
	int hauteur = 4;
	int longeur = 4;
	cout << "hauteur?" << endl;
	cout << "longeur?" << endl;
	cin >> hauteur;
	cin >> longeur;
	int size = hauteur * longeur;
	arrayOfMazes a(size);
	while(!a.isMaze(longeur, hauteur))
	{
		int rand = random(size);
		a.mergeBloc(rand, longeur, hauteur);

	}
	
	a.print(size, longeur, hauteur);

	return 0;
}
