#include "HUD.h"


void HUD::Update(void)
{
	if (m_GameOver)
	{

		//Timer for high score
		if (m_HighScoreTimer < p_Timer->Seconds())
		{
			m_HighScoreTimer = m_HighScoreTimerAmount + p_Timer->Seconds();
			m_HighScoreDisplay++;

			if (m_HighScoreDisplay > 1)
				m_HighScoreDisplay = 0;
		}

		// If there is a new high score, save it and write those to the file.
		if (!m_HighscoreUpdated)
		{
			m_HighscoreUpdated = true;

			for (int rank = 0; rank < 10; rank++)
			{
				if (m_PlayerScore > m_HiScores[rank].Score)
				{
					m_NewScore = true;
					m_Done = false;
					m_UpArrowPressed = false;
					m_DownArrowPressed = false;
					m_LeftArrowPressed = false;
					m_RightArrowPressed = false;
					m_FirePressed = false;
					m_HighScoreRank = rank;

					if (rank < 9)
					{
						HiScoreData oldScores[10];

						for (int oldranks = m_HighScoreRank; oldranks < 10; oldranks++)
						{
							oldScores[oldranks].Score = m_HiScores[oldranks].Score;

							for (int name = 0; name < 3; name++)
								oldScores[oldranks].Name[name] = m_HiScores[oldranks].Name[name];
						}

						for (int oldranks = m_HighScoreRank; oldranks < 9; oldranks++)
						{
							m_HiScores[oldranks + 1].Score = oldScores[oldranks].Score;

							for (int name = 0; name < 3; name++)
								m_HiScores[oldranks + 1].Name[name] = oldScores[oldranks].Name[name];
						}
					}

					m_NewHiScore.Score = m_PlayerScore;
					break;
				}
			}
		}

		// If new high score
		if (m_NewScore)
		{
			if (!m_Done)
			{
				if (m_UpArrowPressed)
				{
					m_HighScoreLetterChoice++;
					m_UpArrowPressed = false;

					if (m_HighScoreLetterChoice > 25)
						m_HighScoreLetterChoice = 0;
				}

				if (m_DownArrowPressed)
				{
					m_HighScoreLetterChoice--;
					m_DownArrowPressed = false;

					if (m_HighScoreLetterChoice < 0)
						m_HighScoreLetterChoice = 25;
				}

				if (m_LeftArrowPressed)
				{
					m_HighScoreLetterEditing--;
					m_LeftArrowPressed = false;

					if (m_HighScoreLetterEditing < 0)
						m_HighScoreLetterEditing = 2;
				}

				if (m_RightArrowPressed)
				{
					m_HighScoreLetterEditing++;
					m_RightArrowPressed = false;

					if (m_HighScoreLetterEditing > 2)
						m_HighScoreLetterEditing = 0;
				}

				if (m_FirePressed)
					m_Done = true;

				m_NewHiScore.Name[m_HighScoreLetterEditing] = m_HighScoreLetterChoice;
			}
			else
			{
				m_NewScore = false;
				m_HiScores[m_HighScoreRank] = m_NewHiScore;
				m_HighScoreTimer = p_Timer->Seconds() + m_HighScoreTimerAmount;
				// Save high scores to file.
				std::ofstream file(m_SaveFileName, std::ios::binary);
				file.write((char *)& m_HiScores, sizeof(m_HiScores));
				file.close();

				if (m_HighScoreRank > 4)
					m_HighScoreDisplay = 1;
				else
					m_HighScoreDisplay = 0;
			}
		}
	}
}

