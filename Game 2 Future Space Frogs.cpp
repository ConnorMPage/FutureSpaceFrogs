// Game 2 Future Space Frogs.cpp: A program using the TL-Engine
//by Connor Michael Page
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <sstream>
using namespace tle;

enum EInPlay { Playing, Paused, Over};// the game states for the entire game
enum EFrogMovement { Waiting, Moving, Dead, Escaped};// states of all frog movement 
enum EWaterSafety { Safe, Poisoned};// this shows if the water is safe or not

bool carToFrogCollision(IModel* Car, float boxLength, float boxWidth, IModel* frog, float sphereRadius);// this function is used to detect box to sphere collisions between the car and frogs
bool carToWaterCollision(IModel* car, float boxLength, float boxWidth, IModel* water, float waterRadius);//this detects box tox collision between the car and the water
bool frogToWaterCollision(IModel* island, float islandRadius, IModel* frog, float sphereRadius);// this detects collisions between the frogs and the water
bool carToPortalCollision(IModel* car, float boxLength, float boxWidth, IModel* portal, float portalRadius);//detects collisons between the car and portal/plants


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	myEngine->AddMediaFolder(".\\media");


	/**** Set up your scene here ****/
	//----------------------------------------------------------------------
	// Basic Scene Layout

	IMesh* skyboxMesh = myEngine->LoadMesh("skybox.x");
	IModel* skyBox = skyboxMesh->CreateModel(0, -960, 0);//this is the skybox, the background seen from 1st and 3rd person perspective

	IMesh* waterFloorMesh = myEngine->LoadMesh("surface.x");
	IModel* waterFloor = waterFloorMesh->CreateModel(0, -5, 0);// this is the water area around the island

	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IModel* island = islandMesh->CreateModel(0, -6, 0);// the main island where the game is played

	ICamera* gameCamera = myEngine->CreateCamera(kManual);
	gameCamera->SetLocalPosition(0, 120, -100);
	gameCamera->RotateLocalX(50);//the camera starts off in the birds eye view location

	IMesh* carMesh = myEngine->LoadMesh("rover.x");
	IModel* car = carMesh->CreateModel(0, 0, -50);// this the player

	IMesh* frogMesh = myEngine->LoadMesh("frog.x");

	IModel* frog1 = frogMesh->CreateModel(5, 0, 0);
	frog1->RotateLocalY(90);//frog 1 faces the right

	IModel* frog2 = frogMesh->CreateModel(-5, 0, 0);
	frog2->RotateLocalY(-90);//frog 2 faces left

	IModel* frog3 = frogMesh->CreateModel(0, 0, -5);
	frog3->RotateLocalY(180);//frog 3 faces down 

	IModel* frog4 = frogMesh->CreateModel(0, 0, 5);//frog 4 faces up

	IMesh* portalMesh = myEngine->LoadMesh("portal.x");
	IModel* portal = portalMesh->CreateModel(0, 0, 0);
	portal->Scale(1.2);// the portal was increased in size to house all four frogs

	IFont* gameText = myEngine->LoadFont("Comic Sans MS", 36);// sets up the font for the game

	IMesh* plantMesh = myEngine->LoadMesh("plant.x");// this is the 6 plants that are scattered across the map
	IModel* plant1 = plantMesh->CreateModel(10, 0, 40);
	IModel* plant2 = plantMesh->CreateModel(50, 0, 20);
	IModel* plant3 = plantMesh->CreateModel(40, 0, -40);
	IModel* plant4 = plantMesh->CreateModel(-10, 0, -50);
	IModel* plant5 = plantMesh->CreateModel(-40, 0, -30);
	IModel* plant6 = plantMesh->CreateModel(-30, 0, 30);
	
	//All keycodes/ user inputs

	const EKeyCode kMoveForwardKey = EKeyCode::Key_W;
	const EKeyCode kMoveBackwardsKey = EKeyCode::Key_S;
	const EKeyCode kRotateRightKey = EKeyCode::Key_D;//these four controls are the basic and most commonly used buttons for player movement
	const EKeyCode kRotateLeftKey = EKeyCode::Key_A;

	const EKeyCode pauseKey = EKeyCode::Key_P;// this will pause the game
	const EKeyCode ExitKey = EKeyCode::Key_Escape;// quickly exits the game
	const EKeyCode changeToBirdsEyeCamKey = EKeyCode::Key_1;//changes camera perspective to birds eye view
	const EKeyCode changeTo3rdPersonCamKey = EKeyCode::Key_2;//changes camera perspective to third person
	const EKeyCode changeTo1stPersonCamKey = EKeyCode::Key_3;//changes camera perspective to first person

	//GameSpeed
	const float kGameSpeed = 0.009f;//this is the speed that the car moves at
	const float kTurningSpeed = 0.027f;// this is how fast the car will turn
	const float kFrogSpeed = 0.0045f;// this is the movement speed of the frogs, it is half the speed of the car
	const int gameStartingScore = 0;// this will be the starting score for the players
	
	const int deadFrogPoints = 10;
	
	//the next six variables are used for creating collision spheres/ boxes
	const float carWidth = 3;
	const float carLength = 6;
	const float frogRadius = 6;
	const float islandRadius = 60;
	const float portalRadius = 15;
	const float plantRadius = 2;
	
	
	float previousCarX, previousCarZ;//the previous location of the car 
	float frog1LastX, frog2LastX, frog3LastZ, frog4LastZ;// previous location of the frogs
	
	
	int userScore = gameStartingScore;// intitialises the user score
    //the initial game states are declared below
	EWaterSafety waterState = Safe;
	EInPlay gamePlayingState = Playing;
	EFrogMovement frog1AliveState = Moving;
	EFrogMovement frog2AliveState = Waiting;
	EFrogMovement frog3AliveState = Waiting;
	EFrogMovement frog4AliveState = Waiting;
	
	bool frog1Collision, frog2Collision, frog3Collision, frog4Collision;//these are the boolean variables used to check if the car collided with the frogs
	bool frog1EscapedCollision, frog2EscapedCollision, frog3EscapedCollision, frog4EscapedCollision;//this checks to see if any of the frogs escaped
	bool carWaterCollision, carPortalcollision;// checks to see if other collisons happened
	bool playerWon = false;// starts of as the player not won
	bool firstPerson = false, thirdperson = false, birdseye = true;// initialises the birds eye view camera first
	bool plant1Collision, plant2Collision, plant3Collision, plant4Collision, plant5Collision, plant6Collision;// boolean for plant collisions 
