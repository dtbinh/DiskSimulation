#ifndef SNS_H
#define SNS_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "Vector2d.h"
#include "behaviour.h"
#include "sensorContact.h"

class BehaviourStaticFC: public Behaviour{

 public:
  BehaviourStaticFC(float constraint, int id){

    activationLevel = 0.;
    staticGain = 1.;

    targetID = id;
    bearingConstraint = constraint;

    leftHanded = false;
    if(Random() > 0.5)
      leftHanded = true;
    //printf("leftHanded = %d\n", leftHanded);
  };

  ~BehaviourStaticFC(){};
 
  // Returns a velocity vector
  Vector2d * GetResponse(){
    delete response;

    if(activationLevel > 0.){
      Vector2d * r = new Vector2d(targetBearing);
      Vector2d * rPerp = r->GetPerp();
      Vector2d * f = new Vector2d(bearingConstraint);
      Vector2d * fPerp = f->GetPerp();
      
      float mag = DotProduct(r, fPerp);
      
      response = ScalarMultiplyVector(mag, rPerp);
      
      delete r;
      delete rPerp;
      delete f;
      delete fPerp;
    }
    else{
      response = new Vector2d(0.,0.);
    }
    
    return response;
  };

  // Takes in a list of vectors giving the local position of neighbours
  void UpdateStimulus( std::vector<SensorContact*> contacts){

    // Find bearing to target named in constraint	
    //targetBearing = contacts[targetID]->GetVector()->GetAngle();
    bool targetFound = false;
    for(unsigned int i = 0; i < contacts.size(); i++){
      if(contacts[i]->GetType() == targetID){
	targetFound = true;
	targetBearing = contacts[i]->GetVector()->GetAngle();
	break;
      }
    }
    
    if(targetFound){
      Vector2d * r = new Vector2d(targetBearing);
      Vector2d * rPerp = r->GetPerp();
      Vector2d * f = new Vector2d(bearingConstraint);
      Vector2d * fPerp = f->GetPerp();
       
      float mag = DotProduct(r, fPerp);
      activationLevel = fabs(mag);
      
      delete r;
      delete rPerp;
      delete f;
      delete fPerp;
    }
    else{
      activationLevel = 0.;
    }

  };
  
  // Display information to the screen.
  void Print(){
    printf("Tangential\t%f\n",activationLevel);
  };

 private:
  float bearingConstraint;
  float targetBearing;
  int targetID;

  bool leftHanded;
 
  float Random(){
    return rand()/(float)(RAND_MAX);
  }

};
#endif