void HUD::Draw(void)
{
	ProcessNumber(m_PlayerScore, m_PlayerScoreLocation, 7);
	ProcessNumber(m_PlayerHighScore, m_PlayerHighScoreLocation, 5);

	for (int ship = 0; ship < m_PlayerHitsLeft; ship++)
	{
		float scale = 1;
		float rotation = 4.7123889803846898576939650749193;
		Vector2f location = Vector2f(10 + ship * 10, 30);
		pShip->Update(&rotation, &location, &scale);
		pShip->Draw(&ShipColor);
	}

	// Draw Game Over text
	if (m_GameOver)
	{
		int size = 15;
		int Textsize = m_GameText[0].size();
		int space = float((-size * 3) * (Textsize - 1) / 2);

		for (int letter = 0; letter < Textsize; letter++)
		{
			if ((int)m_GameText[0][letter] > 64 && (int)m_GameText[0][letter] < 91)
				DrawLetter(Vector2i(m_GameOverLocation.x + space, m_GameOverLocation.y), (int)m_GameText[0][letter] - 65, size);

			space += float(size * 2.666);
		}

		//Instructions
		if (!m_NewScore)
		{
			size = 4;

			for (int line = 1; line < 5; line++)
			{
				Textsize = m_GameText[line].size();
				space = float((-size * 3) * (Textsize - 1) / 2);

				for (int letter = 0; letter < Textsize; letter++)
				{
					if ((int)m_GameText[line][letter] > 64 && (int)m_GameText[line][letter] < 91)
						DrawLetter(Vector2i(m_GameTextLocation.x + space, m_GameTextLocation.y + ((line - 1) * 30)),
						(int)m_GameText[line][letter] - 65, size);

					space += size * 3;
				}
			}
		}

		//High score text If game over, and player shot hits something, new score is ignored for high score. Change when game over.
		size = 5;
		Textsize = m_GameText[5].size();
		space = float((-size * 3) * (Textsize - 1) / 1.45);

		for (int letter = 0; letter < Textsize; letter++)
		{
			if ((int)m_GameText[5][letter] > 64 && (int)m_GameText[5][letter] < 91)
				DrawLetter(Vector2i(m_HighScoreListLocaiton.x + space, m_HighScoreListLocaiton.y - 40),
				(int)m_GameText[5][letter] - 65, size);

			space += size * 3;
		}

		//High score list
		for (int line = 0; line < 5; line++)
		{
			space = 0;

			for (int letter = 0; letter < 3; letter++)
			{
				if ((int)m_HiScores[(m_HighScoreDisplay * 5) + line].Name[letter] > -1 &&
					(int)m_HiScores[(m_HighScoreDisplay * 5) + line].Name[letter] < 26)
					DrawLetter(Vector2i(m_HighScoreListLocaiton.x + 40 + space, m_HighScoreListLocaiton.y + (line * 30)),
						m_HiScores[(m_HighScoreDisplay * 5) + line].Name[letter], size);

				space += size * 3;
			}

			ProcessNumber(m_HiScores[(m_HighScoreDisplay * 5) + line].Score,
				Vector2i(m_HighScoreListLocaiton.x, m_HighScoreListLocaiton.y + (line * 30)), 9);

			ProcessNumber((m_HighScoreDisplay * 5) + line + 1, 
				Vector2i(m_HighScoreListLocaiton.x - 160, m_HighScoreListLocaiton.y + (line * 30)), 9);
		}
		// End Game Over text.

		// If new high score
		if (m_NewScore)
		{
			if (!m_Done)
			{
				int displayScoreY = 170;
				size = 5;
				Textsize = m_GameText[6].size();
				space = float((-size * 3) * (Textsize - 1) / 1.45);

				for (int letter = 0; letter < Textsize; letter++)
				{
					if ((int)m_GameText[6][letter] > 64 && (int)m_GameText[6][letter] < 91)
						DrawLetter(Vector2i(m_HighScoreListLocaiton.x + space, m_HighScoreListLocaiton.y + displayScoreY),
						(int)m_GameText[6][letter] - 65, size);

					space += size * 3;
				}

				Textsize = m_GameText[7].size();
				space = float((-size * 3) * (Textsize - 1) / 1.8);

				for (int letter = 0; letter < Textsize; letter++)
				{
					if ((int)m_GameText[7][letter] > 64 && (int)m_GameText[7][letter] < 91)
						DrawLetter(Vector2i(m_HighScoreListLocaiton.x + space, m_HighScoreListLocaiton.y + displayScoreY + 30),
						(int)m_GameText[7][letter] - 65, size);

					space += size * 3;
				}

				//Display high score editing
				space = -45;

				for (int letter = 0; letter < 3; letter++)
				{
						DrawLetter(Vector2i(m_HighScoreListLocaiton.x + space, m_HighScoreListLocaiton.y + displayScoreY + 70),
							m_NewHiScore.Name[letter], size);

					space += size * 3;
				}
			}
		}
	}
}

void HUD::ProcessNumber(int number, Vector2i locationStart, int size)
{
	int space = 0;
	do
	{
		//Make digit the modulus of 10 from number.
		int digit = number % 10;
		//This sends a digit to the draw function with the locaiton and size.
		DrawNumber(Vector2i(locationStart.x - space, locationStart.y), digit, size);
		// Deviding the int by 10, we discard the digit that was derived from the modulus operation.
		number /= 10;
		// Move the location for the next digit locaiton to the left. We start on the right hand side with the lowest digit.
		space += size * 2;
	} while (number > 0);
}

void HUD::SetPlayerScore(int AddToScore)
{
	m_PlayerScore += AddToScore;

	if (m_PlayerScore > m_PlayerHighScore)
		m_PlayerHighScore = m_PlayerScore;

	if (m_PlayerScore > m_NextNewShipScore)
	{
		Mix_PlayChannel(-1, p_Bonus, 0);
		m_PlayerHitsLeft++;
		m_NumberOfNewShips++;
		m_NextNewShipScore += ((100 * m_WaveNumber) * m_NumberOfNewShips) + m_NextNewShipScore;
	}
}

int HUD::GetPlayerHit()
{
	if (m_PlayerHitsLeft > 1)
	{
		m_PlayerHitsLeft--;
	}
	else
	{
		m_PlayerHitsLeft = 0;
	}

	return m_PlayerHitsLeft;
}

void HUD::UpArrowKey(bool KeyDown)
{
	m_UpArrowPressed = KeyDown;
}

