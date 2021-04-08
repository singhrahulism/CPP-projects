#define MaxScore 9                                          // Max Score is the score on which a player wins...
#define GreenLeds 8                                         // Total Number of Green Leds...
#define YellowLeds 8                                        // Total Number of Yellow Leds...
#define RedLeds 4                                           // Total Number of Red Leds....
#define TotalLeds (GreenLeds+YellowLeds+RedLeds)            // Total Number of Leds (Auto Set)...
#define StartingSpeed 250                                   // Speed at which the Game Starts (Set this Speed in milliseconds)...
// #define ConstantSpeed 50                                    // This Speed is set so that delay function doesn't complain out of memory (Set this speed in milliseconds)
#define ConstantPushSpeed 50
#define Player1Button1Pin A0                                // Pin Number to which **Player 1's First Button** is connected...
#define Player1Button2Pin A1                                // Pin Number to which **Player 1's Second Button** is connected...
#define Player2Button1Pin A2                                // Pin Number to which **Player 2's First Button** is connected...
#define Player2Button2Pin A3                                // Pin Number to which **Player 2's Second Button** is connected...
#define SpeedReduce 20                                      // More the Value of SpeedReduce, more faster the LEDs will become
#define pathLeds (GreenLeds+((YellowLeds+RedLeds)/2))       // Total number of LEDs in one path
#define IncorrectMoveLedBlinkingSpeed 200
#define BlinkCount  6
#define MaxErrorCount 3
#define MaxBlinksSSD  10
#define MinimumSpeed  75
#define MaximumSpeed  275

// Set Pins for all LEDs manually in the Pin[] array initialization...

/****************************************************/

// This Area is under SSD Surveillance....

// Set Pins for Player1's SSD in Pin1 initialization list, and Set Pins for Player2's SSD in Pin2 initialization list...

int Pin1[7]={42, 43, 44, 45, 48, 49, 50}, Pin2[7]={2, 3, 4, 5, 6, 7, 8}, len[10] ;

int player1SSD[10][7]={{Pin1[1], Pin1[2], Pin1[3], Pin1[4], Pin1[5], Pin1[6]},            /* Pin configuration for Number 0 */
                       {Pin1[3], Pin1[4]},                                                /* Pin configuration for Number 1 */
                       {Pin1[0], Pin1[2], Pin1[3], Pin1[5], Pin1[6]},                     /* Pin configuration for Number 2 */
                       {Pin1[0], Pin1[2], Pin1[3], Pin1[4], Pin1[5]},                     /* Pin configuration for Number 3 */
                       {Pin1[0], Pin1[1], Pin1[3], Pin1[4]},                              /* Pin configuration for Number 4 */
                       {Pin1[0], Pin1[1], Pin1[2], Pin1[4], Pin1[5]},                     /* Pin configuration for Number 5 */
                       {Pin1[0], Pin1[1], Pin1[2], Pin1[4], Pin1[5], Pin1[6]},            /* Pin configuration for Number 6 */
                       {Pin1[2], Pin1[3], Pin1[4]},                                       /* Pin configuration for Number 7 */
                       {Pin1[0], Pin1[1], Pin1[2], Pin1[3], Pin1[4], Pin1[5], Pin1[6]},   /* Pin configuration for Number 8 */
                       {Pin1[0], Pin1[1], Pin1[2], Pin1[3], Pin1[4], Pin1[5]}} ;          /* Pin configuration for Number 9 */

