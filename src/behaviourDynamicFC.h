#ifndef DYNAMIC_FC_H
#define DYNAMIC_FC_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "behaviour.h"
#include "Vector2d.h"
#include "sensorContact.h"

class BehaviourDynamicFC: public Behaviour{

 public:

  BehaviourDynamicFC(float normAng = 0., float deadAheadZone = 1.9){
  
    float bearingConstraint = normAng + M_PI/2.; 
    deadAheadAngularWidth = deadAheadZone;
    
    formationNormal = new Vector2d(normAng);
    f = new Vector2d(bearingConstraint);
        
    if(Random() > 0.5) leftHanded = true;
    else leftHanded = true;
  }

  ~BehaviourDynamicFC(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){

    Vector2d * r = new Vector2d(targetBearing);
         
    float mag = DotProduct(r, formationNormal);   

    if(agentBehind and !agentAhead){ 
      response = ScalarMultiplyVector(mag, formationNormal);
    }
    else if(agentDeadAhead){
      if(leftHanded) response = f;
      else response = ScalarMultiplyVector(-1., f);
    }
    else{ 
      response = ScalarMultiplyVector(mag, formationNormal); 
    }

    return response;
  }

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){
    // Reset Stimuli
    agentBehind = false;
    agentAhead = false;
    agentDeadAhead = false;

    targetBearing = 0.;

    float largest = -2.;
    int targetID  = -1;
    
    active = true;

    // Now evaluate the contacts
    for(unsigned int i = 0; i < contacts.size(); i++){
      Vector2d * v = contacts[i]->GetVector();
     
        
      // Get the current bearing vector
      v->Normalize();
      
      // Project contact bearing on to formation normal
      float dot = DotProduct(formationNormal, v);   
  
      if(dot < 0.) agentBehind = true;
      else if( dot > deadAheadAngularWidth ){
	agentDeadAhead = true;
	agentAhead = true;
      }
      else agentAhead = true;
   
      // Find the bearing which is closest to formation Normal
      if( dot > largest ){
	largest = dot;
	targetID = i;
      }
    
    } // End of contact loop.  
    
    // Set the targetBearing
    //printf("Target ID = %i\n", targetID);
    //printf("agentBehind = %i\n", agentBehind);
    //printf("agentAhead = %i\n", agentAhead);
    //printf("agentDeadAhead = %i\n", agentDeadAhead);
    if(targetID != -1){
      targetBearing = contacts[targetID]->GetVector()->GetAngle();
    }
  }

  // Display information to the screen.
  void Print(){
    printf("DynamicFC\t%f\n",activationLevel);
  }

 private:

  int timeStamp;

  // Stimuli variables
  bool agentBehind;
  bool agentAhead;
  bool agentDeadAhead;
  float targetBearing;

  // Variables set during initialization
  float deadAheadAngularWidth;
 
  Vector2d * f; 
  Vector2d * formationNormal;

  bool leftHanded;

  // Random number
  float Random(){
    return rand()/(float)(RAND_MAX);
  }

};
#endif