void HUD::DownArrowKey(bool KeyDown)
{
	m_DownArrowPressed = KeyDown;
}

void HUD::LeftArrowKey(bool KeyDown)
{
	m_LeftArrowPressed = KeyDown;
}

void HUD::RightArrowKey(bool KeyDown)
{
	m_RightArrowPressed = KeyDown;
}

void HUD::FireKey(bool KeyDown)
{
	m_FirePressed = KeyDown;
}

void HUD::SetGameOver(bool GameOver)
{
	if (GameOver != m_GameOver)
	{
		m_HighScoreDisplay = 0;
		m_HighScoreTimer = p_Timer->Seconds() + m_HighScoreTimerAmount;
	}

	m_GameOver = GameOver;
}

int HUD::NewWave(void)
{
	m_WaveNumber++;
	return m_WaveNumber;
}

void HUD::NewGame(void)
{
	m_PlayerScore = 0;
	m_PlayerHitsLeft = 3;
	m_WaveNumber = 1;
	m_NextNewShipScore = 5000;
	m_GameOver = false;
	m_HighscoreUpdated = false;
}

HUD::HUD()
{
	m_GameText[0] = "GAME OVER";
	m_GameText[1] = "N KEY TO START OR RESTART GAME";
	m_GameText[2] = "ARROW KEYS TO CONTROL SHIP UP FOR THRUST";
	m_GameText[3] = "LEFT CTRL KEY OR SPACE TO FIRE";
	m_GameText[4] = "RIGHT CTRL KEY FOR HYPERSPACE";
	m_GameText[5] = "HIGH SCORE HEROES";
	m_GameText[6] = "NEW HIGH SCORE";
	m_GameText[7] = "ARROW KEYS TO SELECT LETTER FIRE KEY WHEN DONE";

	m_PlayerHighScore = 0;
	m_PlayerHitsLeft = 0;
	m_PlayerScore = 0;
	m_NumberOfNewShips = 0;
	m_HighScoreDisplay = 0;
	m_HighScoreLetterEditing = 0; //What letter is being edited
	m_HighScoreLetterChoice = 0; //The ltter that is being edited, what it is.

	m_GameOver = true;
	m_GameOverLocation.x = Window::GetWindowSize().x / 2;
	m_GameOverLocation.y = Window::GetWindowSize().y / 2 - 200;
	m_GameTextLocation.x = Window::GetWindowSize().x / 2;
	m_GameTextLocation.y = Window::GetWindowSize().y / 2 + 180;
	m_HitsLocation = Vector2i(100, 30);
	m_BottomY = Window::GetWindowSize().y - 30;
	m_PlayerScoreLocation = Vector2i(100, 10);
	m_PlayerHighScoreLocation = Vector2i(Window::GetWindowSize().x / 2, 10);
	m_HighScoreListLocaiton = Vector2i(Window::GetWindowSize().x / 2 + 30, Window::GetWindowSize().y / 2 -60);

	InitializeNumberLine();
	InitializeLetterLine();

	//Player Ship
	pShip = new PlayerShip();
	ShipColor.Red = 165;
	ShipColor.Green = 165;
	ShipColor.Blue = 255;
	ShipColor.Alpha = 255;

	p_Bonus = Mix_LoadWAV("Bonus.wav");

	p_Timer = new Timer();
	p_Timer->Start();
	m_HighScoreTimerAmount = 5;
	m_HighScoreTimer = p_Timer->Seconds() + m_HighScoreTimerAmount;

	//High Scores Setup
	// for loop to set all high scores to zero
	for (int rank = 0; rank < 10; rank++)
	{
		for (int name = 0; name < 3; name++)
		{
			m_HiScores[rank].Name[name] = 0;
		}

		m_HiScores[rank].Score = (rank * -2000) + 20000;
	}

	//Read high scores from file, if exist.
	m_SaveFileName = "HighScores";
	std::ifstream file(m_SaveFileName, std::ios::binary);

	if (file)
	{		
		file.read((char *)&m_HiScores, sizeof(m_HiScores));
	}

	file.close();

	// Temp high score test.
	m_PlayerHighScore = m_HiScores[0].Score;
}

HUD::~HUD(void)
{
}

std::string HUD::IntToString(int number)
{
	std::stringstream tostream;
	tostream << number;
	return tostream.str();
}

void HUD::DrawNumber(Vector2i location, int number, int size)
{
	if (number > -1 && number < 10)
	{
		for (int line = 0; line < 7; line++)
		{
			if (Numbers[number].Lines[line])
			{
				Window::DrawLine(m_NumberLineStart[line] * size + location,
					m_NumberLineEnd[line] * size + location,
					165, 165, 255, 255);
			}
		}
	}
}

void HUD::DrawLetter(Vector2i location, int letter, int size)
{
	if (letter > -1 && letter < 26)
	{
		for (int line = 0; line < 16; line++)
		{
			if (Letters[letter].Lines[line])
			{
				Window::DrawLine(m_LetterLineStart[line] * size + location,
					m_LetterLineEnd[line] * size + location,
					165, 165, 255, 255);
			}
		}
	}
}