int player2SSD[10][7]={{Pin2[1], Pin2[2], Pin2[3], Pin2[4], Pin2[5], Pin2[6]},           /* Pin configuration for Number 0 */
                        {Pin2[3], Pin2[4]},                                               /* Pin configuration for Number 1 */
                        {Pin2[0], Pin2[2], Pin2[3], Pin2[5], Pin2[6]},                    /* Pin configuration for Number 2 */
                        {Pin2[0], Pin2[2], Pin2[3], Pin2[4], Pin2[5]},                    /* Pin configuration for Number 3 */
                        {Pin2[0], Pin2[1], Pin2[3], Pin2[4]},                             /* Pin configuration for Number 4 */
                        {Pin2[0], Pin2[1], Pin2[2], Pin2[4], Pin2[5]},                    /* Pin configuration for Number 5 */
                        {Pin2[0], Pin2[1], Pin2[2], Pin2[4], Pin2[5], Pin2[6]},           /* Pin configuration for Number 6 */
                        {Pin2[2], Pin2[3], Pin2[4]},                                      /* Pin configuration for Number 7 */
                        {Pin2[0], Pin2[1], Pin2[2], Pin2[3], Pin2[4], Pin2[5], Pin2[6]},  /* Pin configuration for Number 8 */
                        {Pin2[0], Pin2[1], Pin2[2], Pin2[3], Pin2[4], Pin2[5]}} ;         /* Pin configuration for Number 9 */

/****************************************************/

int i, k, f=0, Button1, Button2, Button3, Button4, redLedIndex, PrimaryButton, SecondaryButton ;

int State1, State2, State, Pin[TotalLeds]={22, 23, 24, 25, 9, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41} ;

int Speed=StartingSpeed, PushSpeed=StartingSpeed+25 ;

int Player1Score=0, Player2Score=0, Index=0, tempState=1, ErrorCount ;

// Setting Paths and buttons for LEDs...

int forwardPath[4][pathLeds], backwardPath[4][pathLeds], *LedPath, oldRandomPath, newRandomPath ;

// Setting States for Prototype and Relay of HIGH and LOW

int high=HIGH, low=LOW ;

// In case of using Relays, uncomment first 2 lines in the setup() function...

// Prototying Functions

void BlinkRedLed(int) ;
void BlinkPath(int *) ;
void BlinkBothRedLed(int, int) ;
void SpeedControl(int &, int) ;

// Description of above Functions

/**********************************
 * BlinkRedLed() :
 * If someone fails to press Primary button at the correct time, then this function will blink the Red Led of that Player
 * ---------------
 * Blinkpath() :
 * If someone presses the button before actually it should be pressed, then this function will blink all the LEDs of that particular path
 * ---------------
 * BlinkBothRedLed() :
 * If both the players at one side presses both the Primary and the Secondary Buttons at the same time, when only Primary Button should be pressed,
 * then this function will blink red LEDs of both the Players..
**********************************/