//------------------------------------------------------------------------------------------------------------
	
	const float TIME_LIMIT = 1;
	float timeLimit = TIME_LIMIT;
	//the next four lines store the initial location of the four frogs
	frog1LastX = frog1->GetX();
	frog2LastX = frog2->GetX();
	frog3LastZ = frog3->GetZ();
	frog4LastZ = frog4->GetZ();

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		const float frameTime = myEngine->Timer();
		/**** Update your scene each frame here ****/
		stringstream textScore;// initalises textScore
		textScore << "Score: " << userScore;//sets up the text in the top left corner with the score on
		gameText->Draw(textScore.str(), 10, 10);
		
		timeLimit -= frameTime;//frame time subtracts from time limit
		
//-----------------------------------------------------------------------------------------------------------
		if (myEngine->KeyHit(ExitKey)) //escape key
		{
			myEngine->Stop();//closes the engine
		}
//------------------------------------------------------------------------------------------------------------
		if (myEngine->KeyHit(changeToBirdsEyeCamKey) && birdseye == false)//birds eye view cam
		{
			if (firstPerson == true)// resets any adjustments made to the camera
			{
				gameCamera->DetachFromParent();
				gameCamera->SetLocalPosition(0, 0, 0);
				firstPerson = false;
			}
			else if (thirdperson == true)// resets any adjustments made to the camera
			{
				gameCamera->DetachFromParent();
				gameCamera->SetLocalPosition(0, 0, 0);
				gameCamera->RotateLocalX(-25);
				thirdperson = false;
			}//sets the cameras new position
			gameCamera->SetLocalPosition(0, 120, -100);
			gameCamera->RotateLocalX(50);
			birdseye = true;

		}
