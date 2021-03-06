/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef QUAD_FIELD_H
#define QUAD_FIELD_H

#include <set>
#include <vector>
#include <list>
#include <boost/noncopyable.hpp>

#include "System/creg/creg_cond.h"
#include "System/float3.h"

class CUnit;
class CFeature;
class CProjectile;
class CSolidObject;

class CQuadField : boost::noncopyable
{
	CR_DECLARE(CQuadField);
	CR_DECLARE_SUB(Quad);

	const static int QUAD_SIZE = 256;

public:
	CQuadField();
	~CQuadField();

	std::vector<int> GetQuadsOnRay(const float3& start, float3 dir, float length);
	std::vector<int> GetQuads(float3 pos, float radius) const;
	std::vector<int> GetQuadsRectangle(const float3& pos, const float3& pos2) const;

	// optimized functions, somewhat less userfriendly
	void GetQuads(float3 pos, float radius, int*& dst) const;
	void GetQuadsOnRay(float3 start, float3 dir, float length, int*& dst);
	void GetUnitsAndFeaturesExact(const float3& pos, float radius, CUnit**& dstUnit, CFeature**& dstFeature);

	/**
	 * Returns all units within @c radius of @c pos,
	 * and treats each unit as a 3D point object
	 */
	std::vector<CUnit*> GetUnits(const float3& pos, float radius);
	/**
	 * Returns all units within @c radius of @c pos,
	 * takes the 3D model radius of each unit into account,
 	 * and performs the search within a sphere or cylinder depending on @c spherical
	 */
	std::vector<CUnit*> GetUnitsExact(const float3& pos, float radius, bool spherical = true);
	/**
	 * Returns all units within the rectangle defined by
	 * mins and maxs, which extends infinitely along the y-axis
	 */
	std::vector<CUnit*> GetUnitsExact(const float3& mins, const float3& maxs);

	/**
	 * Returns all features within @c radius of @c pos,
	 * and takes the 3D model radius of each feature into account
	 */
	std::vector<CFeature*> GetFeaturesExact(const float3& pos, float radius);
	/**
	 * Returns all features within @c radius of @c pos,
	 * and performs the search within a sphere or cylinder depending on @c spherical
	 */
	std::vector<CFeature*> GetFeaturesExact(const float3& pos, float radius, bool spherical);
	/**
	 * Returns all features within the rectangle defined by
	 * mins and maxs, which extends infinitely along the y-axis
	 */
	std::vector<CFeature*> GetFeaturesExact(const float3& mins, const float3& maxs);

	std::vector<CProjectile*> GetProjectilesExact(const float3& pos, float radius);
	std::vector<CProjectile*> GetProjectilesExact(const float3& mins, const float3& maxs);

	std::vector<CSolidObject*> GetSolidsExact(const float3& pos, float radius);

	void MovedUnit(CUnit* unit);
	void RemoveUnit(CUnit* unit);

	void AddFeature(CFeature* feature);
	void RemoveFeature(CFeature* feature);

	void MovedProjectile(CProjectile* projectile);
	void AddProjectile(CProjectile* projectile);
	void RemoveProjectile(CProjectile* projectile);

	struct Quad {
		CR_DECLARE_STRUCT(Quad);
		Quad();
		std::list<CUnit*> units;
		std::vector< std::list<CUnit*> > teamUnits;
		std::list<CFeature*> features;
		std::list<CProjectile*> projectiles;
	};

	const Quad& GetQuad(int i) const {
		assert(i >= 0);
		assert(i < baseQuads.size());
		return baseQuads[i];
	}
	const Quad& GetQuadAt(int x, int z) const {
		return baseQuads[numQuadsX * z + x];
	}
	int GetNumQuadsX() const { return numQuadsX; }
	int GetNumQuadsZ() const { return numQuadsZ; }

private:
	void Serialize(creg::ISerializer& s);

	std::vector<Quad> baseQuads;
	int numQuadsX;
	int numQuadsZ;
	int* tempQuads;
};

extern CQuadField* qf;

#endif /* QUAD_FIELD_H */