void setup()
{
  // Uncomment next 2 lines in case of using Relays
  high = LOW ;
  low = HIGH ;
  Serial.begin(9600) ;
  oldRandomPath = 2 ;
  newRandomPath = 0 ;
  Player1Score = 0 ;
  Player2Score = 0 ;
  Button1 = Player1Button1Pin ;
  Button2 = Player1Button2Pin ;
  Button3 = Player2Button1Pin ;
  Button4 = Player2Button2Pin ;
  
  for( i=0 ; i < 7 ; ++i)
  {
    pinMode(Pin1[i], OUTPUT) ;
    pinMode(Pin2[i], OUTPUT) ;
  }
  
  len[0] = 6 ;
  len[1] = 2 ;
  len[2] = 5 ;
  len[3] = 5 ;
  len[4] = 4 ;
  len[5] = 5 ;
  len[6] = 6 ;
  len[7] = 3 ;
  len[8] = 7 ;
  len[9] = 6 ;
  
  // Setting up all LEDs...
  
  for( i=0 ; i < TotalLeds ; ++i )
  {
    pinMode(Pin[i], OUTPUT) ;
  }
  
  /* Setting Up Forward Paths */
  
  // Path 0 //
  
  for( i=0, k=0 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[0][i] = Pin[k] ;
    if(k == (((RedLeds+YellowLeds)/4)-1))
    {
      k = (((RedLeds+YellowLeds)/2)-1) ;
    }
  }

  // Path 0 //
  
  // Path 1 //
  
  for( i=0, k=0 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[1][i] = Pin[k] ;
    if(k == (((RedLeds+YellowLeds)/4)-1))
    {
      k = (((RedLeds+YellowLeds)/2)-1) ;
    }
    else if(k == ((((RedLeds+YellowLeds)/2)+GreenLeds)-1))
    {
      k = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
  }
  
  // Path 1 //

  // Path 2 //
  
  for( i=0, k=3 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[2][i] = Pin[k] ;
  }
  
  // Path 2 //

  // Path 3 //
  
  for( i=0, k=3 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[3][i] = Pin[k] ;
    if(k == ((((RedLeds+YellowLeds)/2)+GreenLeds)-1))
    {
      k = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
  }
  
  // Path 3 //

  /* Setting of forward Paths done */
  
  /* Setting Up backward Paths */

  // Setting All Backward Paths at once //
  
  for(int j=0 ; j < 4 ; ++j )
  {
    for( i=0, k=pathLeds-1 ; i < pathLeds ; ++i, --k )
    {
      backwardPath[j][i] = forwardPath[j][k] ;
    }
  }

  // Setting Done //
  
  /* Setting of backward Paths done */
  
  // Player 1 Button Pins...
  pinMode(Button1, INPUT) ;
  pinMode(Button2, INPUT) ;
  // Player 2 Button Pins...
  pinMode(Button3, INPUT) ;
  pinMode(Button4, INPUT) ;
  for( i=0 ; i < TotalLeds ; ++i )
  {
    digitalWrite(Pin[i], low) ;
  }
  ShowPlayer1Score(Player1Score) ;
  ShowPlayer2Score(Player2Score) ;
}

void(* resetFunc)(void) = 0x0 ;                          // This function resets Arduino without pushing reset button

/*************************************
/ redLedIndex |
/ ------------
/ Suppose if LEDs goes from PATH 0 to PATH 2, then redLedIndex in the index of the red LED at the end while going from PATH 0 to PATH 2 ( or the starting Index of PATH 2)
/-------------
/ Primary Button is the actual button to be pressed ( State1 variable stores Primary Button's State )
/ ------------
/ Secondary Button is the button adjacent to the Primary Button ( State2 variable stores Secondary Button's State )
*************************************/

void loop()
{
  if(f == 0)
  {
    f = -1 ;
    int pNumber ;
    pNumber = ReadPath() ;
    if(pNumber == 0)
    {
      newRandomPath = 0 ;
    }
    else if(pNumber == 1)
    {
      newRandomPath = 1 ;
    }
    else if(pNumber == 2)
    {
      oldRandomPath = 2 ;
      goto NextPath ;
    }
    else if(pNumber == 2)
    {
      oldRandomPath = 3 ;
      goto NextPath ;
    }
  }
  /* Determining Forward Path and Correct Button */
  
  oldRandomPath = random(3-2+1)+2 ;
  if(newRandomPath == 0)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[0] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[1] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  else if(newRandomPath == 1)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[2] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[3] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  
  /* Forward Path and Correct Button Determined */
  
  State1 = LOW ;
  State2 = LOW ;
  
  for( i=Index ; i < (((RedLeds+YellowLeds)/4)+GreenLeds) ; ++i )
  {
    Index = pathLeds-1 ;
    tempState = 0 ;
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    
    Serial.println("State 1 : ") ;
    Serial.print(State1) ;
    Serial.println() ;
    Serial.println("State 2 : ") ;
    Serial.print(State2) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], high) ;     // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    delay(Speed) ;                      // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], low) ;    // Main Line
  }
  for( k=0 ; k < YellowLeds/4 ; ++i, ++k )
  {
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    State1 = 301 ;
    State2 = 0 ;
    if((State1 > 300) && (State2 > 300))
    {
      BlinkBothRedLed((((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1), TotalLeds-1) ;
      Index = 13 ;
      tempState = 0 ;
      goto NextPath ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      Index = i ;
      tempState = 1 ;
      ++Player1Score ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], high) ;             // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) && (State2 > 300))
    {
      digitalWrite(LedPath[i], low) ;
      tempState = 0 ;
      Index = 13 ;
      BlinkBothRedLed((((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1), TotalLeds-1) ;
      goto NextPath ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      Index = i ;
      tempState = 1 ;
      ++Player1Score ;
      digitalWrite(LedPath[i], low) ;
      goto NextPath ;
    }
    delay(Speed) ;                          // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) && (State2 > 300))
    {
      digitalWrite(LedPath[i], low) ;
      tempState = 0 ;
      Index = 13 ;
      BlinkBothRedLed((((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1), TotalLeds-1) ;
      goto NextPath ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      Index = i ;
      tempState = 1 ;
      ++Player1Score ;
      digitalWrite(LedPath[i], low) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], low) ;            // Main Line
  }
  Serial.println(Index) ;
  Serial.println(i) ;
  if(State1 == 0)
  {
    BlinkRedLed(redLedIndex) ;
    tempState = 0 ;
  }
  
  /* Determining Backward Path and Correct Button */

  NextPath :
  
  SpeedControl(Speed, tempState) ;
  ShowPlayer1Score(Player1Score) ;
  if(Player1Score == MaxScore)
  {
    EndGame(Player1Score, Player2Score) ;
  }
  
  newRandomPath = random(1-0+1)+0 ;
  
  if(oldRandomPath == 2)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[0] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[2] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }
  else if(oldRandomPath == 3)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[1] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[3] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }

  /* Backward Path and Correct Button Determined */
  
  State1 = LOW ;
  State2 = LOW ;
  // Index = 1 ;
  
  for( i=pathLeds-Index ; i < (((RedLeds+YellowLeds)/4)+GreenLeds) ; ++i )
  {
    Index = 1 ;
    tempState = 0 ;
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    digitalWrite(LedPath[i], high) ;           // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    delay(Speed) ;                            // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) || (State2 > 300))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    digitalWrite(LedPath[i], low) ;          // Main Line
  }
  for( k=0 ; k < YellowLeds/4 ; ++i, ++k )
  {
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) && (State2 > 300))
    {
      Index = 1 ;
      tempState = 0 ;
      BlinkBothRedLed(((RedLeds+YellowLeds)/4), 0) ;
      goto End ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      Index = pathLeds-i ;
      tempState = 1 ;
      ++Player2Score ;
      goto End ;
    }
    digitalWrite(LedPath[i], high) ;                           // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) && (State2 > 300))
    {
      Index = 1 ;
      tempState = 0 ;
      digitalWrite(LedPath[i], low) ;
      BlinkBothRedLed(((RedLeds+YellowLeds)/4), 0) ;
      goto End ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      tempState = 1 ;
      digitalWrite(LedPath[i], low) ;
      Index = pathLeds-i ;
      ++Player2Score ;
      goto End ;
    }
    delay(Speed) ;                                        // Main Line
    State1 = analogRead(PrimaryButton) ;
    State2 = analogRead(SecondaryButton) ;
    if((State1 > 300) && (State2 > 300))
    {
      Index = 1 ;
      tempState = 0 ;
      digitalWrite(LedPath[i], low) ;
      BlinkBothRedLed(((RedLeds+YellowLeds)/4), 0) ;
      goto End ;
    }
    else if((State1 > 300) && (State2 < 125))
    {
      tempState = 1 ;
      digitalWrite(LedPath[i], low) ;
      Index = pathLeds-i ;
      ++Player2Score ;
      goto End ;
    }
    digitalWrite(LedPath[i], low) ;                          // Main Line
  }
  if(State1 == 0)
  {
    BlinkRedLed(redLedIndex) ;
    tempState = 0 ;
  }
  End :
  SpeedControl(Speed, tempState) ;
  ShowPlayer2Score(Player2Score) ;
  if(Player2Score == MaxScore)
  {
    EndGame(Player1Score, Player2Score) ;
  }
  
  /*
  Speed = Speed-SpeedReduce ;
  PushSpeed = PushSpeed-SpeedReduce ;
  if(Speed <= 1)
    Speed = ConstantSpeed ;
  if(PushSpeed <= 1)
    PushSpeed = ConstantPushSpeed ;
  */
} 