//--------------------------------------------------------------------------------------------------------
		if (myEngine->KeyHit(changeTo1stPersonCamKey) && firstPerson == false)
		{
			if (birdseye == true)// resets any adjustments made to the camera
			{
				gameCamera->RotateLocalX(-50);
				birdseye = false;
			}
			else if (thirdperson == true)// resets any adjustments made to the camera
			{
				gameCamera->DetachFromParent();
				gameCamera->SetLocalPosition(0, 0, 0);
				gameCamera->RotateLocalX(-25);
				thirdperson = false;
			}//sets the cameras new position
			gameCamera->AttachToParent(car);
			gameCamera->SetLocalPosition(0, 0, 0);
			gameCamera->MoveLocalZ(5);
			gameCamera->MoveLocalY(2.5);
			firstPerson = true;
		}
//------------------------------------------------------------------------------------------------------------
		if (myEngine->KeyHit(changeTo3rdPersonCamKey) && thirdperson == false)
		{
			if (birdseye == true)// resets any adjustments made to the camera
			{
				gameCamera->RotateLocalX(-50);
				birdseye = false;
			}
			else if (firstPerson == true)// resets any adjustments made to the camera
			{
				gameCamera->DetachFromParent();
				gameCamera->SetLocalPosition(0, 0, 0);
				firstPerson = false;
			}//sets the cameras new position
			thirdperson = true;
			gameCamera->AttachToParent(car);
			gameCamera->SetLocalPosition(0, 0, 0);
			gameCamera->MoveLocalZ(-15);
			gameCamera->MoveLocalY(7);
			gameCamera->RotateLocalX(25);
		}
