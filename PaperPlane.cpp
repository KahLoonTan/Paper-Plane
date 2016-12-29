#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>  // for ostringstream

using namespace std; 



int main()
{
  const int myWorldWidth = 800;
  const int myWorldHeight = 600;
	// Create the window of the application
  sf::RenderWindow myWindow(sf::VideoMode(myWorldWidth, myWorldHeight, 32), "My World");
  myWindow.setVerticalSyncEnabled(true);

  sf::Font font;
  font.loadFromFile("resources/sansation.ttf");

  sf::Text distanceText;
  distanceText.setFont(font);
  distanceText.setCharacterSize(20);
  distanceText.setColor(sf::Color::Black);
  distanceText.setStyle(sf::Text::Bold | sf::Text::Underlined);
  stringstream ssDist;
 
  
  sf::Text instructionText;
  instructionText.setFont(font);
  instructionText.setCharacterSize(24);
  instructionText.setColor(sf::Color::Black);
  instructionText.setStyle(sf::Text::Bold);
  instructionText.setPosition(myWorldWidth*0.3,myWorldHeight*0.3);
  instructionText.setString("Press S to Start Game! \nPress R to restart \nPress Spacebar to boost Paperplane");	
  
  sf::Text heightText;
  heightText.setFont(font);
  heightText.setCharacterSize(20);
  heightText.setColor(sf::Color::Black);
  heightText.setStyle(sf::Text::Bold | sf::Text::Underlined);
  heightText.setPosition(0,20);
  stringstream ssHeight;
  

  sf::Text boosterText;
  boosterText.setFont(font);
  boosterText.setCharacterSize(20);
  boosterText.setColor(sf::Color::Black);
  boosterText.setStyle(sf::Text::Bold | sf::Text::Underlined);
  boosterText.setPosition(0,40);
  stringstream ssBooster;

  sf::Texture texture; //load plane texture
  texture.loadFromFile("resources/paperplane.png");

  sf::Sprite sprite(texture); //set up plane characteristic
  sprite.setScale(0.1f,0.1f);
  sprite.setOrigin(sprite.getTexture()->getSize().x*0.5,sprite.getTexture()->getSize().y*0.5);
  
 
  sprite.setColor(sf::Color(255,0,0));

  float velX=0.0;	//velocity for x,y
  float velY=0.0;
  float gravity=-9.8;
  float drag=-1.0;
  float initialForce =15.0; //Force that is used to find velocity in x and y
  const float Pi = 3.14159265;
  float rad = 0.0;
  float runningForce=initialForce; //The force that changes when game starts
  float newRot = atan(velY/velX) * 180/Pi; //to rotate the plane correctly
  float distanceTravelled=0; //to keep track of distance travelled
  float currHeight=0; //to see current height
  bool jump=false; 
  int maxJump=10; //number of jumps
  sf::Clock myClock; 
  
  bool restart=false;
  bool start = false;
  while (myWindow.isOpen())
  {
	myWindow.clear(sf::Color(50, 50, 200));
   if(!start)
  	{
  			 
  		myWindow.draw(instructionText);

  		runningForce = 0;
  		

  	}
  	
  	if(restart)
  	{
  		sprite.setPosition(10,myWorldHeight-100);
 		sprite.setRotation(0);
 		rad = (sprite.getRotation()+45)*Pi/180.0; //Angle that is needed for calculation of velx,vely
 		
 		runningForce=initialForce; //The force that changes when game starts
 		velX=initialForce*cos(rad); //Vx = F*cos(a)
 		velY=initialForce*sin(rad); //Vy = F*sin(a)
 		
  		newRot = atan(velY/velX) * 180/Pi; //to rotate the plane correctly
  		distanceTravelled=0; //to keep track of distance travelled
  		currHeight=0; //to see current height
 		jump=false; 
  		maxJump=10; //number of jumps
  		gravity = -9.8;
  		drag = -1;
  		myClock.restart();
  		restart=false;
  	}
  	
  	  // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
      // cout<<runningForce<<endl;

        float deltaTime = myClock.restart().asSeconds(); 
        //sprite.setRotation(45);
        
       

       if(sprite.getPosition().y<myWorldHeight-sprite.getGlobalBounds().height*0.6)
       {
       	velX=runningForce*cos(newRot * Pi/180);
 		velY=runningForce*sin(newRot * Pi/180);


		if(jump && maxJump>0)
   		 {
   		 	velX=velX+50*deltaTime;
   		 	velY=velY+150*deltaTime;
   		 	jump=false;
   		 	maxJump--;
   		 }

   		 velX = velX + drag*deltaTime;
   		 velY = velY + gravity*deltaTime;
   		 float velR = velY/velX;
   		 newRot = atan(velR) * 180/Pi;
   		 
	 	 sprite.move(velX,-velY);
  	 	 
  	 	 
  	 	 sprite.setRotation(-newRot+45);
       }else if(sprite.getPosition().y>myWorldHeight-sprite.getGlobalBounds().height*0.6)
       {
       		//sprite.setPosition(sprite.getPosition().x,myWorldHeight-50);
       		if(velX>0)
       		{
       			velX-=1;

       		}else
       		{
       			velX=0;

       		}
       		drag=0;
       		gravity=0;
       		velY=0;
       		sprite.move(velX,0);

       }
       
        if(sprite.getPosition().x>myWorldWidth)
        {
        	float y = sprite.getPosition().y;
        	sprite.setPosition(0,y);
        }
        runningForce = sqrt(velX*velX+velY*velY);
        distanceTravelled += velX*deltaTime+drag*deltaTime*deltaTime;
        currHeight += velY*deltaTime+gravity*deltaTime*deltaTime;
       	
        ssDist.str("");
   		ssHeight.str("");
   		ssBooster.str("");

        ssDist<<distanceTravelled;
        ssHeight<<currHeight;
        ssBooster<<maxJump;
        
        string distStr = "Distance Travelled -> "+ssDist.str();
        string heightStr = "Current Height -> "+ssHeight.str();
        string boosterStr = "Booster remaining -> "+ssBooster.str();

        distanceText.setString(distStr);
        heightText.setString(heightStr);
        boosterText.setString(boosterStr);

        while (myWindow.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                myWindow.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
            	{
            		if(!start)
            		{
            			start=true;
            			restart=true;
            		}
            			
            		
            		
            	}
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 		
            	sprite.setRotation(sprite.getRotation()+5);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
            	jump=true;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            	restart=true;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            	myWindow.close();

            		
            
        }
   
  	
    myWindow.draw(sprite);
    myWindow.draw(distanceText);
    myWindow.draw(heightText);
    myWindow.draw(boosterText);
    
  	myWindow.display();

  }

  return EXIT_SUCCESS;
}