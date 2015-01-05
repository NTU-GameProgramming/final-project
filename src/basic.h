#pragma once

#include <FlyWin32.h>
#include <iostream>
#include <math.h>

enum ObjectType{
	PLAYER = 0,
	TERRIAN = 1,
	DEFAULT = 2
};

class GameObject{
public:
	GameObject()
		:isValid(true),
		objectType(DEFAULT){};

	virtual ~GameObject(){

	}

	void getLeftVertDir(float *srcFDir, float *srcUDir, float *dstDir){
		FyCross(dstDir, srcUDir, srcFDir);
		normalizeVector(dstDir, 3);
	}

	void getRightVertDir(float *srcFDir, float *srcUDir, float *dstDir){
		FyCross(dstDir, srcFDir, srcUDir);
		normalizeVector(dstDir, 3);
	}

	void normalizeVector(float *vec, int size){
		float fSquareSum = 0;
		for (int i = 0; i < size; ++i){
			fSquareSum += vec[i] * vec[i];
		}
		fSquareSum = std::sqrt(fSquareSum);

		for (int i = 0; i < size; ++i){
			vec[i] = vec[i] / fSquareSum;
		}
	}

	void getPositionDist2D(float *vec1, float *vec2, float &dst){
		float fDiff[2];
		fDiff[0] = vec1[0] - vec2[0];
		fDiff[1] = vec1[1] - vec2[1];
		float fSquareDst = fDiff[0] * fDiff[0] + fDiff[1] * fDiff[1];
		dst = std::sqrt(fSquareDst);
	}
	void getVectorAngle(float *vec1, float *vec2, float &angle){
		float fDotProduct = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
		float fNorm = std::sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2])*
			std::sqrt(vec2[0] * vec2[0] + vec2[1] * vec2[1] + vec2[2] * vec2[2]);
		float cosValue = fDotProduct / fNorm;
		if (cosValue > 1){
			cosValue = 1;
		}
		else if (cosValue < -1){
			cosValue = -1;
		}
		angle = 180.0*std::acos(cosValue) / 3.1415926;

		float fVertVector[3];
		FyCross(fVertVector, vec1, vec2);
		if (fVertVector[2] > 0){
			angle = -angle;
		}
	}

	virtual void getPos(float *pos, BOOL4 beWorldSpace = TRUE){

	}

	virtual void getDir(float *fDir, float *uDir, BOOL4 beWorldSpace = TRUE){

	}

	virtual OBJECTid getObjectId(){
		return FAILED_ID;
	}

	virtual void update(int skip){
		std::cout << "basic";
	}

	virtual void collisionEvent(OBJECTid collideObject){

	}

	bool isValid;

	ObjectType objectType;
};