void BlinkRedLed(int index)
{
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    digitalWrite(Pin[index], high) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
    digitalWrite(Pin[index], low) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
  }
}

void BlinkPath(int *path)
{
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    for(int blinkPath=0 ; blinkPath < pathLeds ; ++blinkPath )
    {
      digitalWrite(path[blinkPath], high) ;
    }
    delay(IncorrectMoveLedBlinkingSpeed ) ;
    for(int blinkPath=0 ; blinkPath < pathLeds ; ++blinkPath )
    {
      digitalWrite(path[blinkPath], low) ;
    }
    delay(IncorrectMoveLedBlinkingSpeed ) ;
  }
}

void BlinkBothRedLed(int index1, int index2)
{
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    digitalWrite(Pin[index1], high) ;
    digitalWrite(Pin[index2], high) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
    digitalWrite(Pin[index1], low) ;
    digitalWrite(Pin[index2], low) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
  }
}

int ReadPath(void)
{
  int s, pNumber ;
  s = 0 ;
  while(true)
  {
    s = analogRead(Button1) ;
    if(s > 300)
    {
      pNumber = 0 ;
      break ;
    }
    s = analogRead(Button2) ;
    if(s > 300)
    {
      pNumber = 1 ;
      break ;
    }
    s = analogRead(Button3) ;
    if(s > 300)
    {
      pNumber = 2 ;
      break ;
    }
    s = analogRead(Button4) ;
    if(s > 300)
    {
      pNumber = 3 ;
      break ;
    }
  }
  Serial.println(s) ;
  s = 0 ;
  return pNumber ;
}