//---------------------------------------------------------------------------------------------------------	
		if (gamePlayingState == Playing)//start of playing state
		{
			previousCarX = car->GetLocalX();//these two lines get the location of the car
			previousCarZ = car->GetLocalZ();
//----------------------------------------------------------------------------------------------------
			//Car Controls
			if (myEngine->KeyHeld(kMoveForwardKey))
			{
				car->MoveLocalZ(kGameSpeed);//this moves the car forward
			}
			if (myEngine->KeyHeld(kMoveBackwardsKey))
			{
				car->MoveLocalZ(-kGameSpeed);//this moves the car backwards 
			}
			if (myEngine->KeyHeld(kRotateRightKey))
			{
				car->RotateLocalY(kTurningSpeed);//turns the car to the right
			}
			if (myEngine->KeyHeld(kRotateLeftKey))
			{
				car->RotateLocalY(-kTurningSpeed);//turns the car to the left
			}
			if (myEngine->KeyHit(pauseKey))
			{
				gamePlayingState = Paused;//changes the gamestate to paused
			}

//---------------------------------------------------------------------------------------------------

			//Frog1 Alive State
			if (frog1AliveState == Moving)
			{
				if (timeLimit <= 0)
				{
					frog1->MoveX(kFrogSpeed);//moves frogs 1 to the right
					{
						if (abs(frog1->GetX() - frog1LastX) >= 10)// if it moves ten units right then it stops for 1 second
						{
							frog1LastX = frog1->GetX();
							timeLimit = TIME_LIMIT;
						}
					}
				}
				frog1Collision = carToFrogCollision(car, carLength, carWidth, frog1, frogRadius);// initialises frog1Collision with cartofrog function
				if (frog1Collision)//if collision has occured
				{
					frog1AliveState = Dead;
					userScore = userScore + deadFrogPoints;//adds points
					frog1->RotateZ(180);//flips the frog
					frog1->MoveY(2);
					frog2AliveState = Moving;//starts frog2
					timeLimit = TIME_LIMIT;//resets timer for frog 2
				}
			}
//-----------------------------------------------------------------------------------------------------			

			if (frog2AliveState == Moving)
			{
				if (timeLimit <= 0)
				{
					frog2->MoveX(-kFrogSpeed);//moves the frog left
					{
						if (abs(frog2->GetX() - frog2LastX) >= 10)//when it moves ten units it waits a second
						{
							frog2LastX = frog2->GetX();
							timeLimit = TIME_LIMIT;//resets timer
						}
					}
				}
				frog2Collision = carToFrogCollision(car, carLength, carWidth, frog2, frogRadius);
				if (frog2Collision)//checks for a collision between car and frog
				{
					frog2AliveState = Dead;
					userScore = userScore + deadFrogPoints;
					frog2->RotateZ(180);//flips frog
					frog2->MoveY(2);
					frog3AliveState = Moving;//starts frog 3
					timeLimit = TIME_LIMIT;
				}
			}
//-------------------------------------------------------------------------------------------------------

			if (frog3AliveState == Moving)
			{
				if (timeLimit <= 0)
				{
					frog3->MoveZ(-kFrogSpeed);//moves the frog down
					{
						if (abs(frog3->GetZ() - frog3LastZ) >= 10)//checks to see if its moved ten units if so then 1 second wait
						{
							frog3LastZ = frog3->GetZ();
							timeLimit = TIME_LIMIT;//resets timer
						}
					}
				}
				frog3Collision = carToFrogCollision(car, carLength, carWidth, frog3, frogRadius);
				if (frog3Collision)//checks for car to frog collision
				{
					frog3AliveState = Dead;
					userScore = userScore + deadFrogPoints;
					frog3->RotateZ(180);//flips the frog
					frog3->MoveY(2);
					frog4AliveState = Moving;//starts frog 4
					timeLimit = TIME_LIMIT;
				}
			}
//--------------------------------------------------------------------------------------------------------

			if (frog4AliveState == Moving)
			{
				if (timeLimit <= 0)
				{
					frog4->MoveZ(kFrogSpeed);//moves frog up
					{
						if (abs(frog4->GetZ() - frog4LastZ) >= 10)//checks if moved ten units then 1 second wait
						{
							frog4LastZ = frog4->GetZ();
							timeLimit = TIME_LIMIT;
						}
					}
				}
				frog4Collision = carToFrogCollision(car, carLength, carWidth, frog4, frogRadius);
				if (frog4Collision)//checks for car to frog collsion
				{
					frog4AliveState = Dead;
					userScore = userScore + deadFrogPoints;
					frog4->RotateZ(180);
					frog4->MoveY(2);
					playerWon = true;//provides player with winner message
					gamePlayingState = Over;//ends the game
			
				}	
			}
//--------------------------------------------------------------------------------------------------------
			frog1EscapedCollision = frogToWaterCollision(island, islandRadius, frog1, frogRadius);//these four lines check to see if any of the frogs have left the collision area
			frog2EscapedCollision = frogToWaterCollision(island, islandRadius, frog2, frogRadius);
			frog3EscapedCollision = frogToWaterCollision(island, islandRadius, frog3, frogRadius);
			frog4EscapedCollision = frogToWaterCollision(island, islandRadius, frog4, frogRadius);
			if (!frog1EscapedCollision || !frog2EscapedCollision || !frog3EscapedCollision || !frog4EscapedCollision)
			{
				waterState = Poisoned;//changes water state
				gamePlayingState = Over;//ends game
			}
//--------------------------------------------------------------------------------------------------
			carWaterCollision = carToWaterCollision(car, carLength, carWidth, island, islandRadius);
			if (!carWaterCollision)//checks to see if the car has left the playing area
			{
				gamePlayingState = Over;//ends game
			}
//--------------------------------------------------------------------------------------------------
		}
		else if (gamePlayingState == Paused)//the paused state
		{
			if (myEngine->KeyHit(pauseKey))
			{
				gamePlayingState = Playing;//sets the game back to playing
			}
		}
//-----------------------------------------------------------------------------------------------------
		else if (gamePlayingState == Over)
		{
			stringstream overMessage;
			if (!playerWon)//checks to see that player hasn't won
			{
				overMessage << "Game Over";
			}
			if (playerWon)//checks if player has won
			{
				overMessage << "Winner";
			}
			gameText->Draw(overMessage.str(), 575, 300);// creates the game over or winner message
		}
//-----------------------------------------------------------------------------------------------------
		if (waterState == Safe)
		{
			waterFloor->SetSkin("water.png");//sets the water skin to water
		}
		else if (waterState == Poisoned)
		{
			waterFloor->SetSkin("poisoned.png");//sets the water skin to poisoned
		}