void HUD::InitializeNumberLine(void)
{
	// LED Grid locaiton of line start and end points. 0, 0 is the top left corner.
	m_NumberLineStart[0] = Vector2i(0, 0);
	m_NumberLineStart[1] = Vector2i(1, 0);
	m_NumberLineStart[2] = Vector2i(1, 1);
	m_NumberLineStart[3] = Vector2i(0, 2);
	m_NumberLineStart[4] = Vector2i(0, 1);
	m_NumberLineStart[5] = Vector2i(0, 0);
	m_NumberLineStart[6] = Vector2i(0, 1);

	m_NumberLineEnd[0] = Vector2i(1, 0);
	m_NumberLineEnd[1] = Vector2i(1, 1);
	m_NumberLineEnd[2] = Vector2i(1, 2);
	m_NumberLineEnd[3] = Vector2i(1, 2);
	m_NumberLineEnd[4] = Vector2i(0, 2);
	m_NumberLineEnd[5] = Vector2i(0, 1);
	m_NumberLineEnd[6] = Vector2i(1, 1);

	// LED Grid, what lines are on for each number.
	// Line 0 is the top line.
	// Line 1 is upper right side line.
	// Line 2 is lower right side line.
	// Line 3 is bottom line.
	// Line 4 is lower left side line.
	// Line 5 is upper left side line.
	// Line 6 is the middle line.

	Numbers[0].Lines[0] = true;
	Numbers[0].Lines[1] = true;
	Numbers[0].Lines[2] = true;
	Numbers[0].Lines[3] = true;
	Numbers[0].Lines[4] = true;
	Numbers[0].Lines[5] = true;
	Numbers[0].Lines[6] = false;

	Numbers[1].Lines[0] = false;
	Numbers[1].Lines[1] = true;
	Numbers[1].Lines[2] = true;
	Numbers[1].Lines[3] = false;
	Numbers[1].Lines[4] = false;
	Numbers[1].Lines[5] = false;
	Numbers[1].Lines[6] = false;

	Numbers[2].Lines[0] = true;
	Numbers[2].Lines[1] = true;
	Numbers[2].Lines[2] = false;
	Numbers[2].Lines[3] = true;
	Numbers[2].Lines[4] = true;
	Numbers[2].Lines[5] = false;
	Numbers[2].Lines[6] = true;

	Numbers[3].Lines[0] = true;
	Numbers[3].Lines[1] = true;
	Numbers[3].Lines[2] = true;
	Numbers[3].Lines[3] = true;
	Numbers[3].Lines[4] = false;
	Numbers[3].Lines[5] = false;
	Numbers[3].Lines[6] = true;

	Numbers[4].Lines[0] = false;
	Numbers[4].Lines[1] = true;
	Numbers[4].Lines[2] = true;
	Numbers[4].Lines[3] = false;
	Numbers[4].Lines[4] = false;
	Numbers[4].Lines[5] = true;
	Numbers[4].Lines[6] = true;

	Numbers[5].Lines[0] = true;
	Numbers[5].Lines[1] = false;
	Numbers[5].Lines[2] = true;
	Numbers[5].Lines[3] = true;
	Numbers[5].Lines[4] = false;
	Numbers[5].Lines[5] = true;
	Numbers[5].Lines[6] = true;

	Numbers[6].Lines[0] = true;
	Numbers[6].Lines[1] = false;
	Numbers[6].Lines[2] = true;
	Numbers[6].Lines[3] = true;
	Numbers[6].Lines[4] = true;
	Numbers[6].Lines[5] = true;
	Numbers[6].Lines[6] = true;

	Numbers[7].Lines[0] = true;
	Numbers[7].Lines[1] = true;
	Numbers[7].Lines[2] = true;
	Numbers[7].Lines[3] = false;
	Numbers[7].Lines[4] = false;
	Numbers[7].Lines[5] = false;
	Numbers[7].Lines[6] = false;

	Numbers[8].Lines[0] = true;
	Numbers[8].Lines[1] = true;
	Numbers[8].Lines[2] = true;
	Numbers[8].Lines[3] = true;
	Numbers[8].Lines[4] = true;
	Numbers[8].Lines[5] = true;
	Numbers[8].Lines[6] = true;

	Numbers[9].Lines[0] = true;
	Numbers[9].Lines[1] = true;
	Numbers[9].Lines[2] = true;
	Numbers[9].Lines[3] = false;
	Numbers[9].Lines[4] = false;
	Numbers[9].Lines[5] = true;
	Numbers[9].Lines[6] = true;
}

