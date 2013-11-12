/**
 * @file CitySceneGenerator.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef CITY_SCENE_GENERATOR_H
#define CITY_SCENE_GENERATOR_H

#include <random>

class Scene;
class CitySceneGenerator
{
public:
	CitySceneGenerator();

	void generate(Scene* scene);
private:
	std::mt19937 m_rng;
};

#endif // !CITY_SCENE_GENERATOR_H
