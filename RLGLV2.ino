/*
 * Creator:     	Preston Gilmore/Kyle Hawthorne
 * Last updated:	13 September 2022 / 21 January 2023 / 20 June 2023
 * Project Name:	Red-Light Green-Light
*/

// constants won't change. They're used here to set pin numbers:
const int gamePowerPin = A1;    // the number of the game on/off Pin (Not Used)
const int gameSelectPin = A3;   // the number of the game select Pin (Game one/two)
const int switchPin = A0; 	    // the number of the Reset Game Switch on Remote
const int buttonPinP1 = 2; 	    // the number of the pushbutton pin - Player 1
const int buttonPinP2 = 5; 	    // the number of the pushbutton pin - Player 2
                                          // Index =>             0 (Not Used)         1                   2                 3                    4                    5                 6
const int LEDPins [7] = {7, 8, 9, 10, 11, 12, 13};    // { Arduino Game Light, Green LED PLayer 1, Yellow LED Player 1, Red LED Player 1, Green LED Player 2, Yellow LED Player 2, Red LED PLayer 2}
const int NumberOfLEDS = (sizeof(LEDPins)/sizeof(LEDPins[0]));

// Game constants
const int distanceToWin = 10000;
const int afterGameWaitTime = 5000;     // 5 Seconds

// Timing Delays
int delay1 = 350;               // Short Delay
int delay2 = 650;               // Long Delay

// Game variables
int P1Wins = 0;
int P2Wins = 0;
int P1Distance = 0;
int P2Distance = 0;
bool didP1Lose = false;
bool didP2Lose = false;
bool isNewGame = true;
int randomDuration = 0;
int gameState = 1;
int previousGameState = 1;


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
 
  // initialize the LED pins as outputs:
  for (int i = 0; i < NumberOfLEDS; i++) {
    pinMode(LEDPins[i], OUTPUT);    
  }
 
  // initialize the pushbutton pin as an input:
  pinMode(gamePowerPin, INPUT_PULLUP);
  pinMode(gameSelectPin, INPUT_PULLUP);  
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(buttonPinP2, INPUT_PULLUP);
  pinMode(buttonPinP1, INPUT_PULLUP);

  digitalWrite(LEDPins[0], LOW);                       // Set unused Game pin to off
}