void HUD::InitializeLetterLine(void)
{
	m_LetterLineStart[0] = Vector2i(0, 0); //1
	m_LetterLineStart[1] = Vector2i(1, 0); //2
	m_LetterLineStart[2] = Vector2i(2, 0); //3
	m_LetterLineStart[3] = Vector2i(2, 2); //4
	m_LetterLineStart[4] = Vector2i(1, 4); //5
	m_LetterLineStart[5] = Vector2i(0, 4); //6
	m_LetterLineStart[6] = Vector2i(0, 2); //7
	m_LetterLineStart[7] = Vector2i(0, 0); //8
	m_LetterLineStart[8] = Vector2i(0, 0); //9
	m_LetterLineStart[9] = Vector2i(2, 0); //10
	m_LetterLineStart[10] = Vector2i(1, 2); //11
	m_LetterLineStart[11] = Vector2i(1, 2); //12
	m_LetterLineStart[12] = Vector2i(1, 2); //13
	m_LetterLineStart[13] = Vector2i(0, 2); //14
	m_LetterLineStart[14] = Vector2i(1, 0); //15
	m_LetterLineStart[15] = Vector2i(1, 2); //16

	m_LetterLineEnd[0] = Vector2i(1, 0); //1
	m_LetterLineEnd[1] = Vector2i(2, 0); //2
	m_LetterLineEnd[2] = Vector2i(2, 2); //3
	m_LetterLineEnd[3] = Vector2i(2, 4); //4
	m_LetterLineEnd[4] = Vector2i(2, 4); //5
	m_LetterLineEnd[5] = Vector2i(1, 4); //6
	m_LetterLineEnd[6] = Vector2i(0, 4); //7
	m_LetterLineEnd[7] = Vector2i(0, 2); //8
	m_LetterLineEnd[8] = Vector2i(1, 2); //9
	m_LetterLineEnd[9] = Vector2i(1, 2); //10
	m_LetterLineEnd[10] = Vector2i(2, 2); //11
	m_LetterLineEnd[11] = Vector2i(2, 4); //12
	m_LetterLineEnd[12] = Vector2i(0, 4); //13
	m_LetterLineEnd[13] = Vector2i(1, 2); //14
	m_LetterLineEnd[14] = Vector2i(1, 2); //15
	m_LetterLineEnd[15] = Vector2i(1, 4); //16

	// A
	Letters[0].Lines[0] = true;
	Letters[0].Lines[1] = true;
	Letters[0].Lines[2] = true;
	Letters[0].Lines[3] = true;
	Letters[0].Lines[4] = false;
	Letters[0].Lines[5] = false;
	Letters[0].Lines[6] = true;
	Letters[0].Lines[7] = true;
	Letters[0].Lines[8] = false;
	Letters[0].Lines[9] = false;
	Letters[0].Lines[10] = true;
	Letters[0].Lines[11] = false;
	Letters[0].Lines[12] = false;
	Letters[0].Lines[13] = true;
	Letters[0].Lines[14] = false;
	Letters[0].Lines[15] = false;
	// B
	Letters[1].Lines[0] = true;
	Letters[1].Lines[1] = true;
	Letters[1].Lines[2] = true;
	Letters[1].Lines[3] = true;
	Letters[1].Lines[4] = true;
	Letters[1].Lines[5] = true;
	Letters[1].Lines[6] = false;
	Letters[1].Lines[7] = false;
	Letters[1].Lines[8] = false;
	Letters[1].Lines[9] = false;
	Letters[1].Lines[10] = true;
	Letters[1].Lines[11] = false;
	Letters[1].Lines[12] = false;
	Letters[1].Lines[13] = false;
	Letters[1].Lines[14] = true;
	Letters[1].Lines[15] = true;
	// C
	Letters[2].Lines[0] = true;
	Letters[2].Lines[1] = true;
	Letters[2].Lines[2] = false;
	Letters[2].Lines[3] = false;
	Letters[2].Lines[4] = true;
	Letters[2].Lines[5] = true;
	Letters[2].Lines[6] = true;
	Letters[2].Lines[7] = true;
	Letters[2].Lines[8] = false;
	Letters[2].Lines[9] = false;
	Letters[2].Lines[10] = false;
	Letters[2].Lines[11] = false;
	Letters[2].Lines[12] = false;
	Letters[2].Lines[13] = false;
	Letters[2].Lines[14] = false;
	Letters[2].Lines[15] = false;
	// D
	Letters[3].Lines[0] = true;
	Letters[3].Lines[1] = true;
	Letters[3].Lines[2] = true;
	Letters[3].Lines[3] = true;
	Letters[3].Lines[4] = true;
	Letters[3].Lines[5] = true;
	Letters[3].Lines[6] = false;
	Letters[3].Lines[7] = false;
	Letters[3].Lines[8] = false;
	Letters[3].Lines[9] = false;
	Letters[3].Lines[10] = false;
	Letters[3].Lines[11] = false;
	Letters[3].Lines[12] = false;
	Letters[3].Lines[13] = false;
	Letters[3].Lines[14] = true;
	Letters[3].Lines[15] = true;
	// E
	Letters[4].Lines[0] = true;
	Letters[4].Lines[1] = true;
	Letters[4].Lines[2] = false;
	Letters[4].Lines[3] = false;
	Letters[4].Lines[4] = true;
	Letters[4].Lines[5] = true;
	Letters[4].Lines[6] = true;
	Letters[4].Lines[7] = true;
	Letters[4].Lines[8] = false;
	Letters[4].Lines[9] = false;
	Letters[4].Lines[10] = false;
	Letters[4].Lines[11] = false;
	Letters[4].Lines[12] = false;
	Letters[4].Lines[13] = true;
	Letters[4].Lines[14] = false;
	Letters[4].Lines[15] = false;
	// F
	Letters[5].Lines[0] = true;
	Letters[5].Lines[1] = true;
	Letters[5].Lines[2] = false;
	Letters[5].Lines[3] = false;
	Letters[5].Lines[4] = false;
	Letters[5].Lines[5] = false;
	Letters[5].Lines[6] = true;
	Letters[5].Lines[7] = true;
	Letters[5].Lines[8] = false;
	Letters[5].Lines[9] = false;
	Letters[5].Lines[10] = false;
	Letters[5].Lines[11] = false;
	Letters[5].Lines[12] = false;
	Letters[5].Lines[13] = true;
	Letters[5].Lines[14] = false;
	Letters[5].Lines[15] = false;
	// G
	Letters[6].Lines[0] = true;
	Letters[6].Lines[1] = true;
	Letters[6].Lines[2] = false;
	Letters[6].Lines[3] = true;
	Letters[6].Lines[4] = true;
	Letters[6].Lines[5] = true;
	Letters[6].Lines[6] = true;
	Letters[6].Lines[7] = true;
	Letters[6].Lines[8] = false;
	Letters[6].Lines[9] = false;
	Letters[6].Lines[10] = true;
	Letters[6].Lines[11] = false;
	Letters[6].Lines[12] = false;
	Letters[6].Lines[13] = false;
	Letters[6].Lines[14] = false;
	Letters[6].Lines[15] = false;
	// H
	Letters[7].Lines[0] = false;
	Letters[7].Lines[1] = false;
	Letters[7].Lines[2] = true;
	Letters[7].Lines[3] = true;
	Letters[7].Lines[4] = false;
	Letters[7].Lines[5] = false;
	Letters[7].Lines[6] = true;
	Letters[7].Lines[7] = true;
	Letters[7].Lines[8] = false;
	Letters[7].Lines[9] = false;
	Letters[7].Lines[10] = true;
	Letters[7].Lines[11] = false;
	Letters[7].Lines[12] = false;
	Letters[7].Lines[13] = true;
	Letters[7].Lines[14] = false;
	Letters[7].Lines[15] = false;
	// I
	Letters[8].Lines[0] = true;
	Letters[8].Lines[1] = true;
	Letters[8].Lines[2] = false;
	Letters[8].Lines[3] = false;
	Letters[8].Lines[4] = true;
	Letters[8].Lines[5] = true;
	Letters[8].Lines[6] = false;
	Letters[8].Lines[7] = false;
	Letters[8].Lines[8] = false;
	Letters[8].Lines[9] = false;
	Letters[8].Lines[10] = false;
	Letters[8].Lines[11] = false;
	Letters[8].Lines[12] = false;
	Letters[8].Lines[13] = false;
	Letters[8].Lines[14] = true;
	Letters[8].Lines[15] = true;
	// J
	Letters[9].Lines[0] = false;
	Letters[9].Lines[1] = true;
	Letters[9].Lines[2] = true;
	Letters[9].Lines[3] = true;
	Letters[9].Lines[4] = true;
	Letters[9].Lines[5] = true;
	Letters[9].Lines[6] = true;
	Letters[9].Lines[7] = false;
	Letters[9].Lines[8] = false;
	Letters[9].Lines[9] = false;
	Letters[9].Lines[10] = false;
	Letters[9].Lines[11] = false;
	Letters[9].Lines[12] = false;
	Letters[9].Lines[13] = false;
	Letters[9].Lines[14] = false;
	Letters[9].Lines[15] = false;
	// K
	Letters[10].Lines[0] = false;
	Letters[10].Lines[1] = false;
	Letters[10].Lines[2] = false;
	Letters[10].Lines[3] = false;
	Letters[10].Lines[4] = false;
	Letters[10].Lines[5] = false;
	Letters[10].Lines[6] = true;
	Letters[10].Lines[7] = true;
	Letters[10].Lines[8] = false;
	Letters[10].Lines[9] = true;
	Letters[10].Lines[10] = false;
	Letters[10].Lines[11] = true;
	Letters[10].Lines[12] = false;
	Letters[10].Lines[13] = true;
	Letters[10].Lines[14] = false;
	Letters[10].Lines[15] = false;
	// L
	Letters[11].Lines[0] = false;
	Letters[11].Lines[1] = false;
	Letters[11].Lines[2] = false;
	Letters[11].Lines[3] = false;
	Letters[11].Lines[4] = true;
	Letters[11].Lines[5] = true;
	Letters[11].Lines[6] = true;
	Letters[11].Lines[7] = true;
	Letters[11].Lines[8] = false;
	Letters[11].Lines[9] = false;
	Letters[11].Lines[10] = false;
	Letters[11].Lines[11] = false;
	Letters[11].Lines[12] = false;
	Letters[11].Lines[13] = false;
	Letters[11].Lines[14] = false;
	Letters[11].Lines[15] = false;
	// M
	Letters[12].Lines[0] = false;
	Letters[12].Lines[1] = false;
	Letters[12].Lines[2] = true;
	Letters[12].Lines[3] = true;
	Letters[12].Lines[4] = false;
	Letters[12].Lines[5] = false;
	Letters[12].Lines[6] = true;
	Letters[12].Lines[7] = true;
	Letters[12].Lines[8] = true;
	Letters[12].Lines[9] = true;
	Letters[12].Lines[10] = false;
	Letters[12].Lines[11] = false;
	Letters[12].Lines[12] = false;
	Letters[12].Lines[13] = false;
	Letters[12].Lines[14] = false;
	Letters[12].Lines[15] = false;
	// N
	Letters[13].Lines[0] = false;
	Letters[13].Lines[1] = false;
	Letters[13].Lines[2] = true;
	Letters[13].Lines[3] = true;
	Letters[13].Lines[4] = false;
	Letters[13].Lines[5] = false;
	Letters[13].Lines[6] = true;
	Letters[13].Lines[7] = true;
	Letters[13].Lines[8] = true;
	Letters[13].Lines[9] = false;
	Letters[13].Lines[10] = false;
	Letters[13].Lines[11] = true;
	Letters[13].Lines[12] = false;
	Letters[13].Lines[13] = false;
	Letters[13].Lines[14] = false;
	Letters[13].Lines[15] = false;
	// O
	Letters[14].Lines[0] = true;
	Letters[14].Lines[1] = true;
	Letters[14].Lines[2] = true;
	Letters[14].Lines[3] = true;
	Letters[14].Lines[4] = true;
	Letters[14].Lines[5] = true;
	Letters[14].Lines[6] = true;
	Letters[14].Lines[7] = true;
	Letters[14].Lines[8] = false;
	Letters[14].Lines[9] = false;
	Letters[14].Lines[10] = false;
	Letters[14].Lines[11] = false;
	Letters[14].Lines[12] = false;
	Letters[14].Lines[13] = false;
	Letters[14].Lines[14] = false;
	Letters[14].Lines[15] = false;
	// P
	Letters[15].Lines[0] = true;
	Letters[15].Lines[1] = true;
	Letters[15].Lines[2] = true;
	Letters[15].Lines[3] = false;
	Letters[15].Lines[4] = false;
	Letters[15].Lines[5] = false;
	Letters[15].Lines[6] = true;
	Letters[15].Lines[7] = true;
	Letters[15].Lines[8] = false;
	Letters[15].Lines[9] = false;
	Letters[15].Lines[10] = true;
	Letters[15].Lines[11] = false;
	Letters[15].Lines[12] = false;
	Letters[15].Lines[13] = true;
	Letters[15].Lines[14] = false;
	Letters[15].Lines[15] = false;
	// Q
	Letters[16].Lines[0] = true;
	Letters[16].Lines[1] = true;
	Letters[16].Lines[2] = true;
	Letters[16].Lines[3] = true;
	Letters[16].Lines[4] = true;
	Letters[16].Lines[5] = true;
	Letters[16].Lines[6] = true;
	Letters[16].Lines[7] = true;
	Letters[16].Lines[8] = false;
	Letters[16].Lines[9] = false;
	Letters[16].Lines[10] = false;
	Letters[16].Lines[11] = true;
	Letters[16].Lines[12] = false;
	Letters[16].Lines[13] = false;
	Letters[16].Lines[14] = false;
	Letters[16].Lines[15] = false;
	// R
	Letters[17].Lines[0] = true;
	Letters[17].Lines[1] = true;
	Letters[17].Lines[2] = true;
	Letters[17].Lines[3] = false;
	Letters[17].Lines[4] = false;
	Letters[17].Lines[5] = false;
	Letters[17].Lines[6] = true;
	Letters[17].Lines[7] = true;
	Letters[17].Lines[8] = false;
	Letters[17].Lines[9] = false;
	Letters[17].Lines[10] = true;
	Letters[17].Lines[11] = true;
	Letters[17].Lines[12] = false;
	Letters[17].Lines[13] = true;
	Letters[17].Lines[14] = false;
	Letters[17].Lines[15] = false;
	// S
	Letters[18].Lines[0] = true;
	Letters[18].Lines[1] = true;
	Letters[18].Lines[2] = false;
	Letters[18].Lines[3] = true;
	Letters[18].Lines[4] = true;
	Letters[18].Lines[5] = true;
	Letters[18].Lines[6] = false;
	Letters[18].Lines[7] = true;
	Letters[18].Lines[8] = false;
	Letters[18].Lines[9] = false;
	Letters[18].Lines[10] = true;
	Letters[18].Lines[11] = false;
	Letters[18].Lines[12] = false;
	Letters[18].Lines[13] = true;
	Letters[18].Lines[14] = false;
	Letters[18].Lines[15] = false;
	// T
	Letters[19].Lines[0] = true;
	Letters[19].Lines[1] = true;
	Letters[19].Lines[2] = false;
	Letters[19].Lines[3] = false;
	Letters[19].Lines[4] = false;
	Letters[19].Lines[5] = false;
	Letters[19].Lines[6] = false;
	Letters[19].Lines[7] = false;
	Letters[19].Lines[8] = false;
	Letters[19].Lines[9] = false;
	Letters[19].Lines[10] = false;
	Letters[19].Lines[11] = false;
	Letters[19].Lines[12] = false;
	Letters[19].Lines[13] = false;
	Letters[19].Lines[14] = true;
	Letters[19].Lines[15] = true;
	// U
	Letters[20].Lines[0] = false;
	Letters[20].Lines[1] = false;
	Letters[20].Lines[2] = true;
	Letters[20].Lines[3] = true;
	Letters[20].Lines[4] = true;
	Letters[20].Lines[5] = true;
	Letters[20].Lines[6] = true;
	Letters[20].Lines[7] = true;
	Letters[20].Lines[8] = false;
	Letters[20].Lines[9] = false;
	Letters[20].Lines[10] = false;
	Letters[20].Lines[11] = false;
	Letters[20].Lines[12] = false;
	Letters[20].Lines[13] = false;
	Letters[20].Lines[14] = false;
	Letters[20].Lines[15] = false;
	// V
	Letters[21].Lines[0] = false;
	Letters[21].Lines[1] = false;
	Letters[21].Lines[2] = false;
	Letters[21].Lines[3] = false;
	Letters[21].Lines[4] = false;
	Letters[21].Lines[5] = false;
	Letters[21].Lines[6] = true;
	Letters[21].Lines[7] = true;
	Letters[21].Lines[8] = false;
	Letters[21].Lines[9] = true;
	Letters[21].Lines[10] = false;
	Letters[21].Lines[11] = false;
	Letters[21].Lines[12] = true;
	Letters[21].Lines[13] = false;
	Letters[21].Lines[14] = false;
	Letters[21].Lines[15] = false;
	// W
	Letters[22].Lines[0] = false;
	Letters[22].Lines[1] = false;
	Letters[22].Lines[2] = true;
	Letters[22].Lines[3] = true;
	Letters[22].Lines[4] = false;
	Letters[22].Lines[5] = false;
	Letters[22].Lines[6] = true;
	Letters[22].Lines[7] = true;
	Letters[22].Lines[8] = false;
	Letters[22].Lines[9] = false;
	Letters[22].Lines[10] = false;
	Letters[22].Lines[11] = true;
	Letters[22].Lines[12] = true;
	Letters[22].Lines[13] = false;
	Letters[22].Lines[14] = false;
	Letters[22].Lines[15] = false;
	// X
	Letters[23].Lines[0] = false;
	Letters[23].Lines[1] = false;
	Letters[23].Lines[2] = false;
	Letters[23].Lines[3] = false;
	Letters[23].Lines[4] = false;
	Letters[23].Lines[5] = false;
	Letters[23].Lines[6] = false;
	Letters[23].Lines[7] = false;
	Letters[23].Lines[8] = true;
	Letters[23].Lines[9] = true;
	Letters[23].Lines[10] = false;
	Letters[23].Lines[11] = true;
	Letters[23].Lines[12] = true;
	Letters[23].Lines[13] = false;
	Letters[23].Lines[14] = false;
	Letters[23].Lines[15] = false;
	// Y
	Letters[24].Lines[0] = false;
	Letters[24].Lines[1] = false;
	Letters[24].Lines[2] = false;
	Letters[24].Lines[3] = false;
	Letters[24].Lines[4] = false;
	Letters[24].Lines[5] = false;
	Letters[24].Lines[6] = false;
	Letters[24].Lines[7] = false;
	Letters[24].Lines[8] = true;
	Letters[24].Lines[9] = true;
	Letters[24].Lines[10] = false;
	Letters[24].Lines[11] = false;
	Letters[24].Lines[12] = false;
	Letters[24].Lines[13] = false;
	Letters[24].Lines[14] = false;
	Letters[24].Lines[15] = true;
	// Z
	Letters[25].Lines[0] = true;
	Letters[25].Lines[1] = true;
	Letters[25].Lines[2] = false;
	Letters[25].Lines[3] = false;
	Letters[25].Lines[4] = true;
	Letters[25].Lines[5] = true;
	Letters[25].Lines[6] = false;
	Letters[25].Lines[7] = false;
	Letters[25].Lines[8] = false;
	Letters[25].Lines[9] = true;
	Letters[25].Lines[10] = false;
	Letters[25].Lines[11] = false;
	Letters[25].Lines[12] = true;
	Letters[25].Lines[13] = false;
	Letters[25].Lines[14] = false;
	Letters[25].Lines[15] = false;
}