//---------------------------------------------------------------------------------------------------------------------------

		carPortalcollision = carToPortalCollision(car, carLength, carWidth, portal, portalRadius);
		if (carPortalcollision)//checks if the car has collided with the portal
		{
			car->SetX(previousCarX);//resets the player back to their origional position 
			car->SetZ(previousCarZ);

		}

//---------------------------------------------------------------------------------------------------------------------------
		plant1Collision = carToPortalCollision(car, carLength, carWidth, plant1, plantRadius);
		plant2Collision = carToPortalCollision(car, carLength, carWidth, plant2, plantRadius);
		plant3Collision = carToPortalCollision(car, carLength, carWidth, plant3, plantRadius);
		plant4Collision = carToPortalCollision(car, carLength, carWidth, plant4, plantRadius);
		plant5Collision = carToPortalCollision(car, carLength, carWidth, plant5, plantRadius);
		plant6Collision = carToPortalCollision(car, carLength, carWidth, plant6, plantRadius);
		if (plant1Collision || plant2Collision || plant3Collision || plant4Collision || plant5Collision || plant6Collision)//checks to see if the plaer has colided with a plant
		{
			car->SetX(previousCarX);
			car->SetZ(previousCarZ);
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
//--------------------------------------------------------------------------------------------------------------------------
bool carToFrogCollision(IModel* car, float boxLength, float boxWidth, IModel* frog, float sphereRadius)
{
	float carX, carY, carZ;
	carX = car->GetLocalX();//gets car location
	carZ = car->GetLocalZ();
	float frogX, frogY, frogZ;
	frogX = frog->GetX();//gets frog location
	frogZ = frog->GetZ();
	float radiusAndWidth = sphereRadius + boxWidth;//cobines the radius with box size 
	float radiusAndLength = sphereRadius + boxLength;
	return frogX > carX - radiusAndWidth && frogX < carX + radiusAndWidth &&
		frogZ > carZ - radiusAndLength && frogZ < carZ + radiusAndLength;//if the boundarys intersect then return true
}
bool carToWaterCollision(IModel* car, float boxLength, float boxWidth, IModel* water, float waterRadius)
{
	float carX, carZ;
	carX = car->GetLocalX();//gets the car location
	carZ = car->GetLocalZ();
	float waterX, waterZ;
	waterX = water->GetX();//gets water location
	waterZ = water->GetZ();
	float radiusAndWidth = waterRadius + boxWidth;
	float radiusAndLength = waterRadius + boxLength;//combines box sizes
	return waterX > carX - waterRadius && waterX < carX + waterRadius&&
		waterZ > carZ - waterRadius && waterZ < carZ + waterRadius;//if they intersect return true
}
bool frogToWaterCollision(IModel* island, float islandRadius, IModel* frog, float sphereRadius)
{
	float islandX, islandZ;
	islandX = island->GetX();
	islandZ = island->GetZ();//gets island location
	float frogX, frogZ;
	frogX = frog->GetX();
	frogZ = frog->GetZ();//gets frog location
	float combinedRadius = sphereRadius + islandRadius;//combines radius
	return frogX > islandX - combinedRadius && frogX < islandX + combinedRadius &&
		frogZ > islandZ - combinedRadius && frogZ < islandZ + combinedRadius;//if they intersect then returns true
}
bool carToPortalCollision(IModel* car, float boxLength, float boxWidth, IModel* portal, float portalRadius)
{
	float carX, carZ;
	carX = car->GetLocalX();//gets car location
	carZ = car->GetLocalZ();
	float portalX, portalZ;
	portalX = portal->GetX();//gets portal or plants location
	portalZ = portal->GetZ();
	float radiusAndWidth = portalRadius + boxWidth;//combines box sizes
	float radiusAndLength = portalRadius + boxLength;
	return portalX > carX - radiusAndWidth && portalX < carX + radiusAndWidth &&
		portalZ > carZ - radiusAndWidth && portalZ < carZ + radiusAndWidth;//if there is an intersection then return true
}