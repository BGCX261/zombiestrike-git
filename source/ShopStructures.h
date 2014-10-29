#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include <string>



struct UpgradeF
{
	float stat = 0.0f;
	int currTier = 0;
	int maxTier = 0;
};
struct UpgradeN
{
	int stat = 0.0f;
	int currTier = 0;
	int maxTier = 0;
};

struct ButtonF
{
	SGD::Rectangle collisionRect;
	SGD::Point position;
	SGD::HTexture* buttonImage = nullptr;
	UpgradeF upgradedSkill;
	std::string text;
	bool isLocked;

};
struct ButtonN
{
	SGD::Rectangle collisionRect;
	SGD::Point position;
	SGD::HTexture* buttonImage = nullptr;
	UpgradeN upgradedSkill;
	std::string text; 
	bool isBought;
	bool isMaxed;

};

struct PistolUpgrade
{

	ButtonN magSize;
	ButtonN ammoCap;
	ButtonF reloadTime;
	ButtonF recoilTime;
	bool isBought;

};
struct RevolverUpgrade
{
	ButtonN magSize;
	ButtonN ammoCap;
	ButtonN penPower;
	ButtonF recoilTime;
	ButtonF reloadTime;
	ButtonF damage;
	bool isBought;

};
struct SMGUpgrade
{
	
	ButtonN magSize;
	ButtonN ammoCap;
	ButtonF reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	bool isBought;

};

struct AssualtRifleUpgrade
{
	ButtonN magSize;
	ButtonN ammoCap;
	ButtonF reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	ButtonF recoilTime;
	bool isBought;

};

struct SawnOffUpgrade
{
	ButtonN ammoCap;
	ButtonN reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	ButtonF recoilTime;
	bool isBought;



};
struct StdShotgunUpgrade
{
	ButtonN magSize;
	ButtonN ammoCap;
	ButtonF reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	ButtonF recoilTime;
	bool isBought;



};

struct SniperUpgrade
{
	ButtonN magSize;
	ButtonN penPower;

	ButtonN ammoCap;
	ButtonF reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	ButtonF recoilTime;
	ButtonF bulletVelocity;
	bool isBought;




};
struct FlamethrowerUpgrade
{
	UpgradeN magSize;
	UpgradeN ammoCap;
	ButtonF reloadTime;
	ButtonF bulletSpread;
	ButtonF damage;
	ButtonF bulletVelocity;
	bool isBought;

};
struct GrenadeLauncherUpgrade
{
	UpgradeN magSize;
	UpgradeN ammoCap;
	ButtonF reloadTime;
	ButtonF damage;
	ButtonF bulletVelocity;
	bool isBought;

};