void loop() {

  gameState = digitalRead(gameSelectPin);              // Read Game Select Switch
  if(gameState != previousGameState) {
    P1Wins = 0;                                        // Reset All Game Variables if Game Changed
    P2Wins = 0;
    P1Distance = 0;
    P2Distance = 0;
    didP1Lose = false;
    didP2Lose = false;
    isNewGame = true;

    previousGameState = gameState;                      // Reset Previous Game State     
  }
  if (digitalRead(gameSelectPin) == HIGH) {             // Game 1 Selected
  
    // Wait for a user to turn the game switch on
    while(digitalRead(switchPin) == HIGH) {
	    digitalWrite(LEDPins[1], LOW);      // P1 Green On
      digitalWrite(LEDPins[2], HIGH);     // P1 Yellow Off
	    digitalWrite(LEDPins[3], LOW);      // P1 Red On
 	    digitalWrite(LEDPins[4], LOW);      // P2 Green On
      digitalWrite(LEDPins[5], HIGH);     // P2 Yellow Off
	    digitalWrite(LEDPins[6], LOW);      // P2 Red On
	    delay(2000);
	    digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
      digitalWrite(LEDPins[2], HIGH);     // P1 Yellow Off
	    digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
 	    digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
      digitalWrite(LEDPins[5], HIGH);     // P2 Yellow Off
	    digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
	    delay(1000);
    }
    // Start new game
    while(digitalRead(switchPin) == LOW) {
	  // If new game, play the starting sequence
	    if(isNewGame) {
  	   // 1 2 3 Go!!!
  	    for(int i = 0; i < 3; i++) {
	        digitalWrite(LEDPins[3], LOW);      // P1 Red On
          digitalWrite(LEDPins[6], LOW);      // P2 Red On
         delay(delay1);
	        digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	      digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	    delay(delay2);
  	    }
  	    isNewGame = false;
  	  }
 	 
     // Green light!!!
     randomDuration = random(1000, 4000);
	    for(int i = 0; i < randomDuration; i++) {
  	    digitalWrite(LEDPins[1], LOW);        // P1 Green On
        digitalWrite(LEDPins[4], LOW);        // P2 Green On

	      // Increase the player distance while their button is held down
	      if(digitalRead(buttonPinP1) == LOW) {
    	    P1Distance++;
 	      }
        if(digitalRead(buttonPinP2) == LOW) {
          P2Distance++;
 	      }
 	      if(P1Distance >= distanceToWin || P2Distance >= distanceToWin) {
          break;
  	    }
 	 	    delay(1);
	    }

     // Turn the green light off and give a brief reaction time buffer
     digitalWrite(LEDPins[1], HIGH);         // P1 Green Off
     digitalWrite(LEDPins[4], HIGH);         // P2 Green Off
     delay(100);
    
     // If game over, display results
     if(P1Distance >= distanceToWin && P2Distance >= distanceToWin) {
	      for(int i = 0; i < 3; i++) {
  	      digitalWrite(LEDPins[1], LOW);      // P1 Green On
          digitalWrite(LEDPins[4], LOW);      // P2 Green On
  	      delay(delay1);
          digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
          digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
  	      delay(delay1);
 	      }
	      P2Wins++;
 	      P1Wins++;
 	      digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
        digitalWrite(LEDPins[5], LOW);        // P2 Yellow On
   	    isNewGame = true;
     }
     else if(P1Distance >= distanceToWin) {
	      for(int i = 0; i < 3; i++) {
  	      digitalWrite(LEDPins[1], LOW);      // P1 Green On
          digitalWrite(LEDPins[4], LOW);      // P2 Green On
  	      delay(delay1);
          digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
          digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
          delay(delay1);
        }
        P1Wins++;
 	      digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
       	isNewGame = true;
     }
	   else if(P2Distance >= distanceToWin) {
      	for(int i = 0; i < 3; i++) {
    	   digitalWrite(LEDPins[1], LOW);      // P1 Green On
         digitalWrite(LEDPins[4], LOW);      // P2 Green On
         delay(delay1);
	       digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
         digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
         delay(delay1);
  	    }
        P2Wins++;
  	    digitalWrite(LEDPins[5], LOW);        // P2 Yellow on
        isNewGame = true;
	   }

	   // If the game is decided, exit the game loop
	   if(isNewGame) {
       break;
	   }

	   // Red light!!!
	   randomDuration = random(1000, 4000);
	   for(int i = 0; i < randomDuration; i++) {
	     digitalWrite(LEDPins[3], LOW);        // P1 Red On
       digitalWrite(LEDPins[6], LOW);        // P2 Red On

  	   // If a player is still holding down the button they lose
  	   if(digitalRead(buttonPinP1) == LOW) {
    	   didP1Lose = true;
  	   }
  	   if(digitalRead(buttonPinP2) == LOW) {
       	didP2Lose = true;
  	   }

  	   // If either player lost, end the game
  	   if(didP1Lose == true || didP2Lose == true) {
    	   break;
  	   }
 	   	 delay(1);
	   }

	   // Turn the red light off and give a brief reaction time buffer
     digitalWrite(LEDPins[3], HIGH);         // P1 Red Off
     digitalWrite(LEDPins[6], HIGH);         // P2 Red Off
	   delay(100);

	   // If game over, display results
	   if(didP1Lose && didP2Lose) {
  	   for(int i = 0; i < 3; i++) {
	       digitalWrite(LEDPins[3], LOW);      // P1 Red On
  	     digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	   delay(delay1);
	       digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	     digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	   delay(delay1);
    	 }
  	   isNewGame = true;
	   }
	   else if(didP1Lose) {
  	   for(int i = 0; i < 3; i++) {
	       digitalWrite(LEDPins[3], LOW);      // P1 Red On
  	     digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	   delay(delay1);
	       digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	     digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	   delay(delay1);
  	   }
  	   P2Wins++;
  	   digitalWrite(LEDPins[5], LOW);        // P2 Yellow On
    	 isNewGame = true;
	   }
	   else if(didP2Lose) {
  	   for(int i = 0; i < 3; i++) {
	       digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	     digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	   delay(delay1);
	       digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	     digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	   delay(delay1);
    	 }
      P1Wins++;
  	   digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
    	 isNewGame = true;
  	 }

	   //If the game is decided, exit the game loop
	   if(isNewGame) {
  	   break;
	   }
    } //End of the game loop

    //Reset game values
    P1Distance = 0;
    P2Distance = 0;
    didP1Lose = false;
    didP2Lose = false;
    isNewGame = true;

    //Wait for the user to turn the game switch off
    while(digitalRead(switchPin) == LOW) {
	    delay(1);
    }

    //Turn off All LEDs 
    for (int i = 1; i < NumberOfLEDS; i++) {
     digitalWrite(LEDPins[i], HIGH);
    }
    delay(1000);

    //Blink each corresponding LED to the amount of wins that player has
    for(int i = 0; i < P1Wins || i < P2Wins; i++) {
	    if(i < P1Wins) {
       digitalWrite(LEDPins[2], LOW);      // P1 Yellow on
	    }
	    if(i < P2Wins) {
       digitalWrite(LEDPins[5], LOW);         // P2 Yellow On
	    }
      delay(delay1);
      for (int i = 0; i < NumberOfLEDS; i++) {    // Turn off LEDS
        digitalWrite(LEDPins[i], HIGH);
      }
	    delay(delay2);
    }
  }
  if(digitalRead(gameSelectPin) == LOW) {         // Game 2 Selected    

  // Wait for a user to turn the game switch on
    while(digitalRead(switchPin) == HIGH) {
	    digitalWrite(LEDPins[1], LOW);      // P1 Green On
      digitalWrite(LEDPins[2], HIGH);     // P1 Yellow Off
	    digitalWrite(LEDPins[3], LOW);      // P1 Red On
 	    digitalWrite(LEDPins[4], LOW);      // P2 Green On
      digitalWrite(LEDPins[5], HIGH);     // P2 Yellow Off
	    digitalWrite(LEDPins[6], LOW);      // P2 Red On
	    delay(2000);
	    digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
      digitalWrite(LEDPins[2], HIGH);     // P1 Yellow Off
	    digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
 	    digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
      digitalWrite(LEDPins[5], HIGH);     // P2 Yellow Off
	    digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
	    delay(1000);
    }
    // Start new game
    while(digitalRead(switchPin) == LOW) {
	  // If new game, play the starting sequence
	      if(isNewGame) {
  	    // 1 2 3 Go!!!
  	      for(int i = 0; i < 3; i++) {
	          digitalWrite(LEDPins[3], LOW);      // P1 Red On
            digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	      delay(delay1);
	          digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	        digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
      	    delay(delay2);
  	      }
  	      isNewGame = false;
  	    }
 	 
	      // Green light!!!
	      randomDuration = random(500, 2000);
  	    for(int i = 0; i < randomDuration; i++) {
    	    digitalWrite(LEDPins[1], LOW);        // P1 Green On
          digitalWrite(LEDPins[4], LOW);        // P2 Green On

  	      // Check to See if a player has hit the button
  	      if(digitalRead(buttonPinP1) == LOW) {
      	    if(P2Distance >= distanceToWin) {
              P1Distance = 0;
            } else {
              P1Distance = distanceToWin;              
            }  
  	      }
  	      if(digitalRead(buttonPinP2) == LOW) {
    	      if(P1Distance >= distanceToWin) {
              P2Distance = 0;
            } else {
              P2Distance = distanceToWin;
            }
  	      }
  	      if(P1Distance >= distanceToWin || P2Distance >= distanceToWin) {
    	     break;
  	      }
 	 	      delay(1);
	      }

	      // Turn the green light off and give a brief reaction time buffer
	      digitalWrite(LEDPins[1], HIGH);         // P1 Green Off
        digitalWrite(LEDPins[4], HIGH);         // P2 Green Off
	      delay(100);
    
	      // If game over, display results
	      if(P1Distance >= distanceToWin && P2Distance >= distanceToWin) {
  	      for(int i = 0; i < 3; i++) {
    	      digitalWrite(LEDPins[1], LOW);      // P1 Green On
            digitalWrite(LEDPins[4], LOW);      // P2 Green On
    	      delay(delay1);
	          digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
            digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
    	      delay(delay1);
  	      }
  	      P2Wins++;
  	      P1Wins++;
  	      digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
          digitalWrite(LEDPins[5], LOW);        // P2 Yellow On
 	   	    isNewGame = true;
	      }
	      else if(P1Distance >= distanceToWin) {
  	      for(int i = 0; i < 3; i++) {
    	      digitalWrite(LEDPins[1], LOW);      // P1 Green On
            digitalWrite(LEDPins[4], LOW);      // P2 Green On
    	      delay(delay1);
	          digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
            digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
    	     delay(delay1);
        	}
          P1Wins++;
  	      digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
 	       	isNewGame = true;
      	}
	      else if(P2Distance >= distanceToWin) {
        	for(int i = 0; i < 3; i++) {
    	     digitalWrite(LEDPins[1], LOW);      // P1 Green On
           digitalWrite(LEDPins[4], LOW);      // P2 Green On
        	  delay(delay1);
	         digitalWrite(LEDPins[1], HIGH);     // P1 Green Off
           digitalWrite(LEDPins[4], HIGH);     // P2 Green Off
        	  delay(delay1);
  	      }
        	P2Wins++;
  	      digitalWrite(LEDPins[5], LOW);        // P2 Yellow on
        	isNewGame = true;
	      }

	      // If the game is decided, exit the game loop
	      if(isNewGame) {
          break;
	      }

	      // Red light!!!
	      randomDuration = random(1000, 3000);
	      for(int i = 0; i < randomDuration; i++) {
	        digitalWrite(LEDPins[3], LOW);        // P1 Red On
          digitalWrite(LEDPins[6], LOW);        // P2 Red On

  	      // If a player is still holding down the button they lose
  	      if(digitalRead(buttonPinP1) == LOW) {
    	      didP1Lose = true;
  	      }
  	      if(digitalRead(buttonPinP2) == LOW) {
          	didP2Lose = true;
  	      }

  	      // If either player lost, end the game
  	      if(didP1Lose == true || didP2Lose == true) {
    	      break;
  	      }
 	   	    delay(1);
	      }

	      // Turn the red light off and give a brief reaction time buffer
        digitalWrite(LEDPins[3], HIGH);         // P1 Red Off
        digitalWrite(LEDPins[6], HIGH);         // P2 Red Off
	      delay(100);

	      // If game over, display results
	      if(didP1Lose && didP2Lose) {
  	      for(int i = 0; i < 3; i++) {
	          digitalWrite(LEDPins[3], LOW);      // P1 Red On
  	        digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	      delay(delay1);
	          digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	        digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	      delay(delay1);
    	    }
  	      isNewGame = true;
	      }
	      else if(didP1Lose) {
  	      for(int i = 0; i < 3; i++) {
	          digitalWrite(LEDPins[3], LOW);      // P1 Red On
  	        digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	      delay(delay1);
	          digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	        digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	      delay(delay1);
  	      }
  	      P2Wins++;
  	      digitalWrite(LEDPins[5], LOW);        // P2 Yellow On
    	    isNewGame = true;
	      }
	      else if(didP2Lose) {
  	      for(int i = 0; i < 3; i++) {
	          digitalWrite(LEDPins[3], LOW);      // P1 Red On
  	        digitalWrite(LEDPins[6], LOW);      // P2 Red On
    	      delay(delay1);
	          digitalWrite(LEDPins[3], HIGH);     // P1 Red Off
  	        digitalWrite(LEDPins[6], HIGH);     // P2 Red Off
    	      delay(delay1);
    	    }
        	P1Wins++;
  	      digitalWrite(LEDPins[2], LOW);        // P1 Yellow On
    	    isNewGame = true;
  	    }

	      //If the game is decided, exit the game loop
	      if(isNewGame) {
  	      break;
	      }
      } //End of the game loop

      //Reset game values
      P1Distance = 0;
      P2Distance = 0;
      didP1Lose = false;
      didP2Lose = false;
      isNewGame = true;

      //Wait for the user to turn the game switch off
      while(digitalRead(switchPin) == LOW) {
	      delay(1);
      }

      //Turn off All LEDs
      for (int i = 1; i < NumberOfLEDS; i++) {
       digitalWrite(LEDPins[i], HIGH);
      }
      delay(1000);

      //Blink each corresponding LED to the amount of wins that player has
      for(int i = 0; i < P1Wins || i < P2Wins; i++) {
	      if(i < P1Wins) {
  	      digitalWrite(LEDPins[2], LOW);      // P1 Yellow on
	      }
	      if(i < P2Wins) {
  	      digitalWrite(LEDPins[5], LOW);         // P2 Yellow On
	      }
  	    delay(delay1);
        for (int i = 0; i < NumberOfLEDS; i++) {    // Turn off LEDS
          digitalWrite(LEDPins[i], HIGH);
        }
	      delay(delay2);
      }
   }   
}