void SpeedControl(int &Speed, int currentState)
{
  if(currentState == 1)
  {
    Speed = Speed-SpeedReduce ;
    if(Speed < MinimumSpeed)
    {
      Speed = MinimumSpeed ;
    }
    ErrorCount = 0 ;
  }
  else
  {
    ++ErrorCount ;
    if(ErrorCount > MaxErrorCount)
    {
      Speed = Speed+SpeedReduce ;
      if(Speed > MaximumSpeed)
      {
        Speed = MaximumSpeed ;
      }
    }
  }
}

/*
void showNumber(int ind)
{
  
  for( k=0 ; k < 7 ; ++k )
    digitalWrite(Pin[k], LOW) ;
  for( k=0 ; k < len[i] ; ++k )
    digitalWrite(number[i][k], HIGH) ;
  delay(500) ;
  for( k=0 ; k < len[i] ; ++k )
    digitalWrite(number[i][k], LOW) ;
}
*/

void ShowPlayer1Score(int index)
{
  for( int k=0 ; k < 7 ; ++k )
    digitalWrite(Pin1[k], LOW) ;
  for( int k=0 ; k < len[index] ; ++k )
    digitalWrite(player1SSD[index][k], HIGH) ;
}

void ShowPlayer2Score(int index)
{
  for( int k=0 ; k < 7 ; ++k )
    digitalWrite(Pin2[k], LOW) ;
  for( int k=0 ; k < len[index] ; ++k )
    digitalWrite(player2SSD[index][k], HIGH) ;
}

void EndGame(int p1s, int p2s)
{
  for(int i=0 ; i < MaxBlinksSSD ; ++i)
  {
    for(int k=0  ; k < 7 ; ++k)
    {
      ShowPlayer1Score(p1s) ;
      ShowPlayer2Score(p2s) ;
    }
    delay(500) ;
    for(int k=0  ; k < 7 ; ++k)
    {
      digitalWrite(Pin1[k], LOW) ;
      digitalWrite(Pin2[k], LOW) ;
    }
    delay(750) ;
  }
  for(int k=0  ; k < 7 ; ++k)
  {
    ShowPlayer1Score(p1s) ;
    ShowPlayer2Score(p2s) ;
  }
  delay(5000) ;
  resetFunc() ;
}
