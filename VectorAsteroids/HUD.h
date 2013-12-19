#pragma once
#include <iomanip>
#include <locale>
#include <fstream>
#include "Window.h"
#include "Timer.h"
#include "PlayerShip.h"

struct NumberData
{
	bool Lines[7];
};

struct LetterData
{
	bool Lines[16];
};

struct HiScoreData
{
	int Name[3];
	unsigned int Score;
};

class HUD
{
private:
	std::string m_GameText[8];
	std::string m_SaveFileName;
	int m_GameOverLetters[9];
	int m_BottomY;
	Vector2i m_HitsLocation;
	Vector2i m_GameOverLocation;
	Vector2i m_GameTextLocation;
	unsigned int m_PlayerScore;
	Vector2i m_PlayerScoreLocation;
	unsigned int m_PlayerHighScore;
	Vector2i m_PlayerHighScoreLocation;
	Vector2i m_HighScoreListLocaiton;
	unsigned int m_NextNewShipScore;
	HiScoreData m_HiScores[10];
	HiScoreData m_NewHiScore;
	int m_HighScoreLetterEditing; //What letter is being edited
	int m_HighScoreLetterChoice; //The ltter that is being edited, what it is.
	int m_HighScoreRank; //The place where the new high score ranks at.
	float m_HighScoreTimer;
	float m_HighScoreTimerAmount;
	int m_HighScoreDisplay;
	int m_NumberOfNewShips;
	int m_PlayerHitsLeft;
	int m_WaveNumber = 1;
	bool m_GameOver;
	bool m_HighscoreUpdated = false;
	bool m_NewScore = false;
	bool m_Done = false;

	// High Score List Key swtiches
	bool m_LeftArrowPressed;
	bool m_RightArrowPressed;
	bool m_UpArrowPressed;
	bool m_DownArrowPressed;
	bool m_FirePressed;

	// Player Ship
	PlayerShip *pShip;
	Color ShipColor;

	//New number lines
	NumberData Numbers[10];
	Vector2i m_NumberLineStart[7];
	Vector2i m_NumberLineEnd[7];
	void InitializeNumberLine(void);
	LetterData Letters[26];
	Vector2i m_LetterLineStart[16];
	Vector2i m_LetterLineEnd[16];
	void InitializeLetterLine(void);
	void ProcessNumber(int number, Vector2i locationStart, int size);
	void DrawNumber(Vector2i location, int number, int size);
	void DrawLetter(Vector2i location, int letter, int size);

	std::string IntToString(int number);

	Mix_Chunk *p_Bonus;

	Timer *p_Timer;

public:
	HUD(void);
	~HUD(void);

	void Update(void);
	void Draw(void);
	void NewGame(void);
	void SetPlayerScore(int AddToScore);
	void SetGameOver(bool GameOver);
	void UpArrowKey(bool KeyDown);
	void DownArrowKey(bool KeyDown);
	void LeftArrowKey(bool KeyDown);
	void RightArrowKey(bool KeyDown);
	void FireKey(bool KeyDown);
	int GetPlayerHit();
	int NewWave(void);
};

